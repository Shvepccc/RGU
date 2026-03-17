#ifndef MODULAR_ARITHMETIC_H_
#define MODULAR_ARITHMETIC_H_

#include <iostream>
#include <cmath>
#include <cstdint>

int64_t gcd(int64_t a, int64_t b)
{
    a = std::llabs(a);
    b = std::llabs(b);
    while (b != 0)
    {
        int64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

struct Bezout
{
    int64_t gcd;
    int64_t x;
    int64_t y;
};

Bezout extended_gcd(int64_t a, int64_t b)
{
    a = std::llabs(a);
    b = std::llabs(b);
    int64_t s = 0, old_s = 1;
    int64_t t = 1, old_t = 0;
    int64_t r = b, old_r = a;
    while (r != 0)
    {
        int64_t q = old_r / r;
        int64_t tmp = r;
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

int64_t mod_pow(int64_t base, int64_t exp, int64_t mod)
{
    int64_t result = 1;
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

int legendre_symbol(int64_t a, int64_t p)
{
    if (p <= 2 || p % 2 == 0)
    {
        return 0;
    }
    a = a % p;
    if (a < 0) a += p;
    if (a == 0) return 0;
    if (a == 1) return 1;
    int64_t res = mod_pow(a, (p - 1) / 2, p);
    return (res == p - 1) ? -1 : static_cast<int>(res);
}

int jacobi_symbol(int64_t a, int64_t n)
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
            int64_t r = n % 8;
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

int64_t mod_inverse(int64_t a, int64_t n)
{
    Bezout bz = extended_gcd(a, n);
    if (bz.gcd != 1)
    {
        return 0;
    }
    int64_t res = bz.x % n;
    if (res < 0) res += n;
    return res;
}

int64_t phi_definition(int64_t n)
{
    if (n <= 0) return 0;
    int64_t count = 0;
    for (int64_t k = 1; k <= n; ++k)
    {
        if (gcd(k, n) == 1)
        {
            ++count;
        }
    }
    return count;
}

int64_t phi_factorization(int64_t n)
{
    if (n <= 0) return 0;
    int64_t result = n;
    int64_t m = n;
    for (int64_t p = 2; p * p <= m; ++p)
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

double phi_dft(int64_t n)
{
    if (n <= 0) return 0.0;
    double sum = 0.0;
    for (int64_t k = 1; k <= n; ++k)
    {
        sum += static_cast<double>(gcd(k, n)) * std::cos(2.0 * M_PI * k / n);
    }
    return std::llround(sum);
}

#endif