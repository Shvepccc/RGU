#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <set>
#include <map>
#include <chrono>
#include <functional>

struct Edge
{
    int u;
    int v;
    int weight;
};

class Graph
{
public:
    int n;
    std::vector<Edge> edges;

    Graph(int vertices) : n(vertices) {}

    void add_edge(int u, int v, int w)
    {
        edges.push_back({u, v, w});
    }
};

class RandomGraphGenerator
{
private:
    std::mt19937 rng;
    std::uniform_int_distribution<int> weight_dist;

public:
    RandomGraphGenerator() : 
        rng(std::chrono::steady_clock::now().time_since_epoch().count()), 
        weight_dist(1, 10) 
        {}

    Graph generate_random_graph(int n, double edge_probability = 0.5)
    {
        Graph g(n);
        std::uniform_real_distribution<double> edge_dist(0.0, 1.0);

        for (int i = 0; i < n; ++i)
        {
            for (int j = i + 1; j < n; ++j)
            {
                if (edge_dist(rng) < edge_probability)
                {
                    int w = weight_dist(rng);
                    g.add_edge(i, j, w);
                }
            }
        }

        return g;
    }
};

class DisjointSet
{
private:
    std::vector<int> parent;
    std::vector<int> rank;

public:
    DisjointSet(int n)
    {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; ++i)
        {
            parent[i] = i;
        }
    }

    int find(int x)
    {
        if (parent[x] != x)
        {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(int x, int y)
    {
        int rx = find(x);
        int ry = find(y);
        if (rx != ry)
        {
            if (rank[rx] < rank[ry])
            {
                parent[rx] = ry;
            }
            else if (rank[rx] > rank[ry])
            {
                parent[ry] = rx;
            }
            else
            {
                parent[ry] = rx;
                rank[rx]++;
            }
        }
    }

    bool connected(int x, int y)
    {
        return find(x) == find(y);
    }
};

class GraphAnalyzer
{
public:
    static int mst_weight(const Graph& g)
    {
        if (g.n == 0) return 0;

        std::vector<Edge> sorted_edges = g.edges;
        std::sort(sorted_edges.begin(), sorted_edges.end(),
                  [](const Edge& a, const Edge& b) { return a.weight < b.weight; });

        DisjointSet ds(g.n);
        int total_weight = 0;
        int edges_used = 0;

        for (const Edge& e : sorted_edges)
        {
            if (!ds.connected(e.u, e.v))
            {
                ds.unite(e.u, e.v);
                total_weight += e.weight;
                edges_used++;
                if (edges_used == g.n - 1)
                {
                    break;
                }
            }
        }

        if (edges_used < g.n - 1)
        {
            return -1;
        }

        return total_weight;
    }

    static int longest_cycle_by_edges(const Graph& g)
    {
        int max_len = 0;
        std::vector<std::vector<int>> adj(g.n);

        for (const Edge& e : g.edges)
        {
            adj[e.u].push_back(e.v);
            adj[e.v].push_back(e.u);
        }

        std::vector<std::vector<int>> cycles;

        for (int start = 0; start < g.n; ++start)
        {
            std::vector<std::pair<std::vector<int>, std::vector<bool>>> paths;
            std::vector<int> initial_path;
            std::vector<bool> initial_visited(g.n, false);
            initial_path.push_back(start);
            initial_visited[start] = true;
            paths.push_back({initial_path, initial_visited});

            for (int step = 0; step < g.n; ++step)
            {
                std::vector<std::pair<std::vector<int>, std::vector<bool>>> new_paths;
                for (const auto& p : paths)
                {
                    const std::vector<int>& current_path = p.first;
                    std::vector<bool> visited = p.second;
                    int last = current_path.back();

                    for (int neighbor : adj[last])
                    {
                        if (current_path.size() >= 3 && neighbor == start)
                        {
                            if (static_cast<int>(current_path.size()) > max_len)
                            {
                                max_len = current_path.size();
                            }
                            continue;
                        }

                        if (!visited[neighbor])
                        {
                            std::vector<int> new_path = current_path;
                            std::vector<bool> new_visited = visited;
                            new_path.push_back(neighbor);
                            new_visited[neighbor] = true;
                            new_paths.push_back({new_path, new_visited});
                        }
                    }
                }
                if (new_paths.empty()) break;
                paths = new_paths;
            }
        }

        return max_len;
    }

    static int largest_cycle_edge_count(const Graph& g)
    {
        int max_edges = 0;
        std::vector<std::vector<int>> adj(g.n);

        for (const Edge& e : g.edges)
        {
            adj[e.u].push_back(e.v);
            adj[e.v].push_back(e.u);
        }

        for (int start = 0; start < g.n; ++start)
        {
            std::vector<std::vector<int>> paths;
            std::vector<int> path;
            path.push_back(start);
            paths.push_back(path);

            for (int step = 0; step < g.n * g.n; ++step)
            {
                std::vector<std::vector<int>> new_paths;
                for (const std::vector<int>& p : paths)
                {
                    int last = p.back();
                    for (int neighbor : adj[last])
                    {
                        if (p.size() >= 3 && neighbor == p[0])
                        {
                            if (static_cast<int>(p.size()) > max_edges)
                            {
                                max_edges = p.size();
                            }
                        }
                        else if (std::find(p.begin(), p.end(), neighbor) == p.end())
                        {
                            std::vector<int> new_path = p;
                            new_path.push_back(neighbor);
                            new_paths.push_back(new_path);
                        }
                    }
                }
                if (new_paths.empty()) break;
                paths = new_paths;
            }
        }

        return max_edges;
    }

    static int isolated_vertices_count(const Graph& g)
    {
        std::vector<bool> has_edge(g.n, false);
        for (const Edge& e : g.edges)
        {
            has_edge[e.u] = true;
            has_edge[e.v] = true;
        }

        int count = 0;
        for (int i = 0; i < g.n; ++i)
        {
            if (!has_edge[i])
            {
                count++;
            }
        }
        return count;
    }

    static int count_spanning_trees(const Graph& g)
    {
        if (g.n <= 1) return 1;

        std::vector<std::vector<int>> laplacian(g.n, std::vector<int>(g.n, 0));
        std::vector<int> degree(g.n, 0);

        for (const Edge& e : g.edges)
        {
            degree[e.u]++;
            degree[e.v]++;
        }

        for (int i = 0; i < g.n; ++i)
        {
            laplacian[i][i] = degree[i];
        }

        for (const Edge& e : g.edges)
        {
            laplacian[e.u][e.v]--;
            laplacian[e.v][e.u]--;
        }

        std::vector<std::vector<int>> matrix(g.n - 1, std::vector<int>(g.n - 1));
        for (int i = 0; i < g.n - 1; ++i)
        {
            for (int j = 0; j < g.n - 1; ++j)
            {
                matrix[i][j] = laplacian[i][j];
            }
        }

        return determinant(matrix);
    }

    static int connected_components(const Graph& g)
    {
        DisjointSet ds(g.n);
        for (const Edge& e : g.edges)
        {
            ds.unite(e.u, e.v);
        }

        std::set<int> roots;
        for (int i = 0; i < g.n; ++i)
        {
            roots.insert(ds.find(i));
        }
        return roots.size();
    }

    static int complete_subgraph_components(const Graph& g)
    {
        DisjointSet ds(g.n);
        for (const Edge& e : g.edges)
        {
            ds.unite(e.u, e.v);
        }

        std::map<int, std::vector<int>> components;
        for (int i = 0; i < g.n; ++i)
        {
            components[ds.find(i)].push_back(i);
        }

        std::vector<std::vector<bool>> adj_matrix(g.n, std::vector<bool>(g.n, false));
        for (const Edge& e : g.edges)
        {
            adj_matrix[e.u][e.v] = true;
            adj_matrix[e.v][e.u] = true;
        }

        int complete_count = 0;
        for (const auto& comp : components)
        {
            const std::vector<int>& vertices = comp.second;
            if (vertices.size() <= 1)
            {
                complete_count++;
                continue;
            }

            bool is_complete = true;
            for (size_t i = 0; i < vertices.size() && is_complete; ++i)
            {
                for (size_t j = i + 1; j < vertices.size(); ++j)
                {
                    if (!adj_matrix[vertices[i]][vertices[j]])
                    {
                        is_complete = false;
                        break;
                    }
                }
            }
            if (is_complete)
            {
                complete_count++;
            }
        }
        return complete_count;
    }

private:
    static int determinant(std::vector<std::vector<int>> matrix)
    {
        int n = matrix.size();
        int det = 1;

        for (int i = 0; i < n; ++i)
        {
            int pivot = -1;
            for (int j = i; j < n; ++j)
            {
                if (matrix[j][i] != 0)
                {
                    pivot = j;
                    break;
                }
            }

            if (pivot == -1)
            {
                return 0;
            }

            if (pivot != i)
            {
                std::swap(matrix[i], matrix[pivot]);
                det = -det;
            }

            det *= matrix[i][i];

            for (int j = i + 1; j < n; ++j)
            {
                int factor = matrix[j][i] / matrix[i][i];
                for (int k = i; k < n; ++k)
                {
                    matrix[j][k] -= factor * matrix[i][k];
                }
            }
        }

        return det;
    }
};

template<typename T>
class DiscreteRandomVariable
{
private:
    std::vector<T> samples;
    std::function<T()> generator;

public:
    DiscreteRandomVariable(std::function<T()> gen) : generator(gen) {}

    void generate_samples(int count)
    {
        samples.clear();
        for (int i = 0; i < count; ++i)
        {
            samples.push_back(generator());
        }
    }

    double mean() const
    {
        if (samples.empty()) return 0.0;
        double sum = std::accumulate(samples.begin(), samples.end(), 0.0);
        return sum / samples.size();
    }

    double variance() const
    {
        if (samples.size() <= 1) return 0.0;
        double m = mean();
        double sum_sq = 0.0;
        for (const T& val : samples)
        {
            sum_sq += (static_cast<double>(val) - m) * (static_cast<double>(val) - m);
        }
        return sum_sq / samples.size();
    }

    void print_statistics(const std::string& name) const
    {
        std::cout << name << ": E[X] = " << mean() << ", D[X] = " << variance() << "\n";
    }
};

class RandomVariableFactory
{
private:
    RandomGraphGenerator generator;
    int n;

public:
    RandomVariableFactory(int vertices) : n(vertices) {}

    DiscreteRandomVariable<int> create_mst_weight_variable()
    {
        return DiscreteRandomVariable<int>([this]()
        {
            Graph g = generator.generate_random_graph(n, 0.5);
            return GraphAnalyzer::mst_weight(g);
        });
    }

    DiscreteRandomVariable<int> create_longest_cycle_by_edges_variable()
    {
        return DiscreteRandomVariable<int>([this]()
        {
            Graph g = generator.generate_random_graph(n, 0.5);
            return GraphAnalyzer::longest_cycle_by_edges(g);
        });
    }

    DiscreteRandomVariable<int> create_largest_cycle_edge_count_variable()
    {
        return DiscreteRandomVariable<int>([this]()
        {
            Graph g = generator.generate_random_graph(n, 0.5);
            return GraphAnalyzer::largest_cycle_edge_count(g);
        });
    }

    DiscreteRandomVariable<int> create_isolated_vertices_variable()
    {
        return DiscreteRandomVariable<int>([this]()
        {
            Graph g = generator.generate_random_graph(n, 0.5);
            return GraphAnalyzer::isolated_vertices_count(g);
        });
    }

    DiscreteRandomVariable<int> create_spanning_trees_count_variable()
    {
        return DiscreteRandomVariable<int>([this]()
        {
            Graph g = generator.generate_random_graph(n, 0.5);
            return GraphAnalyzer::count_spanning_trees(g);
        });
    }

    DiscreteRandomVariable<int> create_connected_components_variable()
    {
        return DiscreteRandomVariable<int>([this]()
        {
            Graph g = generator.generate_random_graph(n, 0.5);
            return GraphAnalyzer::connected_components(g);
        });
    }

    DiscreteRandomVariable<int> create_complete_subgraph_components_variable()
    {
        return DiscreteRandomVariable<int>([this]()
        {
            Graph g = generator.generate_random_graph(n, 0.5);
            return GraphAnalyzer::complete_subgraph_components(g);
        });
    }
};

int main()
{
    int n = 10;
    int num_samples = 100;

    RandomVariableFactory factory(n);

    DiscreteRandomVariable<int> var_a = factory.create_mst_weight_variable();
    DiscreteRandomVariable<int> var_b = factory.create_longest_cycle_by_edges_variable();
    DiscreteRandomVariable<int> var_d = factory.create_largest_cycle_edge_count_variable();
    DiscreteRandomVariable<int> var_e = factory.create_isolated_vertices_variable();
    DiscreteRandomVariable<int> var_f = factory.create_spanning_trees_count_variable();
    DiscreteRandomVariable<int> var_g = factory.create_connected_components_variable();
    DiscreteRandomVariable<int> var_h = factory.create_complete_subgraph_components_variable();

    var_a.generate_samples(num_samples);
    var_b.generate_samples(num_samples);
    var_d.generate_samples(num_samples);
    var_e.generate_samples(num_samples);
    var_f.generate_samples(num_samples);
    var_g.generate_samples(num_samples);
    var_h.generate_samples(num_samples);

    std::cout << "Results for n = " << n << ", samples = " << num_samples << "\n";
    var_a.print_statistics("A (MST weight)");
    var_b.print_statistics("B (longest cycle by edges)");
    var_d.print_statistics("D (largest cycle edge count)");
    var_e.print_statistics("E (isolated vertices)");
    var_f.print_statistics("F (spanning trees count)");
    var_g.print_statistics("G (connected components)");
    var_h.print_statistics("H (complete subgraph components)");

    return 0;
}