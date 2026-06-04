#include "../include/help.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>

int main(int argc, char* argv[])
{

    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <K> <n>" << std::endl;
        return 1;
    }

    int k = std::atoi(argv[1]);
    int n = std::atoi(argv[2]);

    if (k <= 0 || n <= 0)
    {
        std::cerr << "K and n must be positive integers." << std::endl;
        return 1;
    }

    std::vector<int> counts(n + 1, 0);

    std::cout << "Running " << k << " experiments with " << n << " keys..." << std::endl;
    std::cout << "Expected probability for each outcome: 1/" << n << " = "
              << 1.0 / n << std::endl << std::endl;

    for (int experiment = 1; experiment <= k; ++experiment)
    {
        int outcome = getRandomNumber(1, n);
        if (k < 10e5) std::cout << "Experiment " << experiment << ": success on attempt " << outcome << std::endl;
        counts[outcome]++;
    }

    std::cout << "\nEmpirical probabilities after " << k << " experiments:" << std::endl;
    for (int i = 1; i <= n; ++i)
    {
        double empirical_prob = static_cast<double>(counts[i]) / k;
        std::cout << "Outcome " << i << ": " << empirical_prob
                  << " (" << counts[i] << " times)" << std::endl;
    }

    return 0;
}