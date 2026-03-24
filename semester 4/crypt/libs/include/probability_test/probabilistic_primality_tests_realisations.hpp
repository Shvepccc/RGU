#ifndef PROBABILISTIC_PRIMALITY_TEST_REALISATIONS_H_
#define PROBABILISTIC_PRIMALITY_TEST_REALISATIONS_H_

#include "probabilistic_primality_test.hpp"
#include "../modular_arithmetic.hpp"
#include <random>

// Ferma test ----------------------------------------------------
class fermat_test : public probabilistic_primality_test
{
private:
    std::mt19937_64 gen;
    std::uniform_int_distribution<uint64_t> dist;

public:
    fermat_test() : gen(std::random_device{}()), dist(2, 0) {}

protected:
    bool single_iteration(uint64_t n) override
    {
        dist.param(std::uniform_int_distribution<uint64_t>::param_type(2, n - 2));
        uint64_t a = dist(gen);
        
        uint64_t result = mod_pow(a, n - 1, n);
        return result == 1;
    }
};


// Solovay Strassen test ----------------------------------------------------
class solovay_strassen_test : public probabilistic_primality_test
{
private:
    std::mt19937_64 gen;
    std::uniform_int_distribution<uint64_t> dist;

public:
    solovay_strassen_test() : gen(std::random_device{}()), dist(2, 0) {}

protected:
    bool single_iteration(uint64_t n) override
    {
        dist.param(std::uniform_int_distribution<uint64_t>::param_type(2, n - 2));
        uint64_t a = dist(gen);
        
        int jacobi = jacobi_symbol(a, n);
        if (jacobi == 0)
        {
            return false;
        }
        
        uint64_t pow_result = mod_pow(a, (n - 1) / 2, n);
        return pow_result == static_cast<uint64_t>(jacobi);
    }
};


// Miller Rabin test ----------------------------------------------------
class miller_rabin_test : public probabilistic_primality_test
{
private:
    std::mt19937_64 gen;
    std::uniform_int_distribution<uint64_t> dist;

public:
    miller_rabin_test() : gen(std::random_device{}()), dist(2, 0) {}

protected:
    bool single_iteration(uint64_t n) override
    {
        dist.param(std::uniform_int_distribution<uint64_t>::param_type(2, n - 2));
        uint64_t a = dist(gen);
        
        uint64_t d = n - 1;
        uint64_t s = 0;
        
        while (d % 2 == 0)
        {
            d /= 2;
            ++s;
        }
        
        uint64_t x = mod_pow(a, d, n);
        
        if (x == 1 || x == n - 1)
        {
            return true;
        }
        
        for (uint64_t r = 1; r < s; ++r)
        {
            x = (static_cast<__uint128_t>(x) * x) % n;
            if (x == n - 1)
            {
                return true;
            }
            if (x == 1)
            {
                return false;
            }
        }
        
        return false;
    }
};

#endif