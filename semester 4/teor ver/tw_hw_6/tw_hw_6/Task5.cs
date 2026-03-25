namespace tw_hw_6;

public class Task5 : ITask
{
    public int Number => 5;
    
    public void Run(string[] args)
    {
        Random rnd = new Random();
        int experiments = 10000;
        
        int secondAtLeastOneNew = 0;
        int bothAtLeastOneNew = 0;
        
        for (int exp = 0; exp < experiments; exp++)
        {
            int newBalls = 12;
            int usedBalls = 8;
            
            bool firstHasNew = GetThreeBalls(rnd, ref newBalls, ref usedBalls);
            
            for (int i = 0; i < 3; i++)
                usedBalls++;
            newBalls = newBalls - (firstHasNew ? 1 : 0);
            
            bool secondHasNew = GetThreeBalls(rnd, ref newBalls, ref usedBalls);
            
            if (secondHasNew)
            {
                secondAtLeastOneNew++;
                if (firstHasNew)
                    bothAtLeastOneNew++;
            }
        }
        
        double probability = (double)bothAtLeastOneNew / secondAtLeastOneNew;
        Console.WriteLine($"Условная вероятность: {probability:F6}");
    }
    
    static bool GetThreeBalls(Random rnd, ref int newBalls, ref int usedBalls)
    {
        bool hasNew = false;
        for (int i = 0; i < 3; i++)
        {
            int total = newBalls + usedBalls;
            if (rnd.Next(total) < newBalls)
            {
                hasNew = true;
                newBalls--;
            }
            else
            {
                usedBalls--;
            }
        }
        return hasNew;
    }
}