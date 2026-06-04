namespace task_1.Models;

using System;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;
using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Platform.Storage;

public class GraphLoaderService
{
    public async Task<PersonNode[]?> LoadGraphAsync()
    {
        if (Application.Current?.ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
        {
            var mainWindow = desktop.MainWindow;
            if (mainWindow == null) return null;
            
            var files = await mainWindow.StorageProvider.OpenFilePickerAsync(new FilePickerOpenOptions
            {
                Title = "Выберите файл графа (.txt)",
                AllowMultiple = false,
                FileTypeFilter = new[] { new FilePickerFileType("Текстовые файлы") { Patterns = new[] { "*.txt" } } }
            });

            if (files.Count >= 1)
            {
                await using var stream = await files[0].OpenReadAsync();
                using var reader = new StreamReader(stream);
                return ParseGraph(await reader.ReadToEndAsync());
            }
        }
        return null;
    }

    private PersonNode[] ParseGraph(string content)
    {
        var idToNode = new Dictionary<int, PersonNode>();
        var adjacencyList = new Dictionary<int, List<int>>();
        
        var lines = content.Split(new[] { '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);

        foreach (var line in lines)
        {
            var trimmed = line.Trim();
            if (trimmed.StartsWith("#") || string.IsNullOrWhiteSpace(trimmed)) continue;

            var parts = trimmed.Split(new[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);
            if (parts.Length >= 2 && int.TryParse(parts[0], out int id1) && int.TryParse(parts[1], out int id2))
            {
                if (!idToNode.ContainsKey(id1)) { idToNode[id1] = new PersonNode { Id = id1 }; adjacencyList[id1] = new List<int>(); }
                if (!idToNode.ContainsKey(id2)) { idToNode[id2] = new PersonNode { Id = id2 }; adjacencyList[id2] = new List<int>(); }

                if (!adjacencyList[id1].Contains(id2)) adjacencyList[id1].Add(id2);
                if (!adjacencyList[id2].Contains(id1)) adjacencyList[id2].Add(id1);
            }
        }
        
        var nodesList = new List<PersonNode>(idToNode.Values);
        var idToIndexMap = new Dictionary<int, int>();
        
        for (int i = 0; i < nodesList.Count; i++)
        {
            idToIndexMap[nodesList[i].Id] = i;
        }

        for (int i = 0; i < nodesList.Count; i++)
        {
            var node = nodesList[i];
            var neighborsIds = adjacencyList[node.Id];
            var indices = new int[neighborsIds.Count];
            for (int j = 0; j < neighborsIds.Count; j++)
            {
                indices[j] = idToIndexMap[neighborsIds[j]];
            }
            node.NeighborIndices = indices;
        }

        return nodesList.ToArray();
    }
}