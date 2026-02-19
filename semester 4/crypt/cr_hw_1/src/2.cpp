#include <iostream>
#include <vector>
#include <map>
#include "../include/bit_substitution.hpp"
#include "../include/tests_2.hpp"

int main()
{
    try
    {
        run_unit_tests();
        
        std::cout << "\nDemonstration examples:\n";
        bit_substitution sub;
        std::vector<uint8_t> data = {0xAB, 0xCD};
        std::map<uint8_t, uint8_t> s_box = {
            {0x0,0xF}, {0x1,0xE}, {0x2,0xD}, {0x3,0xC},
            {0x4,0xB}, {0x5,0xA}, {0x6,0x9}, {0x7,0x8},
            {0x8,0x7}, {0x9,0x6}, {0xA,0x5}, {0xB,0x4},
            {0xC,0x3}, {0xD,0x2}, {0xE,0x1}, {0xF,0x0}
        };
        auto res = sub.substitute(data, s_box, 4);
        std::cout << "Input:  " << bit_substitution::format_bytes(data) << "\n";
        std::cout << "Output: " << bit_substitution::format_bytes(res) << "\n";
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}