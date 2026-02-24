
#include <iostream>
#include <vector>
#include <map>
#include "../include/help.hpp"

void simulate_without_replacement(int deck_size, int& count_case1, int& count_case2, int& count_case3, int& count_case4)
{
    std::vector<int> deck(deck_size);
    int values_count = deck_size / 4;
    for (int i = 0; i < deck_size; ++i)
    {
        deck[i] = i / 4;
    }

    std::vector<int> selected;
    std::vector<bool> used(deck_size, false);

    for (int i = 0; i < 4; ++i)
    {
        int random_index;
        do
        {
            random_index = getRandomNumber(0, deck_size - 1);
        } while (used[random_index]);

        used[random_index] = true;
        selected.push_back(deck[random_index]);
    }

    std::map<int, int> value_count;
    for (int i = 0; i < 4; ++i)
    {
        value_count[selected[i]]++;
    }

    if (value_count.size() == 2)
    {
        bool case1 = false;
        bool case2 = false;
        for (auto& pair : value_count)
        {
            if (pair.second == 3) case1 = true;
            if (pair.second == 2) case2 = true;
        }
        if (case1) count_case1++;
        else if (case2) count_case2++;
    }
    else if (value_count.size() == 4)
    {
        count_case3++;
    }
    else if (value_count.size() == 1)
    {
        count_case4++;
    }
}

void simulate_with_replacement(int deck_size, int& count_case1, int& count_case2, int& count_case3, int& count_case4)
{
    int values_count = deck_size / 4;
    std::vector<int> selected;

    for (int i = 0; i < 4; ++i)
    {
        int random_value = getRandomNumber(0, values_count - 1);
        selected.push_back(random_value);
    }

    std::map<int, int> value_count;
    for (int i = 0; i < 4; ++i)
    {
        value_count[selected[i]]++;
    }

    if (value_count.size() == 2)
    {
        bool case1 = false;
        bool case2 = false;
        for (auto& pair : value_count)
        {
            if (pair.second == 3) case1 = true;
            if (pair.second == 2) case2 = true;
        }
        if (case1) count_case1++;
        else if (case2) count_case2++;
    }
    else if (value_count.size() == 4)
    {
        count_case3++;
    }
    else if (value_count.size() == 1)
    {
        count_case4++;
    }
}

int main()
{
    int deck_sizes[] = {36, 52};

    for (int deck_idx = 0; deck_idx < 2; ++deck_idx)
    {
        int deck_size = deck_sizes[deck_idx];

        int without_case1 = 0, without_case2 = 0, without_case3 = 0, without_case4 = 0;
        int with_case1 = 0, with_case2 = 0, with_case3 = 0, with_case4 = 0;

        for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
        {
            simulate_without_replacement(deck_size, without_case1, without_case2, without_case3, without_case4);
            simulate_with_replacement(deck_size, with_case1, with_case2, with_case3, with_case4);
        }

        std::cout << "For " << deck_size << " card deck:" << std::endl;
        std::cout << "Without replacement:" << std::endl;
        std::cout << "  Case 1 (3+1): " << static_cast<double>(without_case1) / NUM_EXPERIMENTS << std::endl;
        std::cout << "  Case 2 (2+2): " << static_cast<double>(without_case2) / NUM_EXPERIMENTS << std::endl;
        std::cout << "  Case 3 (all different): " << static_cast<double>(without_case3) / NUM_EXPERIMENTS << std::endl;
        std::cout << "  Case 4 (all same): " << static_cast<double>(without_case4) / NUM_EXPERIMENTS << std::endl;

        std::cout << "With replacement:" << std::endl;
        std::cout << "  Case 1 (3+1): " << static_cast<double>(with_case1) / NUM_EXPERIMENTS << std::endl;
        std::cout << "  Case 2 (2+2): " << static_cast<double>(with_case2) / NUM_EXPERIMENTS << std::endl;
        std::cout << "  Case 3 (all different): " << static_cast<double>(with_case3) / NUM_EXPERIMENTS << std::endl;
        std::cout << "  Case 4 (all same): " << static_cast<double>(with_case4) / NUM_EXPERIMENTS << std::endl;
        std::cout << std::endl;
    }

    return 0;
}