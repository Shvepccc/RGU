using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Reactive;
using System.Reactive.Concurrency;
using System.Reactive.Linq;
using System.Text;
using System.Threading.Tasks;
using Avalonia.Collections;
using Avalonia.Controls;
using Avalonia.Platform.Storage;
using DynamicData;
using ReactiveUI;
using tw_hw_2_1_test.Models;

namespace tw_hw_2_1_test.ViewModels
{
    public class MainWindowViewModel : ViewModelBase
    {
        // ==================== Свойства ====================
        private ObservableCollection<Person> _people;
        public ObservableCollection<Person> People
        {
            get => _people;
            set => this.RaiseAndSetIfChanged(ref _people, value);
        }
        
        private AvaloniaList<(int, int)> _edges;
        public AvaloniaList<(int, int)> Edges
        {
            get => _edges;
            set => this.RaiseAndSetIfChanged(ref _edges, value);
        }
        
        private double _infectionProbability = 0.3;
        public double InfectionProbability
        {
            get => _infectionProbability;
            set => this.RaiseAndSetIfChanged(ref _infectionProbability, value);
        }
        
        private double _recoveryProbability = 0.1;
        public double RecoveryProbability
        {
            get => _recoveryProbability;
            set => this.RaiseAndSetIfChanged(ref _recoveryProbability, value);
        }
        
        private int _currentStep;
        public int CurrentStep
        {
            get => _currentStep;
            set => this.RaiseAndSetIfChanged(ref _currentStep, value);
        }
        
        private string _statusMessage = "Готов";
        public string StatusMessage
        {
            get => _statusMessage;
            set => this.RaiseAndSetIfChanged(ref _statusMessage, value);
        }
        
        private bool _isSimulationRunning;
        public bool IsSimulationRunning
        {
            get => _isSimulationRunning;
            set => this.RaiseAndSetIfChanged(ref _isSimulationRunning, value);
        }
        
        private ObservableCollection<int> _searchResults;
        public ObservableCollection<int> SearchResults
        {
            get => _searchResults;
            set => this.RaiseAndSetIfChanged(ref _searchResults, value);
        }
        
        private int _activeSearchButton = -1;
        public int ActiveSearchButton
        {
            get => _activeSearchButton;
            set => this.RaiseAndSetIfChanged(ref _activeSearchButton, value);
        }
        
        private HashSet<int> _highlightedPeople;
        public HashSet<int> HighlightedPeople
        {
            get => _highlightedPeople;
            set => this.RaiseAndSetIfChanged(ref _highlightedPeople, value);
        }
        
        private bool _isDataLoaded;
        public bool IsDataLoaded
        {
            get => _isDataLoaded;
            set => this.RaiseAndSetIfChanged(ref _isDataLoaded, value);
        }
        
        // ==================== Команды ====================
        public ReactiveCommand<Unit, Unit> LoadDataCommand { get; }
        public ReactiveCommand<Unit, Unit> StartSimulationCommand { get; }
        public ReactiveCommand<Unit, Unit> StopSimulationCommand { get; }
        public ReactiveCommand<Unit, Unit> ResetSimulationCommand { get; }
        public ReactiveCommand<int, Unit> SearchCommand { get; }
        
        // ==================== Взаимодействия ====================
        public Interaction<Unit, IReadOnlyList<IStorageFile>> ShowOpenFileDialog { get; }
        public Interaction<string, Unit> ShowErrorMessage { get; }
        
        // ==================== Приватные поля ====================
        private IDisposable _timerSubscription;
        private Dictionary<int, List<int>> _adjacencyList;
        private Random _random = new Random();
        
        public MainWindowViewModel()
        {
            People = new ObservableCollection<Person>();
            Edges = new AvaloniaList<(int, int)>();
            SearchResults = new ObservableCollection<int>();
            HighlightedPeople = new HashSet<int>();
            _adjacencyList = new Dictionary<int, List<int>>();
            
            // Инициализация взаимодействий
            ShowOpenFileDialog = new Interaction<Unit, IReadOnlyList<IStorageFile>>();
            ShowErrorMessage = new Interaction<string, Unit>();
            
            // Инициализация команд
            LoadDataCommand = ReactiveCommand.CreateFromTask(LoadDataAsync);
            
            var canStart = this.WhenAnyValue(
                x => x.IsDataLoaded, 
                x => x.IsSimulationRunning,
                (loaded, running) => loaded && !running);
            
            StartSimulationCommand = ReactiveCommand.Create(StartSimulation, canStart);
            
            var canStop = this.WhenAnyValue(x => x.IsSimulationRunning);
            StopSimulationCommand = ReactiveCommand.Create(StopSimulation, canStop);
            
            ResetSimulationCommand = ReactiveCommand.Create(ResetSimulation, canStart);
            
            SearchCommand = ReactiveCommand.Create<int>(PerformSearch);
        }
        
        // ==================== Загрузка данных ====================
        private async Task LoadDataAsync()
        {
            try
            {
                var files = await ShowOpenFileDialog.Handle(Unit.Default);
                var file = files?.FirstOrDefault();
                
                if (file == null)
                    return;
                
                var filePath = file.Path.LocalPath;
                
                if (!File.Exists(filePath))
                {
                    await ShowErrorMessage.Handle("Файл не существует");
                    return;
                }
                
                var lines = await File.ReadAllLinesAsync(filePath);
                var peopleDict = new Dictionary<int, Person>();
                var edgesList = new List<(int, int)>();
                var adjList = new Dictionary<int, List<int>>();
                
                foreach (var line in lines)
                {
                    var trimmedLine = line.Trim();
                    
                    // Пропускаем пустые строки и комментарии
                    if (string.IsNullOrWhiteSpace(trimmedLine) || trimmedLine.StartsWith("#"))
                        continue;
                    
                    var parts = trimmedLine.Split(new[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);
                    
                    if (parts.Length != 2)
                    {
                        await ShowErrorMessage.Handle($"Неверный формат строки: {trimmedLine}");
                        return;
                    }
                    
                    if (!int.TryParse(parts[0], out int id1) || !int.TryParse(parts[1], out int id2))
                    {
                        await ShowErrorMessage.Handle($"ID должны быть числами: {trimmedLine}");
                        return;
                    }
                    
                    // Создаем людей, если их еще нет
                    if (!peopleDict.ContainsKey(id1))
                        peopleDict[id1] = new Person(id1);
                    
                    if (!peopleDict.ContainsKey(id2))
                        peopleDict[id2] = new Person(id2);
                    
                    edgesList.Add((id1, id2));
                    
                    // Заполняем список смежности
                    if (!adjList.ContainsKey(id1))
                        adjList[id1] = new List<int>();
                    if (!adjList.ContainsKey(id2))
                        adjList[id2] = new List<int>();
                    
                    adjList[id1].Add(id2);
                    adjList[id2].Add(id1);
                }
                
                People = new ObservableCollection<Person>(peopleDict.Values.OrderBy(p => p.Id));
                Edges = new AvaloniaList<(int, int)>(edgesList);
                _adjacencyList = adjList;
                IsDataLoaded = true;
                StatusMessage = $"Загружено: {People.Count} людей, {Edges.Count} связей";
                CurrentStep = 0;
                HighlightedPeople.Clear();
                SearchResults.Clear();
                ActiveSearchButton = -1;
            }
            catch (Exception ex)
            {
                await ShowErrorMessage.Handle($"Ошибка загрузки: {ex.Message}");
            }
        }
        
        // ==================== Управление симуляцией ====================
        private void StartSimulation()
        {
            try
            {
                // Проверка вероятностей
                if (InfectionProbability <= 0 || InfectionProbability > 1)
                {
                    ShowErrorMessage.Handle("Вероятность заражения должна быть в диапазоне (0; 1]").Subscribe();
                    return;
                }
                
                if (RecoveryProbability <= 0 || RecoveryProbability > 1)
                {
                    ShowErrorMessage.Handle("Вероятность излечения должна быть в диапазоне (0; 1]").Subscribe();
                    return;
                }
                
                // Если это первый запуск после загрузки, делаем случайную вспышку
                if (CurrentStep == 0 && People.All(p => p.State != PersonState.Infected))
                {
                    var randomIndex = _random.Next(People.Count);
                    People[randomIndex].State = PersonState.Infected;
                }
                
                IsSimulationRunning = true;
                StatusMessage = "Симуляция";
                
                // // Таймер для пошагового моделирования
                // _timerSubscription = Observable.Interval(TimeSpan.FromSeconds(1))
                //     .ObserveOn(RxApp.MainThreadScheduler)
                //     .Subscribe(_ => SimulationStep());
            }
            catch (Exception ex)
            {
                ShowErrorMessage.Handle($"Ошибка запуска: {ex.Message}").Subscribe();
            }
        }
        
        private void StopSimulation()
        {
            _timerSubscription?.Dispose();
            IsSimulationRunning = false;
            StatusMessage = "Пауза";
        }
        
        private void ResetSimulation()
        {
            StopSimulation();
            
            foreach (var person in People)
            {
                person.State = PersonState.Healthy;
            }
            
            CurrentStep = 0;
            HighlightedPeople.Clear();
            SearchResults.Clear();
            ActiveSearchButton = -1;
            StatusMessage = "Готов";
        }
        
        private void SimulationStep()
        {
            if (!IsSimulationRunning)
                return;
            
            var newStates = new Dictionary<int, PersonState>();
            
            // Копируем текущие состояния
            foreach (var person in People)
            {
                newStates[person.Id] = person.State;
            }
            
            // Обновляем состояния
            foreach (var person in People)
            {
                if (person.State == PersonState.Infected)
                {
                    // Проверка на излечение
                    if (_random.NextDouble() < RecoveryProbability)
                    {
                        newStates[person.Id] = PersonState.Recovered;
                    }
                }
                else if (person.State == PersonState.Healthy)
                {
                    // Проверка на заражение от знакомых
                    if (_adjacencyList.ContainsKey(person.Id))
                    {
                        foreach (var neighborId in _adjacencyList[person.Id])
                        {
                            var neighbor = People.FirstOrDefault(p => p.Id == neighborId);
                            if (neighbor != null && neighbor.State == PersonState.Infected)
                            {
                                if (_random.NextDouble() < InfectionProbability)
                                {
                                    newStates[person.Id] = PersonState.Infected;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            
            // Применяем новые состояния
            foreach (var person in People)
            {
                person.State = newStates[person.Id];
            }
            
            CurrentStep++;
        }
        
        // ==================== Поиск ====================
        private void PerformSearch(int searchType)
        {
            try
            {
                if (!IsDataLoaded)
                    return;
                
                // Деактивируем предыдущую кнопку и активируем новую
                ActiveSearchButton = ActiveSearchButton == searchType ? -1 : searchType;
                
                if (ActiveSearchButton == -1)
                {
                    SearchResults.Clear();
                    HighlightedPeople.Clear();
                    return;
                }
                
                var results = new List<int>();
                
                switch (searchType)
                {
                    case 0: // Все не заразившиеся (здоровые)
                        results = People.Where(p => p.State == PersonState.Healthy)
                                       .Select(p => p.Id)
                                       .ToList();
                        break;
                        
                    case 1: // Все исцелившиеся
                        results = People.Where(p => p.State == PersonState.Recovered)
                                       .Select(p => p.Id)
                                       .ToList();
                        break;
                        
                    case 2: // Исцелившиеся, у которых есть знакомый в другом состоянии
                        foreach (var person in People.Where(p => p.State == PersonState.Recovered))
                        {
                            if (_adjacencyList.ContainsKey(person.Id))
                            {
                                foreach (var neighborId in _adjacencyList[person.Id])
                                {
                                    var neighbor = People.FirstOrDefault(p => p.Id == neighborId);
                                    if (neighbor != null && neighbor.State != PersonState.Recovered)
                                    {
                                        results.Add(person.Id);
                                        break;
                                    }
                                }
                            }
                        }
                        break;
                        
                    case 3: // Здоровые, у которых все знакомые заражены
                        foreach (var person in People.Where(p => p.State == PersonState.Healthy))
                        {
                            if (_adjacencyList.ContainsKey(person.Id) && _adjacencyList[person.Id].Count > 0)
                            {
                                bool allInfected = true;
                                foreach (var neighborId in _adjacencyList[person.Id])
                                {
                                    var neighbor = People.FirstOrDefault(p => p.Id == neighborId);
                                    if (neighbor == null || neighbor.State != PersonState.Infected)
                                    {
                                        allInfected = false;
                                        break;
                                    }
                                }
                                
                                if (allInfected)
                                    results.Add(person.Id);
                            }
                        }
                        break;
                        
                    case 4: // Дополнительный поиск для демонстрации (можно заменить)
                        // Здесь можно добавить любой другой поиск
                        // Например, все, кто был в контакте с зараженными
                        var infectedIds = People.Where(p => p.State == PersonState.Infected)
                                               .Select(p => p.Id)
                                               .ToHashSet();
                        
                        foreach (var person in People)
                        {
                            if (_adjacencyList.ContainsKey(person.Id))
                            {
                                foreach (var neighborId in _adjacencyList[person.Id])
                                {
                                    if (infectedIds.Contains(neighborId) && !results.Contains(person.Id))
                                    {
                                        results.Add(person.Id);
                                        break;
                                    }
                                }
                            }
                        }
                        break;
                }
                
                SearchResults = new ObservableCollection<int>(results.OrderBy(id => id));
                HighlightedPeople = new HashSet<int>(results);
            }
            catch (Exception ex)
            {
                ShowErrorMessage.Handle($"Ошибка поиска: {ex.Message}").Subscribe();
            }
        }
    }
}