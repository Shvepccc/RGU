#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
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

std::vector<int> poly_add(
    const std::vector<int>& a,
    const std::vector<int>& b)
{
    size_t max_size = std::max(a.size(), b.size());
    std::vector<int> result(max_size, 0);
    
    for (size_t i = 0; i < max_size; i++)
    {
        int val_a = (i < a.size()) ? a[i] : 0;
        int val_b = (i < b.size()) ? b[i] : 0;
        
        if (val_a != 0 && val_a != 1)
        {
            throw std::invalid_argument(
                "Polynomial A coefficient at position " + 
                std::to_string(i) + " must be 0 or 1");
        }
        if (val_b != 0 && val_b != 1)
        {
            throw std::invalid_argument(
                "Polynomial B coefficient at position " + 
                std::to_string(i) + " must be 0 or 1");
        }
        
        result[i] = val_a ^ val_b;
    }
    
    while (!result.empty() && result.back() == 0)
    {
        result.pop_back();
    }
    
    return result;
}

std::pair<std::vector<int>, std::vector<int>> poly_divmod(
    const std::vector<int>& dividend,
    const std::vector<int>& divisor_input)
{
    std::vector<int> divisor = divisor_input;
    while (!divisor.empty() && divisor.back() == 0)
        divisor.pop_back();

    if (divisor.empty())
        throw std::invalid_argument("Division by zero polynomial");

    std::vector<int> remainder = dividend;

    while (!remainder.empty() && remainder.back() == 0)
        remainder.pop_back();

    if (remainder.size() < divisor.size())
        return { {}, remainder };

    std::vector<int> quotient(remainder.size() - divisor.size() + 1, 0);

    while (!remainder.empty() && remainder.size() >= divisor.size())
    {
        size_t deg_diff = remainder.size() - divisor.size();

        quotient[deg_diff] = 1;

        for (size_t i = 0; i < divisor.size(); i++)
        {
            remainder[deg_diff + i] ^= divisor[i];
        }

        while (!remainder.empty() && remainder.back() == 0)
            remainder.pop_back();
    }

    while (!quotient.empty() && quotient.back() == 0)
        quotient.pop_back();

    return { quotient, remainder };
}

std::vector<int> poly_extended_euclid(
    const std::vector<int>& a,
    const std::vector<int>& f)
{
    if (a.empty() || (a.size() == 1 && a[0] == 0))
    {
        return std::vector<int>();
    }
    
    std::vector<int> r0 = f;
    std::vector<int> r1 = a;
    
    std::vector<int> u0 = {1};
    std::vector<int> u1 = {0};
    
    std::vector<int> v0 = {0};
    std::vector<int> v1 = {1};
    
    while (!(r1.size() == 1 && r1[0] == 0) && !r1.empty())
    {
        auto div_result = poly_divmod(r0, r1);
        std::vector<int> q = div_result.first;
        std::vector<int> r = div_result.second;
        
        std::vector<int> q_mul_u1 = multiply_poly(q, u1);
        std::vector<int> u = poly_add(u0, q_mul_u1);
        
        std::vector<int> q_mul_v1 = multiply_poly(q, v1);
        std::vector<int> v = poly_add(v0, q_mul_v1);
        
        r0 = r1;
        r1 = r;
        
        u0 = u1;
        u1 = u;
        
        v0 = v1;
        v1 = v;
    }
    
    if (r0.size() == 1 && r0[0] == 1)
    {
        return v0;
    }
    
    return std::vector<int>();
}

void print_separator()
{
    std::cout << std::string(60, '-') << "\n";
}

void print_header(const std::string& text)
{
    std::cout << "\n" << text << "\n";
    print_separator();
}

void print_test_result(
    const std::string& element,
    const std::string& inverse,
    const std::string& check)
{
    std::cout << "  " << std::left << std::setw(20) << element 
              << " | " << std::setw(20) << inverse 
              << " | " << check << "\n";
}

int main()
{
    std::cout << "Расширенный алгоритм Евклида для GF(2^n)\n";
    std::cout << "========================================\n";
    
    // Тест 1: GF(2^4) с неприводимым многочленом x^4 + x + 1
    std::cout << "\n1. Поле GF(2^4) с f(x) = x^4 + x + 1\n";
    std::cout << "----------------------------------------\n";
    
    std::vector<int> f = {1, 1, 0, 0, 1};
    
    std::vector<std::vector<int>> elements = 
    {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {1, 1, 0, 0},
        {1, 0, 1, 0},
        {1, 1, 0, 1},
        {1, 0, 0, 1},
        {1, 1, 1, 1}
    };
    
    for (size_t i = 0; i < elements.size(); i++)
    {
        std::vector<int> a = elements[i];
        std::vector<int> inv = poly_extended_euclid(a, f);
        
        std::cout << "a = " << poly_to_string(a) << "\n";
        
        if (inv.empty())
        {
            std::cout << "  обратный элемент не найден\n";
        }
        else
        {
            std::cout << "  a^(-1) = " << poly_to_string(inv) << "\n";
            
            std::vector<int> prod = multiply_poly(a, inv);
            auto div_result = poly_divmod(prod, f);
            std::vector<int> reduced = div_result.second;
            
            std::cout << "  a * a^(-1) = " << poly_to_string(reduced) << " (mod f)\n";
        }
        std::cout << "\n";
    }
    
    // Тест 2: GF(2^3) с разными неприводимыми многочленами
    std::cout << "2. Поле GF(2^3) с разными многочленами\n";
    std::cout << "----------------------------------------\n";
    
    std::vector<std::pair<std::vector<int>, std::string>> fields = 
    {
        {{1, 1, 0, 1}, "x^3 + x + 1"},
        {{1, 0, 1, 1}, "x^3 + x^2 + 1"}
    };
    
    std::vector<int> elem = {1, 1, 0};
    
    for (size_t i = 0; i < fields.size(); i++)
    {
        std::cout << "\nМногочлен: " << fields[i].second << "\n";
        std::cout << "a = x + 1\n";
        
        std::vector<int> inv = poly_extended_euclid(elem, fields[i].first);
        
        if (!inv.empty())
        {
            std::cout << "a^(-1) = " << poly_to_string(inv) << "\n";
            
            std::vector<int> prod = multiply_poly(elem, inv);
            auto div_result = poly_divmod(prod, fields[i].first);
            std::vector<int> check = div_result.second;
            
            std::cout << "a * a^(-1) = " << poly_to_string(check) << " (mod f)\n";
        }
    }
    
    // Тест 3: Граничные случаи
    std::cout << "\n3. Граничные случаи\n";
    std::cout << "----------------------------------------\n";
    
    std::vector<int> f4 = {1, 1, 0, 0, 1};
    
    std::vector<int> zero = {0};
    std::vector<int> inv_zero = poly_extended_euclid(zero, f4);
    std::cout << "a = 0, обратный элемент: ";
    if (inv_zero.empty()) std::cout << "не существует\n";
    
    std::vector<int> one = {1};
    std::vector<int> inv_one = poly_extended_euclid(one, f4);
    std::cout << "a = 1, обратный элемент: " << poly_to_string(inv_one) << "\n";
    
    // Тест 4: Все ненулевые элементы GF(2^3)
    std::cout << "\n4. Все ненулевые элементы GF(2^3) с f(x) = x^3 + x + 1\n";
    std::cout << "----------------------------------------\n";
    
    std::vector<int> f3 = {1, 1, 0, 1};
    
    for (int val = 1; val < 8; val++)
    {
        std::vector<int> a(3, 0);
        a[0] = (val >> 0) & 1;
        a[1] = (val >> 1) & 1;
        a[2] = (val >> 2) & 1;
        
        std::vector<int> inv = poly_extended_euclid(a, f3);
        
        if (!inv.empty())
        {
            std::cout << poly_to_string(a) << " -> " << poly_to_string(inv) << "\n";
        }
    }
    
    // Тест 5: Пример из лекции
    std::cout << "\n5. Пример: x^3 + x + 1 в GF(2^4) с f(x) = x^4 + x + 1\n";
    std::cout << "----------------------------------------\n";
    
    std::vector<int> a5 = {1, 1, 0, 1};
    std::vector<int> inv5 = poly_extended_euclid(a5, f4);
    
    std::cout << "a = " << poly_to_string(a5) << "\n";
    std::cout << "a^(-1) = " << poly_to_string(inv5) << "\n";
    
    std::vector<int> prod5 = multiply_poly(a5, inv5);
    auto div5 = poly_divmod(prod5, f4);
    std::cout << "a * a^(-1) = " << poly_to_string(div5.second) << " (mod f)\n";
    
    std::cout << "\n========================================\n";
    
    return 0;
}