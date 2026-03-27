#ifndef PIMARLITY_TEST_H_
#define PIMARLITY_TEST_H_

#include <iostream>
#include <cstdint>

#include <iostream>
#include <cmath>
#include <cstdint>
#include <random>
#include <boost/multiprecision/cpp_int.hpp>

using bigint = boost::multiprecision::cpp_int;

class i_primality_test
{
public:
    virtual ~i_primality_test() = default;
    virtual bool is_prime(bigint n, double min_probability) = 0;
};

class probabilistic_primality_test : public i_primality_test
{
protected:
    virtual bool single_iteration(bigint n) = 0;

public:
    bool is_prime(bigint n, double min_probability) override
    {
        if (n < 2) return false;
        if (n == 2 || n == 3) return true;
        if (n % 2 == 0) return false;

        if (min_probability < 0.5 || min_probability >= 1.0)
        {
            min_probability = 0.5;
        }

        double error_prob = 1.0 - min_probability;
        int iterations = static_cast<int>(std::ceil(std::log2(1.0 / error_prob)));

        for (int i = 0; i < iterations; ++i)
        {
            if (!single_iteration(n))
            {
                return false;
            }
        }
        return true;
    }
};

#endif