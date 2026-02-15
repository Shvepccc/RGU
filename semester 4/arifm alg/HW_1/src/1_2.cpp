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

std::vector<double> abracadabra(const std::vector<double>& coeffs, double a)
{
    std::vector<double> b = coeffs;
    int n = (int)b.size() - 1;
    for (int i = n - 1; i >= 0; --i)
    {
        for (int j = i; j < n; ++j)
        {
            b[j] += a * b[j + 1];
        }
    }
    return b;
}

int main()
{
    std::vector<double> coeffs = {-5.0, 4.0, -3.0, 2.0};
    double a = 2.0;
    
    std::vector<double> newCoeff = abracadabra(coeffs, a);
    
    std::cout << "Разложение по степеням (x-" << a << "):\n";
    printPolynomial(newCoeff, a);
    
    return 0;
}