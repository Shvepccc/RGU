#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <set>
#include <map>
#include <chrono>
#include <iomanip>
#include <queue>
#include <stack>
#include <cstring>

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
    
    bool has_edge(int u, int v) const
    {
        for (const auto& e : edges)
        {
            if ((e.u == u && e.v == v) || (e.u == v && e.v == u))
                return true;
        }
        return false;
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
    struct CycleInfo
    {
        int length;
        int total_weight;
        std::vector<int> vertices;
    };
    
    static CycleInfo find_best_cycle_bfs(const graph& g, int start, int max_search_depth)
    {
        int n = g.vertex_count;
        std::vector<int> dist(n, -1);
        std::vector<int> parent(n, -1);
        std::vector<int> parent_weight(n, 0);
        std::queue<int> q;
        
        dist[start] = 0;
        q.push(start);
        
        CycleInfo best = {0, 0, {}};
        
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            
            if (dist[u] >= max_search_depth)
                continue;
            
            for (const auto& neighbor : g.adjacency_list[u])
            {
                int v = neighbor.first;
                int w = neighbor.second;
                
                if (dist[v] == -1)
                {
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    parent_weight[v] = w;
                    q.push(v);
                }
                else if (parent[u] != v && parent[v] != u && v != start)
                {
                    int cycle_len = dist[u] + dist[v] + 1;
                    if (cycle_len > best.length && cycle_len <= max_search_depth)
                    {
                        best.length = cycle_len;
                    }
                }
            }
        }
        
        return best;
    }
    
    static int find_longest_cycle(const graph& g)
    {
        int n = g.vertex_count;
        int longest = 0;
        
        int max_depth = std::min(n, 50);
        int start_limit = std::min(n, 200);
        
        std::vector<int> nodes(n);
        std::iota(nodes.begin(), nodes.end(), 0);
        
        std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
        std::shuffle(nodes.begin(), nodes.end(), rng);
        
        for (int idx = 0; idx < start_limit; ++idx)
        {
            int start = nodes[idx];
            CycleInfo cycle = find_best_cycle_bfs(g, start, max_depth);
            if (cycle.length > longest)
            {
                longest = cycle.length;
                if (longest >= max_depth)
                    break;
            }
        }
        
        return longest;
    }
    
    static int find_max_weight_cycle_edges(const graph& g)
    {
        int n = g.vertex_count;
        int max_edges = 0;
        
        std::vector<std::vector<int>> dist(n, std::vector<int>(n, -1));
        
        for (int start = 0; start < std::min(n, 100); ++start)
        {
            std::queue<int> q;
            std::vector<int> parent(n, -1);
            dist[start][start] = 0;
            q.push(start);
            
            while (!q.empty())
            {
                int u = q.front();
                q.pop();
                
                for (const auto& neighbor : g.adjacency_list[u])
                {
                    int v = neighbor.first;
                    
                    if (dist[start][v] == -1)
                    {
                        dist[start][v] = dist[start][u] + 1;
                        parent[v] = u;
                        q.push(v);
                    }
                    else if (parent[u] != v && parent[v] != u && v != start && dist[start][u] + dist[start][v] + 1 > max_edges)
                    {
                        int cycle_edges = dist[start][u] + dist[start][v] + 1;
                        if (cycle_edges > max_edges && cycle_edges <= 100)
                        {
                            max_edges = cycle_edges;
                        }
                    }
                }
            }
        }
        
        return max_edges;
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
                if (edges_count == g.vertex_count - 1)
                    break;
            }
        }
        return total_weight;
    }

    static std::pair<int, int> get_cycle_metrics(const graph& g)
    {
        int max_len = find_longest_cycle(g);
        int max_weight_edges = find_max_weight_cycle_edges(g);
        return {max_len, max_weight_edges};
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
            
            if (v_size == 1)
            {
                continue;
            }
            
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
            
            int complete_edges = v_size * (v_size - 1) / 2;
            if (e_size == complete_edges && v_size > 1)
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
        std::uniform_int_distribution<int> weight_dist(1, 20);
        dsu connection_dsu(n);

        std::vector<int> vertices(n);
        std::iota(vertices.begin(), vertices.end(), 0);
        std::shuffle(vertices.begin(), vertices.end(), rng);
        
        for (int i = 1; i < n; ++i)
        {
            int u = vertices[i];
            int v = vertices[rng() % i];
            int w = weight_dist(rng);
            g.add_edge(u, v, w);
            connection_dsu.unite(u, v);
        }
        
        int extra_edges = rng() % (n * 2);
        for (int i = 0; i < extra_edges; ++i)
        {
            int u = rng() % n;
            int v = rng() % n;
            if (u != v && !g.has_edge(u, v))
            {
                g.add_edge(u, v, weight_dist(rng));
            }
        }
        
        return g;
    }

    static graph generate_random(int n, double p)
    {
        graph g(n);
        std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
        std::uniform_real_distribution<double> prob_dist(0.0, 1.0);
        std::uniform_int_distribution<int> weight_dist(1, 20);

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
    
    static graph generate_sparse(int n)
    {
        double p = 2.0 * std::log(n) / n;
        return generate_random(n, std::min(p, 0.5));
    }
};

int main()
{
    int n = 150;
    int iterations = 100;
    double edge_prob = 0.05;
    
    std::cout << "Testing for n = " << n << " with p = " << edge_prob << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    
    discrete_random_variable var_a, var_b, var_d, var_e, var_f, var_g, var_h;

    auto start_time = std::chrono::steady_clock::now();
    
    int progress_counter = 0;

    for (int i = 0; i < iterations; ++i)
    {
        progress_counter++;
        if (progress_counter >= iterations / 10 || i == iterations - 1)
        {
            auto current_time = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
            auto percent = 100.0 * (i + 1) / iterations;
            std::cout << "\rProgress: " << std::fixed << std::setprecision(1) << percent << "% "
                      << "[" << std::string(int(40 * (i + 1) / iterations), '#')
                      << std::string(40 - int(40 * (i + 1) / iterations), ' ') << "] "
                      << "Elapsed: " << elapsed << "s   ";
            std::cout.flush();
            progress_counter = 0;
        }
        
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
    
    std::cout << "\nRESULTS for n = " << n << " (" << iterations << " iterations)" << std::endl;
    std::cout << std::fixed << std::setprecision(5);
    std::cout << "A (MST weight):         E = " << std::setw(8) << var_a.get_expected_value() 
              << "   D = " << std::setw(8) << var_a.get_variance() << std::endl;
    std::cout << "B (Longest cycle len):  E = " << std::setw(8) << var_b.get_expected_value() 
              << "   D = " << std::setw(8) << var_b.get_variance() << std::endl;
    std::cout << "D (Max weight cycle):   E = " << std::setw(8) << var_d.get_expected_value() 
              << "   D = " << std::setw(8) << var_d.get_variance() << std::endl;
    std::cout << "E (Isolated vertices):  E = " << std::setw(8) << var_e.get_expected_value() 
              << "   D = " << std::setw(8) << var_e.get_variance() << std::endl;
    std::cout << "F (Tree components):    E = " << std::setw(8) << var_f.get_expected_value() 
              << "   D = " << std::setw(8) << var_f.get_variance() << std::endl;
    std::cout << "G (Total components):   E = " << std::setw(8) << var_g.get_expected_value() 
              << "   D = " << std::setw(8) << var_g.get_variance() << std::endl;
    std::cout << "H (Complete components):E = " << std::setw(8) << var_h.get_expected_value() 
              << "   D = " << std::setw(8) << var_h.get_variance() << std::endl;

    return 0;
}