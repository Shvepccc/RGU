namespace task_3.Models;

public class ExperimentResult
{
    public int Id { get; set; }
    public string Status { get; set; } = "";
    public int StepsCompleted { get; set; }
    public int TotalInformed { get; set; }
    public bool NoReturnToOriginator { get; set; }
    public bool NoDuplication { get; set; }
}