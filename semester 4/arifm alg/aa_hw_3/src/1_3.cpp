#include <iostream>
#include <vector>
#include <functional>

#include "../include/iteration.hpp"

int main()
{
    double eps = 1e-6;

    {
        std::cout << "\na)" << std::endl;

        auto f =
        [](double x)
        {
            return x*x*x + 3*x*x - 1;
        };

        auto df =
        [](double x)
        {
            return 3*x*x + 6*x;
        };

        std::vector<double> roots = solve(f, df, -5, 5, 0.5, eps);

        for (double r : roots)
        {
            std::cout << r << std::endl;
        }
    }

    {
        std::cout << "\nb)" << std::endl;

        auto f =
        [](double x)
        {
            return x*x*x*x - x*x*x;
        };

        auto df =
        [](double x)
        {
            return 4*x*x*x - 3*x*x;
        };

        std::vector<double> roots = solve(f, df, -2, 3, 0.2, eps);

        for (double r : roots)
        {
            std::cout << r << std::endl;
        }
    }

    {
        std::cout << "\nc)" << std::endl;

        auto f =
        [](double x)
        {
            return x*x - 3*x + 2;
        };

        auto df =
        [](double x)
        {
            return 2*x - 3;
        };

        std::vector<double> roots = solve(f, df, -1, 5, 0.5, eps);

        for (double r : roots)
        {
            std::cout << r << std::endl;
        }
    }

    return 0;
}