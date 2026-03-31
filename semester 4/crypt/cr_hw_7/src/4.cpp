#include "../include/continued_fractions.hpp"
#include "../include/rsa/rsa_vunerable_key_generator.hpp"
#include <iostream>
#include <vector>

std::pair<bigint, bigint> perform_winner_attack(bigint n, bigint e)
{
    std::vector<continued_fractions::fraction> convergents = continued_fractions::get_convergents(e, n);

    for (const auto& convergent : convergents)
    {
        bigint k = convergent.numerator;
        bigint d = convergent.denominator;

        if (k == 0)
        {
            continue;
        }

        bigint phi_candidate_numerator = e * d - 1;
        if (phi_candidate_numerator % k != 0)
        {
            continue;
        }

        bigint phi = phi_candidate_numerator / k;
        bigint b = n - phi + 1;
        bigint discriminant = b * b - 4 * n;

        if (discriminant >= 0)
        {
            bigint root_discriminant = boost::multiprecision::sqrt(discriminant);
            if (root_discriminant * root_discriminant == discriminant)
            {
                bigint p = (b + root_discriminant) / 2;
                bigint q = (b - root_discriminant) / 2;

                if (p * q == n && p > 1 && q > 1)
                {
                    return {p, q};
                }
            }
        }
    }
    return {0, 0};
}

int main()
{
    rsa_vulnerable_key_generator service(0.99);
    rsa_vulnerable_key_generator::RSAKeyPair keys = 
        service.generate_vulnerable_key(512, rsa_vulnerable_key_generator::VulnerabilityType::Wiener);

    std::cout << "RSA Modulus (N): " << keys.n << std::endl;
    std::cout << "Public Exponent (e): " << keys.e << std::endl;
    std::cout << "Private Exponent (d): " << keys.d << std::endl;

    auto result = perform_winner_attack(keys.n, keys.e);

    if (result.first != 0)
    {
        std::cout << "Wiener attack success" << std::endl;
        std::cout << "Recovered p: " << result.first << std::endl;
        std::cout << "Recovered q: " << result.second << std::endl;
        std::cout << "Verification (p * q == N): " << (result.first * result.second == keys.n ? "Success" : "Failure") << std::endl;
    }
    else
    {
        std::cout << "Wiener attack failed" << std::endl;
    }

    return 0;
}