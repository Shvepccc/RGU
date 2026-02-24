// Models/Person.cs
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace EpidemicSimulator.Models
{
    public enum PersonState { Healthy, Infected, Recovered }

    public class Person : INotifyPropertyChanged
    {
        private double _x;
        private double _y;
        private PersonState _state;
        private bool _isHighlighted;

        public int Id { get; set; }
        public List<Person> Neighbors { get; } = new List<Person>();

        public double X
        {
            get => _x;
            set { _x = value; OnPropertyChanged(); }
        }

        public double Y
        {
            get => _y;
            set { _y = value; OnPropertyChanged(); }
        }

        public PersonState State
        {
            get => _state;
            set { _state = value; OnPropertyChanged(); }
        }

        public bool IsHighlighted
        {
            get => _isHighlighted;
            set { _isHighlighted = value; OnPropertyChanged(); }
        }

        public event PropertyChangedEventHandler PropertyChanged;
        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}