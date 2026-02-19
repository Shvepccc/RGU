#include <iostream>
#include "../include/help.hpp"

int main()
{
    int successCount = 0;

    int a = 100;
    int r = 30;

    for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
    {
        int x = getRandomNumber(0, a);
        int y = getRandomNumber(0, a);

        int distanceToLeft = x;
        int distanceToRight = a - x;
        int distanceToBottom = y;
        int distanceToTop = a - y;

        if (distanceToLeft >= r && distanceToRight >= r &&
            distanceToBottom >= r && distanceToTop >= r)
        {
            successCount++;
        }
    }

    std::cout << "Probability that coin does not intersect any square side: "
              << static_cast<double>(successCount) / NUM_EXPERIMENTS << std::endl;

    return 0;
}