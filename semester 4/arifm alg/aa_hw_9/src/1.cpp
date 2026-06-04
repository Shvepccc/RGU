#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/multiprecision/mpfr.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <cstdlib>

namespace mp = boost::multiprecision;

using high_prec = mp::mpfr_float;

std::pair<high_prec, int> compute_e(int precision)
{
    high_prec e = 0;
    high_prec term = 1;
    int n = 0;
    int operations = 0;
    
    high_prec epsilon = high_prec(1) / mp::pow(high_prec(10), precision + 1);
    
    while (term > epsilon)
    {
        e += term;
        operations++;
        n++;
        term /= n;
        operations++;
    }
    
    return {e, operations};
}

std::pair<high_prec, int> compute_pi(int precision)
{
    high_prec pi = 0;
    int operations = 0;
    
    high_prec epsilon = high_prec(1) / mp::pow(high_prec(10), precision + 1);
    
    auto arctan = [&](high_prec x) -> high_prec
    {
        high_prec sum = 0;
        high_prec x_pow = x;
        int n = 0;
        high_prec term;
        
        do
        {
            term = x_pow / (2 * n + 1);
            if (n % 2 == 0)
            {
                sum += term;
            }
            else
            {
                sum -= term;
            }
            operations += 2;
            n++;
            x_pow *= x * x;
            operations++;
        }
        while (term > epsilon);
        
        return sum;
    };
    
    high_prec a1 = arctan(high_prec(1) / 5);
    high_prec a2 = arctan(high_prec(1) / 239);
    
    pi = 4 * (4 * a1 - a2);
    operations += 3;
    
    return {pi, operations};
}

int count_digits(const high_prec& value, int expected_precision)
{
    std::string str = value.str(expected_precision + 2, std::ios::fixed);
    int digits = 0;
    bool dot_found = false;
    
    for (char c : str)
    {
        if (c == '.')
        {
            dot_found = true;
            continue;
        }
        if (dot_found)
        {
            if (c >= '0' && c <= '9')
            {
                digits++;
            }
        }
    }
    
    return digits;
}

int main()
{
    std::vector<double> e_times;
    std::vector<int> e_operations;
    std::vector<int> e_terms;
    std::vector<double> pi_times;
    std::vector<int> pi_operations;
    std::vector<int> pi_terms;
    
    std::ofstream data_file("complexity_data.csv");
    data_file << "precision,e_time_ms,e_operations,e_terms,pi_time_ms,pi_operations,pi_terms\n";
    high_prec::default_precision(4);
    
    for (int p = 1; p < 1000+1; p++)
    {
        std::cout << "Computing for precision: " << p << std::endl;
        
        high_prec::default_precision(p * 2);
        
        auto start_e = std::chrono::high_resolution_clock::now();
        auto [e_val, e_op] = compute_e(p);
        auto end_e = std::chrono::high_resolution_clock::now();
        double e_time = std::chrono::duration<double, std::milli>(end_e - start_e).count();
        
        auto start_pi = std::chrono::high_resolution_clock::now();
        auto [pi_val, pi_op] = compute_pi(p);
        auto end_pi = std::chrono::high_resolution_clock::now();
        double pi_time = std::chrono::duration<double, std::milli>(end_pi - start_pi).count();
        
        int e_terms_count = (e_op + 1) / 2;
        
        int pi_terms_count = pi_op / 2;
        
        e_times.push_back(e_time);
        e_operations.push_back(e_op);
        e_terms.push_back(e_terms_count);
        pi_times.push_back(pi_time);
        pi_operations.push_back(pi_op);
        pi_terms.push_back(pi_terms_count);
        
        data_file << p << ","
                  << e_time << ","
                  << e_op << ","
                  << e_terms_count << ","
                  << pi_time << ","
                  << pi_op << ","
                  << pi_terms_count << "\n";
        
        {
            std::cout << "  e: " << e_time << " ms, ops=" << e_op << ", terms=" << e_terms_count << std::endl;
            std::cout << "  pi: " << pi_time << " ms, ops=" << pi_op << ", terms=" << pi_terms_count << std::endl;
        }
    }
    
    data_file.close();
    
    std::cout << "\nData saved to complexity_data.csv" << std::endl;
    std::cout << "Launching Python visualization..." << std::endl;
    
    std::system("./src/plot.py");
    
    return 0;
}