namespace task_1.ViewModels;

using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using AvaloniaGraphControl;
using task_1.Models;
using Avalonia.Threading;

public partial class MainWindowViewModel : ViewModelBase
{
    private readonly SimulationEngine _engine = new();
    private readonly GraphLoaderService _loader = new();
    private PersonNode[] _nodes = Array.Empty<PersonNode>();
    
    private bool _isRunning = false;
    private bool _isPaused = false;
    private bool _isDataLoaded = false;

    [ObservableProperty] private double _p = 0.3;
    [ObservableProperty] private double _p1 = 0.1;

    [ObservableProperty] private Graph? _networkGraph;
    [ObservableProperty] private int _currentStep = 0;
    [ObservableProperty] private bool _isGraphVisualizable = true;
    [ObservableProperty] private string _statusText = "Файл не загружен";

    public ObservableCollection<PersonNode> UninfectedList { get; } = new();
    public ObservableCollection<PersonNode> HealedList { get; } = new();
    public ObservableCollection<PersonNode> HealedUnhealedEnvList { get; } = new();
    public ObservableCollection<PersonNode> UninfectedInfectedEnvList { get; } = new();
    
    [ObservableProperty]
    private string _pauseButtonText = "Пауза";
    
    private bool CanStart() => !_isRunning && _isDataLoaded;
    private bool CanPause() => _isRunning;
    private bool CanStop() => _isRunning;

    private const int _minGraphBorder = 700;
    private const int _maxGraphBorder = 1200;

    [RelayCommand]
    private async Task LoadGraphAsync()
    {
        StatusText = "Загрузка файла...";
        var loadedNodes = await _loader.LoadGraphAsync();
        if (loadedNodes != null)
        {
            _nodes = loadedNodes;
            IsGraphVisualizable = _nodes.Length <= _maxGraphBorder;
            StatusText = $"Загружено узлов: {_nodes.Length}. " +
                         $"\nОтрисовка графа: {(IsGraphVisualizable ? "Включена" : "Выключена\n(Граф слишком большой)")} " +
                         $"\n{(_nodes.Length is >= _minGraphBorder and < _maxGraphBorder? "Длительность отрисовки увеличена" : "")}";
            _isDataLoaded = true;
            ResetSimulation();
        }
        else
        {
            StatusText = "Файл не выбран или поврежден";
        }
    }

    [RelayCommand]
    private void ResetSimulation()
    {
        _isRunning = false;
        _isPaused = false;
        CurrentStep = 0;
        _engine.Initialize(_nodes, P);
        UpdateVisuals();
        RefreshCommands();
    }

    [RelayCommand(CanExecute = nameof(CanStart))]
    private async Task StartSimulationAsync()
    {
        _isRunning = true;
        _isPaused = false;
        PauseButtonText = "Пауза";
        RefreshCommands();

        await Task.Run(async () =>
        {
            while (_isRunning)
            {
                if (_isPaused)
                {
                    await Task.Delay(200);
                    continue;
                }

                bool active = _engine.Step(_nodes, P, P1);
                CurrentStep++;

                if (IsGraphVisualizable)
                {
                    await Dispatcher.UIThread.InvokeAsync(UpdateVisuals);
                }

                if (!active)
                {
                    _isRunning = false;
                    break;
                }

                var del = 50;
                if (!_isGraphVisualizable) del = 50;
                else if (_nodes.Length <= _minGraphBorder) del = 400;
                else if (_nodes.Length > _maxGraphBorder) del = 2500;
                await Task.Delay(del);
            }

            _isRunning = false;
            _isPaused = false;
            await Dispatcher.UIThread.InvokeAsync(() =>
            {
                PauseButtonText = "Пауза";
                UpdateStatsTables();
                RefreshCommands();
            });
        });
    }

    [RelayCommand(CanExecute = nameof(CanPause))]
    private void PauseSimulation()
    {
        if (!_isPaused)
        {
            _isPaused = true;
            PauseButtonText = "Продолжить";
            UpdateStatsTables();
        }
        else
        {
            _isPaused = false;
            PauseButtonText = "Пауза";
        }
        RefreshCommands();
    }

    [RelayCommand(CanExecute = nameof(CanStop))]
    private void StopSimulation()
    {
        _isRunning = false;
        _isPaused = false;
        PauseButtonText = "Пауза";
        UpdateStatsTables();
        RefreshCommands();
    }

    private void RefreshCommands()
    {
        StartSimulationCommand.NotifyCanExecuteChanged();
        PauseSimulationCommand.NotifyCanExecuteChanged();
        StopSimulationCommand.NotifyCanExecuteChanged();
    }

    private void UpdateVisuals()
    {
        if (!IsGraphVisualizable) return;

        var graph = new Graph();
        var edgesAdded = new HashSet<(int, int)>();

        for (int i = 0; i < _nodes.Length; i++)
        {
            var node = _nodes[i];
            int[] neighbors = node.NeighborIndices;
            for (int j = 0; j < neighbors.Length; j++)
            {
                var neighbor = _nodes[neighbors[j]];
                int minId = Math.Min(node.Id, neighbor.Id);
                int maxId = Math.Max(node.Id, neighbor.Id);
                if (edgesAdded.Add((minId, maxId)))
                {
                    graph.Edges.Add(new Edge(node, neighbor));
                }
            }
        }
        NetworkGraph = graph;
    }

    private void UpdateStatsTables()
    {
        UninfectedList.Clear();
        foreach (var n in _engine.GetUninfected(_nodes)) UninfectedList.Add(n);

        HealedList.Clear();
        foreach (var n in _engine.GetHealed(_nodes)) HealedList.Add(n);

        HealedUnhealedEnvList.Clear();
        foreach (var n in _engine.GetHealedWithUnhealedEnv(_nodes)) HealedUnhealedEnvList.Add(n);

        UninfectedInfectedEnvList.Clear();
        foreach (var n in _engine.GetUninfectedWithFullyInfectedEnv(_nodes)) UninfectedInfectedEnvList.Add(n);
    }
}