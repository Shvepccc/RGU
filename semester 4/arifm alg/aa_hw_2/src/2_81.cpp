#include <iostream>
#include <complex>

std::complex<double> complex_division(std::complex<double> z1, std::complex<double> z2) 
{
    double a0 = std::real(z1);
    double a1 = std::imag(z1);
    double b0 = std::real(z2);
    double b1 = std::imag(z2);
    
    double denom_inv = 1.0 / (b0 * b0 + b1 * b1);
    
    double tmp1 = a0 * b0;
    double tmp2 = a1 * b1;
    double tmp3 = a1 * b0;
    double tmp4 = a0 * b1;
    
    double real_part = (tmp1 + tmp2) * denom_inv;
    double imag_part = (tmp3 - tmp4) * denom_inv;
    
    return std::complex<double>(real_part, imag_part);
}

int main() 
{
    std::complex<double> z1(3.0, 4.0);
    std::complex<double> z2(1.0, 2.0);
    
    std::complex<double> result = complex_division(z1, z2);
    
    std::cout << result << std::endl;
    
    return 0;
}