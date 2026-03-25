namespace tw_hw_6;

// 3.cs
using System;

public class Task3 : ITask
{
    public int Number => 3;
    
    public void Run(string[] args)
    {
        Random rnd = new Random();
        int experiments = 1000;
        
        int excellentCount = 0;
        int goodCount = 0;
        int satisfactoryCount = 0;
        int poorCount = 0;
        
        int totalPassed = 0;
        
        for (int exp = 0; exp < experiments; exp++)
        {
            int studentType = rnd.Next(10);
            int knowledge;
            string type = "";
            
            if (studentType < 3)
            {
                knowledge = 20;
                type = "excellent";
            }
            else if (studentType < 7)
            {
                knowledge = 16;
                type = "good";
            }
            else if (studentType < 9)
            {
                knowledge = 10;
                type = "satisfactory";
            }
            else
            {
                knowledge = 5;
                type = "poor";
            }
            
            bool passed = true;
            for (int q = 0; q < 3; q++)
            {
                int question = rnd.Next(20);
                if (question >= knowledge)
                {
                    passed = false;
                    break;
                }
            }
            
            if (passed)
            {
                totalPassed++;
                if (type == "excellent") excellentCount++;
                else if (type == "good") goodCount++;
                else if (type == "satisfactory") satisfactoryCount++;
                else poorCount++;
            }
        }
        
        Console.WriteLine($"Условная вероятность подготовки:");
        Console.WriteLine($"а) отлично: {(double)excellentCount / totalPassed:F6}");
        Console.WriteLine($"б) хорошо: {(double)goodCount / totalPassed:F6}");
        Console.WriteLine($"в) удовлетворительно: {(double)satisfactoryCount / totalPassed:F6}");
        Console.WriteLine($"г) плохо: {(double)poorCount / totalPassed:F6}");
    }
}