using System.Collections.Generic;

namespace task_2.Models;

public class tree_node
{
    public int node_id { get; set; }
    public int level { get; set; }
    public double x { get; set; }
    public double y { get; set; }
    public double stop_probability { get; set; }
    public List<tree_node> children { get; set; } = new();
    public bool IsInPath { get; set; }
}