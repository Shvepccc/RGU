#include <iostream>
#include <vector>

#include "../include/probability_test/probabilistic_primality_tests_realisations.hpp"

uint64_t generate_random_uint64(uint64_t a, uint64_t b)
{
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    
    if (a > b)
    {
        std::swap(a, b);
    }
    
    std::uniform_int_distribution<uint64_t> dist(a, b);
    return dist(gen);
}

int main()
{
    double probability = 0.9;
    
    fermat_test fermat;
    solovay_strassen_test solovay;
    miller_rabin_test miller_rabin;
    
    uint64_t lower_bound = static_cast<uint64_t>(static_cast<double>(UINT64_MAX) * 0.9);
    uint64_t upper_bound = UINT64_MAX;
    
    std::cout << "Testing primality with probability: " << probability << "\n";

    std::cout << "Testing with Karlmikel numbers" << std::endl;
    std::vector<uint64_t> test_numbers = {561, 1105, 1729};

    for (int i = 0; i < test_numbers.size(); ++i)
    {
        uint64_t n = test_numbers[i];
        
        bool is_prime_fermat = fermat.is_prime(n, probability);
        bool is_prime_solovay = solovay.is_prime(n, probability);
        bool is_prime_miller = miller_rabin.is_prime(n, probability);
        
        std::cout << "Test " << i + 1 << ": n = " << n << "\n";
        std::cout << "  Fermat test: " << (is_prime_fermat ? "prime" : "composite") << "\n";
        std::cout << "  Solovay-Strassen test: " << (is_prime_solovay ? "prime" : "composite") << "\n";
        std::cout << "  Miller-Rabin test: " << (is_prime_miller ? "prime" : "composite") << "\n";
        std::cout << "\n";
    }
    
    std::cout << "Testing with random numbers: " << probability << "\n";
    std::cout << "In range: [" << lower_bound << ", " << upper_bound << "]\n\n";
    for (int i = 0; i < 10; ++i)
    {
        uint64_t n = generate_random_uint64(lower_bound, upper_bound);
        
        bool is_prime_fermat = fermat.is_prime(n, probability);
        bool is_prime_solovay = solovay.is_prime(n, probability);
        bool is_prime_miller = miller_rabin.is_prime(n, probability);
        
        std::cout << "Test " << i + 1 << ": n = " << n << "\n";
        std::cout << "  Fermat test: " << (is_prime_fermat ? "prime" : "composite") << "\n";
        std::cout << "  Solovay-Strassen test: " << (is_prime_solovay ? "prime" : "composite") << "\n";
        std::cout << "  Miller-Rabin test: " << (is_prime_miller ? "prime" : "composite") << "\n";
        std::cout << "\n";
    }
    
    return 0;
}