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
using task_5.Models;
using task_5.Services;

namespace task_5.ViewModels;

public partial class MainWindowViewModel : ObservableObject
{
    private readonly Random _random = new();
    private readonly ConfigService _configService = new();
    
    // Основные параметры
    [ObservableProperty] private double _h = 1.0;
    [ObservableProperty] private double _m = 10.0;
    [ObservableProperty] private int _tau = 1;
    [ObservableProperty] private int _nTau = 5;
    [ObservableProperty] private int _generationCount = 50;
    [ObservableProperty] private string _selectedDistribution = "Равномерное";
    private int N = 0;
    
    // Параметры распределений
    [ObservableProperty] private double _uniformMin = 0;
    [ObservableProperty] private double _uniformMax = 5;
    [ObservableProperty] private int _binomialTrials = 10;
    [ObservableProperty] private double _binomialProbability = 0.5;
    [ObservableProperty] private double _geometricProbability = 0.3;
    [ObservableProperty] private int _geometricMax = 5;
    [ObservableProperty] private int _triangularLow = 0;
    [ObservableProperty] private int _triangularHigh = 5;
    [ObservableProperty] private int _triangularMode = 2;
    
    // Параметры графика
    [ObservableProperty] private ObservableCollection<ISeries> _series;
    [ObservableProperty] private Axis[] _xAxes;
    [ObservableProperty] private Axis[] _yAxes;
    
    // Результаты
    [ObservableProperty] private ObservableCollection<FigureViewModel> _generatedFigures = [];
    [ObservableProperty] private FigureViewModel? _selectedFigure;
    [ObservableProperty] private int _strictlyIncreasingCount;
    [ObservableProperty] private double _strictlyIncreasingProbability;
    
    // Пагинация
    [ObservableProperty] private int _currentPage = 1;
    [ObservableProperty] private int _pageSize = 10;
    [ObservableProperty] private int _totalPages = 1;
    
    private CancellationTokenSource? _cts;
    private bool _isRunning;
    private List<SimulationResult> _allResults = [];
    
    public string[] Distributions { get; } = { "Равномерное", "Биномиальное", "Геометрическое", "Треугольное" };
    
    public IAsyncRelayCommand GenerateCommand { get; }
    public IRelayCommand StopCommand { get; }
    public IAsyncRelayCommand SaveConfigCommand { get; }
    public IAsyncRelayCommand LoadConfigCommand { get; }
    public IRelayCommand PreviousPageCommand { get; }
    public IRelayCommand NextPageCommand { get; }
    public IRelayCommand<FigureViewModel> ShowFigureCommand { get; }
    
    public bool CanGoPrevious => CurrentPage > 1;
    public bool CanGoNext => CurrentPage < TotalPages;
    public string CurrentPageText => $"{CurrentPage} / {TotalPages}";
    public string PageInfoText => $"Всего: {_allResults.Count} фигур";

public MainWindowViewModel()
{
    GenerateCommand = new AsyncRelayCommand(GenerateAsync);
    StopCommand = new RelayCommand(StopGeneration, () => _isRunning);
    SaveConfigCommand = new AsyncRelayCommand(SaveConfigAsync);
    LoadConfigCommand = new AsyncRelayCommand(LoadConfigAsync);
    PreviousPageCommand = new RelayCommand(PreviousPage, () => CanGoPrevious);
    NextPageCommand = new RelayCommand(NextPage, () => CanGoNext);
    ShowFigureCommand = new RelayCommand<FigureViewModel>(ShowFigure);
    
    N = (int)(M / H);
    
    _series = new ObservableCollection<ISeries>
    {
        new ColumnSeries<double>
        {
            Stroke = new SolidColorPaint(SKColors.Blue) { StrokeThickness = 2 },
            Fill = new SolidColorPaint(SKColors.Blue.WithAlpha(100)),
            Name = "Значения ξ",
            IsVisibleAtLegend =  false,
            Values = new ObservableCollection<double>(),
            DataPadding = new LiveChartsCore.Drawing.LvcPoint(0, 0),
            MaxBarWidth = double.PositiveInfinity,
        }
    };
    
    _xAxes =
    [
        new Axis
        {
            Labeler = value =>
            {
                // Проверяем, является ли значение кратным h с небольшой погрешностью
                double remainder = Math.Abs(value % H);
                if (remainder < 0.01 || Math.Abs(remainder - H) < 0.01)
                {
                    // Округляем для отображения с 1 знаком после запятой
                    return Math.Round(value, 1).ToString();
                }
                return "";
            },
            Name = "Номер отрезка i",
            NameTextSize = 13,
            MinLimit = 0,
            MaxLimit = N,
            UnitWidth = 1,
            MinStep = 1
        }
    ];
    
    _yAxes =
    [
        new Axis
        {
            MinLimit = -0.5,
            MaxLimit = NTau + 0.5,
            Labeler = value => value.ToString("F0"),
            Name = "Значение ξ",
            NameTextSize = 13
        }
    ];
    
    this.PropertyChanged += (s, e) =>
    {
        switch (e.PropertyName)
        {
            case nameof(M) or nameof(H):
                N = (int)(M / H);
                _xAxes[0].MaxLimit = M;
                _xAxes[0].MinStep = H;
                _xAxes[0].UnitWidth = H;
                OnPropertyChanged(nameof(XAxes));
                break;
            case nameof(NTau):
                _yAxes[0].MaxLimit = NTau + 0.5;
                OnPropertyChanged(nameof(YAxes));
                break;
            case nameof(CurrentPage):
            {
                if (CurrentPage < 1)
                    CurrentPage = 1;
                else if (CurrentPage > TotalPages && TotalPages > 0)
                    CurrentPage = TotalPages;
                else
                    UpdateCurrentPage();
                
                OnPropertyChanged(nameof(CanGoPrevious));
                OnPropertyChanged(nameof(CanGoNext));
                break;
            }
            case nameof(TotalPages):
                OnPropertyChanged(nameof(CanGoPrevious));
                OnPropertyChanged(nameof(CanGoNext));
                OnPropertyChanged(nameof(CurrentPageText));
                break;
        }
    };
}

    private void StopGeneration()
    {
        _cts?.Cancel();
        StopCommand.NotifyCanExecuteChanged();
    }

    private async Task SaveConfigAsync()
    {
        var config = GetCurrentConfig();
        await _configService.SaveConfigAsync(config);
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
        H = config.H;
        M = config.M;
        Tau = config.Tau;
        NTau = config.NTau;
        SelectedDistribution = config.SelectedDistribution;
        GenerationCount = config.GenerationCount;
        
        UniformMin = config.UniformMin;
        UniformMax = config.UniformMax;
        BinomialTrials = config.BinomialTrials;
        BinomialProbability = config.BinomialProbability;
        GeometricProbability = config.GeometricProbability;
        GeometricMax = config.GeometricMax;
        TriangularLow = config.TriangularLow;
        TriangularHigh = config.TriangularHigh;
        TriangularMode = config.TriangularMode;
    }

    private SimulationConfig GetCurrentConfig()
    {
        return new SimulationConfig
        {
            H = H,
            M = M,
            Tau = Tau,
            NTau = NTau,
            SelectedDistribution = SelectedDistribution,
            GenerationCount = GenerationCount,
            UniformMin = UniformMin,
            UniformMax = UniformMax,
            BinomialTrials = BinomialTrials,
            BinomialProbability = BinomialProbability,
            GeometricProbability = GeometricProbability,
            GeometricMax = GeometricMax,
            TriangularLow = TriangularLow,
            TriangularHigh = TriangularHigh,
            TriangularMode = TriangularMode
        };
    }

    private void PreviousPage()
    {
        if (CanGoPrevious)
        {
            CurrentPage--;
        }
    }

    private void NextPage()
    {
        if (CanGoNext)
        {
            CurrentPage++;
        }
    }

    private void UpdateCurrentPage()
    {
        if (_allResults.Count == 0)
        {
            GeneratedFigures.Clear();
            return;
        }
        
        int startIndex = (CurrentPage - 1) * PageSize;
        var pageResults = _allResults
            .Skip(startIndex)
            .Take(PageSize)
            .ToList();
        
        GeneratedFigures.Clear();
        foreach (var result in pageResults)
        {
            GeneratedFigures.Add(new FigureViewModel(result));
        }
        
        OnPropertyChanged(nameof(CurrentPageText));
        OnPropertyChanged(nameof(PageInfoText));
    }

    private void ShowFigure(FigureViewModel? figure)
    {
        if (figure?.Result == null) return;
        
        var values = figure.Result.StepValues.Select(p => p.Y).ToList();
        
        if (_series.Count > 0)
        {
            ((ColumnSeries<double>)_series[0]).Values = values;
        }
    }

    private async Task GenerateAsync()
    {
        _isRunning = true;
        StopCommand.NotifyCanExecuteChanged();
        _cts = new CancellationTokenSource();
        var token = _cts.Token;
        
        _allResults.Clear();
        CurrentPage = 1;
        int strictlyIncreasingCount = 0;
        
        await Dispatcher.UIThread.InvokeAsync(() =>
        {
            ((ColumnSeries<double>)_series[0]).Values = new ObservableCollection<double>();
            GeneratedFigures.Clear();
            TotalPages = 1;
        });
        
        try
        {
            await Task.Run(async () =>
            {
                for (int gen = 0; gen < GenerationCount; gen++)
                {
                    if (token.IsCancellationRequested)
                        break;
                    
                    // Генерируем значения для каждого отрезка
                    var stepValues = new List<SimulationResult.StepValue>();
                    bool isStrictlyIncreasing = true;
                    double previousValue = -1;
                    
                    for (int i = 0; i < N; i++)
                    {
                        double x = i * H;
                        double value = GetRandomValue();
                        
                        stepValues.Add(new SimulationResult.StepValue(x, value));
                        
                        // Проверяем строгое возрастание
                        if (value <= previousValue)
                            isStrictlyIncreasing = false;
                        
                        previousValue = value;
                    }
                    
                    var result = new SimulationResult
                    {
                        Timestamp = DateTime.Now,
                        Config = GetCurrentConfig(),
                        StepValues = stepValues,
                        IsStrictlyIncreasing = isStrictlyIncreasing,
                        number = gen+1
                    };
                    
                    lock (_allResults)
                    {
                        _allResults.Add(result);
                        if (isStrictlyIncreasing)
                            strictlyIncreasingCount++;
                    }
                    
                    // Обновляем UI каждые 5 генераций
                    if (gen == GenerationCount - 1) //gen % 5 == 0 || 
                    {
                        await Dispatcher.UIThread.InvokeAsync(() =>
                        {
                            StrictlyIncreasingCount = strictlyIncreasingCount;
                            StrictlyIncreasingProbability = _allResults.Count > 0 
                                ? (double)strictlyIncreasingCount / _allResults.Count : 0;
                            
                            TotalPages = (int)Math.Ceiling(_allResults.Count / (double)PageSize);
                            if (TotalPages == 0) TotalPages = 1;
                            
                            if (CurrentPage > TotalPages)
                                CurrentPage = TotalPages;
                            
                            UpdateCurrentPage();
                        });
                        
                        await Task.Delay(10);
                    }
                    
                }
            }, token);
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
            
            await Dispatcher.UIThread.InvokeAsync(() =>
            {
                StrictlyIncreasingCount = strictlyIncreasingCount;
                StrictlyIncreasingProbability = _allResults.Count > 0 
                    ? (double)strictlyIncreasingCount / _allResults.Count : 0;
                
                TotalPages = (int)Math.Ceiling(_allResults.Count / (double)PageSize);
                if (TotalPages == 0) TotalPages = 1;
                
                if (CurrentPage > TotalPages)
                    CurrentPage = TotalPages;
                
                UpdateCurrentPage();
                
                if (_allResults.Count > 0)
                {
                    // Выбираем первую фигуру в списке
                    SelectedFigure = GeneratedFigures.FirstOrDefault();
                
                    // Показываем её на графике
                    if (SelectedFigure != null)
                    {
                        ShowFigure(SelectedFigure);
                    }
                }
            });
        }
    }

    private double GetRandomValue()
    {
        int value = SelectedDistribution switch
        {
            "Равномерное" => _random.Next((int)UniformMin, (int)UniformMax + 1),
            "Биномиальное" => Binomial(),
            "Геометрическое" => Geometric(),
            "Треугольное" => Triangular(),
            _ => 0
        };
        
        return Math.Clamp(value, 0, NTau) * Tau;
    }

    #region Распределения

    private int Binomial()
    {
        int successes = 0;
        for (int i = 0; i < BinomialTrials; i++)
            if (_random.NextDouble() < BinomialProbability)
                successes++;
        return successes;
    }

    private int Geometric()
    {
        int trials = 1;
        while (_random.NextDouble() > GeometricProbability && trials < GeometricMax)
            trials++;
        return trials;
    }

    private int Triangular()
    {
        double u = _random.NextDouble();
        double fc = (TriangularMode - TriangularLow) / (double)(TriangularHigh - TriangularLow);
        
        double value;
        if (u < fc)
            value = TriangularLow + Math.Sqrt(u * (TriangularHigh - TriangularLow) * (TriangularMode - TriangularLow));
        else
            value = TriangularHigh - Math.Sqrt((1 - u) * (TriangularHigh - TriangularLow) * (TriangularHigh - TriangularMode));
        
        return (int)Math.Round(value);
    }
    
    #endregion
}

public partial class FigureViewModel : ObservableObject
{
    [ObservableProperty] private SimulationResult _result;
    
    public string DisplayText => $"{Result.number}: \n {Result.Timestamp:HH:mm:ss} | {(Result.IsStrictlyIncreasing ? "ВОЗРАСТАЕТ" : "НЕ возрастает")}";
    
    public string StepValuesText
    {
        get
        {
            if (Result.StepValues.Count > 8)
                return string.Join(", ", Result.StepValues.Take(8).Select(v => v.Y)) + "...";
            return string.Join(", ", Result.StepValues.Select(v => v.Y));
        }
    }
    
    public FigureViewModel(SimulationResult result) => _result = result;
}