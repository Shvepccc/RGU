using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using Avalonia;
using Avalonia.Controls;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using LiveChartsCore;
using LiveChartsCore.Defaults;
using LiveChartsCore.SkiaSharpView;
using LiveChartsCore.SkiaSharpView.Painting;
using SkiaSharp;
using tw_hw_9_UI_task_1.Models;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Platform.Storage;

namespace tw_hw_9_UI_task_1.ViewModels;

public class DistributionEntry
{
    public double Value { get; set; }
    public double Probability { get; set; }
}

public partial class MainWindowViewModel : ObservableObject
{
    private Dictionary<char, DiscreteRandomVariable> _variables = new();
    private readonly VariableStorageService _storageService = new();

    [ObservableProperty] private string _commandInput = "A = {1:0.5, 2:0.5}";
    [ObservableProperty] private string _commandOutput = "";

    [ObservableProperty] private ObservableCollection<char> _availableVariables = new();
    
    private char? _selectedVariable;
    public char? SelectedVariable
    {
        get => _selectedVariable;
        set
        {
            if (SetProperty(ref _selectedVariable, value))
            {
                UpdateVisuals();
            }
        }
    }

    [ObservableProperty] private ObservableCollection<DistributionEntry> _distributionLaw = new();
    
    [ObservableProperty] private ObservableCollection<ISeries> _polylineSeries = new();
    [ObservableProperty] private ObservableCollection<ISeries> _cdfSeries = new();
    
    [ObservableProperty] private Axis[] _xAxes = { new Axis { Name = "X" } };
    [ObservableProperty] private Axis[] _polylineYAxes = { new Axis { Name = "P(X = x)", MinLimit = 0, MaxLimit = 1.1 } };
    [ObservableProperty] private Axis[] _cdfYAxes = { new Axis { Name = "F(x)", MinLimit = 0, MaxLimit = 1.1 } };
    

    public IRelayCommand ExecuteCommand { get; }
    public IAsyncRelayCommand ImportJsonCommand { get; }
    public IAsyncRelayCommand ExportJsonCommand { get; }
    public IAsyncRelayCommand ImportCommandsTxtCommand { get; }
    public IAsyncRelayCommand ExportCommandsTxtCommand { get; }
    public IRelayCommand DeleteVariableCommand { get; }
    
    public MainWindowViewModel()
    {
        ExecuteCommand = new RelayCommand(Execute);
        ImportJsonCommand = new AsyncRelayCommand(ImportJson);
        ExportJsonCommand = new AsyncRelayCommand(ExportJson);
        ImportCommandsTxtCommand = new AsyncRelayCommand(ImportCommandsTxt);
        ExportCommandsTxtCommand = new AsyncRelayCommand(ExportCommandsTxt);
        DeleteVariableCommand = new RelayCommand(DeleteVariable);
        
        // Инициализация пустых серий
        PolylineSeries.Add(new LineSeries<ObservablePoint> { LineSmoothness = 0 });
        CdfSeries.Add(new StepLineSeries<ObservablePoint> {});
    }
    
    private void Execute()
    {
        if (string.IsNullOrWhiteSpace(CommandInput)) return;

        var lines = CommandInput.Split(new[] { '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);
        
        foreach (var line in lines)
        {
            try
            {
                ProcessLine(line.Trim());
            }
            catch (Exception ex)
            {
                CommandOutput += $"\n[Ошибка] {line}\n -> {ex.Message}";
            }
        }
    }

    private void ProcessLine(string line)
    {
        // Вывод статистики: mean(A), var(A), skew(A), kurt(A)
        var statMatch = Regex.Match(line, @"^(mean|var|skew|kurt)\(([A-Z])\)$");
        if (statMatch.Success)
        {
            char varName = statMatch.Groups[2].Value[0];
            if (!_variables.TryGetValue(varName, out var drv)) throw new Exception($"Переменная {varName} не найдена.");
            
            double res = statMatch.Groups[1].Value switch
            {
                "mean" => drv.GetExpectedValue(),
                "var" => drv.GetVariance(),
                "skew" => drv.GetSkewness(),
                "kurt" => drv.GetKurtosis(),
                _ => 0
            };
            CommandOutput += $"\n{line} = {res:F4}";
            return;
        }

        // Присваивание: C = A + B, C = A * 2, A = {1:0.5, 2:0.5}
        var assignMatch = Regex.Match(line, @"^([A-Z])\s*=\s*(.+)$");
        if (assignMatch.Success)
        {
            char target = assignMatch.Groups[1].Value[0];
            string expr = assignMatch.Groups[2].Value.Trim();

            // Создание из словаря: {1:0.5, 2:0.5}
            if (expr.StartsWith("{") && expr.EndsWith("}"))
            {
                var dictStr = expr.Trim('{', '}');
                var pairs = dictStr.Split(',');
                var dict = new Dictionary<double, double>();
                
                foreach (var pair in pairs)
                {
                    var kvp = pair.Split(':');
                    double val = double.Parse(kvp[0].Trim(), CultureInfo.InvariantCulture);
                    double prob = double.Parse(kvp[1].Trim(), CultureInfo.InvariantCulture);
                    dict[val] = prob;
                }
                
                _variables[target] = new DiscreteRandomVariable(dict);
                UpdateAvailableVariables();
                CommandOutput += $"\nСоздана переменная {target}.";
                return;
            }

            // Операции: A + B, A * B, A * 2
            var opMatch = Regex.Match(expr, @"^([A-Z])\s*([\+\*])\s*([A-Z]|\d+(?:\.\d+)?)$");
            if (opMatch.Success)
            {
                char op1 = opMatch.Groups[1].Value[0];
                string operation = opMatch.Groups[2].Value;
                string op2Str = opMatch.Groups[3].Value;

                if (!_variables.TryGetValue(op1, out var drv1)) throw new Exception($"Переменная {op1} не найдена.");

                DiscreteRandomVariable result;

                if (op2Str.Length == 1 && char.IsLetter(op2Str[0]))
                {
                    char op2 = op2Str[0];
                    if (!_variables.TryGetValue(op2, out var drv2)) throw new Exception($"Переменная {op2} не найдена.");
                    
                    result = operation == "+" ? drv1 + drv2 : drv1 * drv2;
                }
                else
                {
                    double scalar = double.Parse(op2Str, CultureInfo.InvariantCulture);
                    if (operation != "*") throw new Exception("Со скаляром поддерживается только умножение (*).");
                    result = drv1 * scalar;
                }

                _variables[target] = result;
                UpdateAvailableVariables();
                CommandOutput += $"\nВычислена переменная {target}.";
                return;
            }
        }
        
        throw new Exception("Синтаксис не распознан.");
    }

    private void UpdateAvailableVariables()
    {
        AvailableVariables.Clear();
        foreach (var key in _variables.Keys.OrderBy(k => k))
        {
            AvailableVariables.Add(key);
        }
        if (SelectedVariable == null && AvailableVariables.Any())
        {
            SelectedVariable = AvailableVariables.First();
        }
    }

    private void UpdateVisuals()
    {
        if (SelectedVariable == null || !_variables.TryGetValue(SelectedVariable.Value, out var drv))
        {
            DistributionLaw.Clear();
            PolylineSeries[0].Values = new ObservableCollection<ObservablePoint>();
            CdfSeries[0].Values = new ObservableCollection<ObservablePoint>();
            return;
        }

        var dist = drv.GetDistribution().OrderBy(k => k.Key).ToList();
        
        // 1. Обновление таблицы (DataGrid)
        DistributionLaw.Clear();
        foreach (var kvp in dist)
        {
            DistributionLaw.Add(new DistributionEntry { Value = kvp.Key, Probability = kvp.Value });
        }

        // 2. Обновление Полилайна (Обычный линейный график без сглаживания)
        var polylinePoints = dist.Select(kvp => new ObservablePoint(kvp.Key, kvp.Value)).ToList();
        
        PolylineSeries[0] = new LineSeries<ObservablePoint>
        {
            Values = polylinePoints,
            Fill = null,
            GeometrySize = 8,
            LineSmoothness = 0
        };

        // 3. Обновление Функции распределения (Ступенчатый график)
        var cdfPoints = new List<ObservablePoint>();
        double currentProb = 0;
        

        foreach (var kvp in dist)
        {
            currentProb += kvp.Value;
            cdfPoints.Add(new ObservablePoint(kvp.Key, currentProb));
        }
        
        // Добавляем точку справа для наглядности (после последнего значения вероятность 1)
        if (dist.Any()) cdfPoints.Add(new ObservablePoint(dist.Last().Key + 1, 1));

        CdfSeries[0] = new StepLineSeries<ObservablePoint>
        {
            Values = cdfPoints,
            Fill = new SolidColorPaint(SKColors.Blue.WithAlpha(30)),
            GeometrySize = 5,
        };
    }
    
    private async Task ImportJson()
    {
        var importedVar = await _storageService.ImportVariableAsync();
        if (importedVar != null)
        {
            // Находим свободное имя от A до Z
            for (char c = 'A'; c <= 'Z'; c++)
            {
                if (!_variables.ContainsKey(c))
                {
                    _variables[c] = importedVar;
                    UpdateAvailableVariables();
                    CommandOutput += $"\n[Система] Переменная импортирована как {c}.";
                    return;
                }
            }
            CommandOutput += "\n[Ошибка] Нет свободных имен для импорта (A-Z).";
        }
    }
    
    private async Task ExportJson()
    {
        if (SelectedVariable != null && _variables.TryGetValue(SelectedVariable.Value, out var drv))
        {
            bool success = await _storageService.ExportVariableAsync(drv, SelectedVariable.Value);
            if (success)
            {
                CommandOutput += $"\n[Система] Переменная {SelectedVariable} успешно экспортирована.";
            }
        }
        else
        {
            CommandOutput += "\n[Внимание] Выберите существующую переменную в списке для экспорта.";
        }
    }
    
    private void DeleteVariable()
    {
        if (SelectedVariable != null)
        {
            char toDelete = SelectedVariable.Value;
            _variables.Remove(toDelete);
            UpdateAvailableVariables();
            CommandOutput += $"\n[Система] Переменная {toDelete} удалена.";
            
            if (!AvailableVariables.Any())
            {
                SelectedVariable = null;
                UpdateVisuals();
            }
        }
    }
    
    private async Task ImportCommandsTxt()
    {
        try
        {
            if (Application.Current?.ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
            {
                var topLevel = TopLevel.GetTopLevel(desktop.MainWindow);
                var files = await topLevel.StorageProvider.OpenFilePickerAsync(new FilePickerOpenOptions
                {
                    Title = "Импорт списка команд",
                    FileTypeFilter = new[] { new FilePickerFileType("Text files") { Patterns = new[] { "*.txt" } } }
                });

                if (files.Count > 0)
                {
                    await using var stream = await files[0].OpenReadAsync();
                    using var reader = new StreamReader(stream);
                    CommandInput = await reader.ReadToEndAsync();
                    CommandOutput += $"\n[Система] Команды загружены из {files[0].Name}";
                }
            }
        }
        catch (Exception ex) { CommandOutput += $"\n[Ошибка импорта] {ex.Message}"; }
    }

    private async Task ExportCommandsTxt()
    {
        try
        {
            if (Application.Current?.ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
            {
                var topLevel = TopLevel.GetTopLevel(desktop.MainWindow);
                var file = await topLevel.StorageProvider.SaveFilePickerAsync(new FilePickerSaveOptions
                {
                    Title = "Экспорт списка команд",
                    DefaultExtension = "txt",
                    SuggestedFileName = "commands.txt"
                });

                if (file != null)
                {
                    await using var stream = await file.OpenWriteAsync();
                    using var writer = new StreamWriter(stream);
                    await writer.WriteAsync(CommandInput);
                    CommandOutput += $"\n[Система] Команды сохранены в {file.Name}";
                }
            }
        }
        catch (Exception ex) { CommandOutput += $"\n[Ошибка экспорта] {ex.Message}"; }
    }
}