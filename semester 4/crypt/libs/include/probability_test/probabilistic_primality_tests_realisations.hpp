#ifndef PROBABILISTIC_PRIMALITY_TEST_REALISATIONS_H_
#define PROBABILISTIC_PRIMALITY_TEST_REALISATIONS_H_

#include "probabilistic_primality_test.hpp"
#include "../modular_arithmetic.hpp"
#include <random>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp> 

using bigint = boost::multiprecision::cpp_int;
using boost_dist = boost::random::uniform_int_distribution<bigint>;

// Ferma test ----------------------------------------------------
class fermat_test : public probabilistic_primality_test
{
private:
    std::mt19937_64 gen;

public:
    fermat_test() : gen(std::random_device{}()) {}

protected:
    bool single_iteration(bigint n) override
    {
        boost_dist dist(2, n - 2);
        bigint a = dist(gen);
        
        bigint result = mod_pow(a, n - 1, n);
        return result == 1;
    }
};


// Solovay Strassen test ----------------------------------------------------
class solovay_strassen_test : public probabilistic_primality_test
{
private:
    std::mt19937_64 gen;

public:
    solovay_strassen_test() : gen(std::random_device{}()) {}

protected:
    bool single_iteration(bigint n) override
    {
        boost_dist dist(2, n - 2);
        bigint a = dist(gen);
        
        int jacobi = jacobi_symbol(a, n);
        if (jacobi == 0) return false;
        
        bigint pow_result = mod_pow(a, (n - 1) / 2, n);
        
        bigint target = (jacobi < 0) ? (n - 1) : bigint(jacobi);
        return pow_result == target;
    }
};


// Miller Rabin test ----------------------------------------------------
class miller_rabin_test : public probabilistic_primality_test
{
private:
    std::mt19937_64 gen;

public:
    miller_rabin_test() : gen(std::random_device{}()) {}

protected:
    bool single_iteration(bigint n) override
    {
        boost_dist dist(2, n - 2);
        bigint a = dist(gen);
        
        bigint d = n - 1;
        bigint s = 0;
        
        while (d % 2 == 0)
        {
            d /= 2;
            ++s;
        }
        
        bigint x = mod_pow(a, d, n);
        
        if (x == 1 || x == n - 1) return true;
        
        for (bigint r = 1; r < s; ++r)
        {
            x = (x * x) % n; 
            
            if (x == n - 1) return true;
            if (x == 1) return false;
        }
        
        return false;
    }
};

#endif