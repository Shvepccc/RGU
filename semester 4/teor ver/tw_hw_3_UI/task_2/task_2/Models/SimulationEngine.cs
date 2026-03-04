namespace task_2.Models;

using System;
using System.Collections.Generic;

public class simulation_engine
{
    private Random _rng = new Random();

    public List<tree_node> simulate_point_movement(
        tree_node root,
        double[] transitionProbabilities, // массив вероятностей для детей (должен суммироваться в 1)
        ref Dictionary<int, int> leafHits,
        ref Dictionary<int, int> pathLengths)
    {
        var path = new List<tree_node>();
        var current = root;
        path.Add(current);

        while (current.children.Count > 0)
        {
            // проверка остановки с вероятностью p
            if (_rng.NextDouble() < current.stop_probability)
                break;

            // выбор ребёнка согласно распределению
            if (transitionProbabilities == null || transitionProbabilities.Length != current.children.Count)
                throw new InvalidOperationException("Массив вероятностей должен соответствовать количеству детей.");

            double r = _rng.NextDouble();
            double cumulative = 0.0;
            int selectedIndex = 0;
            for (int i = 0; i < transitionProbabilities.Length; i++)
            {
                cumulative += transitionProbabilities[i];
                if (r < cumulative)
                {
                    selectedIndex = i;
                    break;
                }
            }

            current = current.children[selectedIndex];
            path.Add(current);
        }

        // обновление статистики
        if (current.children.Count == 0) // дошли до листа
        {
            if (!leafHits.ContainsKey(current.node_id))
                leafHits[current.node_id] = 0;
            leafHits[current.node_id]++;
        }

        int length = path.Count;
        if (!pathLengths.ContainsKey(length))
            pathLengths[length] = 0;
        pathLengths[length]++;

        return path;
    }
}