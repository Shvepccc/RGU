using System.Collections.Generic;
using System.Collections.ObjectModel;
using CommunityToolkit.Mvvm.ComponentModel;

namespace test.ViewModels;

public class User
{
    public User(int  id, string name, int age, string email, bool isActive)
    {
        Id = id;
        Name = name;
        Age = age;
    }

    public int Id { get; set; }
    public string Name { get; set; }
    public int Age { get; set; }
}

public partial class MainWindowViewModel : ViewModelBase
{
    [ObservableProperty]
    private double _p1 = 0.5;
    
    [ObservableProperty]
    private double _p2 = 0.2;
    
    public ObservableCollection<User> Users { get; }
    
    
    public MainWindowViewModel()
    {
        P1 = _p1;
        P2 = _p2;
        var temp_use = new List<User>
        {
            new User (1, "Иван Иванов", 30, "ivan@mail.com", true),
            new User (2, "Иван Иванов", 30, "ivan@mail.com", false),
            new User (3, "Иван Иванов", 30, "ivan@mail.com", true)
        };
        
        Users = new ObservableCollection<User>(temp_use);
    }
}
