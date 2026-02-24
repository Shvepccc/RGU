// Models/Graph.cs
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace EpidemicSimulator.Models
{
    public class Graph
    {
        public List<Person> People { get; private set; }
        public List<(Person, Person)> Edges { get; private set; }

        public void LoadFromFile(string path)
        {
            var lines = File.ReadAllLines(path)
                .Where(l => !string.IsNullOrWhiteSpace(l) && !l.StartsWith("#"))
                .ToList();

            if (!lines.Any())
                throw new Exception("Файл пуст или содержит только комментарии.");

            int n;
            if (!int.TryParse(lines[0], out n) || n <= 0)
                throw new Exception("Первая строка должна содержать положительное целое число (количество людей).");

            People = Enumerable.Range(0, n)
                .Select(i => new Person { Id = i })
                .ToList();

            Edges = new List<(Person, Person)>();

            for (int i = 1; i < lines.Count; i++)
            {
                var parts = lines[i].Split(new[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);
                if (parts.Length < 2) continue;

                if (!int.TryParse(parts[0], out int a) || !int.TryParse(parts[1], out int b))
                    continue;

                if (a < 0 || a >= n || b < 0 || b >= n || a == b)
                    continue;

                var personA = People[a];
                var personB = People[b];

                if (!personA.Neighbors.Contains(personB))
                {
                    personA.Neighbors.Add(personB);
                    personB.Neighbors.Add(personA);
                    Edges.Add((personA, personB));
                }
            }

            LayoutCircular();
        }

        private void LayoutCircular()
        {
            int count = People.Count;
            if (count == 0) return;
            double angleStep = 2 * Math.PI / count;
            double radius = Math.Min(400, count * 15);
            double centerX = 500;
            double centerY = 400;
            int i = 0;
            foreach (var p in People)
            {
                p.X = centerX + radius * Math.Cos(i * angleStep);
                p.Y = centerY + radius * Math.Sin(i * angleStep);
                i++;
            }
        }

        public void Reset()
        {
            foreach (var p in People)
            {
                p.State = PersonState.Healthy;
                p.IsHighlighted = false;
            }
        }

        public void Step(double p1, double p2, Random random)
        {
            var newStates = new Dictionary<Person, PersonState>();
            foreach (var p in People)
            {
                if (p.State == PersonState.Healthy)
                {
                    int infectedNeighbors = p.Neighbors.Count(n => n.State == PersonState.Infected);
                    if (infectedNeighbors > 0)
                    {
                        double prob = 1.0 - Math.Pow(1.0 - p1, infectedNeighbors);
                        newStates[p] = random.NextDouble() < prob ? PersonState.Infected : PersonState.Healthy;
                    }
                    else
                    {
                        newStates[p] = PersonState.Healthy;
                    }
                }
                else if (p.State == PersonState.Infected)
                {
                    newStates[p] = random.NextDouble() < p2 ? PersonState.Recovered : PersonState.Infected;
                }
                else // Recovered
                {
                    newStates[p] = PersonState.Recovered;
                }
            }

            foreach (var kv in newStates)
                kv.Key.State = kv.Value;
        }
    }
}