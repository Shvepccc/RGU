namespace tw_hw_6;

public class Task4 : ITask
{
    public int Number => 4;
    
    public void Run(string[] args)
    {
        Random rnd = new Random();
        int experiments = 100000;
        
        int butlerKilled = 0;
        int totalDied = 0;
        
        for (int exp = 0; exp < experiments; exp++)
        {
            double drink = rnd.NextDouble();
            int drinks;
            
            if (drink < 0.25)
                drinks = 0;
            else if (drink < 0.25 + 0.4)
                drinks = 1;
            else
                drinks = 2;
            
            string killer;
            if (drinks == 0)
                killer = "lady";
            else if (drinks == 1)
                killer = "son";
            else
                killer = "butler";
            
            string method;
            if (killer == "lady")
            {
                if (rnd.NextDouble() < 2.0 / 3.0)
                    method = "poison";
                else
                    method = "strangle";
            }
            else if (killer == "butler")
            {
                if (rnd.NextDouble() < 2.0 / 3.0)
                    method = "strangle";
                else
                    method = "poison";
            }
            else
            {
                if (rnd.NextDouble() < 0.5)
                    method = "poison";
                else
                    method = "strangle";
            }
            
            double deathProb;
            if (method == "poison")
                deathProb = 0.75;
            else
                deathProb = 0.25;
            
            bool died = rnd.NextDouble() < deathProb;
            
            if (died)
            {
                totalDied++;
                if (killer == "butler")
                    butlerKilled++;
            }
        }
        
        double probability = (double)butlerKilled / totalDied;
        Console.WriteLine($"Вероятность, что убийца - дворецкий: {probability:F6}");
    }
}