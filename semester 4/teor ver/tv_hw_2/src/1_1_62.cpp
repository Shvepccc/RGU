#include <iostream>
#include <cmath>
#include "../include/help.hpp"

int main()
{
    const double R = 1.0;
    const double sqrt3 = std::sqrt(3);
    int count_a = 0;
    int count_b = 0;

    for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
    {
        int in_triangle = 0;
        int seg_ab = 0;
        int seg_ac = 0;
        int seg_bc = 0;

        for (int point = 0; point < 4; ++point)
        {
            double x, y;
            do
            {
                int ix = getRandomNumber(0, 2000) - 1000;
                int iy = getRandomNumber(0, 2000) - 1000;
                x = ix / 1000.0;
                y = iy / 1000.0;
            } while (x * x + y * y > R * R);

            bool condition_1 = (sqrt3 * x + y - R <= 0);
            bool condition_2 = (-sqrt3 * x + y - R <= 0);
            bool condition_3 = (y + R / 2 >= 0);

            if (condition_1 && condition_2 && condition_3)
            {
                ++in_triangle;
            }
            else
            {
                if (!condition_1)
                {
                    ++seg_ab;
                }
                else if (!condition_2)
                {
                    ++seg_ac;
                }
                else
                {
                    ++seg_bc;
                }
            }
        }

        if (in_triangle == 4)
        {
            ++count_a;
        }
        if (in_triangle == 1 && seg_ab == 1 && seg_ac == 1 && seg_bc == 1)
        {
            ++count_b;
        }
    }

    std::cout << "Probability that all four points fall inside the triangle: "
              << static_cast<double>(count_a) / NUM_EXPERIMENTS << std::endl;
    std::cout << "Probability that one point in triangle and one in each segment: "
              << static_cast<double>(count_b) / NUM_EXPERIMENTS << std::endl;

    return 0;
}