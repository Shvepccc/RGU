namespace tw_hw_6;

public interface ITask
{
    void Run(string[] args);
    int Number { get; }
}