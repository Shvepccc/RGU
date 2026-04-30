#include <vector>
#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include "../include/modular_arithmetic.hpp"
#include <cmath>
#include "../include/diffie_hellman.hpp"

int main()
{
    for (int n = 2; n < 1000; n += std::floor(std::sqrt(n)))
    {
        if (n % 2 == 0 && n > 4) continue;

        std::vector<bigint> roots = primitive_roots(n);
        std::cout << "Primitive roots modulo " << n << " (" << roots.size() << "): ";
        if (roots.empty())
        {
            std::cout << "none";
        }
        else
        {
            for (size_t i = 0; i < roots.size(); ++i)
            {
                std::cout << roots[i];
                if (i < roots.size() - 1) std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }

    return 0;
}