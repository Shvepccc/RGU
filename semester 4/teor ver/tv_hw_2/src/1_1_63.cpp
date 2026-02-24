#include <iostream>
#include "../include/help.hpp"

int main()
{
    int success_count = 0;

    for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
    {
        int part1_count = 0;
        int part2_count = 0;
        int part3_count = 0;

        for (int point = 0; point < 3; ++point)
        {
            int position = getRandomNumber(0, 299);
            double pos_double = position / 100.0;

            if (pos_double < 1.0)
            {
                part1_count++;
            }
            else if (pos_double < 2.0)
            {
                part2_count++;
            }
            else
            {
                part3_count++;
            }
        }

        if (part1_count == 1 && part2_count == 1 && part3_count == 1)
        {
            success_count++;
        }
    }

    std::cout << "Probability that each part gets exactly one point: "
              << static_cast<double>(success_count) / NUM_EXPERIMENTS << std::endl;

    return 0;
}