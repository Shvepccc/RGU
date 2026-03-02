using System;
using System.Globalization;
using Avalonia.Data.Converters;

namespace test.Converters  // замените на имя вашего проекта
{
    public class NullableDoubleConverter : IValueConverter
    {
        public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
        {
            return value?.ToString() ?? "";
        }

        public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
        {
            if (value == null || string.IsNullOrWhiteSpace(value.ToString()))
            {
                return 0.0;
            }
            
            if (double.TryParse(value.ToString(), NumberStyles.Any, CultureInfo.InvariantCulture, out double result))
            {
                // Ограничиваем значение в пределах [0, 1]
                return Math.Max(0, Math.Min(1, result));
            }
            
            return 0.0;
        }
    }
}