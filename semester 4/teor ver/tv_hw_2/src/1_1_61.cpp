
#include <iostream>
#include "../include/help.hpp"

int main()
{
    double hit_probability = 0.8;
    double target_probability = 0.4;
    int max_shots = 20;

    for (int shots = 1; shots <= max_shots; ++shots)
    {
        int no_misses_count = 0;

        for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
        {
            bool has_miss = false;

            for (int shot = 0; shot < shots; ++shot)
            {
                int random_value = getRandomNumber(1, 100);
                if (random_value > hit_probability * 100)
                {
                    has_miss = true;
                    break;
                }
            }

            if (!has_miss)
            {
                no_misses_count++;
            }
        }

        double probability_no_misses = static_cast<double>(no_misses_count) / NUM_EXPERIMENTS;

        std::cout << "Shots: " << shots << ", Probability of no misses: " << probability_no_misses << std::endl;

        if (probability_no_misses < target_probability)
        {
            std::cout << "Answer: " << shots << " shots" << std::endl;
            break;
        }
    }

    return 0;
}