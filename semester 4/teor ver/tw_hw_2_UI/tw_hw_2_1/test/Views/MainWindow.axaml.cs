using Avalonia;
using Avalonia.Controls;
using Avalonia.Controls.Shapes;
using Avalonia.Input;
using Avalonia.Media;
using Microsoft.Msagl.Drawing;
using Microsoft.Msagl.Core.Geometry;
using Point = Avalonia.Point;

namespace test.Views;

public partial class MainWindow : Window
{
    private readonly ScaleTransform _scale = new(1, 1);
    private readonly TranslateTransform _translate = new(0, 0);
    private Point? _lastPan;

    public MainWindow()
    {
        InitializeComponent();

        var tg = new TransformGroup();
        tg.Children.Add(_scale);
        tg.Children.Add(_translate);
        GraphCanvas.RenderTransform = tg;

        BuildGraph();
    }

    private void BuildGraph()
    {
        var graph = new Microsoft.Msagl.Drawing.Graph("graph");
        graph.AddEdge("A", "B");
        graph.AddEdge("B", "C");
        graph.AddEdge("C", "A");
        graph.AddEdge("A", "D");

// 1. создаём geometry
        graph.CreateGeometryGraph();

// 2. Layout
        var settings = new Microsoft.Msagl.Layout.MDS.MdsLayoutSettings();
        graph.LayoutAlgorithmSettings = settings;

// 3. Edge routing (опционально)
        graph.LayoutAlgorithmSettings.EdgeRoutingSettings.EdgeRoutingMode = Microsoft.Msagl.Core.Routing.EdgeRoutingMode.StraightLine;

// 4. Обновляем BoundingBox
        graph.GeometryGraph.UpdateBoundingBox();

        foreach (var edge in graph.Edges)
        {
            var line = new Line
            {
                StartPoint = ToAvalonia(edge.SourceNode.GeometryNode.Center),
                EndPoint = ToAvalonia(edge.TargetNode.GeometryNode.Center),
                Stroke = Brushes.Gray,
                StrokeThickness = 1
            };
            GraphCanvas.Children.Add(line);
        }

        foreach (var node in graph.Nodes)
        {
            var ellipse = new Ellipse
            {
                Width = 25,
                Height = 25,
                Fill = Brushes.CornflowerBlue
            };
            var p = ToAvalonia(node.GeometryNode.Center);
            Canvas.SetLeft(ellipse, p.X);
            Canvas.SetTop(ellipse, p.Y);

            ToolTip.SetTip(ellipse, node.Id);
            GraphCanvas.Children.Add(ellipse);
        }
    }

    private static Point ToAvalonia(Microsoft.Msagl.Core.Geometry.Point p)
        => new Point(p.X, p.Y);

    // Zoom колесом
    private void OnWheel(object? sender, PointerWheelEventArgs e)
    {
        double zoom = e.Delta.Y > 0 ? 1.1 : 0.9;
        _scale.ScaleX *= zoom;
        _scale.ScaleY *= zoom;
    }

    // Pan start
    private void OnPressed(object? sender, PointerPressedEventArgs e)
        => _lastPan = e.GetPosition(this);

    // Pan move
    private void OnMoved(object? sender, PointerEventArgs e)
    {
        if (_lastPan == null) return;
        var pos = e.GetPosition(this);
        var delta = pos - _lastPan.Value;

        _translate.X += delta.X;
        _translate.Y += delta.Y;

        _lastPan = pos;
    }

    private void OnReleased(object? sender, PointerReleasedEventArgs e)
        => _lastPan = null;
}