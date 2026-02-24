#include <iostream>
#include <vector>
#include "../include/help.hpp"

int main()
{
    int count_event_a = 0;
    int count_event_b = 0;

    for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
    {
        std::vector<int> balls = {1, 2, 3, 4, 5};
        std::vector<int> extracted;

        for (int i = 0; i < 3; ++i)
        {
            int random_index = getRandomNumber(0, balls.size() - 1);
            extracted.push_back(balls[random_index]);
            balls.erase(balls.begin() + random_index);
        }

        if (extracted[0] == 1 && extracted[1] == 4 && extracted[2] == 5)
        {
            count_event_a++;
        }

        bool has_1 = false;
        bool has_4 = false;
        bool has_5 = false;

        for (int i = 0; i < 3; ++i)
        {
            if (extracted[i] == 1) has_1 = true;
            if (extracted[i] == 4) has_4 = true;
            if (extracted[i] == 5) has_5 = true;
        }

        if (has_1 && has_4 && has_5)
        {
            count_event_b++;
        }
    }

    std::cout << "Probability of event a: "
              << static_cast<double>(count_event_a) / NUM_EXPERIMENTS << std::endl;

    std::cout << "Probability of event b: "
              << static_cast<double>(count_event_b) / NUM_EXPERIMENTS << std::endl;

    return 0;
}