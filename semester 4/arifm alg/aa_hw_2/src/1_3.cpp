#include <iostream>
#include <string>
#include <cstdint>
#include "../include/help_poly.hpp"

uint64_t multiply_field(uint64_t a, uint64_t b, uint64_t f, int n)
{
    uint64_t result = 0;
    uint64_t mask = (1ULL << n) - 1;
    
    while (b)
    {
        if (b & 1)
        {
            result ^= a;
        }
        
        b >>= 1;
        a <<= 1;
        
        if (a & (1ULL << n))
        {
            a ^= f;
        }
        
        a &= mask;
    }
    
    return result & mask;
}

void print_test_header(const std::string& title)
{
    std::cout << "\n=== " << title << " ===\n";
}

int main()
{
    uint64_t f1 = 0b11001;  // x^4 + x + 1
    uint64_t f2 = 0b10011;  // x^4 + x^3 + 1
    uint64_t f3 = 0b11111;  // x^4 + x^3 + x^2 + x + 1
    int n = 4;

    // Тест 1: (x+1)*(x+1)
    print_test_header("(x+1)*(x+1)");
    
    uint64_t a = 0b0011;  // x+1
    uint64_t b = 0b0011;  // x+1
    
    std::cout << "x^4 + x + 1     : " 
              << poly_to_string(multiply_field(a, b, f1, n), n) << "\n";
    std::cout << "x^4 + x^3 + 1   : " 
              << poly_to_string(multiply_field(a, b, f2, n), n) << "\n";
    std::cout << "x^4 + x^3 + x^2 + x + 1: " 
              << poly_to_string(multiply_field(a, b, f3, n), n) << "\n";
    
    // Тест 2: Умножения в поле x^4 + x + 1
    print_test_header("Умножения в поле x^4 + x + 1");
    
    uint64_t tests[] = {
        0b0001,  // 1
        0b0010,  // x
        0b0101,  // x^2+1
        0b1011,  // x^3+x+1
        0b1001   // x^3+1
    };
    int num_tests = sizeof(tests) / sizeof(tests[0]);
    
    for (int i = 0; i < num_tests; i++)
    {
        for (int j = i; j < num_tests; j++)
        {
            uint64_t res = multiply_field(tests[i], tests[j], f1, n);
            std::cout << poly_to_string(tests[i], n) << " * " 
                     << poly_to_string(tests[j], n) << " = "
                     << poly_to_string(res, n) << "\n";
        }
    }
    
    // Тест 3
    print_test_header("Примеры умножения");
    
    struct TestPair {
        uint64_t first;
        uint64_t second;
    };
    
    TestPair examples[] = {
        {0b0001, 0b0010},  // 1 * x
        {0b0010, 0b0010},  // x * x
        {0b0011, 0b1001},  // (x+1)*(x^3+1)
        {0b0101, 0b0101},  // (x^2+1)*(x^2+1)
        {0b1011, 0b1001}   // (x^3+x+1)*(x^3+1)
    };
    int num_examples = sizeof(examples) / sizeof(examples[0]);
    
    for (int i = 0; i < num_examples; i++)
    {
        uint64_t res = multiply_field(examples[i].first, examples[i].second, f1, n);
        std::cout << poly_to_string(examples[i].first, n) << " * "
                 << poly_to_string(examples[i].second, n) << " = "
                 << poly_to_string(res, n) << "\n";
    }
    
    return 0;
}