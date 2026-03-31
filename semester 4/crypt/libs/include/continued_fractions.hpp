#ifndef CONTINUIED_FRACTION_H_
#define CONTINUIED_FRACTION_H_

#include "modular_arithmetic.hpp"
#include <vector>
#include <algorithm>

class continued_fractions
{
public:
    struct fraction
    {
        bigint numerator;
        bigint denominator;
    };

    static std::vector<bigint> get_continued_fraction(bigint a, bigint b)
    {
        std::vector<bigint> quotients;
        while (b != 0)
        {
            quotients.push_back(a / b);
            bigint temp = a % b;
            a = b;
            b = temp;
        }
        return quotients;
    }

    static fraction get_fraction_from_continued(const std::vector<bigint>& quotients)
    {
        if (quotients.empty())
        {
            return {0, 1};
        }
        bigint h_prev2 = 0;
        bigint h_prev1 = 1;
        bigint k_prev2 = 1;
        bigint k_prev1 = 0;

        for (const auto& q : quotients)
        {
            bigint h = q * h_prev1 + h_prev2;
            bigint k = q * k_prev1 + k_prev2;
            h_prev2 = h_prev1;
            h_prev1 = h;
            k_prev2 = k_prev1;
            k_prev1 = k;
        }
        return {h_prev1, k_prev1};
    }

    static std::vector<fraction> get_convergents(bigint a, bigint b)
    {
        std::vector<bigint> quotients = get_continued_fraction(a, b);
        std::vector<fraction> convergents;
        bigint h_prev2 = 0;
        bigint h_prev1 = 1;
        bigint k_prev2 = 1;
        bigint k_prev1 = 0;

        for (const auto& q : quotients)
        {
            bigint h = q * h_prev1 + h_prev2;
            bigint k = q * k_prev1 + k_prev2;
            convergents.push_back({h, k});
            h_prev2 = h_prev1;
            h_prev1 = h;
            k_prev2 = k_prev1;
            k_prev1 = k;
        }
        return convergents;
    }

    static std::vector<bool> get_calkin_wilf_path(bigint a, bigint b)
    {
        std::vector<bool> path;
        while (a != b)
        {
            if (a > b)
            {
                path.push_back(true);
                a = a - b;
            }
            else
            {
                path.push_back(false);
                b = b - a;
            }
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    static std::vector<bool> get_stern_brocot_path(bigint a, bigint b)
    {
        std::vector<bool> path;
        bigint ln = 0, ld = 1;
        bigint rn = 1, rd = 0;
        while (true)
        {
            bigint mn = ln + rn;
            bigint md = ld + rd;
            if (a == mn && b == md)
            {
                break;
            }
            if (a * md > b * mn)
            {
                path.push_back(true);
                ln = mn;
                ld = md;
            }
            else
            {
                path.push_back(false);
                rn = mn;
                rd = md;
            }
        }
        return path;
    }

    static fraction get_fraction_from_calkin_wilf(const std::vector<bool>& path)
    {
        bigint a = 1, b = 1;
        for (bool bit : path)
        {
            if (bit)
            {
                a = a + b;
            }
            else
            {
                b = a + b;
            }
        }
        return {a, b};
    }

    static fraction get_fraction_from_stern_brocot(const std::vector<bool>& path)
    {
        bigint ln = 0, ld = 1;
        bigint rn = 1, rd = 0;
        for (bool bit : path)
        {
            bigint mn = ln + rn;
            bigint md = ld + rd;
            if (bit)
            {
                ln = mn;
                ld = md;
            }
            else
            {
                rn = mn;
                rd = md;
            }
        }
        return {ln + rn, ld + rd};
    }

    static std::vector<fraction> get_convergents_from_stern_brocot(const std::vector<bool>& path)
    {
        fraction f = get_fraction_from_stern_brocot(path);
        return get_convergents(f.numerator, f.denominator);
    }
};

#endif