#include <iostream>
#include <vector>

std::vector<double> inverse_series_coeffs(const std::vector<double>& f, int n)
{
    std::vector<double> g(n);
    double a0 = f[0];
    g[0] = 1.0 / a0;
    
    if (n == 1)
    {
        return g;
    }
    
    for (int i = 1; i < n; ++i)
    {
        double sum = 0.0;
        for (int k = 1; k <= i; ++k)
        {
            if (k < static_cast<int>(f.size()) && i - k < static_cast<int>(g.size()))
            {
                sum += f[k] * g[i - k];
            }
        }
        g[i] = -sum / a0;
    }
    
    return g;
}

int main()
{
    int M = 10;
    std::vector<double> f1(M + 1);
    for (int k = 0; k <= M; ++k)
    {
        double fact = 1.0;
        for (int j = 1; j <= k; ++j)
        {
            fact *= j;
        }
        f1[k] = 1.0 / fact;
    }
    
    int n = 15;
    std::vector<double> inv_f1 = inverse_series_coeffs(f1, n);
    
    std::cout << "First " << n << " coefficients of the inverse series for f1:" << std::endl;
    for (int i = 0; i < n; ++i)
    {
        std::cout << inv_f1[i];
        if (i < n - 1)
        {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
    std::cout << std::endl;
    
    std::vector<double> f2(3);
    f2[0] = -1.0;
    f2[1] = -1.0;
    f2[2] = 1.0;
    
    std::vector<double> inv_f2 = inverse_series_coeffs(f2, n);
    
    std::cout << "First " << n << " coefficients of the inverse series for f2:" << std::endl;
    for (int i = 0; i < n; ++i)
    {
        std::cout << inv_f2[i];
        if (i < n - 1)
        {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
    
    return 0;
}