#include <iostream>
#include <vector>
#include "../include/help.hpp"

int main()
{
    int success_count = 0;

    for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
    {
        std::vector<int> questions(25);
        for (int i = 0; i < 25; ++i)
        {
            if (i < 20)
            {
                questions[i] = 1;
            }
            else
            {
                questions[i] = 0;
            }
        }

        std::vector<int> selected_questions;
        std::vector<bool> used(25, false);

        for (int i = 0; i < 3; ++i)
        {
            int random_index;
            do
            {
                random_index = getRandomNumber(0, 24);
            } while (used[random_index]);

            used[random_index] = true;
            selected_questions.push_back(questions[random_index]);
        }

        if (selected_questions[0] == 1 && selected_questions[1] == 1 && selected_questions[2] == 1)
        {
            success_count++;
        }
    }

    std::cout << "Probability that student knows all three questions: "
              << static_cast<double>(success_count) / NUM_EXPERIMENTS << std::endl;

    return 0;
}