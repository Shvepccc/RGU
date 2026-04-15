using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Numerics;
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
using task_4.Models;
using task_4.Services;

namespace task_4.ViewModels;

public partial class MainWindowViewModel : ObservableObject
{
    private readonly Random _random = new();
    private readonly ConfigService _configService = new();
    
    [ObservableProperty] private double _rho = 1.0;
    [ObservableProperty] private int _m = 100;
    [ObservableProperty] private int _k = 10;
    [ObservableProperty] private string _selectedDistribution = "Равномерное";
    [ObservableProperty] private int _n = 4;
    
    [ObservableProperty] private int _uniformMin = 0;
    [ObservableProperty] private int _uniformMax = 9;
    
    [ObservableProperty] private int _binomialTrials = 10;
    [ObservableProperty] private double _binomialProbability = 0.5;
    
    [ObservableProperty] private double _geometricProbability = 0.3;
    
    [ObservableProperty] private int _triangularLow = 0;
    [ObservableProperty] private int _triangularHigh = 9;
    [ObservableProperty] private int _triangularMode = 4;
    
    // Параметры графика
    [ObservableProperty] private ObservableCollection<ISeries> _series;
    [ObservableProperty] private Axis[] _xAxes;
    [ObservableProperty] private Axis[] _yAxes;
    
    [ObservableProperty] private int _returnedCount;
    [ObservableProperty] private double _returnedProbability;
    [ObservableProperty] private int _currentRun;
    
    private CancellationTokenSource? _cts;
    private bool _isRunning;
    
    public string[] Distributions { get; } = { "Равномерное", "Биномиальное", "Геометрическое", "Треугольное" };
    
    public IAsyncRelayCommand RunCommand { get; }
    public IRelayCommand StopCommand { get; }
    public IAsyncRelayCommand SaveConfigCommand { get; }
    public IAsyncRelayCommand LoadConfigCommand { get; }

    public MainWindowViewModel()
    {
        RunCommand = new AsyncRelayCommand(RunSimulationAsync);
        StopCommand = new RelayCommand(StopSimulation, () => _isRunning);
        SaveConfigCommand = new AsyncRelayCommand(SaveConfigAsync);
        LoadConfigCommand = new AsyncRelayCommand(LoadConfigAsync);
        
        _series = new ObservableCollection<ISeries>
        {
            new LineSeries<ObservablePoint>
            {
                Stroke = new SolidColorPaint(SKColors.Blue) { StrokeThickness = 2 },
                GeometrySize = 0,
                LineSmoothness = 0,
                IsVisibleAtLegend =  false,
                Fill = null,
                Values = new ObservableCollection<ObservablePoint>(),
            }
        };
        
        _xAxes =
        [
            new Axis
            {
                Name = "Re(z)",
                MinLimit = -5,
                MaxLimit = 5,
                Labeler = value => value.ToString("F1"),
                
                SeparatorsPaint = new SolidColorPaint(SKColors.LightGray) { StrokeThickness = (float)0.5 },
                ShowSeparatorLines = true,
                UnitWidth = 1,
                TextSize = 11
            }
        ];
    
        _yAxes =
        [
            new Axis
            {
                Name = "Im(z)",
                MinLimit = -5,
                MaxLimit = 5,
                Labeler = value => value.ToString("F1"),
                
                SeparatorsPaint = new SolidColorPaint(SKColors.LightGray) { StrokeThickness = (float)0.5 },
                ShowSeparatorLines = true,
                UnitWidth = 1,
                TextSize = 11
            }
        ];
    }

    private void ResetChart()
    {
        _series[0] = new LineSeries<ObservablePoint>
        {
            Stroke = new SolidColorPaint(SKColors.Blue) { StrokeThickness = 2 },
            GeometrySize = 0,
            LineSmoothness = 0,
            IsVisibleAtLegend =  false,
            Fill = null,
            Values = new ObservableCollection<ObservablePoint>(),
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
    }

    private async Task LoadConfigAsync()
    {
        var config = await _configService.LoadConfigAsync();
        if (config != null)
        {
            ApplyConfig(config);
        }
    }

    private void ApplyConfig(SimulationConfig config)
    {
        Rho = config.Rho;
        M = config.M;
        K = config.K;
        SelectedDistribution = config.SelectedDistribution;
        N = config.N;
    
        UniformMin = config.UniformMin;
        UniformMax = config.UniformMax;
        BinomialTrials = config.BinomialTrials;
        BinomialProbability = config.BinomialProbability;
        GeometricProbability = config.GeometricProbability;

        TriangularLow = config.TriangularLow;
        TriangularHigh = config.TriangularHigh;
        TriangularMode = config.TriangularMode;
    }

    private SimulationConfig GetCurrentConfig()
    {
        return new SimulationConfig
        {
            Rho = Rho,
            M = M,
            K = K,
            SelectedDistribution = SelectedDistribution,
            N = N,
            UniformMin = UniformMin,
            UniformMax = UniformMax,
            BinomialTrials = BinomialTrials,
            BinomialProbability = BinomialProbability,
            GeometricProbability = GeometricProbability,
            TriangularLow = TriangularLow,
            TriangularHigh = TriangularHigh,
            TriangularMode = TriangularMode
        };
    }

    private async Task RunSimulationAsync()
    {
        _isRunning = true;
        StopCommand.NotifyCanExecuteChanged();
        _cts = new CancellationTokenSource();
        var token = _cts.Token;
        
        await Dispatcher.UIThread.InvokeAsync(() =>
        {
            ResetChart();
            ReturnedCount = 0;
            ReturnedProbability = 0;
            CurrentRun = 0;
        });
        
        int returned = 0;
        
        try
        {
            await Task.Run(async () =>
            {
                for (int run = 1; run <= M; run++)
                {
                    if (token.IsCancellationRequested)
                        break;

                    await Dispatcher.UIThread.InvokeAsync(() => CurrentRun = run);
                    
                    Complex z = Complex.Zero;
                    var trajectory = new List<ObservablePoint> { new(0, 0) };
                    bool returnedToOrigin = false;

                    for (int step = 1; step <= K; step++)
                    {
                        if (token.IsCancellationRequested)
                            break;
                        
                        int xi = GetRandomValueForModulo(N);
                        double angle = (2 * Math.PI / N) * xi;
                        Complex increment = Rho * new Complex(Math.Cos(angle), Math.Sin(angle));
                        
                        z += increment;
                        
                        trajectory.Add(new ObservablePoint(z.Real, z.Imaginary));
                        
                        if (Math.Abs(z.Real) < 1e-10 && Math.Abs(z.Imaginary) < 1e-10)
                        {
                            returnedToOrigin = true;
                        }
                        
                        if (run == M)
                        {
                            await Dispatcher.UIThread.InvokeAsync(() =>
                            {
                                ((LineSeries<ObservablePoint>)_series[0]).Values = new ObservableCollection<ObservablePoint>(trajectory);
                                
                                if (trajectory.Any())
                                {
                                    var minX = trajectory.Min(p => p.X) - 1;
                                    var maxX = trajectory.Max(p => p.X) + 1;
                                    var minY = trajectory.Min(p => p.Y) - 1;
                                    var maxY = trajectory.Max(p => p.Y) + 1;
                                    
                                    _xAxes[0].MinLimit = minX;
                                    _xAxes[0].MaxLimit = maxX;
                                    _yAxes[0].MinLimit = minY;
                                    _yAxes[0].MaxLimit = maxY;
                                }
                            });
                            
                            await Task.Delay(K > 1000 ? 0 : 25);
                        }
                    }

                    if (returnedToOrigin)
                        returned++;

                    await Dispatcher.UIThread.InvokeAsync(() =>
                    {
                        ReturnedCount = returned;
                        ReturnedProbability = (double)returned / run;
                    });
                }
            });
        }
        catch (OperationCanceledException)
        {
        }
        finally
        {
            _isRunning = false;
            _cts?.Dispose();
            _cts = null;
            StopCommand.NotifyCanExecuteChanged();
        }
    }
    
    private int GetRandomValueForModulo(int n)
    {
        if (n <= 0) return 0;
        
        return SelectedDistribution switch
        {
            "Равномерное" => UniformModulo(n),
            "Биномиальное" => BinomialModulo(n),
            "Геометрическое" => GeometricModulo(n),
            "Треугольное" => TriangularModulo(n),
            _ => 0
        };
    }

#region Распределения

    private int UniformModulo(int n)
    {
        return _random.Next(0, n - 1);
    }
    
    private int BinomialModulo(int n)
    {
        int successes = 0;
        for (int i = 0; i < BinomialTrials; i++)
            if (_random.NextDouble() < BinomialProbability)
                successes++;
        
        return successes % n;
    }
    
    private int GeometricModulo(int n)
    {
        int trials = 1;
        while (_random.NextDouble() > GeometricProbability)
            trials++;
        
        return (trials - 1) % n;
    }

    private int TriangularModulo(int n)
    {
        if (n <= 0) return 0;
        
        int low = Math.Clamp(TriangularLow, 0, n - 1);
        int high = Math.Clamp(TriangularHigh, 0, n - 1);
        int mode = Math.Clamp(TriangularMode, 0, n - 1);
        
        if (low >= high)
        {
            return _random.Next(0, n);
        }
        
        mode = Math.Clamp(mode, low, high);
    
        double u = _random.NextDouble();
        double fc = (mode - low) / (double)(high - low);
    
        double value;
        if (u < fc)
        {
            value = low + Math.Sqrt(u * (high - low) * (mode - low));
        }
        else
        {
            value = high - Math.Sqrt((1 - u) * (high - low) * (high - mode));
        }
        int intValue = (int)Math.Round(value);
        return Math.Clamp(intValue, 0, n - 1);
    }
    
    #endregion
}