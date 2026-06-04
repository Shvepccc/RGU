namespace task_1.Converters;

using Avalonia.Data.Converters;
using Avalonia.Media;
using System;
using System.Globalization;
using task_1.Models;

public class StateToBrushConverter : IValueConverter
{
    public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
    {
        if (value is HealthState state)
        {
            return state switch
            {
                HealthState.Infected => Brushes.Salmon,
                HealthState.Healed => Brushes.LightGreen,
                _ => Brushes.LightGray
            };
        }
        return Brushes.Gray;
    }

    public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture) => throw new NotImplementedException();
}