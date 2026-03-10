using System.IO;
using System.Text.Json;
using System.Threading.Tasks;
using Avalonia.Platform.Storage;
using task_2.Models;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia;

namespace task_2.Services;

public class ConfigService
{
    public async Task<SimulationConfig?> LoadConfigAsync()
    {
        try
        {
            if (Application.Current?.ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
            {
                var mainWindow = desktop.MainWindow;
                if (mainWindow == null) return null;
                
                var files = await mainWindow.StorageProvider.OpenFilePickerAsync(new FilePickerOpenOptions
                {
                    Title = "Загрузить конфигурацию",
                    AllowMultiple = false,
                    FileTypeFilter = new[]
                    {
                        new FilePickerFileType("JSON файлы")
                        {
                            Patterns = new[] { "*.json" }
                        }
                    }
                });

                if (files.Count >= 1)
                {
                    var file = files[0];
                    await using var stream = await file.OpenReadAsync();
                    using var reader = new StreamReader(stream);
                    string json = await reader.ReadToEndAsync();
                    
                    return JsonSerializer.Deserialize<SimulationConfig>(json) ?? new SimulationConfig();
                }
            }
        }
        catch { }
        
        return null;
    }

    public async Task<bool> SaveConfigAsync(SimulationConfig config)
    {
        try
        {
            if (Application.Current?.ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
            {
                var mainWindow = desktop.MainWindow;
                if (mainWindow == null) return false;
                
                var file = await mainWindow.StorageProvider.SaveFilePickerAsync(new FilePickerSaveOptions
                {
                    Title = "Сохранить конфигурацию",
                    DefaultExtension = "json",
                    SuggestedFileName = "config.json",
                    FileTypeChoices = new[]
                    {
                        new FilePickerFileType("JSON файлы")
                        {
                            Patterns = new[] { "*.json" }
                        }
                    }
                });

                if (file != null)
                {
                    string json = JsonSerializer.Serialize(config, new JsonSerializerOptions { WriteIndented = true });
                    await using var stream = await file.OpenWriteAsync();
                    using var writer = new StreamWriter(stream);
                    await writer.WriteAsync(json);
                    
                    return true;
                }
            }
        }
        catch { }
        
        return false;
    }
}