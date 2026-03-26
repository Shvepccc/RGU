namespace tw_hw_6;

public class Task6 : ITask
{
    public int Number => 6;
    
    public void Run(string[] args)
    {
        Random rnd = new Random();
        int experiments = 1000000;
        double p = 0.5;
        
        int countFirst = 0;
        int countSecond = 0;
        
        for (int exp = 0; exp < experiments; exp++)
        {
            int x = 1;
            while (rnd.NextDouble() >= p)
            {
                x++;
            }
            
            double value = (double)x / 2 * (1 + Math.Pow(-1, x));
            
            if (Math.Abs(value) < 1e-10)
            {
                countFirst++;
            }
            
            if (value > 0 && Math.Abs(value % 2) < 1e-10)
            {
                countSecond++;
            }
        }
        
        Console.WriteLine($"P(X/2 * (1 + (-1)^X) = 0) = {(double)countFirst / experiments:F6}");
        Console.WriteLine($"P(X/2 * (1 + (-1)^X) in 2N) = {(double)countSecond / experiments:F6}");
    }
}