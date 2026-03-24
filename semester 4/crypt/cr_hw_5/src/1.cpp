#include <iostream>
#include <cstdint>

#include "../include/modeular_arithmetic.hpp"

int main()
{
    std::cout << "=== Legendre symbol ===\n";
    std::cout << "(2/7) = " << legendre_symbol(5, 10) << "\n";
    std::cout << "(3/7) = " << legendre_symbol(3, 7) << "\n";
    std::cout << "(0/5) = " << legendre_symbol(0, 5) << "\n";
    std::cout << "(-1/13) = " << legendre_symbol(-1, 13) << "\n";

    std::cout << "\n=== Jacobi symbol ===\n";
    std::cout << "(2/15) = " << jacobi_symbol(2, 15) << "\n";
    std::cout << "(3/35) = " << jacobi_symbol(3, 35) << "\n";
    std::cout << "(1001/9907) = " << jacobi_symbol(1001, 9907) << "\n";

    std::cout << "\n=== GCD ===\n";
    std::cout << "gcd(48, 18) = " << gcd(48, 18) << "\n";
    std::cout << "gcd(17, 19) = " << gcd(17, 19) << "\n";

    std::cout << "\n=== Extended GCD ===\n";
    Bezout bz = extended_gcd(48, 18);
    std::cout << "gcd(48,18) = " << bz.gcd << ", x = " << bz.x << ", y = " << bz.y << "\n";
    bz = extended_gcd(17, 19);
    std::cout << "gcd(17,19) = " << bz.gcd << ", x = " << bz.x << ", y = " << bz.y << "\n";

    std::cout << "\n=== Modular exponentiation ===\n";
    std::cout << "2^10 mod 1000 = " << mod_pow(2, 10, 1000) << "\n";
    std::cout << "3^5 mod 17 = " << mod_pow(3, 5, 17) << "\n";

    std::cout << "\n=== Modular inverse ===\n";
    std::cout << "3^-1 mod 7 = " << mod_inverse(3, 7) << "\n";
    std::cout << "5^-1 mod 12 = " << mod_inverse(5, 12) << "\n";

    std::cout << "\n=== Euler's totient ===\n";
    int64_t n = 36;
    std::cout << "phi(" << n << ") by definition: " << phi_definition(n) << "\n";
    std::cout << "phi(" << n << ") by factorization: " << phi_factorization(n) << "\n";
    std::cout << "phi(" << n << ") by DFT: " << phi_dft(n) << "\n";

    n = 1;
    std::cout << "\nphi(" << n << ") by definition: " << phi_definition(n) << "\n";
    std::cout << "phi(" << n << ") by factorization: " << phi_factorization(n) << "\n";
    std::cout << "phi(" << n << ") by DFT: " << phi_dft(n) << "\n";

    n = 97;
    std::cout << "\nphi(" << n << ") by definition: " << phi_definition(n) << "\n";
    std::cout << "phi(" << n << ") by factorization: " << phi_factorization(n) << "\n";
    std::cout << "phi(" << n << ") by DFT: " << phi_dft(n) << "\n";

    return 0;
}