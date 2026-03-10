using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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
using task_2.Models;
using task_2.Services;

namespace task_2.ViewModels;

public partial class MainWindowViewModel : ObservableObject
{
    private readonly Random _random = new();
    private readonly ConfigService _configService = new();
    private readonly LogService _logService;
    
    [ObservableProperty] private double _h = 1.0;
    [ObservableProperty] private int _n = 100;
    [ObservableProperty] private double _y = 10.0;
    [ObservableProperty] private int _k = 10;
    [ObservableProperty] private string _selectedDistribution = "Равномерное";
    [ObservableProperty] private bool _useSArray = false;
    [ObservableProperty] private string _sArrayText = "-2, -1, 0, 1, 2";
    private List<double> _sArray = [-2, -1, 0, 1, 2];
    
    // Параметры распределений
    [ObservableProperty] private double _uniformMin = -2.0;
    [ObservableProperty] private double _uniformMax = 2.0;
    [ObservableProperty] private int _binomialTrials = 10;
    [ObservableProperty] private double _binomialProbability = 0.5;
    [ObservableProperty] private double _geometricProbability = 0.3;
    [ObservableProperty] private int _geometricOffset = 1;
    [ObservableProperty] private int _triangularLow = -3;
    [ObservableProperty] private int _triangularHigh = 3;
    [ObservableProperty] private int _triangularMode = 0;
    
    // Параметры графика
    [ObservableProperty] private ObservableCollection<ISeries> _series;
    [ObservableProperty] private Axis[] _xAxes;
    [ObservableProperty] private Axis[] _yAxes;
    private bool nIsChanged = false;
    
    // Логи и результаты
    [ObservableProperty] private ObservableCollection<LogEntryViewModel> _logs = [];
    [ObservableProperty] private LogEntryViewModel? _selectedLog;
    [ObservableProperty] private int _crossingCount;
    [ObservableProperty] private double _crossingProbability;
    
    private CancellationTokenSource? _cts;
    private bool _isRunning;
    
    public string[] Distributions { get; } = { "Равномерное", "Биномиальное", "Геометрическое", "Треугольное" };
    
    public IAsyncRelayCommand RunCommand { get; }
    public IRelayCommand StopCommand { get; }
    public IAsyncRelayCommand SaveConfigCommand { get; }
    public IAsyncRelayCommand LoadConfigCommand { get; }
    public IRelayCommand ClearLogsCommand { get; }
    public IRelayCommand<LogEntryViewModel> ShowLogCommand { get; }

    public MainWindowViewModel()
    {
        _logService = new LogService(K);
        
        RunCommand = new AsyncRelayCommand(RunSimulationAsync);
        StopCommand = new RelayCommand(StopSimulation, () => _isRunning);
        SaveConfigCommand = new AsyncRelayCommand(SaveConfigAsync);
        LoadConfigCommand = new AsyncRelayCommand(LoadConfigAsync);
        ClearLogsCommand = new RelayCommand(ClearLogs);
        ShowLogCommand = new RelayCommand<LogEntryViewModel>(ShowLog);
        
        _series = new ObservableCollection<ISeries>
        {
            new LineSeries<ObservablePoint>
            {
                Stroke = new SolidColorPaint(SKColors.Blue) { StrokeThickness = 2 },
                GeometrySize = 0,
                LineSmoothness = 0,
                Fill = null,
                Name = "Текущая симуляция",
                Values = new ObservableCollection<ObservablePoint>()
            },
            new LineSeries<ObservablePoint>
            {
                Stroke = new SolidColorPaint(SKColors.Red) { StrokeThickness = 2},
                    // PathEffect = new SKPathEffect { D = SKPathEffect.CreateDash(new float[] { 5, 5 }, 0) } },
                GeometrySize = 0,
                LineSmoothness = 0,
                Fill = null,
                Name = "Из лога",
                Values = new ObservableCollection<ObservablePoint>(),
                IsVisible = false
            }
        };
        
        _xAxes =
        [
            new Axis
            {
                MinLimit = -5,
                MaxLimit = 105,
                Labeler = value => value.ToString("F0")
            }
        ];
        
        _yAxes =
        [
            new Axis
            {
                MinLimit = -30,
                MaxLimit = 30,
                Labeler = value => value.ToString("F0")
            }
        ];
        
        this.PropertyChanged += (s, e) =>
        {
            if (e.PropertyName == nameof(K))
                _logService.SetMaxLogs(K);
            else if (e.PropertyName == nameof(SArrayText))
                ParseSArray();
        };
    }
    
    private void ParseSArray()
    {
        try
        {
            _sArray = SArrayText.Split(',', StringSplitOptions.RemoveEmptyEntries)
                .Select(x => double.Parse(x.Trim()))
                .ToList();
        }
        catch
        {
            SArrayText = string.Join(", ", _sArray);
        }
    }

    private void resetChart()
    {
        _series[0] = new LineSeries<ObservablePoint>
            {
                Stroke = new SolidColorPaint(SKColors.Blue) { StrokeThickness = 2 },
                GeometrySize = 0,
                LineSmoothness = 0,
                Fill = null,
                Name = "Текущая симуляция",
                Values = new ObservableCollection<ObservablePoint>()
            };
        _series[1] = new LineSeries<ObservablePoint>
            {
                Stroke = new SolidColorPaint(SKColors.Red) { StrokeThickness = 2 },
                // PathEffect = new SKPathEffect { D = SKPathEffect.CreateDash(new float[] { 5, 5 }, 0) } },
                GeometrySize = 0,
                LineSmoothness = 0,
                Fill = null,
                Name = "Из лога",
                Values = new ObservableCollection<ObservablePoint>(),
                IsVisible = false
            };
    }

    private void StopSimulation()
    {
        _cts?.Cancel();
        StopCommand.NotifyCanExecuteChanged();
    }
    
    private async Task SaveConfigAsync()
    {
        var config = GetCurrentConfig();
        bool success = await _configService.SaveConfigAsync(config);
    
        if (success)
        {
            _logService.SetMaxLogs(K);
        }
    }

    private async Task LoadConfigAsync()
    {
        var config = await _configService.LoadConfigAsync();
        if (config != null)
        {
            ApplyConfig(config);
            _logService.SetMaxLogs(K);
        }
    }

    private void ApplyConfig(SimulationConfig config)
    {
        H = config.H;
        N = config.N;
        Y = config.Y;
        K = config.K;
        SelectedDistribution = config.SelectedDistribution;
        UseSArray = config.UseSArray;
        _sArray = config.SArray ?? [-2, -1, 0, 1, 2];
        SArrayText = string.Join(", ", _sArray);
    
        UniformMin = config.UniformMin;
        UniformMax = config.UniformMax;
        BinomialTrials = config.BinomialTrials;
        BinomialProbability = config.BinomialProbability;
        GeometricProbability = config.GeometricProbability;
        GeometricOffset = config.GeometricOffset;
        TriangularLow = config.TriangularLow;
        TriangularHigh = config.TriangularHigh;
        TriangularMode = config.TriangularMode;
    }

    private SimulationConfig GetCurrentConfig()
    {
        ParseSArray();
    
        return new SimulationConfig
        {
            H = H,
            N = N,
            Y = Y,
            K = K,
            SelectedDistribution = SelectedDistribution,
            UseSArray = UseSArray,
            SArray = _sArray,
            UniformMin = UniformMin,
            UniformMax = UniformMax,
            BinomialTrials = BinomialTrials,
            BinomialProbability = BinomialProbability,
            GeometricProbability = GeometricProbability,
            GeometricOffset = GeometricOffset,
            TriangularLow = TriangularLow,
            TriangularHigh = TriangularHigh,
            TriangularMode = TriangularMode
        };
    }

    private void ClearLogs()
    {
        _logService.Clear();
        UpdateLogs();
        if (_series.Count > 1)
            _series[1].IsVisible = false;
    }

    private void ShowLog(LogEntryViewModel? log)
    {
        if (log?.Result == null) return;
        
        var points = new ObservableCollection<ObservablePoint>();
        foreach (var p in log.Result.Trajectory)
            points.Add(new ObservablePoint(p.X, p.Y));
        
        if (_series.Count > 1)
        {
            ((LineSeries<ObservablePoint>)_series[1]).Values = points;
            _series[1].IsVisible = true;
        }
    }

    private void UpdateLogs()
    {
        Logs.Clear();
        foreach (var result in _logService.GetAll())
        {
            Logs.Add(new LogEntryViewModel(result));
        }
    }

    private async Task RunSimulationAsync()
    {
        _isRunning = true;
        StopCommand.NotifyCanExecuteChanged();
        _cts = new CancellationTokenSource();
        var token = _cts.Token;
        
        await Dispatcher.UIThread.InvokeAsync(() =>
        {
            resetChart();
            if (_series.Count > 1)
                _series[1].IsVisible = false;
        });
        
        var points = new List<SimulationResult.Point>();
        int crosses = 0;
        try
        {
            await Task.Run(async () =>
            {
                double currentY = Y;
                points.Add(new SimulationResult.Point(0, currentY));
                bool wasNegative = currentY < 0;

                for (int i = 1; i <= N; i++)
                {
                    if (token.IsCancellationRequested)
                    {
                        break;
                    }

                    double step = GetRandomValue();
                    currentY += step;
                    points.Add(new SimulationResult.Point(i * H, currentY));

                    if (currentY < 0 && !wasNegative)
                    {
                        crosses++;
                        wasNegative = true;
                    }
                    else if (currentY >= 0 && wasNegative)
                    {
                        wasNegative = false;
                    }

                    await Dispatcher.UIThread.InvokeAsync(() =>
                    {
                        var obsPoints = new ObservableCollection<ObservablePoint>();
                        foreach (var p in points)
                            obsPoints.Add(new ObservablePoint(p.X, p.Y));

                        ((LineSeries<ObservablePoint>)_series[0]).Values = obsPoints;

                        if (points.Any())
                        {
                            var maxX = points.Max(p => p.X);
                            var minY = points.Min(p => p.Y) - 5;
                            var maxY = points.Max(p => p.Y) + 5;

                            _xAxes[0].MaxLimit = maxX + 5;
                            _yAxes[0].MinLimit = minY;
                            _yAxes[0].MaxLimit = maxY;
                        }
                    });

                    await Task.Delay(20);
                }
            });
        }
        catch (OperationCanceledException)
        {
        }
        finally
        {
            var result = new SimulationResult
            {
                Timestamp = DateTime.Now,
                Config = GetCurrentConfig(),
                Trajectory = points,
                CrossingCount = crosses,
                CrossingProbability = N > 0 ? (double)crosses / N : 0
            };

            await Dispatcher.UIThread.InvokeAsync(() =>
            {
                _logService.Add(result);
                UpdateLogs();

                CrossingCount = crosses;
                CrossingProbability = N > 0 ? (double)crosses / N : 0;
            });
            
            _isRunning = false;
            _cts?.Dispose();
            _cts = null;
            StopCommand.NotifyCanExecuteChanged();
        }
        
    }

private double GetRandomValue()
{
    if (UseSArray && _sArray.Any())
    {
        int index = GetIndexFromDistribution();
        return _sArray[Math.Clamp(index, 0, _sArray.Count - 1)];
    }
    
    return SelectedDistribution switch
    {
        "Равномерное" => UniformMin + _random.NextDouble() * (UniformMax - UniformMin),
        "Биномиальное" => Binomial(),
        "Геометрическое" => Geometric(),
        "Треугольное" => Triangular(),
        _ => 0
    };
}

private int GetIndexFromDistribution()
{
    return SelectedDistribution switch
    {
        "Равномерное" => _random.Next(0, _sArray.Count),
        "Биномиальное" => GetBinomialIndex(),
        "Геометрическое" => GetGeometricIndex(),
        "Треугольное" => GetTriangularIndex(),
        _ => 0
    };
}

#region Распределения

private int GetBinomialIndex()
{
    int successes = 0;
    for (int i = 0; i < BinomialTrials; i++)
        if (_random.NextDouble() < BinomialProbability)
            successes++;
    
    // Нормализуем к индексу массива
    double normalized = successes / (double)BinomialTrials;
    return (int)(normalized * (_sArray.Count - 1));
}

private int GetGeometricIndex()
{
    int trials = 1;
    while (_random.NextDouble() > GeometricProbability)
        trials++;
    
    // Ограничиваем максимальное значение
    trials = Math.Min(trials - GeometricOffset, _sArray.Count - 1);
    return Math.Max(0, trials);
}

private int GetTriangularIndex()
{
    double u = _random.NextDouble();
    double fc = (TriangularMode - TriangularLow) / (double)(TriangularHigh - TriangularLow);
    
    double value;
    if (u < fc)
        value = TriangularLow + Math.Sqrt(u * (TriangularHigh - TriangularLow) * (TriangularMode - TriangularLow));
    else
        value = TriangularHigh - Math.Sqrt((1 - u) * (TriangularHigh - TriangularLow) * (TriangularHigh - TriangularMode));
    
    // Нормализуем значение к индексу массива
    double normalized = (value - TriangularLow) / (TriangularHigh - TriangularLow);
    return (int)(normalized * (_sArray.Count - 1));
}



    private double Binomial()
    {
        int successes = 0;
        for (int i = 0; i < BinomialTrials; i++)
            if (_random.NextDouble() < BinomialProbability)
                successes++;
        return successes - BinomialTrials / 2.0;
    }

    private double Geometric()
    {
        int trials = 1;
        while (_random.NextDouble() > GeometricProbability)
            trials++;
        return trials - GeometricOffset;
    }

    private double Triangular()
    {
        double u = _random.NextDouble();
        double fc = (TriangularMode - TriangularLow) / (double)(TriangularHigh - TriangularLow);
        
        if (u < fc)
            return TriangularLow + Math.Sqrt(u * (TriangularHigh - TriangularLow) * (TriangularMode - TriangularLow));
        else
            return TriangularHigh - Math.Sqrt((1 - u) * (TriangularHigh - TriangularLow) * (TriangularHigh - TriangularMode));
    }
    
#endregion
}

public partial class LogEntryViewModel : ObservableObject
{
    [ObservableProperty] private SimulationResult _result;
    public string DisplayText => $"{Result.Timestamp:HH:mm:ss} | l={Result.CrossingProbability:F3} | {Result.Config.SelectedDistribution}";
    
    public LogEntryViewModel(SimulationResult result) => _result = result;
}