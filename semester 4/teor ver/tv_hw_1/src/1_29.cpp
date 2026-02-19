#include <iostream>
#include "../include/help.hpp"

int main()
{
    int successCount = 0;

    int a = 50;
    int r = 20;

    for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
    {
        int distanceToNearestLine = getRandomNumber(0, 2 * a);

        if (distanceToNearestLine >= r && distanceToNearestLine <= 2 * a - r)
        {
            successCount++;
        }
    }

    std::cout << "Probability that coin does not intersect any line: "
              << static_cast<double>(successCount) / NUM_EXPERIMENTS << std::endl;

    return 0;
}