// Program.cs
using System.Reflection;

namespace tw_hw_6;

internal class Program
{
    static void Main(string[] args)
    {
        if (args.Length == 0)
        {
            ShowHelp();
            return;
        }

        if (!int.TryParse(args[0], out int taskNumber))
        {
            Console.WriteLine($"Ошибка: '{args[0]}' не является номером задания");
            return;
        }

        // Находим и запускаем задание
        var task = FindTask(taskNumber);
        
        if (task != null)
        {
            //Console.Clear(); // Опционально: очищаем консоль перед запуском
            task.Run(args);
        }
        else
        {
            Console.WriteLine($"Задание #{taskNumber} не найдено");
            ShowAvailableTasks();
        }
    }

    /// <summary>
    /// Находит класс задания по номеру через рефлексию
    /// </summary>
    static ITask? FindTask(int number)
    {
        // Получаем все типы в текущей сборке
        var taskTypes = Assembly.GetExecutingAssembly()
            .GetTypes()
            .Where(t => typeof(ITask).IsAssignableFrom(t) && !t.IsInterface && !t.IsAbstract);

        foreach (var type in taskTypes)
        {
            // Создаём экземпляр задания
            var task = Activator.CreateInstance(type) as ITask;
            if (task != null && task.Number == number)
            {
                return task;
            }
        }

        return null;
    }

    static void ShowHelp()
    {
        Console.WriteLine("=== Диспетчер заданий ===");
        Console.WriteLine("Использование: dotnet run -- <номер_задания>");
        Console.WriteLine("Пример: dotnet run -- 1");
        Console.WriteLine();
        ShowAvailableTasks();
    }

    static void ShowAvailableTasks()
    {
        Console.WriteLine("Доступные задания:");
        
        var taskTypes = Assembly.GetExecutingAssembly()
            .GetTypes()
            .Where(t => typeof(ITask).IsAssignableFrom(t) && !t.IsInterface && !t.IsAbstract);

        var tasks = new List<ITask>();
        foreach (var type in taskTypes)
        {
            var task = Activator.CreateInstance(type) as ITask;
            if (task != null)
            {
                tasks.Add(task);
            }
        }

        // Сортируем по номеру и выводим
        foreach (var task in tasks.OrderBy(t => t.Number))
        {
            Console.WriteLine($"  {task.Number} - {task.GetType().Name}");
        }
        
        if (!tasks.Any())
        {
            Console.WriteLine("  (нет доступных заданий)");
        }
    }
}