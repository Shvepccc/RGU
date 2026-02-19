#include <iostream>
#include <vector>
#include "../include/help.hpp"

int main()
{
    int successOneCount = 0;
    int successTwoCount = 0;
    int successAtLeastOneCount = 0;

    for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
    {
        std::vector<int> items(5);
        for (int i = 0; i < 5; ++i)
        {
            if (i < 3)
            {
                items[i] = 1;
            }
            else
            {
                items[i] = 0;
            }
        }

        std::vector<int> selectedItems;
        std::vector<bool> used(5, false);

        for (int i = 0; i < 2; ++i)
        {
            int randomIndex;
            do
            {
                randomIndex = getRandomNumber(0, 4);
            } while (used[randomIndex]);

            used[randomIndex] = true;
            selectedItems.push_back(items[randomIndex]);
        }

        int paintedCount = 0;
        for (int i = 0; i < 2; ++i)
        {
            if (selectedItems[i] == 1)
            {
                paintedCount++;
            }
        }

        if (paintedCount == 1)
        {
            successOneCount++;
            successAtLeastOneCount++;
        }
        else if (paintedCount == 2)
        {
            successTwoCount++;
            successAtLeastOneCount++;
        }
    }

    std::cout << "Probability of exactly one painted item: "
              << static_cast<double>(successOneCount) / NUM_EXPERIMENTS << std::endl;
    std::cout << "Probability of exactly two painted items: "
              << static_cast<double>(successTwoCount) / NUM_EXPERIMENTS << std::endl;
    std::cout << "Probability of at least one painted item: "
              << static_cast<double>(successAtLeastOneCount) / NUM_EXPERIMENTS << std::endl;

    return 0;
}