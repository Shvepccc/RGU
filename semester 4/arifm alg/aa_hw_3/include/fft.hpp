#ifndef FFT_H_
#define FFT_H_

#include <vector>
#include <complex>

void fft(std::vector<std::complex<double>>& a, std::complex<double> root, bool inverse)
{
    int n = a.size();
    if (n <= 1) return;
    
    int power = 1;
    while (power < n) power *= 2;
    
    if (power != n)
    {
        a.resize(power, 0.0);
        n = power;
    }
    
    std::vector<std::complex<double>> a0(n / 2);
    std::vector<std::complex<double>> a1(n / 2);
    
    for (int i = 0; 2 * i < n; i++)
    {
        a0[i] = a[2 * i];
        a1[i] = a[2 * i + 1];
    }
    
    fft(a0, root * root, inverse);
    fft(a1, root * root, inverse);
    
    std::complex<double> w = 1.0;
    for (int i = 0; i < n / 2; i++)
    {
        std::complex<double> t = w * a1[i];
        a[i] = a0[i] + t;
        a[i + n / 2] = a0[i] - t;
        
        if (inverse)
        {
            a[i] /= 2.0;
            a[i + n / 2] /= 2.0;
        }
        
        w *= root;
    }
}

#endif