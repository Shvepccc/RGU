namespace task_3.Models;

using System;
using System.Collections.Generic;

public class RumorSimulationEngine
{
    private readonly Random _rng = new();

    public ExperimentResult RunSingleExperiment(int id, int n, int r, int N, out List<(int from, int to)> stepsEdges)
    {
        stepsEdges = new List<(int, int)>();
        var informedSet = new HashSet<int> { 0 };
        int currentSpreader = 0;

        bool returnedToOriginator = false;
        bool duplicateFound = false;
        
        List<int> currentRecipients = new List<int>(N);

        for (int step = 1; step <= r; step++)
        {
            currentRecipients.Clear();
            var chosenSet = new HashSet<int>();
            
            while (chosenSet.Count < N && chosenSet.Count < n)
            {
                int val = _rng.Next(0, n + 1);
                if (val != currentSpreader)
                {
                    chosenSet.Add(val);
                }
            }

            foreach (var person in chosenSet)
            {
                currentRecipients.Add(person);
                stepsEdges.Add((currentSpreader, person));

                if (person == 0) 
                    returnedToOriginator = true;
                
                if (informedSet.Contains(person)) 
                    duplicateFound = true;

                informedSet.Add(person);
            }
            
            if (currentRecipients.Count > 0)
            {
                currentSpreader = currentRecipients[_rng.Next(currentRecipients.Count)];
            }
        }

        string status;
        if (!returnedToOriginator && !duplicateFound) status = "Идеальная передача (слух уникален и не вернулся к источнику)";
        else if (returnedToOriginator && duplicateFound) status = "Полное смешивание (слух вернулся к источнику и был пересказан повторно)";
        else if (returnedToOriginator) status = "Замкнутый круг (слух вернулся к тому, кто его начал)";
        else status = "Испорченный телефон (кто-то уже слышал новость ранее)";

        return new ExperimentResult
        {
            Id = id,
            Status = status,
            StepsCompleted = r,
            TotalInformed = informedSet.Count,
            NoReturnToOriginator = !returnedToOriginator,
            NoDuplication = !duplicateFound
        };
    }

    public double GetAnalyticalProbabilityA(int n, int r, int N)
    {
        if (r <= 1) return 1.0;
        if (N > n) return 0.0;
        return Math.Pow(1.0 - (double)N / n, r - 1);
    }

    public double GetAnalyticalProbabilityB(int n, int r, int N)
    {
        if (r <= 1) return 1.0;
        double totalProb = 1.0;

        for (int i = 1; i <= r - 1; i++)
        {
            int A = n - i * N;
            int B = n;
            if (A - N + 1 < 0) return 0.0;

            double stepProb = 1.0;
            for (int j = 0; j < N; j++)
            {
                stepProb *= (double)(A - j) / (B - j);
            }
            totalProb *= stepProb;
        }
        return totalProb;
    }
}