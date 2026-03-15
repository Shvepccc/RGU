#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

#define EPS_N 5

struct SystemInfo
{
    std::vector<double> x;
    int iterations;
    std::vector<std::vector<double>> approximations;
};

double norm_inf(const std::vector<double>& x, const std::vector<double>& y)
{
    double max_diff = 0.0;
    for (size_t i = 0; i < x.size(); ++i)
    {
        double diff = std::fabs(x[i] - y[i]);
        if (diff > max_diff)
        {
            max_diff = diff;
        }
    }
    return max_diff;
}

std::vector<double> operator-(const std::vector<double>& a, const std::vector<double>& b)
{
    std::vector<double> result(a.size());
    for (size_t i = 0; i < a.size(); ++i)
    {
        result[i] = a[i] - b[i];
    }
    return result;
}

std::vector<double> gauss_solve(std::vector<std::vector<double>> A, std::vector<double> b)
{
    int n = A.size();
    
    for (int i = 0; i < n; ++i)
    {
        int max_row = i;
        for (int k = i + 1; k < n; ++k)
        {
            if (std::fabs(A[k][i]) > std::fabs(A[max_row][i]))
            {
                max_row = k;
            }
        }
        
        std::swap(A[i], A[max_row]);
        std::swap(b[i], b[max_row]);
        
        if (std::fabs(A[i][i]) < 1e-15)
        {
            return std::vector<double>();
        }
        
        for (int k = i + 1; k < n; ++k)
        {
            double factor = A[k][i] / A[i][i];
            for (int j = i; j < n; ++j)
            {
                A[k][j] -= factor * A[i][j];
            }
            b[k] -= factor * b[i];
        }
    }
    
    std::vector<double> x(n);
    for (int i = n - 1; i >= 0; --i)
    {
        x[i] = b[i];
        for (int j = i + 1; j < n; ++j)
        {
            x[i] -= A[i][j] * x[j];
        }
        x[i] /= A[i][i];
    }
    
    return x;
}

bool solve_system_newton(
    std::function<void(const std::vector<double>&, std::vector<double>&)> F,
    std::function<void(const std::vector<double>&, std::vector<std::vector<double>>&)> JF,
    std::vector<double>& x,
    int maxIter,
    SystemInfo& info)
{
    info.iterations = 0;
    info.approximations.clear();
    info.approximations.push_back(x);
    
    double eps = std::pow(10.0, -EPS_N);
    
    for (int iter = 0; iter < maxIter; ++iter)
    {
        std::vector<double> x_prev = x;
        
        std::vector<double> f;
        F(x, f);
        
        std::vector<std::vector<double>> J;
        JF(x, J);
        
        std::vector<double> delta = gauss_solve(J, f);
        
        if (delta.empty())
        {
            return false;
        }
        
        for (size_t i = 0; i < x.size(); ++i)
        {
            x[i] = x_prev[i] - delta[i];
        }
        
        info.approximations.push_back(x);
        info.iterations++;
        
        if (norm_inf(x, x_prev) < eps)
        {
            info.x = x;
            return true;
        }
    }
    
    info.x = x;
    return false;
}

void print_solution(const std::string& system_name, 
                    const std::vector<double>& x0,
                    bool success,
                    const SystemInfo& info,
                    const std::vector<double>& final_x)
{
    std::cout << system_name << std::endl;
    std::cout << "Начальное приближение: (";
    for (size_t i = 0; i < x0.size(); ++i)
    {
        std::cout << x0[i];
        if (i < x0.size() - 1) std::cout << ", ";
    }
    std::cout << ")" << std::endl;
    
    if (success)
    {
        std::cout << "Решение найдено за " << info.iterations << " итераций:" << std::endl;
        std::cout << "  x = (";
        for (size_t i = 0; i < info.x.size(); ++i)
        {
            std::cout << std::setprecision(10) << info.x[i];
            if (i < info.x.size() - 1) std::cout << ", ";
        }
        std::cout << ")" << std::endl;
        
        // std::cout << "Первые 5 приближений:" << std::endl;
        // int count = std::min(5, (int)info.approximations.size());
        // for (int j = 0; j < count; ++j)
        // {
        //     std::cout << "  x[" << j << "] = (";
        //     for (size_t k = 0; k < info.approximations[j].size(); ++k)
        //     {
        //         std::cout << info.approximations[j][k];
        //         if (k < info.approximations[j].size() - 1) std::cout << ", ";
        //     }
        //     std::cout << ")" << std::endl;
        // }
    }
    else
    {
        std::cout << "Решение НЕ найдено (последнее приближение за " 
                  << info.iterations << " итераций):" << std::endl;
        std::cout << "  x = (";
        for (size_t i = 0; i < final_x.size(); ++i)
        {
            std::cout << std::setprecision(10) << final_x[i];
            if (i < final_x.size() - 1) std::cout << ", ";
        }
        std::cout << ")" << std::endl;
    }
    
    std::cout << "----------------------------------------" << std::endl;
}

void system_a()
{
    std::cout << "СИСТЕМА A:" << std::endl;
    std::cout << "x1^3 + x1^2*x2 - x1*x3 + 6 = 0" << std::endl;
    std::cout << "e^x1 + e^x2 - x3 = 0" << std::endl;
    std::cout << "x2^2 - 2*x1*x3 = 4" << std::endl;
    std::cout << std::endl;
    
    auto F = [](const std::vector<double>& x, std::vector<double>& f)
    {
        f.resize(3);
        f[0] = x[0]*x[0]*x[0] + x[0]*x[0]*x[1] - x[0]*x[2] + 6.0;
        f[1] = std::exp(x[0]) + std::exp(x[1]) - x[2];
        f[2] = x[1]*x[1] - 2.0*x[0]*x[2] - 4.0;
    };
    
    auto JF = [](const std::vector<double>& x, std::vector<std::vector<double>>& J)
    {
        J.resize(3, std::vector<double>(3));
        J[0][0] = 3.0*x[0]*x[0] + 2.0*x[0]*x[1] - x[2];
        J[0][1] = x[0]*x[0];
        J[0][2] = -x[0];
        
        J[1][0] = std::exp(x[0]);
        J[1][1] = std::exp(x[1]);
        J[1][2] = -1.0;
        
        J[2][0] = -2.0*x[2];
        J[2][1] = 2.0*x[1];
        J[2][2] = -2.0*x[0];
    };
    
    std::vector<std::vector<double>> initial_guesses = 
    {
        {-1.0, -1.0, 1.0},
        {-2.0, -1.0, 2.0},
        {-1.5, -0.5, 1.5},
        {-1.0, -2.0, 1.0}
    };
    
    int maxIter = 100;
    
    for (const auto& x0 : initial_guesses)
    {
        std::vector<double> x = x0;
        SystemInfo info;
        bool success = solve_system_newton(F, JF, x, maxIter, info);
        print_solution("Система A", x0, success, info, x);
    }
    
    std::cout << "========================================" << std::endl;
}

void system_b()
{
    std::cout << "СИСТЕМА B:" << std::endl;
    std::cout << "6x1 - 2cos(x2*x3) = 1" << std::endl;
    std::cout << "9x2 + sqrt(x1^2 + sin(x3)) + 1.06 = -0.9" << std::endl;
    std::cout << "60x3 + 3e^(-x1*x2) + 10π = 3" << std::endl;
    std::cout << std::endl;
    
    auto F = [](const std::vector<double>& x, std::vector<double>& f)
    {
        f.resize(3);
        f[0] = 6.0*x[0] - 2.0*std::cos(x[1]*x[2]) - 1.0;
        f[1] = 9.0*x[1] + std::sqrt(x[0]*x[0] + std::sin(x[2])) + 1.06 + 0.9;
        f[2] = 60.0*x[2] + 3.0*std::exp(-x[0]*x[1]) + 10.0*M_PI - 3.0;
    };
    
    auto JF = [](const std::vector<double>& x, std::vector<std::vector<double>>& J)
    {
        J.resize(3, std::vector<double>(3));
        J[0][0] = 6.0;
        J[0][1] = 2.0*std::sin(x[1]*x[2])*x[2];
        J[0][2] = 2.0*std::sin(x[1]*x[2])*x[1];
        
        double denom = std::sqrt(x[0]*x[0] + std::sin(x[2]));
        J[1][0] = x[0] / denom;
        J[1][1] = 9.0;
        J[1][2] = (0.5*std::cos(x[2])) / denom;
        
        J[2][0] = -3.0*std::exp(-x[0]*x[1])*x[1];
        J[2][1] = -3.0*std::exp(-x[0]*x[1])*x[0];
        J[2][2] = 60.0;
    };
    
    std::vector<std::vector<double>> initial_guesses = 
    {
        {0.5, -0.5, -0.5},
        {0.3, -0.3, -0.3},
        {0.4, -0.4, -0.4},
        {0.6, -0.6, -0.6}
    };
    
    int maxIter = 100;
    
    for (const auto& x0 : initial_guesses)
    {
        std::vector<double> x = x0;
        SystemInfo info;
        bool success = solve_system_newton(F, JF, x, maxIter, info);
        print_solution("Система B", x0, success, info, x);
    }
    
    std::cout << "========================================" << std::endl;
}

void system_c()
{
    std::cout << "СИСТЕМА C:" << std::endl;
    std::cout << "4x1 - x2 + x3 = x1*x4" << std::endl;
    std::cout << "-x1 + 3x2 - 2x3 = x2*x4" << std::endl;
    std::cout << "x1 - 2x2 + 3x3 = x3*x4" << std::endl;
    std::cout << "x1^2 + x2^2 + x3^2 = 1" << std::endl;
    std::cout << std::endl;
    
    auto F = [](const std::vector<double>& x, std::vector<double>& f)
    {
        f.resize(4);
        f[0] = 4.0*x[0] - x[1] + x[2] - x[0]*x[3];
        f[1] = -x[0] + 3.0*x[1] - 2.0*x[2] - x[1]*x[3];
        f[2] = x[0] - 2.0*x[1] + 3.0*x[2] - x[2]*x[3];
        f[3] = x[0]*x[0] + x[1]*x[1] + x[2]*x[2] - 1.0;
    };
    
    auto JF = [](const std::vector<double>& x, std::vector<std::vector<double>>& J)
    {
        J.resize(4, std::vector<double>(4));
        J[0][0] = 4.0 - x[3];
        J[0][1] = -1.0;
        J[0][2] = 1.0;
        J[0][3] = -x[0];
        
        J[1][0] = -1.0;
        J[1][1] = 3.0 - x[3];
        J[1][2] = -2.0;
        J[1][3] = -x[1];
        
        J[2][0] = 1.0;
        J[2][1] = -2.0;
        J[2][2] = 3.0 - x[3];
        J[2][3] = -x[2];
        
        J[3][0] = 2.0*x[0];
        J[3][1] = 2.0*x[1];
        J[3][2] = 2.0*x[2];
        J[3][3] = 0.0;
    };
    
    std::vector<std::vector<double>> initial_guesses = 
    {
        {0.5, 0.5, 0.5, 2.0},
        {0.6, 0.4, 0.3, 3.0},
        {0.4, 0.6, 0.2, 4.0},
        {0.3, 0.5, 0.6, 5.0},
        {0.7, 0.2, 0.3, 6.0}
    };
    
    int maxIter = 100;
    
    for (const auto& x0 : initial_guesses)
    {
        std::vector<double> x = x0;
        SystemInfo info;
        bool success = solve_system_newton(F, JF, x, maxIter, info);
        print_solution("Система C", x0, success, info, x);
    }
    
    std::cout << "========================================" << std::endl;
}

int main()
{
    std::cout << std::fixed << std::setprecision(8);
    std::cout << "МЕТОД НЬЮТОНА ДЛЯ СИСТЕМ НЕЛИНЕЙНЫХ УРАВНЕНИЙ" << std::endl;
    std::cout << "Критерий остановки: ||X^k - X^(k-1)||_inf < 10^(-" << EPS_N << ")" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    system_a();
    std::cout << std::endl;
    
    system_b();
    std::cout << std::endl;
    
    system_c();
    
    return 0;
}