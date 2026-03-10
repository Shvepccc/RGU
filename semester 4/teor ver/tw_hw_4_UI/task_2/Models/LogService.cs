using System.Collections.Generic;
using System.Linq;
using task_2.Models;

namespace task_2.Services;

public class LogService
{
    private readonly List<SimulationResult> _logs = new();
    private int _maxLogs;

    public LogService(int maxLogs = 10) => _maxLogs = maxLogs;

    public void Add(SimulationResult result)
    {
        _logs.Insert(0, result);
        while (_logs.Count > _maxLogs)
            _logs.RemoveAt(_logs.Count - 1);
    }

    public void SetMaxLogs(int maxLogs)
    {
        _maxLogs = maxLogs;
    }

    public List<SimulationResult> GetAll() => _logs.ToList();
    public void Clear() => _logs.Clear();
}