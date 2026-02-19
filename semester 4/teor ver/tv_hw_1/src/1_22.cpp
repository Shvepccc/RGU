#include <iostream>
#include <vector>
#include "../include/help.hpp"

int main()
{
    int successCount = 0;

    std::vector<int> secretCode(4);
    for (int i = 0; i < 4; ++i)
    {
        secretCode[i] = getRandomNumber(0, 4);
    }

    for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
    {
        std::vector<int> currentCode(4);
        for (int i = 0; i < 4; ++i)
        {
            currentCode[i] = getRandomNumber(0, 4);
        }

        bool isOpen = true;
        for (int i = 0; i < 4; ++i)
        {
            if (currentCode[i] != secretCode[i])
            {
                isOpen = false;
                break;
            }
        }

        if (isOpen)
        {
            successCount++;
        }
    }

    std::cout << "Probability of opening the lock: "
            << static_cast<double>(successCount) / NUM_EXPERIMENTS << std::endl;

    return 0;
}