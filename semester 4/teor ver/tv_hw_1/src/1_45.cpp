#include <iostream>
#include "../include/help.hpp"

int main()
{
    int successCount = 0;

    for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
    {
        int xInt = getRandomNumber(0, 100);
        int yInt = getRandomNumber(0, 100);

        double x = xInt / 100.0;
        double y = yInt / 100.0;

        if (x + y <= 1.0 && x * y >= 0.09)
        {
            successCount++;
        }
    }

    std::cout << "Probability that x+y <= 1 and x*y >= 0.09: "
            << static_cast<double>(successCount) / NUM_EXPERIMENTS << std::endl;

    return 0;
}