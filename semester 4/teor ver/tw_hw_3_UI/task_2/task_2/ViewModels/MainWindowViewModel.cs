using System;
using System.Linq;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Data;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using Avalonia.Threading;
using AvaloniaGraphControl;
using task_2.Models;

namespace task_2.ViewModels;

public partial class MainWindowViewModel : ViewModelBase
{
    bool first_start = true;

    [ObservableProperty]
    private int _m = 2;

    [ObservableProperty]
    private int _depth = 3;

    [ObservableProperty]
    private double _stopProbability = 0.1;
    
    [ObservableProperty]
    private int _countExperiments = 1;
    
    [ObservableProperty]
    private Graph _myGraph;
    
    [ObservableProperty]
    private bool _disableSimulationDelay = false;
    
    [ObservableProperty]
    private ObservableCollection<ProbabilityValue> _transitionProbabilities;

    public ObservableCollection<LeafHitInfo> LeafHitCollectonObservableCollection { get; set; } =
        new ObservableCollection<LeafHitInfo>();
    
    public ObservableCollection<PathLengthInfo> PathLengthsObservableCollection { get; set; } = 
        new ObservableCollection<PathLengthInfo>();
    
    [ObservableProperty]
    private ObservableCollection<tree_node> _treeNodes = new();

    [ObservableProperty]
    private ObservableCollection<Edge> _treeEdges = new();
    
    public class LeafHitInfo
    {
        public LeafHitInfo(int id, double hits)
        {
            LeafId = id;
            Hits = hits;
        }

        public int LeafId { get; set; }
        public double Hits { get; set; }
    }
    
    public class PathLengthInfo
    {
        public PathLengthInfo(int _id, double _length)
        {
            pathId = _id;
            length = _length;
        }
        
        public int pathId { get; set; }
        public double length { get; set; }
    }

    // Статистика
    private Dictionary<int, int> _leafHits = new();
    private Dictionary<int, int> _pathLengths = new();
    private int _totalExperiments = 0;

    private List<tree_node>? _currentPath;
    
    public IAsyncRelayCommand StartExperimentCommand { get; }
    public IRelayCommand ResetStatisticsCommand { get; }

    // Строковое представление статистики для отображения
    public string LeafHitsText => string.Join("\n", _leafHits.Select(kvp => $"Лист {kvp.Key}: {kvp.Value} раз"));
    public string PathLengthsText => string.Join("\n", _pathLengths.Select(kvp => $"Длина {kvp.Key}: {kvp.Value} раз"));
    public string TotalExperimentsText => $"Кол-во экспериментов:\n{_totalExperiments}";
    
    // Добавлено: свойство для получения текущих вероятностей в виде массива double
    public double[] CurrentProbabilities => TransitionProbabilities?.Select(p => p.Value).ToArray() ?? Array.Empty<double>();

    public MainWindowViewModel()
    {
        UpdateProbabilitiesCollection();

        StartExperimentCommand = new AsyncRelayCommand(StartExperimentAsync);
        ResetStatisticsCommand = new RelayCommand(ResetStatistics);
        
        this.PropertyChanged += (s, e) =>
        {
            if (e.PropertyName == nameof(M))
                UpdateProbabilitiesCollection();
        };
    }
   
    private void UpdateProbabilitiesCollection()
    {
        var newProbs = new ObservableCollection<ProbabilityValue>();
        for (int i = 0; i < M; i++)
        {
            // По умолчанию равные вероятности
            double defaultValue = 1.0 / M;
            newProbs.Add(new ProbabilityValue(defaultValue));
        }
        TransitionProbabilities = newProbs;
        
        // Добавлено: подписка на изменения каждого элемента
        foreach (var prob in TransitionProbabilities)
        {
            prob.PropertyChanged += (s, e) =>
            {
                if (e.PropertyName == nameof(ProbabilityValue.Value))
                {
                    // Здесь можно добавить валидацию или уведомления при изменении
                    OnPropertyChanged(nameof(CurrentProbabilities));
                }
            };
        }
    }

    private async Task StartExperimentAsync()
    {
        await Task.Run(async () =>
        {
            if (!first_start)
            {
                _leafHits.Clear();
                _pathLengths.Clear();
                _totalExperiments = 0;
            
                // Очищаем UI-коллекции через диспетчер
                await Dispatcher.UIThread.InvokeAsync(() =>
                {
                    LeafHitCollectonObservableCollection.Clear();
                    PathLengthsObservableCollection.Clear();
                
                    foreach (var node in TreeNodes)
                        node.IsInPath = false;
                    
                    OnPropertyChanged(nameof(TreeNodes));
                    OnPropertyChanged(nameof(TotalExperimentsText));
                });
            }
            first_start = false;
            
            var builder = new tree_builder();
            var root = builder.build_m_ary_tree(M, Depth, StopProbability);
            
            var allNodes = new List<tree_node>();
            CollectNodesAndEdges(root, allNodes);
            
            for (int i = 0; i < CountExperiments; i++)
            {
                foreach (var node in allNodes)
                    node.IsInPath = false;
                
                var engine = new simulation_engine();
                
                // Исправлено: получаем актуальные вероятности из UI
                var probsArray = CurrentProbabilities;
                
                var path = engine.simulate_point_movement(root, probsArray, ref _leafHits, ref _pathLengths);
            
                var graph = new Graph();
                foreach (var parent in allNodes)
                {
                    if (path.Contains(parent)) parent.IsInPath = true;
                    foreach (var child in parent.children)
                    {
                        if (path.Contains(child)) child.IsInPath = true;
                        graph.Edges.Add(new AvaloniaGraphControl.Edge(parent, child));
                    }
                }

                if (!_disableSimulationDelay || _countExperiments - 1 == i)
                {
                    await Dispatcher.UIThread.InvokeAsync(() =>
                    {
                        MyGraph = graph;
                    });
                }

                _totalExperiments++;
                
                var temp = new ObservableCollection<LeafHitInfo>();
                foreach (var kvp in _leafHits)
                    temp.Add(new LeafHitInfo(kvp.Key, (double)kvp.Value/_totalExperiments));
                
                var temp_2 = new ObservableCollection<PathLengthInfo>();
                foreach (var kvp in _pathLengths)
                    temp_2.Add(new PathLengthInfo(kvp.Key, (double)kvp.Value/_totalExperiments));
                
                // Обновляем статистику в UI потоке
                await Dispatcher.UIThread.InvokeAsync(() =>
                {
                    LeafHitCollectonObservableCollection = 
                        new ObservableCollection<LeafHitInfo>(temp.OrderBy(x => x.LeafId));
                    
                    PathLengthsObservableCollection =
                        new ObservableCollection<PathLengthInfo>(temp_2.OrderBy(x => x.pathId));
                    
                    OnPropertyChanged(nameof(TotalExperimentsText));
                    OnPropertyChanged(nameof(LeafHitCollectonObservableCollection));
                    OnPropertyChanged(nameof(PathLengthsObservableCollection));
                });
                
                Task.Delay((_disableSimulationDelay || _countExperiments - 1 == i) ? 0 : 1250).Wait();
            }
        });
    }

    private void ResetStatistics()
    {
        // Ваш код для сброса статистики
    }
    
    private void CollectNodesAndEdges(tree_node node, List<tree_node> nodes)
    {
        if (!nodes.Contains(node))
        {
            nodes.Add(node);
        }
        foreach (var child in node.children)
        {
            CollectNodesAndEdges(child, nodes);
        }
    }
}