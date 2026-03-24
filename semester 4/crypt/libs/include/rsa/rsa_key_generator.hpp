#ifndef RSA_KEY_GENERATOR_H_
#define RSA_KEY_GENERATOR_H_

#include "../probability_test/probabilistic_primality_tests_realisations.hpp"
#include "../modular_arithmetic.hpp"
#include <random>
#include <stdexcept>
#include <algorithm>
#include <cmath>

class rsa_key_generator
{
private:
    std::mt19937_64 gen;
    double min_probability;
    size_t prime_bits;

    bool is_prime(uint64_t n)
    {
        if (n < 2) return false;
        if (n == 2 || n == 3) return true;
        if (n % 2 == 0) return false;
        
        miller_rabin_test mr_test;
        return mr_test.is_prime(n, min_probability);
    }

    uint64_t generate_random_prime()
    {
        std::uniform_int_distribution<uint64_t> dist(
            static_cast<uint64_t>(1) << (prime_bits - 1),
            (static_cast<uint64_t>(1) << prime_bits) - 1
        );
        
        uint64_t candidate;
        do
        {
            candidate = dist(gen);
        } while (!is_prime(candidate));
        
        return candidate;
    }

    uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod)
    {
        return static_cast<uint64_t>((static_cast<__uint128_t>(a) * b) % mod);
    }

    uint64_t mod_pow_secure(uint64_t base, uint64_t exp, uint64_t mod)
    {
        uint64_t result = 1;
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

    bool fermat_attack_check(uint64_t p, uint64_t q)
    {
        uint64_t n = p * q;
        uint64_t sqrt_n = static_cast<uint64_t>(std::sqrt(n));
        
        for (uint64_t a = sqrt_n - 10; a <= sqrt_n + 10; ++a)
        {
            uint64_t b_squared = a * a - n;
            uint64_t b = static_cast<uint64_t>(std::sqrt(b_squared));
            if (b * b == b_squared)
            {
                uint64_t p_candidate = a - b;
                uint64_t q_candidate = a + b;
                if (p_candidate == p || p_candidate == q)
                {
                    return false;
                }
            }
        }
        return true;
    }

    bool wiener_attack_check(uint64_t p, uint64_t q)
    {
        uint64_t n = p * q;
        uint64_t phi_n = (p - 1) * (q - 1);
        
        uint64_t e = 65537;
        while (e < phi_n)
        {
            if (::gcd(e, phi_n) == 1)
            {
                break;
            }
            ++e;
        }
        
        uint64_t d = mod_inverse(e, phi_n);
        
        if (d == 0)
        {
            return true;
        }
        
        uint64_t k = e * d - 1;
        if (k % phi_n != 0)
        {
            return true;
        }
        
        k /= phi_n;
        
        uint64_t d_approx = static_cast<uint64_t>(std::pow(n, 0.25) / 3);
        
        if (d < d_approx)
        {
            return false;
        }
        
        return true;
    }

public:
    struct rsa_key_pair
    {
        uint64_t public_key;
        uint64_t private_key;
        uint64_t modulus;
    };

    rsa_key_generator(size_t bits, double prob) 
        : gen(std::random_device{}()), min_probability(prob), prime_bits(bits)
    {
        if (bits < 8 || bits > 32)
        {
            throw std::invalid_argument("Prime bits must be between 8 and 32");
        }
    }

    rsa_key_pair generate_keys()
    {
        uint64_t p, q;
        
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
        
        uint64_t n = p * q;
        uint64_t phi_n = (p - 1) * (q - 1);
        
        uint64_t e = 65537;
        while (e < phi_n)
        {
            if (gcd(e, phi_n) == 1)
            {
                break;
            }
            e += 2;
        }
        
        uint64_t d = mod_inverse(e, phi_n);
        
        if (d == 0)
        {
            throw std::runtime_error("Failed to compute modular inverse");
        }
        
        return {e, d, n};
    }
};

#endif