namespace task_1.Models;

public enum HealthState : byte
{
    Uninfected = 0,
    Infected = 1,
    Healed = 2
}

public class PersonNode
{
    public int Id { get; set; }
    public HealthState State { get; set; }
    
    // Храним только индексы соседей в общем массиве для быстрой итерации
    public int[] NeighborIndices { get; set; } = System.Array.Empty<int>();

    public override string ToString() => Id.ToString();
}