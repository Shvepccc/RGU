namespace task_2.Models;

public class GraphItem
{
    public GraphItem(string name, tree_node node) { Name = name; Node = node; }
    public string Name { get; private set; }
    public tree_node Node { get; set; }
}