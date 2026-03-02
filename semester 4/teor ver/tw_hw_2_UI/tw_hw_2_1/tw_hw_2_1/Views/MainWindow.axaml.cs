// Views/MainWindow.axaml.cs
using System;
using System.Linq;
using Avalonia;
using Avalonia.Controls;
using Avalonia.Controls.Shapes;
using Avalonia.Input;
using Avalonia.Media;
using Avalonia.Interactivity;
using Avalonia.VisualTree;
using EpidemicSimulator.Models;
using EpidemicSimulator.ViewModels;

namespace EpidemicSimulator.Views
{
    public partial class MainWindow : Window
    {
        private Person _draggedPerson;
        private Point _dragOffset;
        private double _scale = 1.0;
        private Canvas _graphCanvas;
        private MainWindowViewModel ViewModel => DataContext as MainWindowViewModel;

        public MainWindow()
        {
            InitializeComponent();
            
            // Находим Canvas после инициализации компонентов
            _graphCanvas = this.FindControl<Canvas>("GraphCanvas");
            
            if (ViewModel != null)
            {
                ViewModel.GraphUpdated += OnGraphUpdated;
            }
            
            // Подписываемся на события Canvas
            if (_graphCanvas != null)
            {
                _graphCanvas.PointerWheelChanged += OnCanvasPointerWheelChanged;
                _graphCanvas.PointerPressed += OnCanvasPointerPressed;
                _graphCanvas.PointerMoved += OnCanvasPointerMoved;
                _graphCanvas.PointerReleased += OnCanvasPointerReleased;
            }
        }

        private void InitializeComponent()
        {
            Avalonia.Markup.Xaml.AvaloniaXamlLoader.Load(this);
        }

        private void OnGraphUpdated(object sender, EventArgs e)
        {
            DrawGraph();
        }

        private void DrawGraph()
        {
            if (_graphCanvas == null || ViewModel?.Graph?.People == null) return;

            _graphCanvas.Children.Clear();

            // Рисуем рёбра
            foreach (var edge in ViewModel.Graph.Edges)
            {
                var line = new Line
                {
                    Stroke = Brushes.Gray,
                    StrokeThickness = 1,
                    StartPoint = new Point(edge.Item1.X * _scale, edge.Item1.Y * _scale),
                    EndPoint = new Point(edge.Item2.X * _scale, edge.Item2.Y * _scale)
                };
                
                _graphCanvas.Children.Add(line);
            }

            // Рисуем узлы
            foreach (var person in ViewModel.Graph.People)
            {
                var ellipse = new Ellipse
                {
                    Width = 20,
                    Height = 20,
                    Fill = GetBrushForPerson(person),
                    Tag = person
                };
                
                // Создаем всплывающую подсказку
                ToolTip.SetTip(ellipse, $"ID: {person.Id}, Status: {person.State}");
                
                // Сохраняем ссылку на эллипс в теге для обработки перетаскивания
                ellipse.Tag = person;
                
                // Обработчик для начала перетаскивания
                ellipse.PointerPressed += (sender, e) =>
                {
                    if (sender is Ellipse el && el.Tag is Person p)
                    {
                        _draggedPerson = p;
                        var pos = ToLogical(e.GetPosition(_graphCanvas));
                        _dragOffset = new Point(pos.X - p.X, pos.Y - p.Y);
                        e.Handled = true;
                    }
                };
                
                // Устанавливаем позицию
                Canvas.SetLeft(ellipse, person.X * _scale - 10);
                Canvas.SetTop(ellipse, person.Y * _scale - 10);
                
                _graphCanvas.Children.Add(ellipse);
            }
        }

        private IBrush GetBrushForPerson(Person p)
        {
            if (p.IsHighlighted)
                return Brushes.Orange;
                
            return p.State switch
            {
                PersonState.Healthy => new SolidColorBrush(Color.Parse("#90EE90")),
                PersonState.Infected => new SolidColorBrush(Color.Parse("#F08080")),
                PersonState.Recovered => new SolidColorBrush(Color.Parse("#ADD8E6")),
                _ => Brushes.Gray
            };
        }

        private Point ToLogical(Point screenPoint)
        {
            if (_scale != 0)
                return new Point(screenPoint.X / _scale, screenPoint.Y / _scale);
            return screenPoint;
        }

        private void OnCanvasPointerWheelChanged(object sender, PointerWheelEventArgs e)
        {
            const double zoomSpeed = 1.2;
            var delta = e.Delta.Y;
            var newScale = _scale * (delta > 0 ? zoomSpeed : 1.0 / zoomSpeed);
            newScale = Math.Clamp(newScale, 0.1, 10.0);
            _scale = newScale;
            
            DrawGraph();
            e.Handled = true;
        }

        private void OnCanvasPointerPressed(object sender, PointerPressedEventArgs e)
        {
            // Ничего не делаем, так как обработка нажатия на узел происходит в эллипсе
        }

        private void OnCanvasPointerMoved(object sender, PointerEventArgs e)
        {
            if (_draggedPerson != null)
            {
                var pos = ToLogical(e.GetPosition(_graphCanvas));
                _draggedPerson.X = pos.X - _dragOffset.X;
                _draggedPerson.Y = pos.Y - _dragOffset.Y;
                
                DrawGraph();
                e.Handled = true;
            }
        }

        private void OnCanvasPointerReleased(object sender, PointerReleasedEventArgs e)
        {
            if (_draggedPerson != null)
            {
                _draggedPerson = null;
                e.Handled = true;
            }
        }
        
        // Освобождаем ресурсы
        protected override void OnClosed(EventArgs e)
        {
            if (ViewModel != null)
            {
                ViewModel.GraphUpdated -= OnGraphUpdated;
            }
            
            if (_graphCanvas != null)
            {
                _graphCanvas.PointerWheelChanged -= OnCanvasPointerWheelChanged;
                _graphCanvas.PointerPressed -= OnCanvasPointerPressed;
                _graphCanvas.PointerMoved -= OnCanvasPointerMoved;
                _graphCanvas.PointerReleased -= OnCanvasPointerReleased;
            }
            
            base.OnClosed(e);
        }

        private void Button_Click(object? sender, RoutedEventArgs e)
        {
            throw new NotImplementedException();
        }
    }
}