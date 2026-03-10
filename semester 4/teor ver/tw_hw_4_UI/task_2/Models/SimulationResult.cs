using System;
using System.Collections.Generic;

namespace task_2.Models;

public class SimulationResult
{
    public DateTime Timestamp { get; set; }
    public SimulationConfig Config { get; set; } = new();
    public List<Point> Trajectory { get; set; } = [];
    public int CrossingCount { get; set; }
    public double CrossingProbability { get; set; }
    
    public class Point
    {
        public double X { get; set; }
        public double Y { get; set; }
        public Point(double x, double y) { X = x; Y = y; }
    }
}