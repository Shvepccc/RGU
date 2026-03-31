#ifndef RSA_VUNERABLE_KEY_GEN_H_
#define RSA_VUNERABLE_KEY_GEN_H_

#include "../modular_arithmetic.hpp"
#include "../probability_test/probabilistic_primality_tests_realisations.hpp"
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp> 

using bigint = boost::multiprecision::cpp_int;
using boost_dist = boost::random::uniform_int_distribution<bigint>;

class rsa_vulnerable_key_generator
{
public:

    rsa_vulnerable_key_generator(double min_prob) : min_probability(min_prob)
    {}

    struct RSAKeyPair
    {
        bigint n;
        bigint e;
        bigint d;
    };

    enum class VulnerabilityType
    {
        Fermat,
        Wiener
    };

    RSAKeyPair generate_vulnerable_key(std::size_t bits, VulnerabilityType type)
    {
        if (type == VulnerabilityType::Fermat)
        {
            return generate_fermat_weak(bits);
        }
        else
        {
            return generate_wiener_weak(bits);
        }
    }

private:
    double min_probability = 0;

    bigint generate_prime(std::size_t bits)
    {
        std::random_device rd;
        boost::random::mt19937 gen(rd());
        bigint min = bigint(1) << (bits - 1);
        bigint max = (bigint(1) << bits) - 1;
        boost::random::uniform_int_distribution<bigint> dist(min, max);

        while (true)
        {
            miller_rabin_test mr_test;
            bigint p = dist(gen);
            if (mr_test.is_prime(p, min_probability))
            {
                return p;
            }
        }
    }

    RSAKeyPair generate_fermat_weak(std::size_t bits)
    {
        miller_rabin_test mr_test;
        bigint p = generate_prime(bits / 2);
        bigint q = p + 2;
        while (!mr_test.is_prime(q, min_probability))
        {
            q += 2;
        }

        bigint n = p * q;
        bigint phi = (p - 1) * (q - 1);
        bigint e = 65537;
        
        while (gcd(e, phi) != 1)
        {
            e += 2;
        }
        
        bigint d = mod_inverse(e, phi);

        return {n, e, d};
    }

    RSAKeyPair generate_wiener_weak(std::size_t bits)
    {
        bigint p = generate_prime(bits / 2);
        bigint q;
        do
        {
            q = generate_prime(bits / 2);
        } while (p == q || (p > q ? p < 2 * q : q < 2 * p) == false);

        bigint n = p * q;
        bigint phi = (p - 1) * (q - 1);

        bigfloat n_f = static_cast<bigfloat>(n);
        bigint d_limit = static_cast<bigint>(boost::multiprecision::pow(n_f, 0.25) / 3.0);

        std::random_device rd;
        boost::random::mt19937 gen(rd());
        boost::random::uniform_int_distribution<bigint> dist(bigint(1) << (bits / 8), d_limit);

        bigint d, e;
        while (true)
        {
            d = dist(gen);
            if (gcd(d, phi) == 1)
            {
                e = mod_inverse(d, phi);
                if (e != 0)
                {
                    break;
                }
            }
        }

        return {n, e, d};
    }
};

#endif