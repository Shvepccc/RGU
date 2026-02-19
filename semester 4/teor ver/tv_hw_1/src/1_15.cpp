#include <iostream>
#include <vector>
#include "../include/help.hpp"

int main()
{
    int successCount = 0;

    for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
    {
        std::vector<int> elements = {1, 1, 0, 0, 0};

        std::vector<int> selectedElements;
        int selectedCount = 0;

        while (selectedCount < 2)
        {
            int randomIndex = getRandomNumber(0, 4);
            bool alreadySelected = false;

            for (int j = 0; j < selectedCount; ++j)
            {
                if (randomIndex == selectedElements[j])
                {
                    alreadySelected = true;
                    break;
                }
            }

            if (!alreadySelected)
            {
                selectedElements.push_back(randomIndex);
                selectedCount++;
            }
        }

        if (elements[selectedElements[0]] == 0 && elements[selectedElements[1]] == 0)
        {
            successCount++;
        }
    }

    std::cout << "Probability that both selected elements are unworn: "
              << static_cast<double>(successCount) / NUM_EXPERIMENTS << std::endl;

    return 0;
}