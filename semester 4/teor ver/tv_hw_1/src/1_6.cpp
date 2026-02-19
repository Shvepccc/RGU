#include <iostream>
#include <cstdlib>
#include "../include/help.hpp"

int main()
{
    const int totalCubes = 1000;
    const int edgeLength = 10;

    int cubesWithOne = 0;
    int cubesWithTwo = 0;
    int cubesWithThree = 0;

    for (int i = 0; i < edgeLength; ++i)
    {
        for (int j = 0; j < edgeLength; ++j)
        {
            for (int k = 0; k < edgeLength; ++k)
            {
                int paintedFaces = 0;

                if (i == 0 || i == edgeLength - 1) paintedFaces++;
                if (j == 0 || j == edgeLength - 1) paintedFaces++;
                if (k == 0 || k == edgeLength - 1) paintedFaces++;

                if (paintedFaces == 1) cubesWithOne++;
                else if (paintedFaces == 2) cubesWithTwo++;
                else if (paintedFaces == 3) cubesWithThree++;
            }
        }
    }

    int successOne = 0;
    int successTwo = 0;
    int successThree = 0;

    for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
    {
        int randomCubeIndex = getRandomNumber(1, totalCubes);

        if (randomCubeIndex <= cubesWithOne)
        {
            successOne++;
        }
        else if (randomCubeIndex <= cubesWithOne + cubesWithTwo)
        {
            successTwo++;
        }
        else if (randomCubeIndex <= cubesWithOne + cubesWithTwo + cubesWithThree)
        {
            successThree++;
        }
    }

    std::cout << "Probability of one painted face: " << static_cast<double>(successOne) / NUM_EXPERIMENTS << std::endl;
    std::cout << "Probability of two painted faces: " << static_cast<double>(successTwo) / NUM_EXPERIMENTS << std::endl;
    std::cout << "Probability of three painted faces: " << static_cast<double>(successThree) / NUM_EXPERIMENTS << std::endl;

    return 0;
}