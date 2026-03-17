#include <iostream>
#include <cstdint>
#include <random>

#include "../include/probabilistic_primality_test.hpp"
#include "../include/modeular_arithmetic.hpp"

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

class fermat_primality_test : public probabilistic_primality_test
{
private:
    std::mt19937_64 rng;

    uint64_t get_random_base(uint64_t n)
    {
        std::uniform_int_distribution<uint64_t> dist(2, n - 2);
        return dist(rng);
    }

protected:
    bool single_iteration(uint64_t n) override
    {
        uint64_t a = get_random_base(n);
        int64_t res = mod_pow(a, n - 1, n);
        return res == 1;
    }

public:
    fermat_primality_test() : rng(std::random_device{}()) {}
};

int main()
{
    fermat_primality_test fermat_test;

    std::cout << "=== Fermat Primality Test ===\n";

    uint64_t test_numbers[] = {2, 3, 4, 5, 7, 9, 11, 13, 15, 17, 19,
        21, 23, 29, 341, 561, 1105, 1729,
        static_cast<uint64_t>(UINT64_MAX*0.9)};

    // for (uint64_t n : test_numbers)
    // {
    //     bool is_prime_result = fermat_test.is_prime(n, 0.999);
    //     std::cout << n << " is " << (is_prime_result ? "probably prime" : "composite") << "\n";
    // }

    for (int i = 0; i < 10; i++)
    {
        uint64_t n = generate_random_uint64(static_cast<uint64_t>(UINT64_MAX*0.9), UINT64_MAX);
        bool is_prime_result = fermat_test.is_prime(n, 0.999);
        std::cout << n << " is " << (is_prime_result ? "probably prime" : "composite") << "\n";
    }

    std::cout << "\n=== Testing with different probabilities ===\n";
    uint64_t n = generate_random_uint64(static_cast<uint64_t>(UINT64_MAX*0.9), UINT64_MAX);

    for (double prob : {0.5, 0.75, 0.9, 0.99, 0.999})
    {
        bool result = fermat_test.is_prime(n, prob);
        std::cout << "n=" << n << ", prob=" << prob << " -> "
                  << (result ? "probably prime" : "composite") << "\n";
    }

    return 0;
}