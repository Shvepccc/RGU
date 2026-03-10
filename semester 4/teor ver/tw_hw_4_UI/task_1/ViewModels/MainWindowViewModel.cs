using System;
using System.Collections.ObjectModel;
using System.Threading.Tasks;
using Avalonia.Threading;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using LiveChartsCore;
using LiveChartsCore.Defaults;
using LiveChartsCore.SkiaSharpView;
using LiveChartsCore.SkiaSharpView.Painting;
using SkiaSharp;

namespace task_1.ViewModels;

public partial class MainWindowViewModel : ViewModelBase
{
    [ObservableProperty] private int _countExperiments = 1;
    [ObservableProperty] private int _b = 20;

    partial void OnBChanged(int value)
    {
        UpdateChartBounds();
        UpdateSections();
        UpdatePersonPosition();
    }

    private void UpdateChartBounds()
    {
        XAxes = new[]
        {
            new Axis
            {
                MinLimit = -5,
                MaxLimit = B * 3,
                MinStep = 1,
                SeparatorsPaint = new SolidColorPaint(SKColors.LightSlateGray) { StrokeThickness = (float)0.5 }
            }
        };
    }

    private void UpdateSections()
    {
        Sections = new ObservableCollection<RectangularSection>
        {
            new RectangularSection
            {
                Xi = 0,
                Xj = B * 3,
                Yi = -30,
                Yj = 0,
                Fill = new SolidColorPaint(new SKColor(100, 150, 200, 80))
            },
            new RectangularSection
            {
                Xi = B - 0.5,
                Xj = B + 0.5,
                Yi = 2,
                Yj = 0,
                Fill = new SolidColorPaint(SKColors.SaddleBrown)
            }
        };
    }

    private void UpdatePersonPosition()
    {
        var points = new ObservableCollection<ObservablePoint> { new ObservablePoint(B, 0) };
        Series[0].Values = points;
    }
    
    [ObservableProperty] private double _p = 0.5;
    [ObservableProperty] private double _q = 0.5;
    [ObservableProperty] private bool _fastMode;
    
    [ObservableProperty] private ObservableCollection<ISeries> _series;
    [ObservableProperty] private ObservableCollection<RectangularSection> _sections;
    [ObservableProperty] private Axis[] _xAxes;
    [ObservableProperty] private Axis[] _yAxes;
    [ObservableProperty] private double _personX = 0;
    [ObservableProperty] private double _personY = 0;
    
    [ObservableProperty] private int _completedExperiments;
    [ObservableProperty] private int _infinityCount;
    [ObservableProperty] private int _cafeCount;
    [ObservableProperty] private int _cliffCount;
    
    public double InfinityPercent => CompletedExperiments > 0 
        ? Math.Round((double)InfinityCount / CompletedExperiments * 100, 2) 
        : 0;
    
    public double CafePercent => CompletedExperiments > 0 
        ? Math.Round((double)CafeCount / CompletedExperiments * 100, 2) 
        : 0;
    
    public double CliffPercent => CompletedExperiments > 0 
        ? Math.Round((double)CliffCount / CompletedExperiments * 100, 2) 
        : 0;
    
    public IAsyncRelayCommand StartExperimentCommand { get; }

    public MainWindowViewModel()
    {
        StartExperimentCommand = new AsyncRelayCommand(StartExperimentAsync);
        
        _sections = new ObservableCollection<RectangularSection>
        {
            new RectangularSection
            {
                Xi = 0,
                Xj = B * 3,
                Yi = -30,
                Yj = 0,
                Fill = new SolidColorPaint(new SKColor(100, 150, 200, 80))
            },
            new RectangularSection
            {
                Xi = B - 0.5,
                Xj = B + 0.5,
                Yi = 2,
                Yj = 0,
                Fill = new SolidColorPaint(SKColors.SaddleBrown)
            }
        };
        
        Series = new ObservableCollection<ISeries>
        {
            new ScatterSeries<double>
            {
                Values = new ObservableCollection<double> { 0 },
                GeometrySize = 10,
                Fill = new SolidColorPaint(SKColors.Red),
                Name = "Человек"
            }
        };
        
        XAxes =
        [
            new Axis
            {
                MinLimit = -5,
                MaxLimit = B*3,
                MinStep = 1,
                SeparatorsPaint = new SolidColorPaint(SKColors.LightSlateGray) { StrokeThickness = (float)0.5}
            }
        ];
        
        YAxes =
        [
            new Axis
            {
                MinLimit = -30,
                MaxLimit = 20,
                MinStep = 2,
                IsVisible = false
            }
        ];
        
        var temp = new ObservableCollection<ObservablePoint> { new ObservablePoint(B, 0) };
        Series[0].Values = temp;
        
        this.PropertyChanged += (_, e) =>
        {
            switch (e.PropertyName)
            {
                case nameof(P) or nameof(Q):
                    UpdateProbabilitiesCollection(e.PropertyName == nameof(P));
                    break;
                case nameof(B):
                    UpdateChartBounds();
                    UpdateSections();
                    UpdatePersonPosition();
                    break;
                case nameof(InfinityCount):
                case nameof(CafeCount):
                case nameof(CliffCount):
                case nameof(CompletedExperiments):
                    OnPropertyChanged(nameof(InfinityPercent));
                    OnPropertyChanged(nameof(CafePercent));
                    OnPropertyChanged(nameof(CliffPercent));
                    break;
            }
        };
    }

    private void UpdateProbabilitiesCollection(bool type)
    {
        if (type)
            Q = 1 - P;
        else
            P = 1 - Q;
    }
    
    private async Task StartExperimentAsync()
    {
        await Task.Run(async () =>
        {
            var random = new Random();
            var points = new ObservableCollection<ObservablePoint>();
            
            await Dispatcher.UIThread.InvokeAsync(() =>
            {
                InfinityCount = 0;
                CafeCount = 0;
                CliffCount = 0;
                CompletedExperiments = 0;
                Series[0].Values = points;
            });
            
            for (var i = 0; i < CountExperiments; i++)
            {
                var currentPosition = B;
                var start = true;
                var experimentCompleted = false;
                
                while (!experimentCompleted)
                {
                    start = false;
                    var prob = random.NextDouble();
                    currentPosition += (prob > P ? 1 : -1);
                
                    var newPoint = new ObservablePoint(currentPosition, 0);
                    if (!FastMode) await Dispatcher.UIThread.InvokeAsync(() =>
                    {
                        points.Clear();
                        points.Add(newPoint);
                    });

                    if (currentPosition > B * 3)
                    {
                        InfinityCount++;
                        experimentCompleted = true;
                    }
                    else if (currentPosition == B && !start)
                    {
                        CafeCount++;
                        experimentCompleted = true;
                    }
                    else if (currentPosition == -1)
                    {
                        if (!FastMode) await Dispatcher.UIThread.InvokeAsync(() =>
                        {
                            CliffCount++;
                            experimentCompleted = true;
                            
                            if (Series[0] is ScatterSeries<double> scatterSeries && !FastMode)
                            {
                                scatterSeries.GeometrySize = 50;
                            }
                            
                            for (var j = 0; j <= 35; j += 5)
                            {
                                Task.Delay(FastMode ? 0 : 200).Wait();
                                newPoint = new ObservablePoint(currentPosition, -j);
                                points.Clear();
                                points.Add(newPoint);
                            }
                            
                            if (Series[0] is ScatterSeries<double> scatterSeries2)
                            {
                                scatterSeries2.GeometrySize = 10;
                            }
                        });
                        else
                        {
                            CliffCount++;
                            experimentCompleted = true;
                        }
                    }

                    if (!experimentCompleted )
                    {
                        Task.Delay(FastMode ? 0 : 500).Wait();
                    }
                }
                
                await Dispatcher.UIThread.InvokeAsync(() =>
                {
                    CompletedExperiments = i + 1;
                });
            }
            
            if (!FastMode) await Dispatcher.UIThread.InvokeAsync(() =>
            {
                points.Clear();
                points.Add(new ObservablePoint(B, 0));
            });
        });
    }
}