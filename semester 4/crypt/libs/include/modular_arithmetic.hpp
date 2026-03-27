#ifndef MODULAR_ARITHMETIC_H_
#define MODULAR_ARITHMETIC_H_

#include <iostream>
#include <cmath>
#include <cstdint>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/math/constants/constants.hpp>

using bigint = boost::multiprecision::cpp_int;
using bigfloat = boost::multiprecision::cpp_bin_float_50;

bigint gcd(bigint a, bigint b)
{
    a = boost::multiprecision::abs(a);
    b = boost::multiprecision::abs(b);
    while (b != 0)
    {
        bigint t = b;
        b = a % b;
        a = t;
    }
    return a;
}

struct Bezout
{
    bigint gcd;
    bigint x;
    bigint y;
};

Bezout extended_gcd(bigint a, bigint b)
{
    a = boost::multiprecision::abs(a);
    b = boost::multiprecision::abs(b);
    bigint s = 0, old_s = 1;
    bigint t = 1, old_t = 0;
    bigint r = b, old_r = a;
    while (r != 0)
    {
        bigint q = old_r / r;
        bigint tmp = r;
        r = old_r - q * r;
        old_r = tmp;

        tmp = s;
        s = old_s - q * s;
        old_s = tmp;

        tmp = t;
        t = old_t - q * t;
        old_t = tmp;
    }
    return {old_r, old_s, old_t};
}

bigint mod_pow(bigint base, bigint exp, bigint mod)
{
    bigint result = 1;
    base %= mod;
    while (exp > 0)
    {
        if (exp & 1)
        {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

int legendre_symbol(bigint a, bigint p)
{
    if (p <= 2 || p % 2 == 0)
    {
        return 0;
    }
    a = a % p;
    if (a < 0) a += p;
    if (a == 0) return 0;
    if (a == 1) return 1;
    bigint res = mod_pow(a, (p - 1) / 2, p);
    return (res == p - 1) ? -1 : static_cast<int>(res);
}

int jacobi_symbol(bigint a, bigint n)
{
    if (n <= 0 || n % 2 == 0)
    {
        return 0;
    }
    a = a % n;
    if (a < 0) a += n;
    int t = 1;
    while (a != 0)
    {
        while (a % 2 == 0)
        {
            a /= 2;
            bigint r = n % 8;
            if (r == 3 || r == 5)
            {
                t = -t;
            }
        }
        std::swap(a, n);
        if (a % 4 == 3 && n % 4 == 3)
        {
            t = -t;
        }
        a = a % n;
    }
    return (n == 1) ? t : 0;
}

bigint mod_inverse(bigint a, bigint n)
{
    Bezout bz = extended_gcd(a, n);
    if (bz.gcd != 1)
    {
        return 0;
    }
    bigint res = bz.x % n;
    if (res < 0) res += n;
    return res;
}

bigint phi_definition(bigint n)
{
    if (n <= 0) return 0;
    bigint count = 0;
    for (bigint k = 1; k <= n; ++k)
    {
        if (gcd(k, n) == 1)
        {
            ++count;
        }
    }
    return count;
}

bigint phi_factorization(bigint n)
{
    if (n <= 0) return 0;
    bigint result = n;
    bigint m = n;
    for (bigint p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            while (m % p == 0)
            {
                m /= p;
            }
            result -= result / p;
        }
    }
    if (m > 1)
    {
        result -= result / m;
    }
    return result;
}

double phi_dft(bigint n)
{
    if (n <= 0) return 0.0;
    
    double sum = 0.0;
    bigfloat n_float = static_cast<bigfloat>(n);
    bigfloat pi = boost::math::constants::pi<bigfloat>();
    
    for (bigint k = 1; k <= n; ++k)
    {
        double gcd_val = static_cast<double>(gcd(k, n)); 
        
        bigfloat angle = 2.0 * pi * static_cast<bigfloat>(k) / n_float;
        
        double cos_val = static_cast<double>(boost::multiprecision::cos(angle));
        
        sum += gcd_val * cos_val;
    }
    
    return std::llround(sum);
}

#endif