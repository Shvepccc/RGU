// ViewModels/MainWindowViewModel.cs
using System;
using System.Collections.ObjectModel;
using System.Linq;
using System.Reactive;
using System.Threading.Tasks;
using Avalonia.Controls;
using Avalonia.Threading;
using EpidemicSimulator.Models;
using ReactiveUI;

namespace EpidemicSimulator.ViewModels
{
    public class MainWindowViewModel : ReactiveObject
    {
        private Graph _graph;
        private double _p1 = 0.5;
        private double _p2 = 0.2;
        private bool _simulationRunning;
        private int _step;
        private string _status = "Готов";
        private ObservableCollection<string> _foundIds = new ObservableCollection<string>();
        private readonly Random _random = new Random();
        private DispatcherTimer _timer;

        public Graph Graph
        {
            get => _graph;
            private set => this.RaiseAndSetIfChanged(ref _graph, value);
        }

        public double P1
        {
            get => _p1;
            set
            {
                if (value > 0 && value <= 1.0)
                    this.RaiseAndSetIfChanged(ref _p1, value);
            }
        }

        public double P2
        {
            get => _p2;
            set
            {
                if (value > 0 && value <= 1.0)
                    this.RaiseAndSetIfChanged(ref _p2, value);
            }
        }

        public bool SimulationRunning
        {
            get => _simulationRunning;
            private set => this.RaiseAndSetIfChanged(ref _simulationRunning, value);
        }

        public int Step
        {
            get => _step;
            private set => this.RaiseAndSetIfChanged(ref _step, value);
        }

        public string Status
        {
            get => _status;
            private set => this.RaiseAndSetIfChanged(ref _status, value);
        }

        public ObservableCollection<string> FoundIds
        {
            get => _foundIds;
            set => this.RaiseAndSetIfChanged(ref _foundIds, value);
        }

        public int PeopleCount => _graph?.People.Count ?? 0;
        public int EdgeCount => _graph?.Edges.Count ?? 0;

        public string StatusText => $"Людей: {PeopleCount}, Связей: {EdgeCount}, p1={P1:F2}, p2={P2:F2}, Шаг: {Step}, Статус: {Status}";

        // Команды
        public ReactiveCommand<Unit, Unit> LoadDataCommand { get; }
        public ReactiveCommand<Unit, Unit> StartCommand { get; }
        public ReactiveCommand<Unit, Unit> StopCommand { get; }
        public ReactiveCommand<Unit, Unit> ResetCommand { get; }
        public ReactiveCommand<Unit, Unit> FindHealthyCommand { get; }
        public ReactiveCommand<Unit, Unit> FindRecoveredCommand { get; }
        public ReactiveCommand<Unit, Unit> FindRecoveredWithUnrecoveredNeighborsCommand { get; }
        public ReactiveCommand<Unit, Unit> FindHealthyWithAllNeighborsInfectedCommand { get; }
        public ReactiveCommand<Unit, Unit> ShowAllCommand { get; }

        // Событие для уведомления View о необходимости перерисовки графа
        public event EventHandler GraphUpdated;
        protected virtual void OnGraphUpdated() => GraphUpdated?.Invoke(this, EventArgs.Empty);

        public MainWindowViewModel()
        {
            Graph = new Graph();

            LoadDataCommand = ReactiveCommand.CreateFromTask(LoadDataAsync);
            StartCommand = ReactiveCommand.Create(StartSimulation, this.WhenAnyValue(x => x.SimulationRunning, v => !v));
            StopCommand = ReactiveCommand.Create(StopSimulation, this.WhenAnyValue(x => x.SimulationRunning));
            ResetCommand = ReactiveCommand.Create(ResetSimulation);

            FindHealthyCommand = ReactiveCommand.Create(FindHealthy);
            FindRecoveredCommand = ReactiveCommand.Create(FindRecovered);
            FindRecoveredWithUnrecoveredNeighborsCommand = ReactiveCommand.Create(FindRecoveredWithUnrecoveredNeighbors);
            FindHealthyWithAllNeighborsInfectedCommand = ReactiveCommand.Create(FindHealthyWithAllNeighborsInfected);
            ShowAllCommand = ReactiveCommand.Create(ShowAll);

            _timer = new DispatcherTimer { Interval = TimeSpan.FromMilliseconds(500) };
            _timer.Tick += TimerTick;
        }

        private async Task LoadDataAsync()
        {
            try
            {
                // Используем стандартный OpenFileDialog без Interaction
                var dialog = new OpenFileDialog();
                dialog.Title = "Выберите файл с данными";
                dialog.Filters.Add(new FileDialogFilter { Name = "Text Files", Extensions = { "txt" } });

                // Получаем главное окно
                var mainWindow = GetMainWindow();
                if (mainWindow == null)
                {
                    await ShowMessageAsync("Не удалось получить главное окно приложения");
                    return;
                }

                // Показываем диалог
                var result = await dialog.ShowAsync(mainWindow);
                if (result == null || result.Length == 0)
                    return;

                Graph.LoadFromFile(result[0]);
                ResetSimulation();
                UpdateProperties();
                OnGraphUpdated();
            }
            catch (Exception ex)
            {
                await ShowMessageAsync($"Ошибка загрузки: {ex.Message}");
            }
        }

        private void StartSimulation()
        {
            if (Graph.People == null || Graph.People.Count == 0)
                return;

            // Если нет заражённых, вызываем вспышку (случайный здоровый становится болен)
            if (!Graph.People.Any(p => p.State == PersonState.Infected))
            {
                var healthy = Graph.People.Where(p => p.State == PersonState.Healthy).ToList();
                if (healthy.Count == 0)
                {
                    Status = "Нет здоровых для начала вспышки";
                    return;
                }
                int index = _random.Next(healthy.Count);
                healthy[index].State = PersonState.Infected;
            }

            SimulationRunning = true;
            Status = "Симуляция запущена";
            _timer.Start();
        }

        private void StopSimulation()
        {
            SimulationRunning = false;
            Status = "Пауза";
            _timer.Stop();
        }

        private void ResetSimulation()
        {
            _timer.Stop();
            SimulationRunning = false;
            Step = 0;
            Graph?.Reset();
            ClearHighlight();
            UpdateProperties();
            Status = "Сброс";
            OnGraphUpdated();
        }

        private void TimerTick(object sender, EventArgs e)
        {
            if (!SimulationRunning) return;

            Graph.Step(P1, P2, _random);
            Step++;

            // Если не осталось заражённых, останавливаем симуляцию
            if (!Graph.People.Any(p => p.State == PersonState.Infected))
            {
                StopSimulation();
                Status = "Завершено (нет заражённых)";
            }

            UpdateProperties();
            OnGraphUpdated();
        }

        private void UpdateProperties()
        {
            this.RaisePropertyChanged(nameof(PeopleCount));
            this.RaisePropertyChanged(nameof(EdgeCount));
            this.RaisePropertyChanged(nameof(StatusText));
        }

        private void ClearHighlight()
        {
            if (Graph?.People == null) return;
            foreach (var p in Graph.People)
                p.IsHighlighted = false;
        }

        private void FindHealthy()
        {
            ClearHighlight();
            var found = Graph.People.Where(p => p.State == PersonState.Healthy).ToList();
            foreach (var p in found) p.IsHighlighted = true;
            FoundIds = new ObservableCollection<string>(found.Select(p => p.Id.ToString()));
            OnGraphUpdated();
        }

        private void FindRecovered()
        {
            ClearHighlight();
            var found = Graph.People.Where(p => p.State == PersonState.Recovered).ToList();
            foreach (var p in found) p.IsHighlighted = true;
            FoundIds = new ObservableCollection<string>(found.Select(p => p.Id.ToString()));
            OnGraphUpdated();
        }

        private void FindRecoveredWithUnrecoveredNeighbors()
        {
            ClearHighlight();
            var found = Graph.People
                .Where(p => p.State == PersonState.Recovered &&
                            p.Neighbors.Any(n => n.State != PersonState.Recovered))
                .ToList();
            foreach (var p in found) p.IsHighlighted = true;
            FoundIds = new ObservableCollection<string>(found.Select(p => p.Id.ToString()));
            OnGraphUpdated();
        }

        private void FindHealthyWithAllNeighborsInfected()
        {
            ClearHighlight();
            var found = Graph.People
                .Where(p => p.State == PersonState.Healthy &&
                            p.Neighbors.Count > 0 &&
                            p.Neighbors.All(n => n.State == PersonState.Infected))
                .ToList();
            foreach (var p in found) p.IsHighlighted = true;
            FoundIds = new ObservableCollection<string>(found.Select(p => p.Id.ToString()));
            OnGraphUpdated();
        }

        private void ShowAll()
        {
            FoundIds = new ObservableCollection<string>(Graph.People.Select(p => p.Id.ToString()));
        }

        private Window? GetMainWindow()
        {
            if (Avalonia.Application.Current?.ApplicationLifetime is Avalonia.Controls.ApplicationLifetimes.IClassicDesktopStyleApplicationLifetime desktop)
            {
                return desktop.MainWindow;
            }
            return null;
        }

        private async Task ShowMessageAsync(string message)
        {
            var mainWindow = GetMainWindow();
            var msgBox = new MessageBox(message);
            
            if (mainWindow != null)
            {
                await msgBox.ShowDialog(mainWindow);
            }
            else
            {
                msgBox.Show();
            }
        }
    }

    public class MessageBox : Window
    {
        public MessageBox(string message)
        {
            Title = "Сообщение";
            Width = 300;
            Height = 150;
            WindowStartupLocation = WindowStartupLocation.CenterOwner;
            
            var textBlock = new TextBlock 
            { 
                Text = message, 
                Margin = new Avalonia.Thickness(10),
                TextWrapping = Avalonia.Media.TextWrapping.Wrap
            };
            
            var button = new Button 
            { 
                Content = "OK", 
                HorizontalAlignment = Avalonia.Layout.HorizontalAlignment.Center,
                Margin = new Avalonia.Thickness(10)
            };
            button.Click += (s, e) => Close();
            
            var stackPanel = new StackPanel();
            stackPanel.Children.Add(textBlock);
            stackPanel.Children.Add(button);
            
            Content = stackPanel;
        }
    }
}