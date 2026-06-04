namespace task_3.ViewModels;

using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using AvaloniaGraphControl;
using task_3.Models;
using Avalonia.Threading;

public partial class MainWindowViewModel : ViewModelBase
{
    private readonly RumorSimulationEngine _engine = new();
    private bool _isRunning;
    private bool _isPaused;

    [ObservableProperty] private int _n = 100;
    [ObservableProperty] private int _r = 5;
    [ObservableProperty] private int _numN = 2;
    [ObservableProperty] private int _k = 500;

    [ObservableProperty] private int _currentExperimentProgress = 0;
    [ObservableProperty] private Graph? _rumorGraph;
    [ObservableProperty] private bool _isGraphVisualizable = true;
    [ObservableProperty] private string _pauseButtonText = "Пауза";
    [ObservableProperty] private string _statusInfo = "Система готова к запуску серии";
    
    [ObservableProperty] private string _empProbA = "0.00%";
    [ObservableProperty] private string _empProbB = "0.00%";
    [ObservableProperty] private string _anProbA = "0.00%";
    [ObservableProperty] private string _anProbB = "0.00%";

    public ObservableCollection<ExperimentResult> ExperimentsList { get; } = new();

    private bool CanStart() => !_isRunning;
    private bool CanPause() => _isRunning;
    private bool CanStop() => _isRunning;

    private void RefreshCommands()
    {
        StartSimulationCommand.NotifyCanExecuteChanged();
        PauseSimulationCommand.NotifyCanExecuteChanged();
        StopSimulationCommand.NotifyCanExecuteChanged();
    }

    [RelayCommand(CanExecute = nameof(CanStart))]
    private async Task StartSimulationAsync()
    {
        _isRunning = true;
        _isPaused = false;
        PauseButtonText = "Пауза";
        CurrentExperimentProgress = 0;
        ExperimentsList.Clear();
        RefreshCommands();

        IsGraphVisualizable = (_r * _numN + 1) <= 1000;
        StatusInfo = IsGraphVisualizable ? "Выполнение серий..." : "Серия запущена. Визуализация отключена";

        int totalSuccessA = 0;
        int totalSuccessB = 0;
        List<(int from, int to)> lastEdges = new();

        await Task.Run(async () =>
        {
            for (int i = 1; i <= K; i++)
            {
                while (_isPaused && _isRunning)
                {
                    await Task.Delay(200);
                }

                if (!_isRunning) break;

                var res = _engine.RunSingleExperiment(i, N, R, NumN, out var edges);
                if (res.NoReturnToOriginator) totalSuccessA++;
                if (res.NoDuplication) totalSuccessB++;

                if (i == K || i <= 10)
                {
                    lastEdges = edges;
                }

                await Dispatcher.UIThread.InvokeAsync(() =>
                {
                    ExperimentsList.Add(res);
                    CurrentExperimentProgress = i;
                    
                    EmpProbA = $"{((double)totalSuccessA / i * 100):F2}%";
                    EmpProbB = $"{((double)totalSuccessB / i * 100):F2}%";
                });
                
                if (K <= 200) await Task.Delay(10);
            }

            _isRunning = false;
            await Dispatcher.UIThread.InvokeAsync(() =>
            {
                PauseButtonText = "Пауза";
                StatusInfo = "Серия экспериментов завершена.";
                
                AnProbA = $"{(_engine.GetAnalyticalProbabilityA(N, R, NumN) * 100):F2}%";
                AnProbB = $"{(_engine.GetAnalyticalProbabilityB(N, R, NumN) * 100):F2}%";
                
                if (IsGraphVisualizable && lastEdges.Count > 0)
                {
                    var graph = new Graph();
                    var nodesMap = new Dictionary<int, string>();
                    
                    foreach (var edge in lastEdges)
                    {
                        graph.Edges.Add(new Edge(edge.from.ToString(), edge.to.ToString()));
                    }
                    RumorGraph = graph;
                }
                RefreshCommands();
            });
        });
    }

    [RelayCommand(CanExecute = nameof(CanPause))]
    private void PauseSimulation()
    {
        _isPaused = !_isPaused;
        PauseButtonText = _isPaused ? "Продолжить" : "Пауза";
        StatusInfo = _isPaused ? "Симуляция приостановлена" : "Выполнение серий...";
    }

    [RelayCommand(CanExecute = nameof(CanStop))]
    private void StopSimulation()
    {
        _isRunning = false;
        _isPaused = false;
        PauseButtonText = "Пауза";
        StatusInfo = "Моделирование принудительно остановлено";
        RefreshCommands();
    }

    [RelayCommand]
    private void ResetSimulation()
    {
        StopSimulation();
        CurrentExperimentProgress = 0;
        ExperimentsList.Clear();
        RumorGraph = null;
        EmpProbA = "0.00%";
        EmpProbB = "0.00%";
        AnProbA = "0.00%";
        AnProbB = "0.00%";
        StatusInfo = "Сброс выполнен.";
    }
}