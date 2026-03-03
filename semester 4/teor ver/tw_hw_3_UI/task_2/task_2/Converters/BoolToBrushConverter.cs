using Avalonia.Data.Converters;
using Avalonia.Media;
using System;
using System.Globalization;

namespace task_2.Converters
{
    public class BoolToBrushConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is bool isInPath && isInPath)
            {
                return Brushes.LightGreen;
            }
            return Brushes.LightBlue;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}