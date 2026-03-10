#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "../include/help.hpp"
#include "../include/json.hpp"

using json = nlohmann::json;

int main(int argc, char* argv[])
{
    std::cout << "=== Random Walk Simulation ===" << std::endl;
    std::cout << "Starting program..." << std::endl;
    
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
        return 1;
    }

    std::cout << "Config file: " << argv[1] << std::endl;
    std::cout << "Reading configuration..." << std::endl;

    std::ifstream config_file(argv[1]);
    if (!config_file.is_open())
    {
        std::cerr << "Error: Could not open config file" << std::endl;
        return 1;
    }

    json config;
    try
    {
        config_file >> config;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        return 1;
    }

    double p = config["p"];
    double q = config["q"];
    double s = config["s"];
    int max_steps = config["N"];
    int num_experiments = config["num_experiments"];

    if (std::abs(p + q - 1.0) > 1e-10)
    {
        std::cerr << "Error: p + q must equal 1" << std::endl;
        return 1;
    }

    std::cout << "Parameters loaded:" << std::endl;
    std::cout << "  p = " << p << " (right)" << std::endl;
    std::cout << "  q = " << q << " (left)" << std::endl;
    std::cout << "  s = " << s << std::endl;
    std::cout << "  Max steps to observe = " << max_steps << std::endl;
    std::cout << "  Number of experiments = " << num_experiments << std::endl;
    
    if (std::abs(p - 0.5) < 1e-10)
    {
        std::cout << "  Symmetric walk (p = 0.5)" << std::endl;
    }
    else
    {
        std::cout << "  Asymmetric walk: вероятность вернуться когда-либо = " 
                  << 1.0 - std::abs(p - q) << std::endl;
    }
    std::cout << std::endl;

    std::vector<int> return_step_counts(max_steps + 1, 0);
    int never_returned = 0;

    std::cout << "Starting simulations..." << std::endl;
    
    int progress_interval = num_experiments / 10;
    if (progress_interval < 1) progress_interval = 1;

    const double EPSILON = 1e-10;

    for (int exp = 0; exp < num_experiments; exp++)
    {
        if (exp % progress_interval == 0 && exp > 0)
        {
            std::cout << "  Progress: " << (exp * 100 / num_experiments) << "% (" 
                      << exp << "/" << num_experiments << " experiments)" << std::endl;
        }

        double position = 0.0;
        bool returned = false;

        for (int step = 1; step <= max_steps; ++step)
        {
            int r = getRandomNumber(0, 99);

            if (r < p * 100)
            {
                position += s;
            }
            else
            {
                position -= s;
            }

            if (std::abs(position) < EPSILON)
            {
                return_step_counts[step]++;
                returned = true;
                break;
            }
        }

        if (!returned)
        {
            never_returned++;
        }
    }

    std::cout << "  Progress: 100% (" << num_experiments << "/" 
              << num_experiments << " experiments)" << std::endl;
    std::cout << "Simulations complete!" << std::endl;
    std::cout << std::endl;

    std::cout << "=== Results ===" << std::endl;
    std::cout << "Step\tProbability\tCumulative\tCount" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    
    int total_returned = 0;
    double cumulative = 0.0;
    
    for (int k = 1; k <= max_steps; ++k)
    {
        if (return_step_counts[k] > 0)
        {
            double probability = static_cast<double>(return_step_counts[k]) / num_experiments;
            cumulative += probability;
            
            std::cout << k << "\t" 
                      << probability << "\t"
                      << cumulative << "\t(" 
                      << return_step_counts[k] << ")" << std::endl;
            
            total_returned += return_step_counts[k];
        }
    }
    
    if (never_returned > 0)
    {
        double probability = static_cast<double>(never_returned) / num_experiments;
        cumulative += probability;
        std::cout << ">" << max_steps << "\t" 
                  << probability << "\t"
                  << cumulative << "\t(" 
                  << never_returned << ")" << std::endl;
    }
    
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Total experiments: " << num_experiments << std::endl;
    std::cout << "Returned within " << max_steps << " steps: " << total_returned 
              << " (" << (100.0 * total_returned / num_experiments) << "%)" << std::endl;
    
    if (std::abs(p - 0.5) > 1e-10)
    {
        double theoretical_never_return = std::abs(p - q);
        std::cout << "Theoretical never return: " << theoretical_never_return 
                  << " (for infinite steps)" << std::endl;
    }

    return 0;
}