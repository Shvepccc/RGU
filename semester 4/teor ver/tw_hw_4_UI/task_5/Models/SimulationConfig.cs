using System.Collections.Generic;

namespace task_5.Models;

public class SimulationConfig
{
    public double H { get; set; } = 1.0;
    public double M { get; set; } = 10.0;
    public int Tau { get; set; } = 1;
    public int NTau { get; set; } = 5;
    public string SelectedDistribution { get; set; } = "Равномерное";
    public int GenerationCount { get; set; } = 50;

    // Параметры распределений
    public double UniformMin { get; set; } = 0;
    public double UniformMax { get; set; } = 5;
    public int BinomialTrials { get; set; } = 10;
    public double BinomialProbability { get; set; } = 0.5;
    public double GeometricProbability { get; set; } = 0.3;
    public int GeometricMax { get; set; } = 5;
    public int TriangularLow { get; set; } = 0;
    public int TriangularHigh { get; set; } = 5;
    public int TriangularMode { get; set; } = 2;
}