using System;
using System.IO;
using System.Threading.Tasks;
using Avalonia;
using Avalonia.Controls;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Platform.Storage;
using tw_hw_9_UI_task_2.Models;

namespace tw_hw_9_UI_task_2.Models;

public class VariableStorageService
{
    public async Task<DiscreteRandomVariable?> ImportVariableAsync()
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
        return null;
    }
}