#include <iostream>
#include <cmath>
#include "../include/help.hpp"

int main()
{
    int successCount = 0;

    int bigRadius = 10;
    int smallRadius = 5;
    int centerX = 50;
    int centerY = 50;

    for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
    {
        int pointX = getRandomNumber(centerX - bigRadius, centerX + bigRadius);
        int pointY = getRandomNumber(centerY - bigRadius, centerY + bigRadius);

        double distance = std::sqrt(std::pow(pointX - centerX, 2) + std::pow(pointY - centerY, 2));

        if (distance <= bigRadius && distance >= smallRadius)
        {
            successCount++;
        }
    }

    std::cout << "Probability of point falling into the ring: "
              << static_cast<double>(successCount) / NUM_EXPERIMENTS << std::endl;

    return 0;
}