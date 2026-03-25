namespace tw_hw_6;

public class Task2 : ITask
{
    public int Number => 2;
    
    public void Run(string[] args)
    {
        Random rnd = new Random();
        int experiments = 10000;
        int n = 10;
        int k = 3;
        
        int validExperiments = 0;
        int fourthEagle = 0;
        
        for (int exp = 0; exp < experiments; exp++)
        {
            int coinIndex = rnd.Next(n);
            bool isDoubleHeaded = coinIndex < k;
            
            bool firstEagle = rnd.NextDouble() < 0.5;
            if (!firstEagle) continue;
            
            bool secondEagle = rnd.NextDouble() < 0.5;
            if (!secondEagle) continue;
            
            bool thirdEagle = rnd.NextDouble() < 0.5;
            if (!thirdEagle) continue;
            
            validExperiments++;
            
            double probFourthEagle = isDoubleHeaded ? 1.0 : 0.5;
            bool fourthResult = rnd.NextDouble() < probFourthEagle;
            
            if (fourthResult)
                fourthEagle++;
        }
        
        double conditionalProbability = (double)fourthEagle / validExperiments;
        
        Console.WriteLine($"Экспериментов: {experiments}");
        Console.WriteLine($"Валидных экспериментов (3 орла): {validExperiments}");
        Console.WriteLine($"Четвертый орел: {fourthEagle}");
        Console.WriteLine($"Вероятность: {conditionalProbability:F6}");
    }
}