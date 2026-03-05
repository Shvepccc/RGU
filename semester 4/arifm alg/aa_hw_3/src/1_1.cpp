#include <cstdint>
#include <stdexcept>
#include <vector>
#include <iostream>
#include "../include/help_poly.hpp"

std::vector<int> to_poly(const uint64_t value, int field_degree_n)
{
    uint64_t max_value = (field_degree_n == 64) 
        ? UINT64_MAX 
        : (static_cast<uint64_t>(1) << field_degree_n) - 1;
    
    if (value > max_value)
    {
        throw std::invalid_argument("Value exceeds field size");
    }

    std::vector<int> result;
    result.reserve(field_degree_n);
    
    for (int i = 0; i < field_degree_n; ++i)
    {
        result.push_back(static_cast<int>((value >> i) & 1));
    }
    
    return result;
}

uint64_t to_int(const std::vector<int>& input_polynom, int field_degree_n)
{
    if (input_polynom.empty())
    {
        return 0;
    }

    uint64_t res = 0;
    
    for (size_t i = 0; i < input_polynom.size() && i < static_cast<size_t>(field_degree_n); ++i)
    {
        if (input_polynom[i] != 0)
        {
            if (input_polynom[i] != 1)
            {
                throw std::invalid_argument("Polynomial coefficients must be 0 or 1");
            }
            res |= (static_cast<uint64_t>(1) << i);
        }
    }
    
    return res;
}

int main()
{
    int n = 4;
    std::cout << "Поле GF(2^" << n << ")\n";
    std::cout << "================\n\n";
    
    std::cout << "1. Число -> полином:\n";
    uint64_t test_numbers[] = {0, 1, 2, 3, 5, 9, 11, 15};
    
    for (uint64_t num : test_numbers)
    {
        try
        {
            std::vector<int> poly = to_poly(num, n);
            std::cout << "   " << num << " -> ";
            print_poly_vector(poly);
            std::cout << "\n";
        }
        catch (const std::exception& e)
        {
            std::cout << "   " << num << " -> Ошибка: " << e.what() << "\n";
        }
    }
    
    std::cout << "\n2. Полином -> число:\n";
    std::vector<std::vector<int>> test_polys = {
        {0, 0, 0, 0},
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {1, 1, 0, 0},
        {1, 0, 1, 0},
        {1, 0, 0, 1},
        {1, 1, 0, 1},
        {1, 1, 1, 1}
    };
    
    for (const auto& poly : test_polys)
    {
        try
        {
            uint64_t num = to_int(poly, n);
            std::cout << "   ";
            print_poly_vector(poly);
            std::cout << " -> " << num << "\n";
        }
        catch (const std::exception& e)
        {
            std::cout << "   ";
            print_poly_vector(poly);
            std::cout << " -> Ошибка: " << e.what() << "\n";
        }
    }
    
    std::cout << "\n3. Проверка: число -> полином -> число:\n";
    for (uint64_t num : test_numbers)
    {
        try
        {
            std::vector<int> poly = to_poly(num, n);
            uint64_t back_num = to_int(poly, n);
            
            std::cout << "   " << num << " -> ";
            print_poly_vector(poly);
            std::cout << " -> " << back_num;
            
            if (num == back_num)
            {
                std::cout << "  (PASS)\n";
            }
            else
            {
                std::cout << "  (FAIL)\n";
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "   " << num << " -> Ошибка: " << e.what() << "\n";
        }
    }
    
    std::cout << "\n4. Граничные значения:\n";
    uint64_t max_val = (1 << n) - 1;
    std::cout << "   Максимальное число для GF(2^" << n << "): " << max_val << "\n";
    
    try
    {
        std::vector<int> poly = to_poly(max_val, n);
        uint64_t back_num = to_int(poly, n);
        std::cout << "   " << max_val << " -> ";
        print_poly_vector(poly);
        std::cout << " -> " << back_num << "\n";
    }
    catch (const std::exception& e)
    {
        std::cout << "   Ошибка: " << e.what() << "\n";
    }
    
    // Тест 5: выход за границы
    std::cout << "\n5. Выход за границы поля:\n";
    uint64_t too_big = 20;
    
    try
    {
        std::vector<int> poly = to_poly(too_big, n);
        std::cout << "   " << too_big << " -> ";
        print_poly_vector(poly);
        std::cout << "\n";
    }
    catch (const std::exception& e)
    {
        std::cout << "   " << too_big << " -> Ошибка: " << e.what() << "\n";
    }
    
    return 0;
}