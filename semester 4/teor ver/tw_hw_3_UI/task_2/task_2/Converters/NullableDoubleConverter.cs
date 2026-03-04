using Avalonia.Data.Converters;
using System;
using System.Globalization;

namespace task_2.Converters;

public class NullableDoubleConverter : IValueConverter
{
    public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        // При конвертации из источника в UI - возвращаем как есть
        return value;
    }

    public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
    {
        // Если пришла пустая строка (пользователь стер всё) - возвращаем 0.0
        if (value is string stringValue && string.IsNullOrWhiteSpace(stringValue))
        {
            return 0.0;
        }
        
        // Пробуем преобразовать в double, если не получается - возвращаем 0.0
        if (value != null)
        {
            string? strValue = value.ToString();
            if (!string.IsNullOrWhiteSpace(strValue) && 
                double.TryParse(strValue, NumberStyles.Any, CultureInfo.InvariantCulture, out double result))
            {
                return result;
            }
        }
        
        return 0.0;
    }
}