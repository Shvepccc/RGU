#include <iostream>
#include <cstdlib>
#include "../include/help.hpp"

int main()
{
    int successCount = 0;

    for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
    {
        std::vector<int> cubes = {1, 2, 3, 4, 5, 6};
        std::vector<int> extracted;

        while (cubes.size() > 0)
        {
            int randomIndex = getRandomNumber(0, cubes.size() - 1);
            extracted.push_back(cubes[randomIndex]);
            cubes[randomIndex] = cubes.back();
            cubes.pop_back();
        }

        bool isIncreasing = true;
        for (size_t i = 0; i < extracted.size() - 1; ++i)
        {
            if (extracted[i] > extracted[i + 1])
            {
                isIncreasing = false;
                break;
            }
        }

        if (isIncreasing)
        {
            successCount++;
        }
    }

    std::cout << "Probability that numbers appear in increasing order: "
            << static_cast<double>(successCount) / NUM_EXPERIMENTS << std::endl;

    return 0;
}