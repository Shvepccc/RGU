using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using Avalonia.Threading;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using LiveChartsCore;
using LiveChartsCore.Defaults;
using LiveChartsCore.SkiaSharpView;
using LiveChartsCore.SkiaSharpView.Painting;
using SkiaSharp;
using tw_hw_9_UI_task_2.Models;

namespace tw_hw_9_UI_task_2.ViewModels;

public partial class MainWindowViewModel : ObservableObject
{
    private readonly VariableStorageService _storageService = new();
    private readonly Random _random = new();

    [ObservableProperty] private double _initialPosition = 0;
    [ObservableProperty] private int _stepsCount = 5;
    [ObservableProperty] private string _movementLawInfo = "Не загружен";
    [ObservableProperty] private string _currentStatusText = "Ожидание запуска...";
    
    private DiscreteRandomVariable? _movementLaw;
    private CancellationTokenSource? _cts;
    [ObservableProperty]
    [NotifyPropertyChangedFor(nameof(IsSimulationNotRunning))]
    private bool _isRunning;
    public bool IsSimulationNotRunning => !IsRunning;
    
    [ObservableProperty] private ObservableCollection<ISeries> _simulationSeries;
    [ObservableProperty] private Axis[] _simXAxes;
    [ObservableProperty] private Axis[] _simYAxes;
    
    [ObservableProperty] private ObservableCollection<ISeries> _distributionSeries;
    [ObservableProperty] private Axis[] _distXAxes;

    public IAsyncRelayCommand LoadLawCommand { get; }
    public IAsyncRelayCommand StartCommand { get; }
    public IRelayCommand StopCommand { get; }

    public MainWindowViewModel()
    {
        LoadLawCommand = new AsyncRelayCommand(LoadLawAsync);
        StartCommand = new AsyncRelayCommand(StartSimulationAsync, () => _movementLaw != null && !_isRunning);
        StopCommand = new RelayCommand(StopSimulation, () => _isRunning);
        
        SimulationSeries = new ObservableCollection<ISeries>
        {
            new ScatterSeries<ObservablePoint>
            {
                Values = new ObservableCollection<ObservablePoint> { new ObservablePoint(0, 0) },
                GeometrySize = 25,
                Fill = new SolidColorPaint(SKColors.Red),
                Name = "Точка"
            }
        };

        SimXAxes = new[] { new Axis { Name = "Координата X", NameTextSize = 14} };
        SimYAxes = new[] { new Axis { IsVisible = false, MinLimit = -1, MaxLimit = 1 } }; 
        
        DistributionSeries = new ObservableCollection<ISeries>
        {
            new ColumnSeries<ObservablePoint>
            {
                Values = new ObservableCollection<ObservablePoint>(),
                Fill = new SolidColorPaint(SKColors.SteelBlue),
                Name = "Вероятность P(X = x)"
            }
        };
        DistXAxes = new[] { new Axis { Name = "Координата X", NameTextSize = 14} };
    }

    private async Task LoadLawAsync()
    {
        try
        {
            CurrentStatusText = "Выбор файла...";
            
            var loadedVar = await _storageService.ImportVariableAsync();
        
            if (loadedVar != null)
            {
                _movementLaw = loadedVar;
                MovementLawInfo = $"Загружено: {_movementLaw.Name}\n" +
                                  $"Значений: {_movementLaw.GetDistribution().Count}\n" +
                                  $"Мат. ожид.: {_movementLaw.GetExpectedValue():F2}";
            
                CurrentStatusText = $"Закон успешно загружен.";
                StartCommand.NotifyCanExecuteChanged();
            }
            else
            {
                CurrentStatusText = "Загрузка отменена пользователем.";
            }
        }
        catch (InvalidOperationException ex)
        {
            CurrentStatusText = $"Ошибка формата: {ex.Message}";
            MovementLawInfo = "Ошибка загрузки";
        }
        catch (Exception ex)
        {
            CurrentStatusText = $"Критическая ошибка: {ex.Message}";
            MovementLawInfo = "Ошибка загрузки";
        }
    }

    private void StopSimulation()
    {
        _cts?.Cancel();
    }

    private async Task StartSimulationAsync()
    {
        if (_movementLaw == null) return;

        IsRunning = true;
        StartCommand.NotifyCanExecuteChanged();
        StopCommand.NotifyCanExecuteChanged();
        _cts = new CancellationTokenSource();
        var token = _cts.Token;

        try
        {
            CurrentStatusText = "Подготовка...";
            
            await CalculateAndPlotFinalDistributionAsync();

            double currentPos = InitialPosition;
            
            UpdateSimulationPoint(currentPos);
            
            SimXAxes[0].MinLimit = currentPos - 5;
            SimXAxes[0].MaxLimit = currentPos + 5;
            
            for (int step = 1; step <= StepsCount; step++)
            {
                if (token.IsCancellationRequested) break;

                CurrentStatusText = $"Шаг {step} из {StepsCount}...";
                
                double jumpValue = SampleRandomValue(_movementLaw);
                
                await AnimateStepAsync(currentPos, currentPos + jumpValue, token);
                
                currentPos += jumpValue;
            }

            if (!token.IsCancellationRequested)
                CurrentStatusText = $"Завершено. Конечная позиция: {currentPos:F2}";
            else
                CurrentStatusText = $"Остановлено на позиции: {currentPos:F2}";
        }
        catch (OperationCanceledException) { }
        finally
        {
            IsRunning = false;
            _cts?.Dispose();
            _cts = null;
            StartCommand.NotifyCanExecuteChanged();
            StopCommand.NotifyCanExecuteChanged();
        }
    }
    
    private async Task AnimateStepAsync(double startPos, double targetPos, CancellationToken token)
    {
        var sw = Stopwatch.StartNew();
        const double durationMs = 1000.0;
        
        while (sw.ElapsedMilliseconds < durationMs)
        {
            if (token.IsCancellationRequested) break;

            double progress = sw.ElapsedMilliseconds / durationMs;
            double interpolatedPos = startPos + (targetPos - startPos) * progress;

            UpdateSimulationPoint(interpolatedPos);
            
            Dispatcher.UIThread.Post(() =>
            {
                if (interpolatedPos > SimXAxes[0].MaxLimit - 2)
                    SimXAxes[0].MaxLimit = interpolatedPos + 5;
                if (interpolatedPos < SimXAxes[0].MinLimit + 2)
                    SimXAxes[0].MinLimit = interpolatedPos - 5;
            });

            await Task.Delay(16, token);
        }
        
        UpdateSimulationPoint(targetPos);
    }

    private void UpdateSimulationPoint(double x)
    {
        Dispatcher.UIThread.Post(() =>
        {
            var scatterSeries = (ScatterSeries<ObservablePoint>)SimulationSeries[0];
            scatterSeries.Values = new ObservableCollection<ObservablePoint> { new ObservablePoint(x, 0) };
        });
    }
    
    private double SampleRandomValue(DiscreteRandomVariable drv)
    {
        double roll = _random.NextDouble();
        double cumulative = 0;
        foreach (var kvp in drv.GetDistribution())
        {
            cumulative += kvp.Value;
            if (roll <= cumulative) return kvp.Key;
        }
        return drv.GetDistribution().Last().Key;
    }
    
    private Task CalculateAndPlotFinalDistributionAsync()
    {
        return Task.Run(() =>
        {
            DiscreteRandomVariable sumDrv = _movementLaw!;
            
            for (int i = 1; i < StepsCount; i++)
            {
                sumDrv = sumDrv + _movementLaw!;
            }
            
            var finalDict = new Dictionary<double, double>();
            foreach (var kvp in sumDrv.GetDistribution())
            {
                double newKey = Math.Round(kvp.Key + InitialPosition, 6); 
                
                if (finalDict.ContainsKey(newKey))
                    finalDict[newKey] += kvp.Value;
                else
                    finalDict[newKey] = kvp.Value;
            }

            var points = finalDict
                .OrderBy(x => x.Key)
                .Select(kvp => new ObservablePoint(kvp.Key, kvp.Value))
                .ToList();

            Dispatcher.UIThread.Post(() =>
            {
                var columnSeries = (ColumnSeries<ObservablePoint>)DistributionSeries[0];
                columnSeries.Values = new ObservableCollection<ObservablePoint>(points);
            });
        });
    }
}