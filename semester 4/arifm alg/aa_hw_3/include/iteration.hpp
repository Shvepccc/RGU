#ifndef ITERATION_H_
#define ITERATION_H_

#include <vector>
#include <cmath>
#include <functional>

double iterate(std::function<double(double)> psi, double x0, double eps)
{
    double x = x0;

    for (int i = 0; i < 10000; i++)
    {
        double xn = psi(x);

        if (std::fabs(xn - x) < eps)
        {
            return xn;
        }

        x = xn;
    }

    return x;
}

bool is_unique(const std::vector<double>& roots, double root, double eps = 1e-3)
{
    for (double r : roots)
    {
        if (std::fabs(r - root) < eps)
        {
            return false;
        }
    }

    return true;
}

std::vector<double> solve(std::function<double(double)> f,
                          std::function<double(double)> df,
                          double L,
                          double R,
                          double step,
                          double eps)
{
    std::vector<double> roots;

    for (double a = L; a < R; a += step)
    {
        double b = a + step;

        if (f(a) * f(b) > 0)
        {
            continue;
        }

        double m = (a + b) / 2.0;

        double d = df(m);

        if (std::fabs(d) < 1e-8)
        {
            continue;
        }

        double lambda = 1.0 / d;

        std::function<double(double)> psi =
        [&](double x)
        {
            return x - lambda * f(x);
        };

        double root = iterate(psi, m, eps);

        if (std::fabs(f(root)) < 1e-5)
        {
            if (is_unique(roots, root))
            {
                roots.push_back(root);
            }
        }
    }

    return roots;
}

#endif