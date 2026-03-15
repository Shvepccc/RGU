#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

#define ITERATIONS_PRINT_COUNT 0
#define EPS 1e-8
#define MAX_ITERATIONS 1000

struct SystemInfo
{
    double x;
    double y;
    int iterations;
    std::vector<double> x_approximations;
    std::vector<double> y_approximations;
};

double norm2(double fx, double fy)
{
    return std::sqrt(fx * fx + fy * fy);
}

double norm2_diff(double x1, double y1, double x2, double y2)
{
    return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

bool solve_system_newton(
    std::function<void(double, double, double&, double&)> F,
    std::function<void(double, double, double&, double&, double&, double&)> JF,
    double& x,
    double& y,
    double eps,
    int maxIter,
    SystemInfo& info)
{
    info.iterations = 0;
    info.x_approximations.clear();
    info.y_approximations.clear();
    info.x_approximations.push_back(x);
    info.y_approximations.push_back(y);
    
    double x_prev, y_prev;
    
    for (int iter = 0; iter < maxIter; ++iter)
    {
        x_prev = x;
        y_prev = y;
        
        double f, g;
        F(x, y, f, g);
        
        double dfdx, dfdy, dgdx, dgdy;
        JF(x, y, dfdx, dfdy, dgdx, dgdy);
        
        double det = dfdx * dgdy - dfdy * dgdx;
        
        if (std::fabs(det) < 1e-15)
        {
            return false;
        }
        
        double dx = (-f * dgdy + g * dfdy) / det;
        double dy = (-dfdx * g + dgdx * f) / det;
        
        x = x_prev + dx;
        y = y_prev + dy;
        
        info.x_approximations.push_back(x);
        info.y_approximations.push_back(y);
        info.iterations++;
        
        double f_new, g_new;
        F(x, y, f_new, g_new);
        
        if (norm2(f_new, g_new) < eps && norm2_diff(x, y, x_prev, y_prev) < eps)
        {
            info.x = x;
            info.y = y;
            return true;
        }
    }
    
    info.x = x;
    info.y = y;
    return false;
}

void system1(double A, double alpha2, double beta2)
{
    std::cout << "Система 1: tg(xy + " << A << ") = x^2, x^2/" << alpha2 
              << " + y^2/" << beta2 << " = 1" << std::endl;
    std::cout << "Параметры: A = " << A << ", alpha^2 = " << alpha2 
              << ", beta^2 = " << beta2 << std::endl;
    std::cout << std::endl;
    
    auto F = [A, alpha2, beta2](double x, double y, double& f, double& g)
    {
        f = std::tan(x * y + A) - x * x;
        g = (x * x) / alpha2 + (y * y) / beta2 - 1.0;
    };
    
    auto JF = [A, alpha2, beta2](double x, double y, 
                                  double& dfdx, double& dfdy, 
                                  double& dgdx, double& dgdy)
    {
        double sec2 = 1.0 / (std::cos(x * y + A) * std::cos(x * y + A));
        dfdx = sec2 * y - 2.0 * x;
        dfdy = sec2 * x;
        dgdx = 2.0 * x / alpha2;
        dgdy = 2.0 * y / beta2;
    };
    
    std::vector<std::pair<double, double>> initial_guesses = 
    {
        {-1, -0.5},
        {-0.5, 1},
        {1, 0.5},
        {0.5, -1},
    };

    
    for (size_t i = 0; i < initial_guesses.size(); ++i)
    {
        double x0 = initial_guesses[i].first;
        double y0 = initial_guesses[i].second;
        double x = x0;
        double y = y0;
        
        SystemInfo info;
        bool success = solve_system_newton(F, JF, x, y, EPS, MAX_ITERATIONS, info);
        
        double f_val, g_val;
        F(x, y, f_val, g_val);
        
        std::cout << "Начальное приближение (" << x0 << ", " << y0 << "):" << std::endl;
        
        if (success)
        {
            std::cout << "  Решение найдено: x = " << std::setprecision(10) << x 
                      << ", y = " << y << std::endl;
            std::cout << "  Итераций: " << info.iterations << std::endl;
            
            if (info.iterations <= ITERATIONS_PRINT_COUNT)
            {
                std::cout << "  Первые приближения:" << std::endl;
                int count = std::min(5, (int)info.x_approximations.size());
                for (int j = 0; j < count; ++j)
                {
                    std::cout << "    x[" << j << "] = " << info.x_approximations[j]
                              << ", y[" << j << "] = " << info.y_approximations[j] << std::endl;
                }
            }
        }
        else
        {
            std::cout << "  Решение НЕ найдено (сошлось к (" << x << ", " << y << ") за "
                      << info.iterations << " итераций)" << std::endl;
            std::cout << "  Невязка: ||F|| = " << norm2(f_val, g_val) << std::endl;
        }
        
        std::cout << std::endl;
    }
    
    std::cout << "----------------------------------------" << std::endl;
}

void system2()
{
    std::cout << "Система 2: x1^2 + x2^2 - 2 = 0, e^(x1-1) + x2^3 - 2 = 0" << std::endl;
    std::cout << std::endl;
    
    auto F = [](double x1, double x2, double& f, double& g)
    {
        f = x1 * x1 + x2 * x2 - 2.0;
        g = std::exp(x1 - 1.0) + x2 * x2 * x2 - 2.0;
    };
    
    auto JF = [](double x1, double x2, 
                 double& dfdx1, double& dfdx2, 
                 double& dgdx1, double& dgdx2)
    {
        dfdx1 = 2.0 * x1;
        dfdx2 = 2.0 * x2;
        dgdx1 = std::exp(x1 - 1.0);
        dgdx2 = 3.0 * x2 * x2;
    };
    
    std::vector<std::pair<double, double>> initial_guesses = 
    {
        {-0.5, 1.5},
        {1.1, 1.1}
    };
    
    for (size_t i = 0; i < initial_guesses.size(); ++i)
    {
        double x10 = initial_guesses[i].first;
        double x20 = initial_guesses[i].second;
        double x1 = x10;
        double x2 = x20;
        
        SystemInfo info;
        bool success = solve_system_newton(F, JF, x1, x2, EPS, MAX_ITERATIONS, info);
        
        double f_val, g_val;
        F(x1, x2, f_val, g_val);
        
        std::cout << "Начальное приближение (" << x10 << ", " << x20 << "):" << std::endl;
        
        if (success)
        {
            std::cout << "  Решение найдено: x1 = " << std::setprecision(10) << x1 
                      << ", x2 = " << x2 << std::endl;
            std::cout << "  Итераций: " << info.iterations << std::endl;
            
            if (info.iterations <= ITERATIONS_PRINT_COUNT)
            {
                std::cout << "  Первые приближения:" << std::endl;
                int count = std::min(5, (int)info.x_approximations.size());
                for (int j = 0; j < count; ++j)
                {
                    std::cout << "    x1[" << j << "] = " << info.x_approximations[j]
                              << ", x2[" << j << "] = " << info.y_approximations[j] << std::endl;
                }
            }
        }
        else
        {
            std::cout << "  Решение НЕ найдено (сошлось к (" << x1 << ", " << x2 << ") за "
                      << info.iterations << " итераций)" << std::endl;
            std::cout << "  Невязка: ||F|| = " << norm2(f_val, g_val) << std::endl;
        }
        
        std::cout << std::endl;
    }
    
    std::cout << "========================================" << std::endl;
}

int main()
{
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "МЕТОД НЬЮТОНА ДЛЯ СИСТЕМ НЕЛИНЕЙНЫХ УРАВНЕНИЙ" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    std::cout << "ЧАСТЬ A: Система с параметрами" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    std::cout << "СЛУЧАЙ i: A = 0.2, alpha^2 = 1/0.6, beta^2 = 1/2" << std::endl;
    system1(0.2, 1.0/0.6, 1.0/2.0);
    
    std::cout << "СЛУЧАЙ ii: A = 0.4, alpha^2 = 1/0.8, beta^2 = 1/2" << std::endl;
    system1(0.4, 1.0/0.8, 1.0/2.0);
    
    std::cout << "СЛУЧАЙ iii: A = 0.3, alpha^2 = 1/0.2, beta^2 = 1/3" << std::endl;
    system1(0.3, 1.0/0.2, 1.0/3.0);
    
    std::cout << "СЛУЧАЙ iv: A = 0.0, alpha^2 = 1/0.6, beta^2 = 1/2" << std::endl;
    system1(0.0, 1.0/0.6, 1.0/2.0);
    
    std::cout << std::endl;
    std::cout << "ЧАСТЬ B: Система x1^2 + x2^2 = 2, e^(x1-1) + x2^3 = 2" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    system2();
    
    return 0;
}