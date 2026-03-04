namespace task_2.Models;

public class tree_builder
{
    private int _node_counter = 0;

    public tree_node build_m_ary_tree(int m, int depth, double stop_probability)
    {
        return build_node_recursive(m, depth, 0, stop_probability, 0);
    }

    private tree_node build_node_recursive(int m, int max_depth, int current_level, double stop_probability, int child_number)
    {
        var node = new tree_node();
        node.node_id = _node_counter;
        _node_counter++;
        node.level = current_level;
        node.stop_probability = stop_probability;

        if(current_level < max_depth)
        {
            for(int i = 0; i < m; i++)
            {
                node.children.Add(build_node_recursive(m, max_depth, current_level + 1, stop_probability, child_number+i));
            }
        }

        return node;
    }
}