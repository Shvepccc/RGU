#include <iostream>
#include "../../common_libs/t_list.h"
#include "../libs/polynom.h"
#include "../../common_libs/helpers.h"

int program_3_main(int argc, char* argv[])
{
    polynom p1("3x^2 + 2x - 5");
    polynom p2("x^2 - x + 1");

    std::cout << "P1: " << p1;
    std::cout << "P2: " << p2 << std::endl;

    polynom sum = p1 + p2;
    std::cout << "P1 + P2: " << sum;

    polynom diff = p1 - p2;
    std::cout << "P1 - P2: " << diff;

    polynom prod = p1 * p2;
    std::cout << "P1 * P2: " << prod;

    polynom quot = p1 / p2;
    std::cout << "P1 / P2: " << quot;
 
    polynom dp1 = p1.diff();
    std::cout << "diff(P1): " << dp1;

    polynom int_p1 = p1.intgr();
    std::cout << "intgr(P1): " << int_p1;

    std::cout << "P1 == P2: " << (p1 == p2 ? "True" : "False") << std::endl;
    std::cout << "P1 != P2: " << (p1 != p2 ? "True" : "False") << std::endl;

    polynom user_poly;
    std::cout << "Enter polynom without spaces (for example, -2x^3 + 4x - 1): ";
    std::cin >> user_poly;
    std::cout << "You entered: " << user_poly;

    return 0;
}