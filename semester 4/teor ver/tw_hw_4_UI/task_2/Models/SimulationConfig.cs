using System.Collections.Generic;

namespace task_2.Models;

public class SimulationConfig
{
    public double H { get; set; } = 1.0;
    public int N { get; set; } = 100;
    public double Y { get; set; } = 10.0;
    public int K { get; set; } = 10;
    public string SelectedDistribution { get; set; } = "Равномерное";
    public bool UseSArray { get; set; } = false;
    public List<double> SArray { get; set; } = new() { -2, -1, 0, 1, 2 };
    
    public double UniformMin { get; set; } = -2.0;
    public double UniformMax { get; set; } = 2.0;
    public int BinomialTrials { get; set; } = 10;
    public double BinomialProbability { get; set; } = 0.5;
    public double GeometricProbability { get; set; } = 0.3;
    public int GeometricOffset { get; set; } = 1;
    public int TriangularLow { get; set; } = -3;
    public int TriangularHigh { get; set; } = 3;
    public int TriangularMode { get; set; } = 0;
}