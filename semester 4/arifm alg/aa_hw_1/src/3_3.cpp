#include <iostream>
#include <vector>

double evaluatePolynomial(double x, double y, const std::vector<std::vector<double>>& coeffs, int n)
{
    std::vector<double> C(n + 1, 0.0);
    
    for (int i = 0; i <= n; ++i)
    {
        double val = 0.0;
        for (int j = n - i; j >= 0; --j)
        {
            val = val * y + coeffs[i][j];
        }
        C[i] = val;
    }
    
    double result = 0.0;
    for (int i = n; i >= 0; --i)
    {
        result = result * x + C[i];
    }
    
    return result;
}

int main()
{
    int n = 2;
    std::vector<std::vector<double>> coeffs(n + 1, std::vector<double>(n + 1, 0.0));
    
    coeffs[0][0] = 1;
    coeffs[0][1] = 2;
    coeffs[0][2] = 3;
    coeffs[1][0] = 4;
    coeffs[1][1] = 5;
    coeffs[2][0] = 6;
    
    double x = 2;
    double y = 3;
    
    double result = evaluatePolynomial(x, y, coeffs, n);
    
    std::cout << "Result: " << result << std::endl;
    
    double check = 1 + 4*x + 6*x*x + 2*y + 5*x*y + 3*y*y;
    std::cout << "Check: " << check << std::endl;
    
    return 0;
}