#include <iostream>
#include <vector>
#include "../include/help.hpp"

//TODO: change variable names
//TODO: correct analytic solution

int main()
{
    int countNotMoreThanThree = 0;
    int countNotLessThanTwo = 0;

    std::vector<double> probabilities = {0.6, 0.7, 0.8, 0.9};

    for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
    {
        int boxesWithDetail = 0;

        for (int i = 0; i < 4; ++i)
        {
            int randomValue = getRandomNumber(1, 100);
            double probabilityPercent = probabilities[i] * 100;

            if (randomValue <= probabilityPercent)
            {
                boxesWithDetail++;
            }
        }

        if (boxesWithDetail <= 3)
        {
            countNotMoreThanThree++;
        }

        if (boxesWithDetail >= 2)
        {
            countNotLessThanTwo++;
        }
    }

    std::cout << "Probability that detail is in not more than three boxes: "
              << static_cast<double>(countNotMoreThanThree) / NUM_EXPERIMENTS << std::endl;

    std::cout << "Probability that detail is in not less than two boxes: "
              << static_cast<double>(countNotLessThanTwo) / NUM_EXPERIMENTS << std::endl;

    return 0;
}