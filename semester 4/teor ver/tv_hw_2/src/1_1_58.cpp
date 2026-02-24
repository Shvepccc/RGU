
#include <iostream>
#include "../include/help.hpp"

//TODO: change variable names

int main()
{
    int countAllFives = 0;
    int countAllSame = 0;

    for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
    {
        int die1 = getRandomNumber(1, 6);
        int die2 = getRandomNumber(1, 6);
        int die3 = getRandomNumber(1, 6);

        if (die1 == 5 && die2 == 5 && die3 == 5)
        {
            countAllFives++;
        }

        if (die1 == die2 && die2 == die3)
        {
            countAllSame++;
        }
    }

    std::cout << "Probability of all fives: "
              << static_cast<double>(countAllFives) / NUM_EXPERIMENTS << std::endl;

    std::cout << "Probability of all same number: "
              << static_cast<double>(countAllSame) / NUM_EXPERIMENTS << std::endl;

    return 0;
}