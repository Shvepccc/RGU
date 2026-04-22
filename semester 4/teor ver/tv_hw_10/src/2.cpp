#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <set>
#include <map>
#include <chrono>
#include <iomanip>

class discrete_random_variable
{
private:
    std::vector<double> samples;

public:
    void add_sample(double value)
    {
        samples.push_back(value);
    }

    double get_expected_value() const
    {
        if (samples.empty())
        {
            return 0.0;
        }
        double sum = std::accumulate(samples.begin(), samples.end(), 0.0);
        return sum / static_cast<double>(samples.size());
    }

    double get_variance() const
    {
        if (samples.size() < 2)
        {
            return 0.0;
        }
        double mean = get_expected_value();
        double sum_sq = 0.0;
        for (double val : samples)
        {
            sum_sq += (val - mean) * (val - mean);
        }
        return sum_sq / static_cast<double>(samples.size());
    }
};

struct edge
{
    int u;
    int v;
    int weight;
};

class graph
{
public:
    int vertex_count;
    std::vector<edge> edges;
    std::vector<std::vector<std::pair<int, int>>> adjacency_list;

    graph(int n) : vertex_count(n)
    {
        adjacency_list.resize(n);
    }

    void add_edge(int u, int v, int w)
    {
        edges.push_back({u, v, w});
        adjacency_list[u].push_back({v, w});
        adjacency_list[v].push_back({u, w});
    }
};

class dsu
{
private:
    std::vector<int> parent;
    std::vector<int> rank;

public:
    dsu(int n)
    {
        parent.resize(n);
        rank.resize(n, 0);
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int i)
    {
        if (parent[i] == i)
        {
            return i;
        }
        return parent[i] = find(parent[i]);
    }

    bool unite(int i, int j)
    {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j)
        {
            if (rank[root_i] < rank[root_j])
            {
                parent[root_i] = root_j;
            }
            else if (rank[root_i] > rank[root_j])
            {
                parent[root_j] = root_i;
            }
            else
            {
                parent[root_i] = root_j;
                rank[root_j]++;
            }
            return true;
        }
        return false;
    }
};

class graph_analyzer
{
private:
    static void find_cycle_heuristic(int u, int start_node, int current_len, int current_weight, const graph& g, std::vector<bool>& visited, int& max_len, int& max_weight, int& edges_at_max_weight, int depth_limit, int& steps)
    {
        if (steps > 10000)
        {
            return;
        }
        steps++;
        visited[u] = true;

        for (const auto& neighbor : g.adjacency_list[u])
        {
            int v = neighbor.first;
            int w = neighbor.second;

            if (v == start_node && current_len >= 2)
            {
                int total_len = current_len + 1;
                int total_weight = current_weight + w;
                if (total_len > max_len)
                {
                    max_len = total_len;
                }
                if (total_weight > max_weight)
                {
                    max_weight = total_weight;
                    edges_at_max_weight = total_len;
                }
            }
            else if (!visited[v] && current_len < depth_limit)
            {
                find_cycle_heuristic(v, start_node, current_len + 1, current_weight + w, g, visited, max_len, max_weight, edges_at_max_weight, depth_limit, steps);
            }
        }
        visited[u] = false;
    }

public:
    static double get_mst_weight(const graph& g)
    {
        std::vector<edge> sorted_edges = g.edges;
        std::sort(sorted_edges.begin(), sorted_edges.end(), [](const edge& a, const edge& b)
        {
            return a.weight < b.weight;
        });

        dsu tree_dsu(g.vertex_count);
        double total_weight = 0;
        int edges_count = 0;

        for (const auto& e : sorted_edges)
        {
            if (tree_dsu.unite(e.u, e.v))
            {
                total_weight += e.weight;
                edges_count++;
            }
        }
        return total_weight;
    }

    static std::pair<int, int> get_cycle_metrics(const graph& g)
    {
        int max_len = 0;
        int max_weight = 0;
        int edges_at_max_weight = 0;
        int n = g.vertex_count;

        std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
        std::vector<int> nodes(n);
        std::iota(nodes.begin(), nodes.end(), 0);
        std::shuffle(nodes.begin(), nodes.end(), rng);

        int nodes_to_check = std::min(n, 1000);
        for (int i = 0; i < nodes_to_check; ++i)
        {
            std::vector<bool> visited(n, false);
            int steps = 0;
            find_cycle_heuristic(nodes[i], nodes[i], 0, 0, g, visited, max_len, max_weight, edges_at_max_weight, 200, steps);
        }

        return {max_len, edges_at_max_weight};
    }

    static int count_isolated(const graph& g)
    {
        int count = 0;
        for (int i = 0; i < g.vertex_count; ++i)
        {
            if (g.adjacency_list[i].empty())
            {
                count++;
            }
        }
        return count;
    }

    static std::vector<int> get_components_data(const graph& g, int& total_components)
    {
        dsu comp_dsu(g.vertex_count);
        for (const auto& e : g.edges)
        {
            comp_dsu.unite(e.u, e.v);
        }

        std::map<int, std::vector<int>> components;
        for (int i = 0; i < g.vertex_count; ++i)
        {
            components[comp_dsu.find(i)].push_back(i);
        }

        total_components = static_cast<int>(components.size());
        int tree_count = 0;
        int complete_count = 0;

        for (const auto& pair : components)
        {
            const std::vector<int>& vertices = pair.second;
            int v_size = static_cast<int>(vertices.size());
            int e_size = 0;

            std::set<int> v_set(vertices.begin(), vertices.end());
            for (int v : vertices)
            {
                for (const auto& neighbor : g.adjacency_list[v])
                {
                    if (v < neighbor.first && v_set.count(neighbor.first))
                    {
                        e_size++;
                    }
                }
            }

            if (e_size == v_size - 1)
            {
                tree_count++;
            }
            if (e_size == (v_size * (v_size - 1)) / 2)
            {
                complete_count++;
            }
        }

        return {tree_count, complete_count};
    }
};

class graph_generator
{
public:
    static graph generate_connected(int n)
    {
        graph g(n);
        std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<int> weight_dist(1, 10);
        dsu connection_dsu(n);

        int connected_edges = 0;
        while (connected_edges < n - 1)
        {
            int u = rng() % n;
            int v = rng() % n;
            if (u != v && connection_dsu.unite(u, v))
            {
                g.add_edge(u, v, weight_dist(rng));
                connected_edges++;
            }
        }
        return g;
    }

    static graph generate_random(int n, double p)
    {
        graph g(n);
        std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
        std::uniform_real_distribution<double> prob_dist(0.0, 1.0);
        std::uniform_int_distribution<int> weight_dist(1, 10);

        for (int i = 0; i < n; ++i)
        {
            for (int j = i + 1; j < n; ++j)
            {
                if (prob_dist(rng) < p)
                {
                    g.add_edge(i, j, weight_dist(rng));
                }
            }
        }
        return g;
    }
};

int main()
{
    int n = 100;
    int iterations = 100;
    double edge_prob = 0.5;

    discrete_random_variable var_a, var_b, var_d, var_e, var_f, var_g, var_h;

    auto start_time = std::chrono::steady_clock::now();

    for (int i = 0; i < iterations; ++i)
    {
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
        auto estimated_total = (elapsed * iterations) / (i + 1);
        auto remaining = estimated_total - elapsed;
        
        std::cout << "\rПрогресс: " << std::setw(3) << (100 * (i + 1) / iterations) << "% "
                << "[" << std::string(40 * (i + 1) / iterations, '#')
                << std::string(40 - 40 * (i + 1) / iterations, ' ') << "] "
                << "Осталось: " << remaining << "с   ";
        std::cout.flush();

        graph g_conn = graph_generator::generate_connected(n);
        var_a.add_sample(graph_analyzer::get_mst_weight(g_conn));

        graph g_rand = graph_generator::generate_random(n, edge_prob);
        
        auto cycle_res = graph_analyzer::get_cycle_metrics(g_rand);
        var_b.add_sample(cycle_res.first);
        var_d.add_sample(cycle_res.second);

        var_e.add_sample(graph_analyzer::count_isolated(g_rand));

        int total_comp = 0;
        auto comp_metrics = graph_analyzer::get_components_data(g_rand, total_comp);
        var_f.add_sample(comp_metrics[0]);
        var_g.add_sample(total_comp);
        var_h.add_sample(comp_metrics[1]);
    }

    std::cout << "\n\nResults for n = " << n << " (" << iterations << " iterations):" << std::endl;
    std::cout << "A (MST weight): E=" << var_a.get_expected_value() << " D=" << var_a.get_variance() << std::endl;
    std::cout << "B (Longest cycle): E=" << var_b.get_expected_value() << " D=" << var_b.get_variance() << std::endl;
    std::cout << "D (Max weight cycle edges): E=" << var_d.get_expected_value() << " D=" << var_d.get_variance() << std::endl;
    std::cout << "E (Isolated vertices): E=" << var_e.get_expected_value() << " D=" << var_e.get_variance() << std::endl;
    std::cout << "F (Tree components): E=" << var_f.get_expected_value() << " D=" << var_f.get_variance() << std::endl;
    std::cout << "G (Total components): E=" << var_g.get_expected_value() << " D=" << var_g.get_variance() << std::endl;
    std::cout << "H (Complete components): E=" << var_h.get_expected_value() << " D=" << var_h.get_variance() << std::endl;

    return 0;
}