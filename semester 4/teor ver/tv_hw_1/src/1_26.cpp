#include <iostream>
#include "../include/help.hpp"

int main()
{
    int successCount = 0;

    int bigSegmentLength = 20;
    int smallSegmentLength = 10;
    int smallSegmentStart = getRandomNumber(0, bigSegmentLength - smallSegmentLength);

    for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
    {
        int point = getRandomNumber(0, bigSegmentLength - 1);

        if (point >= smallSegmentStart && point < smallSegmentStart + smallSegmentLength)
        {
            successCount++;
        }
    }

    std::cout << "Probability of point falling into smaller segment: "
              << static_cast<double>(successCount) / NUM_EXPERIMENTS << std::endl;

    return 0;
}