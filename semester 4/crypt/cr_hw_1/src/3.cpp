#include <iostream>
#include "../include/bit_manipulator.hpp"
#include "../include/tests_3.hpp"

int main()
{
    try
    {
        run_unit_tests();
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}