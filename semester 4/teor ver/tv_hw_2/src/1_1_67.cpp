
#include <iostream>
#include <vector>
#include "../include/help.hpp"

int main()
{
    int success_count = 0;

    for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
    {
        std::vector<int> details(10);
        for (int i = 0; i < 10; ++i)
        {
            if (i < 6)
            {
                details[i] = 1;
            }
            else
            {
                details[i] = 0;
            }
        }

        std::vector<int> selected_details;
        std::vector<bool> used(10, false);

        for (int i = 0; i < 4; ++i)
        {
            int random_index;
            do
            {
                random_index = getRandomNumber(0, 9);
            } while (used[random_index]);

            used[random_index] = true;
            selected_details.push_back(details[random_index]);
        }

        if (selected_details[0] == 1 && selected_details[1] == 1 &&
            selected_details[2] == 1 && selected_details[3] == 1)
        {
            success_count++;
        }
    }

    std::cout << "Probability that all four details are painted: "
              << static_cast<double>(success_count) / NUM_EXPERIMENTS << std::endl;

    return 0;
}