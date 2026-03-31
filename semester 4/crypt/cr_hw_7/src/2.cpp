#include "../include/modular_arithmetic.hpp"
#include "../include/probability_test/probabilistic_primality_tests_realisations.hpp"
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp> 
#include "../include/rsa/rsa_vunerable_key_generator.hpp"
#include <iostream>
#include <utility>

using bigint = boost::multiprecision::cpp_int;
using boost_dist = boost::random::uniform_int_distribution<bigint>;

std::pair<bigint, bigint> perform_fermat_attack(bigint n)
{
    bigint x = boost::multiprecision::sqrt(n);
    if (x * x < n)
    {
        ++x;
    }
    while (true)
    {
        bigint y_sq = x * x - n;
        bigint y = boost::multiprecision::sqrt(y_sq);
        if (y * y == y_sq)
        {
            return std::make_pair(x + y, x - y);
        }
        ++x;
    }
}

int main()
{
    rsa_vulnerable_key_generator service(0.999);
    rsa_vulnerable_key_generator::RSAKeyPair key_pair = 
        service.generate_vulnerable_key(512, rsa_vulnerable_key_generator::VulnerabilityType::Fermat);

    std::pair<bigint, bigint> factors = perform_fermat_attack(key_pair.n);

    std::cout << "RSA Modulus (N): " << key_pair.n << std::endl;
    std::cout << "Public Exponent (e): " << key_pair.e << std::endl;
    std::cout << "Fermat Attack Results:" << std::endl;
    std::cout << "p = " << factors.first << std::endl;
    std::cout << "q = " << factors.second << std::endl;

    if (factors.first * factors.second == key_pair.n)
    {
        std::cout << "Verification Successful: p * q == N" << std::endl;
    }

    return 0;
}