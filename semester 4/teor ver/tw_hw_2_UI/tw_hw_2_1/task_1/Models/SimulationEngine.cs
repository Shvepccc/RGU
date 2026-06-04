namespace task_1.Models;

using System;
using System.Collections.Generic;

public class SimulationEngine
{
    private readonly Random _rng = new();
    
    private byte[] _currentStates = Array.Empty<byte>();
    private byte[] _nextStates = Array.Empty<byte>();

    public void Initialize(PersonNode[] nodes, double p2)
    {
        int count = nodes.Length;
        if (_currentStates.Length != count)
        {
            _currentStates = new byte[count];
            _nextStates = new byte[count];
        }

        for (int i = 0; i < count; i++)
        {
            byte state = (byte)(_rng.NextDouble() < p2 ? HealthState.Infected : HealthState.Uninfected);
            nodes[i].State = (HealthState)state;
            _currentStates[i] = state;
        }
    }

    public bool Step(PersonNode[] nodes, double p, double p1)
    {
        int count = nodes.Length;
        bool hasInfected = false;
        
        for (int i = 0; i < count; i++)
        {
            byte currentState = _currentStates[i];

            if (currentState == (byte)HealthState.Healed)
            {
                _nextStates[i] = (byte)HealthState.Healed;
            }
            else if (currentState == (byte)HealthState.Infected)
            {
                if (_rng.NextDouble() < p1)
                    _nextStates[i] = (byte)HealthState.Healed;
                else
                {
                    _nextStates[i] = (byte)HealthState.Infected;
                    hasInfected = true;
                }
            }
            else // Uninfected
            {
                int infectedNeighbors = 0;
                int[] neighbors = nodes[i].NeighborIndices;
                int neighborsCount = neighbors.Length;

                for (int j = 0; j < neighborsCount; j++)
                {
                    if (_currentStates[neighbors[j]] == (byte)HealthState.Infected)
                    {
                        infectedNeighbors++;
                    }
                }

                if (infectedNeighbors > 0)
                {
                    double survivalProb = Math.Pow(1.0 - p, infectedNeighbors);
                    if (_rng.NextDouble() > survivalProb)
                    {
                        _nextStates[i] = (byte)HealthState.Infected;
                        hasInfected = true;
                    }
                    else
                    {
                        _nextStates[i] = (byte)HealthState.Uninfected;
                    }
                }
                else
                {
                    _nextStates[i] = (byte)HealthState.Uninfected;
                }
            }
        }
        
        Array.Copy(_nextStates, _currentStates, count);
        for (int i = 0; i < count; i++)
        {
            nodes[i].State = (HealthState)_currentStates[i];
        }

        return hasInfected;
    }
    
    public List<PersonNode> GetUninfected(PersonNode[] nodes)
    {
        var list = new List<PersonNode>(nodes.Length / 2);
        for (int i = 0; i < nodes.Length; i++)
            if (nodes[i].State == HealthState.Uninfected) list.Add(nodes[i]);
        return list;
    }

    public List<PersonNode> GetHealed(PersonNode[] nodes)
    {
        var list = new List<PersonNode>(nodes.Length / 2);
        for (int i = 0; i < nodes.Length; i++)
            if (nodes[i].State == HealthState.Healed) list.Add(nodes[i]);
        return list;
    }

    public List<PersonNode> GetHealedWithUnhealedEnv(PersonNode[] nodes)
    {
        var list = new List<PersonNode>();
        for (int i = 0; i < nodes.Length; i++)
        {
            if (nodes[i].State != HealthState.Healed) continue;
            
            bool safe = true;
            int[] neighbors = nodes[i].NeighborIndices;
            for (int j = 0; j < neighbors.Length; j++)
            {
                if (nodes[neighbors[j]].State == HealthState.Healed)
                {
                    safe = false;
                    break;
                }
            }
            if (safe) list.Add(nodes[i]);
        }
        return list;
    }

    public List<PersonNode> GetUninfectedWithFullyInfectedEnv(PersonNode[] nodes)
    {
        var list = new List<PersonNode>();
        for (int i = 0; i < nodes.Length; i++)
        {
            if (nodes[i].State != HealthState.Uninfected) continue;
            
            int[] neighbors = nodes[i].NeighborIndices;
            if (neighbors.Length == 0) continue;

            bool fullyInfected = true;
            for (int j = 0; j < neighbors.Length; j++)
            {
                if (nodes[neighbors[j]].State != HealthState.Infected)
                {
                    fullyInfected = false;
                    break;
                }
            }
            if (fullyInfected) list.Add(nodes[i]);
        }
        return list;
    }
}