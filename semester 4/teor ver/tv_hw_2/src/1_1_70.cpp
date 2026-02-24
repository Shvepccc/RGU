#include <iostream>
#include <vector>
#include "../include/help.hpp"

int main()
{
    int count_without_replacement = 0;
    int count_with_replacement = 0;

    for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
    {
        std::vector<int> cubes = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::vector<int> extracted_without;

        for (int i = 0; i < 3; ++i)
        {
            int random_index = getRandomNumber(0, cubes.size() - 1);
            extracted_without.push_back(cubes[random_index]);
            cubes.erase(cubes.begin() + random_index);
        }

        if (extracted_without[0] == 1 && extracted_without[1] == 2 && extracted_without[2] == 3)
        {
            count_without_replacement++;
        }

        std::vector<int> extracted_with;
        for (int i = 0; i < 3; ++i)
        {
            int random_number = getRandomNumber(1, 10);
            extracted_with.push_back(random_number);
        }

        if (extracted_with[0] == 1 && extracted_with[1] == 2 && extracted_with[2] == 3)
        {
            count_with_replacement++;
        }
    }

    std::cout << "Probability without replacement: "
              << static_cast<double>(count_without_replacement) / NUM_EXPERIMENTS << std::endl;

    std::cout << "Probability with replacement: "
              << static_cast<double>(count_with_replacement) / NUM_EXPERIMENTS << std::endl;

    return 0;
}