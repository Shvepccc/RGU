using System.Collections.Generic;

namespace task_4.Models;

public class SimulationConfig
{
    public double Rho { get; set; } = 1.0; // ρ
    public int M { get; set; } = 100; // Количество запусков
    public int K { get; set; } = 10; // Количество шагов
    public string SelectedDistribution { get; set; } = "Равномерное";
    public int N { get; set; } = 4;
    
    // Все параметры распределений теперь целые (для Z_n)
    public int UniformMin { get; set; } = 0;
    public int UniformMax { get; set; } = 9;
    
    public int BinomialTrials { get; set; } = 10;
    public double BinomialProbability { get; set; } = 0.5;
    
    public double GeometricProbability { get; set; } = 0.3;
    
    public int TriangularLow { get; set; } = 0;
    public int TriangularHigh { get; set; } = 9;
    public int TriangularMode { get; set; } = 4;
}