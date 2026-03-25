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
using tw_hw_6_UI.Models;

namespace tw_hw_6_UI.ViewModels;

public partial class MainWindowViewModel : ObservableObject
{
    private readonly Random _random = new();

    [ObservableProperty] private double _needleLength = 0.8;
    [ObservableProperty] private double _lineSpacing = 1.0;
    [ObservableProperty] private int _throwCount = 1000;
    [ObservableProperty] private int _currentThrow;
    [ObservableProperty] private bool _showAllThrows = false;
    [ObservableProperty] private bool _accelerate = false;

    [ObservableProperty] private int _crossCount;
    [ObservableProperty] private double _experimentalProbability;
    [ObservableProperty] private double _theoreticalProbability;
    [ObservableProperty] private double _error;

    [ObservableProperty] private ObservableCollection<ISeries> _series = new();
    [ObservableProperty] private Axis[] _xAxes;
    [ObservableProperty] private Axis[] _yAxes;

    private List<NeedleThrow> _throws = new();
    private CancellationTokenSource? _cts;
    private bool _isRunning;

    public IAsyncRelayCommand RunCommand { get; }
    public IRelayCommand StopCommand { get; }
    public IRelayCommand ClearCommand { get; }

    public MainWindowViewModel()
    {
        RunCommand = new AsyncRelayCommand(RunSimulationAsync);
        StopCommand = new RelayCommand(StopSimulation, () => _isRunning);
        ClearCommand = new RelayCommand(ClearSimulation);

        InitializeCharts();
        CalculateTheoreticalProbability();
    }

    private void InitializeCharts()
    {
        _xAxes = new[]
        {
            new Axis
            {
                Name = "X",
                MinLimit = -0.5,
                MaxLimit = 1.5,
                Labeler = value => value.ToString("F2"),
                SeparatorsPaint = new SolidColorPaint(SKColors.LightGray) { StrokeThickness = 0.5f }
            }
        };

        _yAxes = new[]
        {
            new Axis
            {
                Name = "Y",
                MinLimit = -0.2,
                MaxLimit = 1.2,
                Labeler = value => value.ToString("F2"),
                SeparatorsPaint = new SolidColorPaint(SKColors.LightGray) { StrokeThickness = 0.5f }
            }
        };

        UpdateAxesLimits();
        UpdateLines();
    }

    private void UpdateAxesLimits()
    {
        if (_xAxes != null && _xAxes.Length > 0)
        {
            _xAxes[0].MinLimit = -0.5 * LineSpacing;
            _xAxes[0].MaxLimit = 1.5 * LineSpacing;
            OnPropertyChanged(nameof(XAxes));
        }
        
        if (_yAxes != null && _yAxes.Length > 0)
        {
            _yAxes[0].MinLimit = -0.2 * LineSpacing;
            _yAxes[0].MaxLimit = 1.2 * LineSpacing;
            OnPropertyChanged(nameof(YAxes));
        }
    }

    private void UpdateLines()
    {
        var nonLines = _series.Where(s => s.Name != "Прямые").ToList();
        _series.Clear();
        
        var xMin = -0.5 * LineSpacing;
        var xMax = 1.5 * LineSpacing;

        var bottomLine = new LineSeries<ObservablePoint>
        {
            Name = "Прямые",
            Stroke = new SolidColorPaint(SKColors.Red) { StrokeThickness = 2 },
            GeometrySize = 0,
            LineSmoothness = 0,
            Values = new ObservableCollection<ObservablePoint>
            {
                new ObservablePoint(xMin, 0),
                new ObservablePoint(xMax, 0)
            },
            IsVisibleAtLegend = false
        };
        _series.Add(bottomLine);

        var topLine = new LineSeries<ObservablePoint>
        {
            Name = "Прямые",
            Stroke = new SolidColorPaint(SKColors.Red) { StrokeThickness = 2 },
            GeometrySize = 0,
            LineSmoothness = 0,
            Values = new ObservableCollection<ObservablePoint>
            {
                new ObservablePoint(xMin, LineSpacing),
                new ObservablePoint(xMax, LineSpacing)
            },
            IsVisibleAtLegend = false
        };
        _series.Add(topLine);
        
        foreach (var series in nonLines)
        {
            _series.Add(series);
        }
    }

    private void CalculateTheoreticalProbability()
    {
        TheoreticalProbability = (2 * NeedleLength) / (Math.PI * LineSpacing);
        TheoreticalProbability = Math.Min(1.0, Math.Max(0.0, TheoreticalProbability));
        OnPropertyChanged(nameof(TheoreticalProbability));
    }

    partial void OnNeedleLengthChanged(double value)
    {
        if (value > LineSpacing)
            NeedleLength = LineSpacing;
        CalculateTheoreticalProbability();
    }

    partial void OnLineSpacingChanged(double value)
    {
        if (NeedleLength > value)
            NeedleLength = value;
        CalculateTheoreticalProbability();
        UpdateAxesLimits();
        UpdateLines();
        if (!_isRunning && _throws.Any())
            UpdateThrowsDisplay();
    }

    private bool DoesNeedleCrossLine(double distanceToNearestLine, double angle)
    {
        double halfProjection = (NeedleLength / 2) * Math.Sin(angle);
        return distanceToNearestLine <= halfProjection;
    }
    
    private async Task RunSimulationAsync()
{
    _isRunning = true;
    StopCommand.NotifyCanExecuteChanged();
    _cts = new CancellationTokenSource();
    var token = _cts.Token;

    // Сброс данных перед началом
    _throws.Clear();
    CrossCount = 0;
    CurrentThrow = 0;
    ExperimentalProbability = 0;
    Error = 0;
    UpdateThrowsDisplay();

    try
    {
        await Task.Run(async () =>
        {
            int localCrossCount = 0;
            
            for (int i = 1; i <= ThrowCount; i++)
            {
                if (token.IsCancellationRequested) break;

                double distanceToNearestLine = _random.NextDouble() * (LineSpacing / 2);
                bool nearestIsBottom = _random.Next(2) == 0;
                double centerX = _random.NextDouble() * LineSpacing;
                double angle = _random.NextDouble() * (Math.PI / 2);
                
                bool crosses = DoesNeedleCrossLine(distanceToNearestLine, angle);
                if (crosses) localCrossCount++;

                var needleThrow = new NeedleThrow {
                    CenterX = centerX,
                    DistanceToNearestLine = distanceToNearestLine,
                    NearestIsBottom = nearestIsBottom,
                    Angle = angle,
                    CrossesLine = crosses
                };
                _throws.Add(needleThrow);
                
                bool shouldUpdateUI = !Accelerate || (i % 50 == 0) || (i == ThrowCount);

                if (shouldUpdateUI)
                {
                    await Dispatcher.UIThread.InvokeAsync(() =>
                    {
                        CrossCount = localCrossCount;
                        CurrentThrow = i;
                        ExperimentalProbability = (double)CrossCount / i;
                        Error = TheoreticalProbability > 0 
                            ? Math.Abs(ExperimentalProbability - TheoreticalProbability)
                            : 0;

                        UpdateThrowsDisplay();
                    });
                }

                if (!Accelerate)
                {
                    await Task.Delay(700, token);
                }
            }
        }, token);
    }
    catch (OperationCanceledException) { }
    finally
    {
        _isRunning = false;
        await Dispatcher.UIThread.InvokeAsync(() => StopCommand.NotifyCanExecuteChanged());
    }
}

    private void UpdateThrowsDisplay()
    {
        var lines = _series.Where(s => s.Name == "Прямые").ToList();
        _series.Clear();
        foreach (var line in lines)
            _series.Add(line);

        if (ShowAllThrows && _throws.Any())
        {
            var normalPoints = new ObservableCollection<ObservablePoint>();
            var crossPoints = new ObservableCollection<ObservablePoint>();

            foreach (var t in _throws)
            {
                double centerY = t.NearestIsBottom ? t.DistanceToNearestLine : LineSpacing - t.DistanceToNearestLine;
                var point = new ObservablePoint(t.CenterX, centerY);
                if (t.CrossesLine)
                    crossPoints.Add(point);
                else
                    normalPoints.Add(point);
            }

            var normalSeries = new ScatterSeries<ObservablePoint>
            {
                Name = "Центры игл (не пересекающие)",
                Stroke = null,
                Fill = new SolidColorPaint(SKColors.Gray),
                GeometrySize = 5,
                Values = normalPoints
            };
            _series.Add(normalSeries);

            var crossSeries = new ScatterSeries<ObservablePoint>
            {
                Name = "Центры игл (пересекающие)",
                Stroke = null,
                Fill = new SolidColorPaint(SKColors.Green),
                GeometrySize = 7,
                Values = crossPoints
            };
            _series.Add(crossSeries);
        }
        else if (_throws.Any())
        {
            var last = _throws.Last();
            
            double centerY = last.NearestIsBottom ? last.DistanceToNearestLine : LineSpacing - last.DistanceToNearestLine;
            
            var needlePoints = GetNeedlePoints(last, centerY);
            var needleSeries = new LineSeries<ObservablePoint>
            {
                Name = "Текущая игла",
                Stroke = new SolidColorPaint(last.CrossesLine ? SKColors.Green : SKColors.Blue) { StrokeThickness = 3 },
                GeometrySize = 0,
                LineSmoothness = 0,
                Values = needlePoints,
                IsVisibleAtLegend = false
            };
            _series.Add(needleSeries);
            
            var centerPoints = new ObservableCollection<ObservablePoint>
            {
                new ObservablePoint(last.CenterX, centerY)
            };
            var centerSeries = new ScatterSeries<ObservablePoint>
            {
                Name = "Центр иглы",
                Stroke = null,
                Fill = new SolidColorPaint(SKColors.Red),
                GeometrySize = 8,
                Values = centerPoints,
                IsVisibleAtLegend = false
            };
            _series.Add(centerSeries);
        }
    }

    private ObservableCollection<ObservablePoint> GetNeedlePoints(NeedleThrow needle, double centerY)
    {
        var points = new ObservableCollection<ObservablePoint>();
        double halfLength = NeedleLength / 2;
        
        double x1 = needle.CenterX - halfLength * Math.Cos(needle.Angle);
        double y1 = centerY - halfLength * Math.Sin(needle.Angle);
        double x2 = needle.CenterX + halfLength * Math.Cos(needle.Angle);
        double y2 = centerY + halfLength * Math.Sin(needle.Angle);
        
        points.Add(new ObservablePoint(x1, y1));
        points.Add(new ObservablePoint(x2, y2));
        
        return points;
    }

    private void StopSimulation() => _cts?.Cancel();

    private void ClearSimulation()
    {
        if (_isRunning)
        {
            StopSimulation();
            return;
        }

        _throws.Clear();
        CrossCount = 0;
        CurrentThrow = 0;
        ExperimentalProbability = 0;
        Error = 0;
        UpdateThrowsDisplay();
    }

    partial void OnShowAllThrowsChanged(bool value)
    {
        if (!_isRunning && _throws.Any())
            UpdateThrowsDisplay();
    }
}