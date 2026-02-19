#include <iostream>
#include <vector>
#include <stdexcept>

double evaluate_odd_polynomial(const std::vector<double>& coefficients, double x)
{
    if (coefficients.empty())
    {
        throw std::invalid_argument("Coefficient vector cannot be empty");
    }

    double x_squared = x * x;
    double result = coefficients[coefficients.size() - 1];

    for (size_t i = coefficients.size() - 1; i > 0; --i)
    {
        result = result * x_squared + coefficients[i - 1];
    }

    return x * result;
}

int main()
{
    try
    {
        std::vector<double> coeffs = {1.0, 2.0, 3.0, 4.0};

        double x = 2.0;
        double result = evaluate_odd_polynomial(coeffs, x);
        std::cout << "Polynomial: 1*x + 2*x^3 + 3*x^5 + 4*x^7" << std::endl;
        std::cout << "x = " << x << std::endl;
        std::cout << "Result = " << result << std::endl;

        x = -1.5;
        result = evaluate_odd_polynomial(coeffs, x);
        std::cout << "\nx = " << x << std::endl;
        std::cout << "Result = " << result << std::endl;

        x = 0.0;
        result = evaluate_odd_polynomial(coeffs, x);
        std::cout << "\nx = " << x << std::endl;
        std::cout << "Result = " << result << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}