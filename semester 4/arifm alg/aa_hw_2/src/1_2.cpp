#include <cstddef>
#include <stdexcept>
#include <vector>
#include <iostream>
#include "../include/help_poly.hpp"

std::vector<int> multiply_poly(
    const std::vector<int> poly_a,
    const std::vector<int> poly_b)
{
    size_t deg_a = poly_a.size();
    size_t deg_b = poly_b.size();

    if (deg_a > 32 || deg_b > 32)
    {
        throw std::invalid_argument("Polynomial degree cannot exceed 32");
    }

    std::vector<int> result(deg_a + deg_b - 1, 0);

    for (int i = 0; i < deg_a; i++)
    {
        if (poly_a[i] != 0)
        {
            if (poly_a[i] != 1)
            {
                throw std::invalid_argument(
                "Polynomial A coefficient at position " + 
                std::to_string(i) + " must be 0 or 1");
            }

            for (int j = 0; j < deg_b; j++)
            {
                if (poly_b[j] != 0)
                {
                    if (poly_b[j] != 1)
                    {
                        throw std::invalid_argument(
                        "Polynomial B coefficient at position " + 
                        std::to_string(i) + " must be 0 or 1");
                    }
                    
                    result[i + j] ^= 1;
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

int main()
{
    std::cout << "========================================\n";
    
    // Тест 1: простые умножения
    std::cout << "Тест 1: Простые умножения\n";
    std::cout << "----------------------------------------\n";
    
    std::vector<std::pair<std::vector<int>, std::vector<int>>> test_cases = 
    {
        {{0}, {0}},                    // 0 * 0 = 0
        {{1}, {1}},                    // 1 * 1 = 1
        {{1, 0}, {1}},                 // x * 1 = x
        {{1, 0}, {0, 1}},              // x * x = x^2
        {{1, 1}, {1, 1}},              // (x + 1) * (x + 1) = x^2 + 1
        {{1, 1, 0}, {1, 0, 1}},        // (x^2 + x) * (x^2 + 1) = x^4 + x^3 + x^2 + x
        {{1, 0, 1}, {1, 0, 1}},        // (x^2 + 1) * (x^2 + 1) = x^4 + 1
        {{1, 1, 0, 1}, {1, 0, 0, 1}},   // (x^3 + x + 1) * (x^3 + 1) = x^6 + x^4 + x^3 + x + 1
        {{1, 1, 0, 1}, {0}},
        {{1, 1, 0, 1}, {1}}
    };
    
    for (size_t i = 0; i < test_cases.size(); ++i)
    {
        std::cout << "Пример " << i + 1 << ":\n";
        
        std::cout << "  A: ";
        print_poly_vector(test_cases[i].first);
        std::cout << "  (" << poly_to_string(test_cases[i].first) << ")\n";
        
        std::cout << "  B: ";
        print_poly_vector(test_cases[i].second);
        std::cout << "  (" << poly_to_string(test_cases[i].second) << ")\n";
        
        try
        {
            std::vector<int> result = multiply_poly(test_cases[i].first, test_cases[i].second);
            
            std::cout << "  A * B: ";
            print_poly_vector(result);
            std::cout << "  (" << poly_to_string(result) << ")\n";
        }
        catch (const std::exception& e)
        {
            std::cout << "  Ошибка: " << e.what() << "\n";
        }
        
        std::cout << "\n";
    }
    
    // Тест 2: проверка на некорректные коэффициенты
    std::cout << "Тест 2: Некорректные коэффициенты\n";
    std::cout << "----------------------------------------\n";
    
    std::vector<int> bad_poly = {1, 2, 1};
    std::vector<int> normal_poly = {1, 1};
    
    try
    {
        std::vector<int> result = multiply_poly(bad_poly, normal_poly);
        std::cout << "  Должно быть исключение, но результат получен\n";
    }
    catch (const std::exception& e)
    {
        std::cout << "  Исключение: " << e.what() << " (PASS)\n\n";
    }
    
    // Тест 3: степень больше 32
    std::cout << "Тест 3: Степень больше 32\n";
    std::cout << "----------------------------------------\n";
    
    std::vector<int> big_poly(33, 1);
    std::vector<int> normal = {1, 1};
    
    try
    {
        std::vector<int> result = multiply_poly(big_poly, normal);
        std::cout << "  Должно быть исключение, но результат получен\n";
    }
    catch (const std::exception& e)
    {
        std::cout << "  Исключение: " << e.what() << " (PASS)\n\n";
    }
    
    return 0;
}