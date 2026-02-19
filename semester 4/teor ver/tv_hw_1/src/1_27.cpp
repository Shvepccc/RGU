#include <iostream>
#include "../include/help.hpp"

int main()
{
    int successCount = 0;

    int C = 100;
    int L = C;

    for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
    {
        int x = getRandomNumber(0, C);

        int OB = x;
        int BA = C - x;

        int smallerSegment;
        if (OB < BA)
        {
            smallerSegment = OB;
        }
        else
        {
            smallerSegment = BA;
        }

        if (smallerSegment > L / 3)
        {
            successCount++;
        }
    }

    std::cout << "Probability that smaller segment length > L/3: "
              << static_cast<double>(successCount) / NUM_EXPERIMENTS << std::endl;

    return 0;
}