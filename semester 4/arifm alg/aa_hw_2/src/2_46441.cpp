#include <iostream>
#include <complex>

std::complex<double> multiply_complex_numbers
(
    std::complex<double> first_number, 
    std::complex<double> second_number
) 
{
    double real_first = first_number.real();
    double imag_first = first_number.imag();
    double real_second = second_number.real();
    double imag_second = second_number.imag();
    
    double first_product = real_second * (real_first + imag_first);
    double second_product = real_first * (imag_second - real_second);
    double third_product = imag_first * (real_second + imag_second);
    
    double result_real = first_product - third_product;
    double result_imag = first_product + second_product;
    
    return std::complex<double>(result_real, result_imag);
}

int main() 
{
    std::complex<double> first_complex(3.0, 2.0);
    std::complex<double> second_complex(1.0, 4.0);
    
    std::complex<double> result = multiply_complex_numbers(first_complex, second_complex);
    
    std::cout << "(" << first_complex.real() << " + " << first_complex.imag() << "i) * ";
    std::cout << "(" << second_complex.real() << " + " << second_complex.imag() << "i) = ";
    std::cout << result.real() << " + " << result.imag() << "i" << std::endl;
    
    return 0;
}