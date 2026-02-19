#include <iostream>
#include <vector>
#include "../include/help.hpp"

int main()
{
    int successCount = 0;

    for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
    {
        std::vector<int> students(12);
        for (int i = 0; i < 12; ++i)
        {
            if (i < 8)
            {
                students[i] = 1;
            }
            else
            {
                students[i] = 0;
            }
        }

        std::vector<int> selectedStudents;
        std::vector<bool> used(12, false);

        for (int i = 0; i < 9; ++i)
        {
            int randomIndex;
            do
            {
                randomIndex = getRandomNumber(0, 11);
            } while (used[randomIndex]);

            used[randomIndex] = true;
            selectedStudents.push_back(students[randomIndex]);
        }

        int excellentCount = 0;
        for (int i = 0; i < 9; ++i)
        {
            if (selectedStudents[i] == 1)
            {
                excellentCount++;
            }
        }

        if (excellentCount == 5)
        {
            successCount++;
        }
    }

    std::cout << "Probability of exactly five excellent students: "
            << static_cast<double>(successCount) / NUM_EXPERIMENTS << std::endl;

    return 0;
}