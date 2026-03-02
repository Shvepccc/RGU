// App.axaml.cs
using Avalonia;
using Avalonia.Controls;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Markup.Xaml;
using EpidemicSimulator.ViewModels;
using EpidemicSimulator.Views;
using System;

namespace tw_hw_2_1
{
    public partial class App : Application
    {
        public override void Initialize()
        {
            Console.WriteLine("App.Initialize() начат");
            
            try
            {
                AvaloniaXamlLoader.Load(this);
                Console.WriteLine("XAML загружен успешно");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"ОШИБКА в App.Initialize: {ex.Message}");
                throw;
            }
            
            Console.WriteLine("App.Initialize() успешно завершен");
        }

        public override void OnFrameworkInitializationCompleted()
        {
            Console.WriteLine("App.OnFrameworkInitializationCompleted начат");
            
            try
            {
                if (ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
                {
                    Console.WriteLine("Обнаружен Desktop ApplicationLifetime");
                    
                    Console.WriteLine("Создание MainWindow...");
                    var mainWindow = new MainWindow();
                    
                    Console.WriteLine("Создание MainWindowViewModel...");
                    var viewModel = new MainWindowViewModel();
                    
                    Console.WriteLine("Установка DataContext...");
                    mainWindow.DataContext = viewModel;
                    
                    desktop.MainWindow = mainWindow;
                    
                    // ПРИНУДИТЕЛЬНО показываем окно
                    Console.WriteLine("Показываем окно...");
                    mainWindow.Show();
                    
                    // Активируем окно (выводим на передний план)
                    mainWindow.Activate();
                    
                    Console.WriteLine($"Окно показано. Заголовок: {mainWindow.Title}, Размеры: {mainWindow.Width}x{mainWindow.Height}");
                }
                else
                {
                    Console.WriteLine($"ApplicationLifetime имеет другой тип: {ApplicationLifetime?.GetType().Name}");
                }
                
                base.OnFrameworkInitializationCompleted();
                Console.WriteLine("App.OnFrameworkInitializationCompleted успешно завершен");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"ОШИБКА в OnFrameworkInitializationCompleted: {ex.Message}");
                Console.WriteLine($"Стек: {ex.StackTrace}");
            }
        }
    }
}