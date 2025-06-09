#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>

#include "../libs/bigfloat.h"
//#include "../../common_libs/t_list.h"

using bigint = boost::multiprecision::cpp_int;

#pragma region point_1

std::vector<bigint> calculate_coeff(bigfloat const& arg, std::size_t max_terms = 100)
{
    bigfloat x = arg;
    std::vector<bigint> result;
    std::size_t count = 0;

    while (count++ < max_terms)
    {
        bigint a = floor_div(x, bigfloat(1.0));
        result.push_back(a);

        x = x - a;          
        if (x == 0) break;  

        x = bigfloat(1) / x;
    }

    return result;
}

bigfloat calculate_coeff(std::vector<bigint> const& arg)
{
    if (arg.empty()) return bigfloat(0);

    bigfloat result = arg.back();

    for (int i = static_cast<int>(arg.size()) - 2; i >= 0; --i)
    {
        result = bigfloat(arg[i], 1) + bigfloat(1) / result;
    }

    return result;
}

#pragma endregion


#pragma region point_2

std::vector<bigfloat> calculate_convergents(bigfloat const& arg, std::size_t max_terms = 100)
{
    bigfloat x = arg;
    std::vector<bigfloat> result;

    bigint p_prev2 = 0, p_prev = 1;
    bigint q_prev2 = 1, q_prev = 0;

    for (std::size_t i = 0; i < max_terms; ++i)
    {
        bigint a = floor_div(x, bigfloat(1));
        bigint p = a * p_prev + p_prev2;
        bigint q = a * q_prev + q_prev2;

        result.emplace_back(bigfloat(p) / bigfloat(q));

        if (x == a) break;

        x = bigfloat(1) / (x - a);

        p_prev2 = p_prev;
        p_prev = p;
        q_prev2 = q_prev;
        q_prev = q;
    }

    return result;
}

#pragma endregion


#pragma region point_3 and point_4

std::vector<bool> Shtern_Broko_path(const bigfloat& arg)
{
    std::vector<bool> result;

    bigfloat a = 0, b = 1; // left start point: 0/1
    bigfloat c = 1, d = 0; // right start point: 1/0

    while (true)
    {
        bigfloat x = a + c;
        bigfloat y = b + d;
        bigfloat mediant = x / y;

        if (arg == mediant)
            break;

        if (arg < mediant)
        {
            result.push_back(false);
            c = x;
            d = y;
        }
        else
        {
            result.push_back(true);
            a = x;
            b = y;
        }
    }

    return result;
}


std::vector<bool> Kalkin_Uilf_path(bigfloat const& arg)
{
    // right = true
    // left = false

    std::vector<bool> result;
    double m = 1.0, n = 1.0;
    bigfloat current(1.0);

    while (true)
    {
        if (arg > current)
        {
            result.push_back(true);
            current = bigfloat(m + n, n);
            m += n;
        }
        else if (arg < current)
        {
            result.push_back(false);
            current = bigfloat(m, m + n);
            n += m;
        }
        else
        {
            break;
        }
    }
    return result;
}

#pragma endregion

int program_8_main(int argc, char* argv[])
{
    std::cout << "---" << std::endl;

    //point 1 part 1
    std::cout << "Coefficients for pi:\n";
    std::vector<bigint> bigint_vec = calculate_coeff(bigfloat(355, 113));
    for (auto it = bigint_vec.begin(); it != bigint_vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n" << std::endl;

    //point 1 part 2
    std::cout << "Collect bigfloat from coefficients of pi:\n";
    bigfloat calc_pi = calculate_coeff(bigint_vec);
    std::cout << calc_pi << " (double: " << to_double(calc_pi) << ")\n" << std::endl;

    //point 2
    std::cout << "Calculate convergents for pi:" << '\n';
    std::vector<bigfloat> bigfloat_vec = calculate_convergents(bigfloat(355, 113));
    for (auto it = bigfloat_vec.begin(); it != bigfloat_vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n" << std::endl;

    //point 3 and 4
    bigfloat a(3, 5);

    std::cout << "Shtern_Broko_path:\n";
    std::vector<bool> vec = Shtern_Broko_path(a);
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << ((*it) ? "R" : "L") << " ";
    }
    std::cout << std::endl;

    std::cout << "\nKalkin_Uilf_path:\n";
    vec = Kalkin_Uilf_path(a);
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << ((*it) ? "R" : "L") << " ";
    }
    std::cout << std::endl;

    return 0;
}