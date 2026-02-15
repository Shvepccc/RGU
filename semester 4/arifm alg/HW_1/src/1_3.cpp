#include <iostream>
#include <vector>

void printPolynomial(const std::vector<double>& coeffs, double a = 0)
{
    int n = coeffs.size();
    if (n == 0)
    {
        std::cout << "0" << std::endl;
        return;
    }
    
    bool first = true;
    
    for (int i = 0; i < n; ++i)
    {
        double coef = coeffs[i];
        if (coef == 0) continue;
        
        if (!first)
        {
            std::cout << (coef > 0 ? " + " : " - ");
        }
        else if (coef < 0)
        {
            std::cout << "-";
        }
        
        double absCoef = fabs(coef);
        
        if (i == 0)
        {
            std::cout << absCoef;
        } 
        else
        {
            if (absCoef != 1) std::cout << absCoef;
            
            if (a != 0)
            {
                std::cout << "(x " << (a >= 0 ? "- " : "+ ") << fabs(a) << ")";
                if (i > 1) std::cout << "^" << i;
            }
            else
            {
                std::cout << "x";
                if (i > 1) std::cout << "^" << i;
            }
        }
        
        first = false;
    }
    
    if (first) std::cout << "0";
    std::cout << std::endl;
}


std::vector<double> rebasePolynomial(const std::vector<double>& coeffs, double a, double b)
{
    int n = coeffs.size() - 1;
    if (n < 0) return {};
    std::vector<double> result = coeffs;
    double shift = b - a;
    for (int i = n; i >= 1; --i)
    {
        for (int j = i; j <= n; ++j)
        {
            result[j - 1] += result[j] * shift;
        }
    }
    return result;
}

int main()
{
    // 1 + 2(x-1) + 3(x-1)^2
    std::vector<double> coeffs = {1, 2, 3};
    double a = 1;
    double b = 3;

    std::vector<double> newCoeffs = rebasePolynomial(coeffs, a, b);

    printPolynomial(newCoeffs, b);

    return 0;
}