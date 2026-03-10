using System;
using System.Collections.Generic;

namespace task_5.Models;

public class SimulationResult
{
    public DateTime Timestamp { get; set; }
    public SimulationConfig Config { get; set; } = new();
    public List<StepValue> StepValues { get; set; } = [];
    public bool IsStrictlyIncreasing { get; set; }
    public int number { get; set; }

    public class StepValue
    {
        public double X { get; set; }
        public double Y { get; set; }
        public StepValue(double x, double y) { X = x; Y = y; }
    }
}