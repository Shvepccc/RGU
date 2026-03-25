namespace tw_hw_6;

public class Task7 : ITask
{
    public int Number => 7;
    
    private Random rnd = new Random();
    
    private int GenerateGeometric(double p)
    {
        int failures = 0;
        while (rnd.NextDouble() > p)
        {
            failures++;
        }
        return failures;
    }
    
    public void Run(string[] args)
    {
        int experiments = 100000;
        double p = 0.3;
        int n = 3;
        int k = 2;
        
        int countXeqNplusK = 0;
        int countXgeK = 0;
        
        for (int exp = 0; exp < experiments; exp++)
        {
            int x = GenerateGeometric(p);
            
            if (x == n + k)
                countXeqNplusK++;
            
            if (x >= k)
                countXgeK++;
        }
        
        double empiricalLeft = (double)countXeqNplusK / countXgeK;
        
        int countXeqN = 0;
        for (int exp = 0; exp < experiments; exp++)
        {
            int x = GenerateGeometric(p);
            if (x == n)
                countXeqN++;
        }
        
        double empiricalRight = (double)countXeqN / experiments;
        
        double theoretical = Math.Pow(1 - p, n) * p;
        
        Console.WriteLine($"P(X = {n+k} | X >= {k}) = {empiricalLeft:F6}");
        Console.WriteLine($"P(X = {n}) = {empiricalRight:F6}");
        Console.WriteLine($"Разница: {Math.Abs(empiricalLeft - empiricalRight):F6}");
        Console.WriteLine($"Теоретическое значение: {theoretical:F6}");
    }
}