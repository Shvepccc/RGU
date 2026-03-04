using CommunityToolkit.Mvvm.ComponentModel;
namespace task_2.Models;

public partial class ProbabilityValue : ObservableObject
{
    [ObservableProperty]
    private double _value;
        
    public ProbabilityValue(double initialValue)
    {
        _value = initialValue;
    }
}