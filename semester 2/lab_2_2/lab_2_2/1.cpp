#include <iostream>
#include "../libs/bigint.h"

//TODO: write funnction to count trailing zeros

int program_1_main(int argc, char* argv[])
{
    int value_1[4] = { INT_MIN, INT_MAX, 1, INT_MAX };
    bigint test_1(value_1, 1);

    int value_2[10] = { -1, 2, 3, 4, 5, 6, INT_MIN };
    bigint test_2(value_2, 1);

    std::cout << "Value 1: " << test_1 << std::endl;
    std::cout << "Value 2: " << test_2 << std::endl;

    std::cout << "test_1 + test_2 : " << test_1 + test_2 << std::endl;

	return 0;
}