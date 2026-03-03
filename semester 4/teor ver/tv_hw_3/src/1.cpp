#include <iostream>
#include <vector>
#include <string>
#include <random>

struct simulation_config
{
    int m;
    int n;
    int k;
    int d;
    int experiments;
    std::string pattern;
    std::vector<char> alphabet;
    std::vector<double> probabilities;
};

class cluster_generator
{
private:
    std::vector<char> alphabet;
    std::discrete_distribution<int> distribution;
    std::mt19937 rng;

public:
    cluster_generator(const std::vector<char>& a, const std::vector<double>& p)
        : alphabet(a), distribution(p.begin(), p.end()), rng(std::random_device{}())
    {
    }

    std::string generate_cluster(int n)
    {
        std::string result;
        result.reserve(n);

        for(int i = 0; i < n; ++i)
        {
            int index = distribution(rng);
            result.push_back(alphabet[index]);
        }

        return result;
    }
};

bool are_connectable(const std::string& left, const std::string& right, const std::string& pattern)
{
    int k = pattern.size();
    int n = left.size();

    for(int a = 1; a < k; a++)
    {
        int b = k - a;

        if(a <= n && b <= n)
        {
            std::string combined = left.substr(n - a, a) + right.substr(0, b);

            if(combined == pattern)
            {
                return true;
            }
        }
    }

    return false;
}

struct simulation_result
{
    double prob_all_connectable;
    double prob_exact_d;
    double prob_all_not_connectable;
};

simulation_result run_simulation(const simulation_config& config)
{
    cluster_generator generator(config.alphabet, config.probabilities);

    int count_all_connectable = 0;
    int count_exact_d = 0;
    int count_all_not_connectable = 0;

    for(int exp = 0; exp < config.experiments; exp++)
    {
        std::vector<std::string> clusters;

        for(int i = 0; i < config.m; ++i)
        {
            clusters.push_back(generator.generate_cluster(config.n));
        }

        int connectable_count = 0;

        for(int i = 0; i < config.m - 1; ++i)
        {
            if(are_connectable(clusters[i], clusters[i + 1], config.pattern))
            {
                ++connectable_count;
            }
        }

        if(connectable_count == config.m - 1)
        {
            ++count_all_connectable;
        }

        if(connectable_count == config.d)
        {
            ++count_exact_d;
        }

        if(connectable_count == 0)
        {
            ++count_all_not_connectable;
        }
    }

    simulation_result result;

    result.prob_all_connectable =
        static_cast<double>(count_all_connectable) / config.experiments;

    result.prob_exact_d =
        static_cast<double>(count_exact_d) / config.experiments;

    result.prob_all_not_connectable =
        static_cast<double>(count_all_not_connectable) / config.experiments;

    return result;
}

int main()
{
    simulation_config config;

    config.m = 5;
    config.n = 5;
    config.k = 3;
    config.d = 2;
    config.experiments = 100000;
    config.pattern = "101";

    config.alphabet = {'0', '1', '2'};
    config.probabilities = {0.33, 0.33, 0.33};

    simulation_result result = run_simulation(config);

    std::cout << "prob_all_connectable = " << result.prob_all_connectable << std::endl;
    std::cout << "prob_exact_d = " << result.prob_exact_d << std::endl;
    std::cout << "prob_all_not_connectable = " << result.prob_all_not_connectable << std::endl;

    return 0;
}   