#include <iostream>
#include <vector>
#include <string>
#include "../include/help_poly.hpp"

std::vector<int> multiply_field(
    const std::vector<int>& a,
    const std::vector<int>& b,
    const std::vector<int>& f,
    int n)
{
    std::vector<int> result(n, 0);
    
    for (size_t i = 0; i < a.size(); i++)
    {
        if (a[i] == 0) continue;
        if (a[i] != 1) return std::vector<int>();
        
        for (size_t j = 0; j < b.size(); j++)
        {
            if (b[j] == 0) continue;
            if (b[j] != 1) return std::vector<int>();
            
            int pos = i + j;
            
            if (pos < n)
            {
                result[pos] ^= 1;
            }
            else
            {
                int shift = pos - n;
                for (int k = 0; k < n; k++)
                {
                    if (f[k] == 1)
                    {
                        result[shift + k] ^= 1;
                    }
                }
            }
        }
    }
    
    while (!result.empty() && result.back() == 0)
    {
        result.pop_back();
    }
    
    return result;
}

void print_test_header(const std::string& title)
{
    std::cout << "\n=== " << title << " ===\n";
}

int main()
{
    std::vector<int> f1 = {1, 1, 0, 0, 1};  // x^4 + x + 1
    std::vector<int> f2 = {1, 0, 0, 1, 1};  // x^4 + x^3 + 1
    std::vector<int> f3 = {1, 1, 1, 1, 1};  // x^4 + x^3 + x^2 + x + 1
    
    // Тест 1: (x+1)*(x+1)
    print_test_header("(x+1)*(x+1)");
    
    std::vector<int> a = {1, 1, 0, 0};  // x+1
    std::vector<int> b = {1, 1, 0, 0};  // x+1
    
    std::cout << "x^4 + x + 1     : " << poly_to_string(multiply_field(a, b, f1, 4)) << "\n";
    std::cout << "x^4 + x^3 + 1   : " << poly_to_string(multiply_field(a, b, f2, 4)) << "\n";
    std::cout << "x^4 + x^3 + x^2 + x + 1: " << poly_to_string(multiply_field(a, b, f3, 4)) << "\n";
    
    // Тест 2
    print_test_header("Умножения в поле x^4 + x + 1");
    
    std::vector<std::vector<int>> tests = {
        {1, 0, 0, 0},  // 1
        {0, 1, 0, 0},  // x
        {1, 0, 1, 0},  // x^2+1
        {1, 1, 0, 1},  // x^3+x+1
        {1, 0, 0, 1}   // x^3+1
    };
    
    for (size_t i = 0; i < tests.size(); i++)
    {
        for (size_t j = i; j < tests.size(); j++)
        {
            std::vector<int> res = multiply_field(tests[i], tests[j], f1, 4);
            std::cout << poly_to_string(tests[i]) << " * " 
                     << poly_to_string(tests[j]) << " = "
                     << poly_to_string(res) << "\n";
        }
    }
    
    // Тест 3
    print_test_header("Примеры умножения");
    
    std::vector<std::pair<std::vector<int>, std::vector<int>>> examples = {
        {{1, 0, 0, 0}, {0, 1, 0, 0}},  // 1 * x
        {{0, 1, 0, 0}, {0, 1, 0, 0}},  // x * x
        {{1, 1, 0, 0}, {1, 0, 0, 1}},  // (x+1)*(x^3+1)
        {{1, 0, 1, 0}, {1, 0, 1, 0}},  // (x^2+1)*(x^2+1)
        {{1, 1, 0, 1}, {1, 0, 0, 1}}   // (x^3+x+1)*(x^3+1)
    };
    
    for (auto& ex : examples)
    {
        std::vector<int> res = multiply_field(ex.first, ex.second, f1, 4);
        std::cout << poly_to_string(ex.first) << " * "
                 << poly_to_string(ex.second) << " = "
                 << poly_to_string(res) << "\n";
    }
    
    return 0;
}