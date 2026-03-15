#include <iostream>
#include <cmath>
#include <functional>

struct Result
{
    double root;
    int iterations;
    bool converged;
};

Result iterate(std::function<double(double)> psi, double x0, double eps, int maxIter = 10000)
{
    double x = x0;
    for (int i = 0; i < maxIter; i++)
    {
        double xn = psi(x);
        if (std::fabs(xn - x) < eps)
            return {xn, i+1, true};
        x = xn;
    }
    return {x, maxIter, false};
}

void test_iteration(const std::string& label, std::function<double(double)> psi, double start, double end, double step, double eps)
{
    std::cout << label << std::endl;
    for (double x0 = start; x0 <= end; x0 += step)
    {
        Result res = iterate(psi, x0, eps);
        std::cout << "x0 = " << x0 << " -> ";
        if (res.converged)
            std::cout << "root ≈ " << res.root << " in " << res.iterations << " iter." << std::endl;
        else
            std::cout << "did not converge in " << res.iterations << " iter." << std::endl;
    }
    std::cout << "-------------------------" << std::endl;
}

int main()
{
    double eps = 1e-6;

    auto psi_a = [](double x){ return 2 - 1.0/x; };
    test_iteration("a) x_{n+1} = 2 - 1/x_n", psi_a, -3, 3.0, 0.2, eps);

    auto psi_b = [](double x){ return std::exp(2*x - 1); };
    test_iteration("b) x_{n+1} = exp(2*x_n - 1)", psi_b, -0.5, 1.0, 0.1, eps);

    double A = 2.0;
    auto psi_c = [A](double x){ return A - std::log(x); };
    test_iteration("c) x_{n+1} = A - ln(x_n), A = 2", psi_c, 0.1, 3.0, 0.2, eps);

    double alpha = 1.0, beta = 0.5;
    auto psi_d = [alpha, beta](double x){ return alpha * std::exp(-x) + beta * x; };
    test_iteration("d) x_{n+1} = alpha*exp(-x_n) + beta*x_n, alpha=1, beta=0.5", psi_d, -1.0, 3.0, 0.2, eps);

    return 0;
}