#include <iostream>
#include "../include/help.hpp"

int main()
{
    int count_event_a = 0;
    int count_event_b = 0;
    int count_event_c = 0;

    for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
    {
        int die1 = getRandomNumber(1, 6);
        int die2 = getRandomNumber(1, 6);
        int die3 = getRandomNumber(1, 6);

        int ones_count = (die1 == 1 ? 1 : 0) + (die2 == 1 ? 1 : 0) + (die3 == 1 ? 1 : 0);

        if (ones_count == 2)
        {
            count_event_a++;
        }

        if ((die1 == die2 && die2 != die3) ||
            (die1 == die3 && die1 != die2) ||
            (die2 == die3 && die2 != die1))
        {
            count_event_b++;
        }

        if (die1 != die2 && die1 != die3 && die2 != die3)
        {
            count_event_c++;
        }
    }

    std::cout << "Probability of event a: "
              << static_cast<double>(count_event_a) / NUM_EXPERIMENTS << std::endl;

    std::cout << "Probability of event b: "
              << static_cast<double>(count_event_b) / NUM_EXPERIMENTS << std::endl;

    std::cout << "Probability of event c: "
              << static_cast<double>(count_event_c) / NUM_EXPERIMENTS << std::endl;

    return 0;
}