namespace tw_hw_6;

public class Task1 : ITask
{
    public int Number => 1;
    
    public void Run(string[] args)
    {
        double p = 0.3;
        int n = 10;
        int experiments = 100000;
        
        Random random = new Random();
        int explosions = 0;
        
        for (int exp = 0; exp < experiments; exp++)
        {
            int hits = 0;
            bool exploded = false;
            
            for (int shot = 0; shot < n; shot++)
            {
                if (random.NextDouble() < p)
                {
                    hits++;
                    
                    if (hits == 1)
                    {
                        if (random.NextDouble() < p)
                        {
                            exploded = true;
                            break;
                        }
                    }
                    else if (hits >= 2)
                    {
                        exploded = true;
                        break;
                    }
                }
            }
            
            if (exploded)
                explosions++;
        }
        
        double empiricalProbability = (double)explosions / experiments;
        
        double theoreticalProbability = 0;
        double q = 1 - p;
        
        double probZeroHits = Math.Pow(q, n);
        double probOneHit = n * p * Math.Pow(q, n - 1);
        
        theoreticalProbability = 1 - probZeroHits - probOneHit * (1 - p);
        
        Console.WriteLine($"p = {p}, n = {n}, экспериментов = {experiments}");
        Console.WriteLine($"Эмпирическая вероятность: {empiricalProbability:F6}");
        Console.WriteLine($"Теоретическая вероятность: {theoreticalProbability:F6}");
        Console.WriteLine($"Отклонение: {Math.Abs(empiricalProbability - theoreticalProbability):F6}");
    }
}