using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Platform.Storage;

namespace aa_hw_6_UI.ViewModels;

using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Numerics;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using LiveChartsCore;
using LiveChartsCore.Defaults;
using LiveChartsCore.SkiaSharpView;
using LiveChartsCore.SkiaSharpView.Painting;
using SkiaSharp;
using PolynomialAlgebra; // Подключаем предоставленную библиотеку

public partial class MainWindowViewModel : ObservableObject
{
    private readonly string[] _variables = { "x", "y" };

    [ObservableProperty] 
    private string _generatorsText = "x^6, x^2y^3, xy^7";

    [ObservableProperty] 
    private string _statusMessage = "Ожидание запуска...";

    [ObservableProperty] 
    private ObservableCollection<ISeries> _series;

    [ObservableProperty] 
    private Axis[] _xAxes;

    [ObservableProperty] 
    private Axis[] _yAxes;

    public IRelayCommand PlotCommand { get; }
    public IAsyncRelayCommand LoadFromFileCommand { get; }
    public IAsyncRelayCommand SaveToFileCommand { get; }

    public MainWindowViewModel()
    {
        PlotCommand = new RelayCommand(PlotIdeals);
        LoadFromFileCommand = new AsyncRelayCommand(LoadFromFileAsync);
        SaveToFileCommand = new AsyncRelayCommand(SaveToFileAsync);
        
        _series = new ObservableCollection<ISeries>();

        _xAxes = new[]
        {
            new Axis
            {
                Name = "Степень x (m)",
                MinStep = 1,
                SeparatorsPaint = new SolidColorPaint(SKColors.LightGray) { StrokeThickness = 1 }
            }
        };

        _yAxes = new[]
        {
            new Axis
            {
                Name = "Степень y (n)",
                MinStep = 1,
                SeparatorsPaint = new SolidColorPaint(SKColors.LightGray) { StrokeThickness = 1 }
            }
        };

        PlotIdeals();
    }

    private Polynomial[] ParseGenerators(string input)
    {
        var parts = input.Split(new[] { ',', ';' }, StringSplitOptions.RemoveEmptyEntries);
        var generators = new List<Polynomial>();

        foreach (var part in parts)
        {
            string s = part.Trim().ToLower();
            int xDeg = 0;
            int yDeg = 0;

            var xMatch = Regex.Match(s, @"x(?:\^(\d+))?");
            if (xMatch.Success)
                xDeg = xMatch.Groups[1].Success ? int.Parse(xMatch.Groups[1].Value) : 1;

            var yMatch = Regex.Match(s, @"y(?:\^(\d+))?");
            if (yMatch.Success)
                yDeg = yMatch.Groups[1].Success ? int.Parse(yMatch.Groups[1].Value) : 1;

            // Если константа 1
            if (!xMatch.Success && !yMatch.Success && int.TryParse(s, out int c) && c == 1)
            {
                xDeg = 0;
                yDeg = 0;
            }

            var poly = new Polynomial(_variables, TermOrderType.Lex);
            poly.AddTerm(new[] { xDeg, yDeg }, 1);
            generators.Add(poly);
        }

        return generators.ToArray();
    }
    
    private IStorageProvider? GetStorageProvider()
    {
        if (Application.Current?.ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
        {
            return desktop.MainWindow?.StorageProvider;
        }
        return null;
    }

    private async Task LoadFromFileAsync()
    {
        var storage = GetStorageProvider();
        if (storage is null) return;

        try
        {
            var files = await storage.OpenFilePickerAsync(new FilePickerOpenOptions
            {
                Title = "Открыть список мономов",
                AllowMultiple = false,
                FileTypeFilter = new[] 
                { 
                    new FilePickerFileType("Текстовые файлы") { Patterns = new[] { "*.txt" } } 
                }
            });

            if (files.Count > 0)
            {
                await using var stream = await files[0].OpenReadAsync();
                using var reader = new StreamReader(stream);
                GeneratorsText = await reader.ReadToEndAsync();
                
                PlotIdeals();
                StatusMessage = $"Загружено из: {files[0].Name}";
            }
        }
        catch (Exception ex)
        {
            StatusMessage = $"Ошибка загрузки: {ex.Message}";
        }
    }

    private async Task SaveToFileAsync()
    {
        var storage = GetStorageProvider();
        if (storage is null) return;

        try
        {
            var file = await storage.SaveFilePickerAsync(new FilePickerSaveOptions
            {
                Title = "Сохранить список мономов",
                DefaultExtension = "txt",
                SuggestedFileName = "monomials.txt",
                FileTypeChoices = new[] 
                { 
                    new FilePickerFileType("Текстовые файлы") { Patterns = new[] { "*.txt" } } 
                }
            });

            if (file != null)
            {
                await using var stream = await file.OpenWriteAsync();
                using var writer = new StreamWriter(stream);
                await writer.WriteAsync(GeneratorsText);
                
                StatusMessage = $"Сохранено в: {file.Name}";
            }
        }
        catch (Exception ex)
        {
            StatusMessage = $"Ошибка сохранения: {ex.Message}";
        }
    }

    private void PlotIdeals()
    {
        try
        {
            Polynomial[] idealGenerators = ParseGenerators(GeneratorsText);

            if (idealGenerators.Length == 0)
            {
                StatusMessage = "Ошибка: не найдено ни одного образующего.";
                return;
            }

            // Находим границы сетки для визуализации. Ищем максимальные степени среди образующих.
            int maxX = 0;
            int maxY = 0;
            foreach (var gen in idealGenerators)
            {
                var term = gen.LeadingTerm();
                if (term.Exps[0] > maxX) maxX = term.Exps[0];
                if (term.Exps[1] > maxY) maxY = term.Exps[1];
            }

            // Добавляем отступ для наглядности (бесконечная часть идеала)
            maxX += 5;
            maxY += 5;

            var idealPoints = new List<ObservablePoint>();
            var remainderPoints = new List<ObservablePoint>();
            var mainPoints = new List<ObservablePoint>();

            // Перебираем точки сетки
            for (int m = 0; m <= maxX; m++)
            {
                for (int n = 0; n <= maxY; n++)
                {
                    // Создаем тестируемый моном P = x^m * y^n
                    var testMonomial = new Polynomial(_variables, TermOrderType.Lex);
                    testMonomial.AddTerm(new[] { m, n }, 1);

                    // Делим тестовый моном на образующие идеала
                    var (_, remainder) = testMonomial.Divide(idealGenerators);

                    // Если остаток 0, моном принадлежит идеалу I
                    if (remainder.IsZero() && idealGenerators.Contains(testMonomial))
                    {
                        mainPoints.Add(new ObservablePoint(m, n));
                    }
                    else if (remainder.IsZero())
                    {
                        idealPoints.Add(new ObservablePoint(m, n));
                    }
                    else
                    {
                        remainderPoints.Add(new ObservablePoint(m, n));
                    }
                }
            }

            // Формируем графики
            Series = new ObservableCollection<ISeries>
            {
                new ScatterSeries<ObservablePoint>
                {
                    Name = "Мономы в идеале I",
                    Values = idealPoints,
                    GeometrySize = 10,
                    Fill = new SolidColorPaint(SKColors.CornflowerBlue),
                    Stroke = null
                },
                new ScatterSeries<ObservablePoint>
                {
                    Name = "Остатки от деления ",
                    Values = remainderPoints,
                    GeometrySize = 10,
                    Fill = new SolidColorPaint(SKColors.Salmon),
                    Stroke = null
                },
                new ScatterSeries<ObservablePoint>
                {
                    Name = "Образующие мономы",
                    Values = mainPoints,
                    GeometrySize = 18,
                    Fill = new SolidColorPaint(SKColors.ForestGreen),
                    Stroke = null
                }
            };

            XAxes[0].MinLimit = -0.5;
            XAxes[0].MaxLimit = maxX + 0.5;
            YAxes[0].MinLimit = -0.5;
            YAxes[0].MaxLimit = maxY + 0.5;

            StatusMessage = $"Образующих: {idealGenerators.Length}. Возможных остатков (до границы): {remainderPoints.Count}.";
        }
        catch (Exception ex)
        {
            StatusMessage = $"Ошибка обработки: {ex.Message}";
        }
    }
}