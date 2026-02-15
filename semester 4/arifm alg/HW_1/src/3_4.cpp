#include <iostream>
#include <complex>

std::complex<double> gorner(const std::complex<double> coeff[], int n, const std::complex<double>& z)
{
    std::complex<double> result = coeff[n];
    for (int i = n - 1; i >= 0; --i)
    {
        result = result * z + coeff[i];
    }
    return result;
}

std::complex<double> scheme3(const std::complex<double> coeff[], int n, const std::complex<double>& z)
{
    double x = z.real();
    double y = z.imag();
    
    double r = 2.0 * x;
    double s = x * x + y * y;
    
    std::complex<double> a = coeff[n];
    std::complex<double> b = coeff[n - 1];
    
    for (int j = 2; j <= n; ++j)
    {
        std::complex<double> a_next = b + std::complex<double>(r, 0.0) * a;
        b = coeff[n - j] - std::complex<double>(s, 0.0) * a;
        a = a_next;
    }
    
    return a * z + b;
}

void runTest(const std::complex<double> coeff[], int n, const std::complex<double>& z, int testNum)
{
    std::complex<double> res1 = gorner(coeff, n, z);
    std::complex<double> res2 = scheme3(coeff, n, z);
    
    std::cout << "Test " << testNum << ": z = " << z << "\n";
    std::cout << "Horner: " << res1 << "\n";
    std::cout << "Scheme3: " << res2 << "\n";
    
    double diff = std::abs(res1 - res2);
    if (diff < 1e-10)
    {
        std::cout << "Results match (difference " << diff << ")\n";
    }
    else
    {
        std::cout << "Results differ! (difference " << diff << ")\n";
    }
    std::cout << "------------------------\n";
}

int main()
{
    std::cout << "Example 1: Simple polynomial x^2 + 1\n";
    std::complex<double> coeff1[] = {1.0, 0.0, 1.0};
    runTest(coeff1, 2, std::complex<double>(1.0, 1.0), 1);
    
    std::cout << "Example 2: Polynomial with complex coefficients (1+i)x^2 + (2-2i)x + (3+3i)\n";
    std::complex<double> coeff2[] = {3.0, 3.0, 2.0, -2.0, 1.0, 1.0};
    runTest(coeff2, 2, std::complex<double>(0.0, 1.0), 2);
    
    std::cout << "Example 3: Degree 3 polynomial with real roots\n";
    std::complex<double> coeff3[] = {-6.0, 11.0, -6.0, 1.0};
    runTest(coeff3, 3, std::complex<double>(2.0, 0.0), 3);
    
    std::cout << "Example 4: Zero argument test\n";
    std::complex<double> coeff4[] = {5.0, -4.0, 3.0, -2.0, 1.0};
    runTest(coeff4, 4, std::complex<double>(0.0, 0.0), 4);
    
    std::cout << "Example 5: Purely imaginary argument\n";
    std::complex<double> coeff5[] = {1.0, 1.0, 1.0, 1.0, 1.0};
    runTest(coeff5, 4, std::complex<double>(0.0, 2.0), 5);
    
    return 0;
}