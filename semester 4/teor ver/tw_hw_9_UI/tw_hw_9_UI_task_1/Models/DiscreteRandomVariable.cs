// DiscreteRandomVariable.cs
namespace tw_hw_9_UI_task_1.Models;

using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text.Json;
using System.Text.Json.Serialization;

[Serializable]
public class DiscreteRandomVariable
{
    private Dictionary<double, double> _distribution;
    private bool _isValidated;
    
    public string Name { get; set; } = string.Empty;

    public DiscreteRandomVariable()
    {
        _distribution = new Dictionary<double, double>();
        _isValidated = false;
    }

    public DiscreteRandomVariable(Dictionary<double, double> distribution, string name = "")
    {
        SetDistribution(distribution);
        Name = name;
    }

    public void SetDistribution(Dictionary<double, double> distribution)
    {
        if (distribution == null)
            throw new ArgumentNullException(nameof(distribution));

        if (distribution.Count == 0)
            throw new ArgumentException("Распределение не может быть пустым");

        if (distribution.Keys.Count != distribution.Keys.Distinct().Count())
            throw new ArgumentException("Значения случайной величины должны быть попарно различны");

        foreach (var prob in distribution.Values)
        {
            if (prob < 0 || prob > 1)
                throw new ArgumentException($"Вероятность {prob} не находится в диапазоне [0, 1]");
        }

        double sum = distribution.Values.Sum();
        if (Math.Abs(sum - 1.0) > 1e-10)
            throw new ArgumentException($"Сумма вероятностей должна равняться 1. Текущая сумма: {sum}");

        _distribution = new Dictionary<double, double>(distribution);
        _isValidated = true;
    }

    public static DiscreteRandomVariable operator *(DiscreteRandomVariable rv, double scalar)
    {
        if (rv == null) throw new ArgumentNullException(nameof(rv));
        var newDistribution = new Dictionary<double, double>();
        foreach (var kvp in rv._distribution)
            newDistribution[kvp.Key * scalar] = kvp.Value;
        return new DiscreteRandomVariable(newDistribution, rv.Name);
    }

    public static DiscreteRandomVariable operator *(double scalar, DiscreteRandomVariable rv) => rv * scalar;

    public static DiscreteRandomVariable operator +(DiscreteRandomVariable rv1, DiscreteRandomVariable rv2)
    {
        if (rv1 == null || rv2 == null) throw new ArgumentNullException();
        var newDistribution = new Dictionary<double, double>();
        foreach (var kvp1 in rv1._distribution)
            foreach (var kvp2 in rv2._distribution)
            {
                double sumValue = kvp1.Key + kvp2.Key;
                double probability = kvp1.Value * kvp2.Value;
                if (newDistribution.ContainsKey(sumValue))
                    newDistribution[sumValue] += probability;
                else
                    newDistribution[sumValue] = probability;
            }
        return new DiscreteRandomVariable(newDistribution, $"{rv1.Name}+{rv2.Name}");
    }

    public static DiscreteRandomVariable operator *(DiscreteRandomVariable rv1, DiscreteRandomVariable rv2)
    {
        if (rv1 == null || rv2 == null) throw new ArgumentNullException();
        var newDistribution = new Dictionary<double, double>();
        foreach (var kvp1 in rv1._distribution)
            foreach (var kvp2 in rv2._distribution)
            {
                double productValue = kvp1.Key * kvp2.Key;
                double probability = kvp1.Value * kvp2.Value;
                if (newDistribution.ContainsKey(productValue))
                    newDistribution[productValue] += probability;
                else
                    newDistribution[productValue] = probability;
            }
        return new DiscreteRandomVariable(newDistribution, $"{rv1.Name}*{rv2.Name}");
    }

    public double GetExpectedValue()
    {
        EnsureValidated();
        return _distribution.Sum(kvp => kvp.Key * kvp.Value);
    }

    public double GetVariance()
    {
        EnsureValidated();
        double expectedValue = GetExpectedValue();
        return _distribution.Sum(kvp => Math.Pow(kvp.Key - expectedValue, 2) * kvp.Value);
    }

    public double GetStandardDeviation() => Math.Sqrt(GetVariance());

    public double GetSkewness()
    {
        EnsureValidated();
        double expectedValue = GetExpectedValue();
        double variance = GetVariance();
        if (variance < 1e-10) return 0;
        double stdDev = Math.Sqrt(variance);
        double thirdCentralMoment = _distribution.Sum(kvp => Math.Pow(kvp.Key - expectedValue, 3) * kvp.Value);
        return thirdCentralMoment / Math.Pow(stdDev, 3);
    }

    public double GetKurtosis()
    {
        EnsureValidated();
        double expectedValue = GetExpectedValue();
        double variance = GetVariance();
        if (variance < 1e-10) return 0;
        double fourthCentralMoment = _distribution.Sum(kvp => Math.Pow(kvp.Key - expectedValue, 4) * kvp.Value);
        return fourthCentralMoment / Math.Pow(variance, 2) - 3;
    }

    public void SerializeJson(string filePath)
    {
        try
        {
            var data = new SerializableData { Distribution = _distribution, Name = Name };
            var options = new JsonSerializerOptions { WriteIndented = true };
            string jsonString = JsonSerializer.Serialize(data, options);
            File.WriteAllText(filePath, jsonString);
        }
        catch (Exception ex)
        {
            throw new InvalidOperationException($"Ошибка при JSON-сериализации: {ex.Message}", ex);
        }
    }

    public static DiscreteRandomVariable DeserializeJson(string filePath)
    {
        try
        {
            string jsonString = File.ReadAllText(filePath);
            var data = JsonSerializer.Deserialize<SerializableData>(jsonString);
            if (data == null || data.Distribution == null)
                throw new InvalidOperationException("Не удалось десериализовать распределение");
            var rv = new DiscreteRandomVariable(data.Distribution, data.Name ?? "");
            return rv;
        }
        catch (Exception ex)
        {
            throw new InvalidOperationException($"Ошибка при JSON-десериализации: {ex.Message}", ex);
        }
    }

    public IReadOnlyDictionary<double, double> GetDistribution()
    {
        EnsureValidated();
        return new Dictionary<double, double>(_distribution);
    }

    public override string ToString()
    {
        EnsureValidated();
        return $"Дискретная случайная величина {Name}\n" +
               $"Значения: {string.Join(", ", _distribution.Keys)}\n" +
               $"Вероятности: {string.Join(", ", _distribution.Values)}\n" +
               $"Мат. ожидание: {GetExpectedValue():F4}\n" +
               $"Дисперсия: {GetVariance():F4}\n" +
               $"Асимметрия: {GetSkewness():F4}\n" +
               $"Эксцесс: {GetKurtosis():F4}";
    }

    private void EnsureValidated()
    {
        if (!_isValidated)
            throw new InvalidOperationException("Распределение не было корректно инициализировано");
    }

    [Serializable]
    private class SerializableData
    {
        public Dictionary<double, double> Distribution { get; set; } = new();
        public string Name { get; set; } = "";
    }
}