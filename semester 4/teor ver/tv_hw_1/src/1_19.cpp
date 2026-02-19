#include <iostream>
#include <vector>
#include "../include/help.hpp"

int main()
{
    int successCount = 0;

    for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
    {
        std::vector<int> kinescopes(15);
        for (int i = 0; i < 15; ++i)
        {
            if (i < 10)
            {
                kinescopes[i] = 1;
            }
            else
            {
                kinescopes[i] = 0;
            }
        }

        std::vector<int> selectedKinescopes;
        std::vector<bool> used(15, false);

        for (int i = 0; i < 5; ++i)
        {
            int randomIndex;
            do
            {
                randomIndex = getRandomNumber(0, 14);
            } while (used[randomIndex]);

            used[randomIndex] = true;
            selectedKinescopes.push_back(kinescopes[randomIndex]);
        }

        int lvovCount = 0;
        for (int i = 0; i < 5; ++i)
        {
            if (selectedKinescopes[i] == 1)
            {
                lvovCount++;
            }
        }

        if (lvovCount == 3)
        {
            successCount++;
        }
    }

    std::cout << "Probability of exactly three Lvov plant kinescopes: "
            << static_cast<double>(successCount) / NUM_EXPERIMENTS << std::endl;

    return 0;
}