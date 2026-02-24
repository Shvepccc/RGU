#include <iostream>
#include <vector>
#include "../include/help.hpp"

int main()
{
    int success_count = 0;

    for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
    {
        std::vector<int> tickets(100);
        for (int i = 0; i < 100; ++i)
        {
            if (i < 5)
            {
                tickets[i] = 1;
            }
            else
            {
                tickets[i] = 0;
            }
        }

        std::vector<int> selected_tickets;
        std::vector<bool> used(100, false);

        for (int i = 0; i < 2; ++i)
        {
            int random_index;
            do
            {
                random_index = getRandomNumber(0, 99);
            } while (used[random_index]);

            used[random_index] = true;
            selected_tickets.push_back(tickets[random_index]);
        }

        if (selected_tickets[0] == 1 && selected_tickets[1] == 1)
        {
            success_count++;
        }
    }

    std::cout << "Probability that both tickets are winning: "
              << static_cast<double>(success_count) / NUM_EXPERIMENTS << std::endl;

    return 0;
}