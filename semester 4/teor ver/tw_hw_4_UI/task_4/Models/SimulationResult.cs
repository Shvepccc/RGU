using System;

namespace task_4.Models;

public class SimulationResult
{
    public DateTime Timestamp { get; set; }
    public SimulationConfig Config { get; set; } = new();
    public int RunNumber { get; set; }
    public bool ReturnedToOrigin { get; set; }
}