using System;
using System.Linq;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using Avalonia.Threading;
using AvaloniaGraphControl;
using task_2.Models;

namespace task_2.ViewModels;

public partial class MainWindowViewModel : ViewModelBase
{
    // Параметры эксперимента
    [ObservableProperty]
    private int _m = 2;

    [ObservableProperty]
    private int _depth = 3;

    [ObservableProperty]
    private double _stopProbability = 0.1;
    
    [ObservableProperty]
    private Graph _myGraph;

    [ObservableProperty]
    private ObservableCollection<double> _transitionProbabilities;

    // Коллекции для отрисовки дерева
    [ObservableProperty]
    private ObservableCollection<tree_node> _treeNodes = new();

    [ObservableProperty]
    private ObservableCollection<Edge> _treeEdges = new();

    // Статистика
    private Dictionary<string, int> _leafHits = new();
    private Dictionary<int, int> _pathLengths = new();
    private int _totalExperiments = 0;

    // Текущий путь (последний эксперимент)
    private List<tree_node>? _currentPath;

    // Команды
    public IAsyncRelayCommand StartExperimentCommand { get; }
    public IRelayCommand ResetStatisticsCommand { get; }

    // Строковое представление статистики для отображения
    public string LeafHitsText => string.Join("\n", _leafHits.Select(kvp => $"Лист {kvp.Key}: {kvp.Value} раз"));
    public string PathLengthsText => string.Join("\n", _pathLengths.Select(kvp => $"Длина {kvp.Key}: {kvp.Value} раз"));
    public string TotalExperimentsText => $"Всего экспериментов: {_totalExperiments}";
    

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
        var newProbs = new ObservableCollection<double>();
        for (int i = 0; i < M; i++)
            newProbs.Add(1.0 / M);
        TransitionProbabilities = newProbs;
    }

    private async Task StartExperimentAsync()
    {
        await Task.Run(() =>
        {
            var builder = new tree_builder();
            var root = builder.build_m_ary_tree(M, Depth, StopProbability);
            
            var allNodes = new List<tree_node>();
            CollectNodesAndEdges(root, allNodes);
            
            var engine = new simulation_engine();
            var probsArray = TransitionProbabilities.ToArray();
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
            
            MyGraph = graph;
        });
    }

    private void ResetStatistics()
    {
        _leafHits.Clear();
        _pathLengths.Clear();
        _totalExperiments = 0;
        foreach (var node in TreeNodes)
            node.IsInPath = false;
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