using System;
using System.IO;
using System.Threading.Tasks;
using Avalonia;
using Avalonia.Controls;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Platform.Storage;
using tw_hw_9_UI_task_1.Models;

namespace tw_hw_9_UI_task_1.Models;

public class VariableStorageService
{
    public async Task<DiscreteRandomVariable?> ImportVariableAsync()
    {
        try
        {
            if (Application.Current?.ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
            {
                var topLevel = TopLevel.GetTopLevel(desktop.MainWindow);
                if (topLevel == null) return null;

                var files = await topLevel.StorageProvider.OpenFilePickerAsync(new FilePickerOpenOptions
                {
                    Title = "Импорт случайной величины",
                    AllowMultiple = false,
                    FileTypeFilter = new[]
                    {
                        new FilePickerFileType("JSON файлы") { Patterns = new[] { "*.json" } }
                    }
                });

                if (files.Count >= 1)
                {
                    return DiscreteRandomVariable.DeserializeJson(files[0].Path.LocalPath);
                }
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Ошибка импорта: {ex.Message}");
        }

        return null;
    }
    
    public async Task<bool> ExportVariableAsync(DiscreteRandomVariable variable, char name)
    {
        try
        {
            if (Application.Current?.ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
            {
                var topLevel = TopLevel.GetTopLevel(desktop.MainWindow);
                if (topLevel == null) return false;

                var file = await topLevel.StorageProvider.SaveFilePickerAsync(new FilePickerSaveOptions
                {
                    Title = $"Экспорт переменной {name}",
                    DefaultExtension = "json",
                    SuggestedFileName = $"variable_{name}.json",
                    FileTypeChoices = new[]
                    {
                        new FilePickerFileType("JSON файлы") { Patterns = new[] { "*.json" } }
                    }
                });

                if (file != null)
                {
                    variable.SerializeJson(file.Path.LocalPath);
                    return true;
                }
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Ошибка экспорта: {ex.Message}");
        }

        return false;
    }
}