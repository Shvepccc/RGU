#ifndef DIFFIE_HELLMAN_HPP
#define DIFFIE_HELLMAN_HPP

#include <stdexcept>
#include <vector>
#include <algorithm>
#include <random>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/optional.hpp>
#include "modular_arithmetic.hpp"
#include "probability_test/probabilistic_primality_tests_realisations.hpp"

using bigint = boost::multiprecision::cpp_int;

// PRIMITIVE ROOTS ----------------------------------------------------------------------

std::vector<bigint> prime_factors(bigint n)
{
    std::vector<bigint> factors;
    if (n % 2 == 0)
    {
        factors.push_back(2);
        while (n % 2 == 0) n /= 2;
    }
    bigint limit = boost::multiprecision::sqrt(n);
    for (bigint i = 3; i <= limit; i += 2)
    {
        if (n % i == 0)
        {
            factors.push_back(i);
            while (n % i == 0) n /= i;
            limit = boost::multiprecision::sqrt(n);
        }
    }
    if (n > 1) 
    {
        factors.push_back(n);
    }
    return factors;
}

bigint phi_by_n(bigint n)
{
    if (n == 1) return 1;
    bigint result = n;
    for (const auto& p : prime_factors(n))
    {
        result -= result / p;
    }
    return result;
}

bool exists_primitive_roots(bigint n)
{
    if (n <= 0) return false;
    if (n == 1 || n == 2 || n == 4) return true;

    bigint odd = n;
    int twos = 0;
    while (odd % 2 == 0)
    {
        odd /= 2;
        ++twos;
    }

    if (twos > 1) return false;
    if (odd == 1) return false;

    return prime_factors(odd).size() == 1;
}

bool is_primitive_root(const bigint& g, const bigint& n,
                       const bigint& phi,
                       const std::vector<bigint>& prime_factors_of_phi)
{
    if (boost::multiprecision::gcd(g, n) != 1)
        return false;
    
    if (phi == 1) return true;
    
    for (const auto& factor : prime_factors_of_phi)
    {
        if (mod_pow(g, phi / factor, n) == 1)
            return false;
    }
    return true;
}

bigint find_first_primitive_root(const bigint& n, const bigint& phi, const std::vector<bigint>& factors_phi)
{
    if (n == 2) return 1;
    for (bigint g = 2; g < n; ++g)
    {
        if (is_primitive_root(g, n, phi, factors_phi))
            return g;
    }
    return -1;
}

std::vector<bigint> primitive_roots(bigint n)
{
    std::vector<bigint> result;
    if (n < 1 || !exists_primitive_roots(n)) return result;

    bigint phi_n = phi_by_n(n);
    std::vector<bigint> factors_phi = prime_factors(phi_n);

    bigint g = find_first_primitive_root(n, phi_n, factors_phi);

    if (phi_n == 1)
    {
        result.push_back(g);
        return result;
    }

    for (bigint i = 1; i < phi_n; ++i)
    {
        if (boost::multiprecision::gcd(i, phi_n) == 1)
        {
            result.push_back(mod_pow(g, i, n));
        }
    }

    std::sort(result.begin(), result.end());
    return result;
}


// DH ----------------------------------------------------------------------------------------

bigint generate_safe_prime(int bits)
{
    if (bits < 2) throw std::invalid_argument("[DH] bits must be at least 2");
    
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<unsigned long long> dist;
    
    while (true)
    {
        bigint candidate = 0;
        
        int full_blocks = bits / 64;
        for (int i = 0; i < full_blocks; ++i)
        {
            candidate <<= 64;
            candidate |= dist(gen);
        }
        
        int remaining_bits = bits % 64;
        if (remaining_bits > 0)
        {
            unsigned long long last_block = dist(gen);
            last_block &= (1ULL << remaining_bits) - 1;
            candidate <<= remaining_bits;
            candidate |= last_block;
        }
        
        candidate |= (bigint(1) << (bits - 1));
        candidate |= 1;
        
        miller_rabin_test mlt;
        bigint q = (candidate - 1) / 2;
        if (mlt.is_prime(q, 0.999) && 
            mlt.is_prime(candidate, 0.999))
        {
            return candidate;
        }
    }
}

struct DHParameters
{
    bigint p;
    bigint g;
};

DHParameters generate_parameters(int bits = 512)
{
    if (bits < 2) throw std::invalid_argument("[DH] bits must be at least 2");
    
    DHParameters params;
    params.p = generate_safe_prime(bits);
    bigint q = (params.p - 1) / 2;
    params.g = 2;
    while (true)
    {
        if (mod_pow(params.g, 2, params.p) != 1 && 
            mod_pow(params.g, q, params.p) != 1)
        {
            break;
        }
        params.g++;
    }
    
    return params;
}

#endif