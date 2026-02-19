#include <iostream>
#include <vector>

double compute_optimal(const std::vector<double>& u, double x)
{
    int n = u.size() - 1;
    if (n < 0) return 0.0;
    if (n == 0) return u[0];

    double t = x - (n - 1);
    double result = u[0];

    for (int i = 1; i < n; ++i)
    {
        result = result * t + u[i];
        t = t + 1.0;
    }

    result = result * t + u[n];
    return result;
}

double compute_direct(const std::vector<double>& u, double x)
{
    int n = u.size() - 1;
    if (n < 0) return 0.0;

    double current = 1.0;
    double sum = u[n] * current;

    for (int deg = 1; deg <= n; ++deg)
    {
        current *= (x - deg + 1);
        sum += u[n - deg] * current;
    }

    return sum;
}

int main()
{
    std::vector<double> coeffs1 = {2.0, 3.0, 4.0};
    double x1 = 5.0;

    std::cout << "Test 1: coefficients (highest degree first): [2, 3, 4], x = " << x1 << "\n";
    std::cout << "Optimal method: " << compute_optimal(coeffs1, x1) << "\n";
    std::cout << "Direct method:  " << compute_direct(coeffs1, x1) << "\n";
    std::cout << "Expected (2x^2 + x + 4): " << 2*x1*x1 + x1 + 4 << "\n\n";

    std::vector<double> coeffs2 = {1.0, 0.0, -2.0, 5.0};
    double x2 = 3.0;

    std::cout << "Test 2: coefficients: [1, 0, -2, 5], x = " << x2 << "\n";
    std::cout << "Optimal method: " << compute_optimal(coeffs2, x2) << "\n";
    std::cout << "Direct method:  " << compute_direct(coeffs2, x2) << "\n";
    std::cout << "Expected (x^3 - 3x^2 + 5): " << x2*x2*x2 - 3*x2*x2 + 5 << "\n\n";

    std::vector<double> coeffs3 = {0.0, 1.0, 0.0};
    double x3 = -2.5;

    std::cout << "Test 3: coefficients: [0, 1, 0], x = " << x3 << "\n";
    std::cout << "Optimal method: " << compute_optimal(coeffs3, x3) << "\n";
    std::cout << "Direct method:  " << compute_direct(coeffs3, x3) << "\n";
    std::cout << "Expected (x): " << x3 << "\n\n";

    std::vector<double> coeffs4 = {4.0};
    double x4 = 100.0;

    std::cout << "Test 4: coefficients: [4], x = " << x4 << "\n";
    std::cout << "Optimal method: " << compute_optimal(coeffs4, x4) << "\n";
    std::cout << "Direct method:  " << compute_direct(coeffs4, x4) << "\n";
    std::cout << "Expected (4): 4\n";

    return 0;
}