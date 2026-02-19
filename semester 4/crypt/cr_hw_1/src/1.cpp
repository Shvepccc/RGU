#include <iostream>
#include "../include/tests_1.hpp"

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