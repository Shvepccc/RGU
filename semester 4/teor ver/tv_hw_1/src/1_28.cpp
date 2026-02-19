#include <iostream>
#include <cmath>
#include "../include/help.hpp"

int main()
{
    int successCount = 0;

    int R = 100;
    int r = 30;

    int centerX = getRandomNumber(r, R - r);
    int centerY = getRandomNumber(r, R - r);

    for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
    {
        int pointX = getRandomNumber(0, R);
        int pointY = getRandomNumber(0, R);

        double distance = std::sqrt(std::pow(pointX - centerX, 2) + std::pow(pointY - centerY, 2));

        if (distance <= r)
        {
            successCount++;
        }
    }

    std::cout << "Probability of point falling into small circle: "
              << static_cast<double>(successCount) / NUM_EXPERIMENTS << std::endl;

    return 0;
}