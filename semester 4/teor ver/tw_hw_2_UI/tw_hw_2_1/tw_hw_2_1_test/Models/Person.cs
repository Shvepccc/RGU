using System;

namespace tw_hw_2_1_test.Models
{
    public class Person
    {
        public int Id { get; set; }
        public double X { get; set; }
        public double Y { get; set; }
        public PersonState State { get; set; }
        
        public Person(int id)
        {
            Id = id;
            State = PersonState.Healthy;
            
            // Случайные координаты для отображения
            Random rand = new Random();
            X = rand.NextDouble() * 800 + 100;
            Y = rand.NextDouble() * 600 + 50;
        }
    }
    
    public enum PersonState
    {
        Healthy,    // Здоров - зеленый
        Infected,   // Болен - красный
        Recovered   // Исцелен - синий
    }
}