#include <iostream>
#include <vector>
#include "../include/help.hpp"

int main()
{
    int successCount = 0;

    for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
    {
        std::vector<int> photos(100);
        for (int i = 0; i < 100; ++i)
        {
            photos[i] = i + 1;
        }

        int wantedPhotoIndex = getRandomNumber(0, 99);
        int wantedPhoto = photos[wantedPhotoIndex];

        std::vector<int> selectedPhotos;
        std::vector<bool> used(100, false);

        for (int i = 0; i < 10; ++i)
        {
            int randomIndex;
            do
            {
                randomIndex = getRandomNumber(0, 99);
            } while (used[randomIndex]);

            used[randomIndex] = true;
            selectedPhotos.push_back(photos[randomIndex]);
        }

        bool foundWanted = false;
        for (int i = 0; i < 10; ++i)
        {
            if (selectedPhotos[i] == wantedPhoto)
            {
                foundWanted = true;
                break;
            }
        }

        if (foundWanted)
        {
            successCount++;
        }
    }

    std::cout << "Probability of finding the wanted photo: "
            << static_cast<double>(successCount) / NUM_EXPERIMENTS << std::endl;

    return 0;
}