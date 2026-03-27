#ifndef RSA_KEY_GENERATOR_H_
#define RSA_KEY_GENERATOR_H_

#include "../probability_test/probabilistic_primality_tests_realisations.hpp"
#include "../modular_arithmetic.hpp"
#include <random>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp> 

using bigint = boost::multiprecision::cpp_int;
using boost_dist = boost::random::uniform_int_distribution<bigint>;

class rsa_key_generator
{
private:
    std::mt19937_64 gen;
    double min_probability;
    size_t prime_bits;

    bool is_prime(bigint n)
    {
        if (n < 2) return false;
        if (n == 2 || n == 3) return true;
        if (n % 2 == 0) return false;
        
        miller_rabin_test mr_test;
        return mr_test.is_prime(n, min_probability);
    }

    bigint generate_random_prime()
    {
        boost_dist dist(
            static_cast<bigint>(1) << (prime_bits - 1),
            (static_cast<bigint>(1) << prime_bits) - 1
        );
        
        bigint candidate;
        do
        {
            candidate = dist(gen);
        } while (!is_prime(candidate));
        
        return candidate;
    }

    bigint mod_mul(bigint a, bigint b, bigint mod)
    {
        return (a * b) % mod;
    }

    bigint mod_pow_secure(bigint base, bigint exp, bigint mod)
    {
        bigint result = 1;
        base %= mod;
        while (exp > 0)
        {
            if (exp & 1)
            {
                result = mod_mul(result, base, mod);
            }
            base = mod_mul(base, base, mod);
            exp >>= 1;
        }
        return result;
    }

    bool fermat_attack_check(bigint p, bigint q)
    {
        bigint n = p * q;
        bigint sqrt_n = boost::multiprecision::sqrt(n);
        
        for (bigint a = sqrt_n - 10; a <= sqrt_n + 10; ++a)
        {
            bigint b_squared = a * a - n;
            if (b_squared < 0) continue;
            
            bigint b = boost::multiprecision::sqrt(b_squared);
            if (b * b == b_squared)
            {
                bigint p_candidate = a - b;
                bigint q_candidate = a + b;
                if (p_candidate == p || p_candidate == q)
                {
                    return false;
                }
            }
        }
        return true;
    }

    bool wiener_attack_check(bigint p, bigint q)
    {
        bigint e = 65537;
        bigint n = p * q;
        
        bigfloat n_f = n.convert_to<bigfloat>();
        bigint dw = static_cast<bigint>(boost::multiprecision::pow(n_f, 0.25) / 3.0);
    
        bigint phi_n = (p - 1) * (q - 1);

        bigint d = mod_inverse(e, phi_n);
        
        if (d < dw)
        {
            return false;
        }
        
        return true;
    }

public: 
    struct rsa_key_pair
    {
        bigint public_key;
        bigint private_key;
        bigint modulus;
    };

    rsa_key_generator(size_t bits, double prob) 
        : gen(std::random_device{}()), min_probability(prob), prime_bits(bits)
    {
        if (bits < 8 || bits > 4096)
        {
            throw std::invalid_argument("Prime bits must be between 8 and 4096");
        }
    }

    rsa_key_pair generate_keys()
    {
        bigint p, q;
        
        do
        {
            p = generate_random_prime();
            q = generate_random_prime();
        } while (p == q);

        if (p > q)
        {
            std::swap(p, q);
        }
        
        if (!fermat_attack_check(p, q))
        {
            throw std::runtime_error("Fermat attack vulnerability detected");
        }
        if (!wiener_attack_check(p, q))
        {
            throw std::runtime_error("Wiener attack vulnerability detected");
        }
        
        bigint n = p * q;
        bigint phi_n = (p - 1) * (q - 1);
        
        bigint e = 65537;
        while (e < phi_n)
        {
            if (gcd(e, phi_n) == 1)
            {
                break;
            }
            e += 2;
        }
        
        bigint d = mod_inverse(e, phi_n);
        
        if (d == 0)
        {
            throw std::runtime_error("Failed to compute modular inverse");
        }
        
        return {e, d, n};
    }
};

#endif