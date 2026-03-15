#include <iostream>
#include <vector>
#include <functional>
#include <cmath>
#include <limits>
#include <iomanip>
#include "../include/newton.hpp"

double modified_newton_method(
    std::function<double(double)> f,
    std::function<double(double)> df,
    double x0,
    double eps,
    int maxIter,
    double sigma,
    NewtonRootInfo& info)
{
    double x = x0;
    info.iterations = 0;
    info.approximations.clear();
    info.approximations.push_back(x);
    
    for (int i = 0; i < maxIter; ++i)
    {
        double fx = f(x);
        double dfx = df(x);
        
        if (std::isnan(fx) || std::isinf(fx) || 
            std::isnan(dfx) || std::isinf(dfx))
        {
            info.root = std::numeric_limits<double>::quiet_NaN();
            return info.root;
        }
        
        if (std::fabs(dfx) < 1e-12)
        {
            info.root = std::numeric_limits<double>::quiet_NaN();
            return info.root;
        }
        
        double x1 = x + sigma * fx / dfx;
        info.approximations.push_back(x1);
        ++info.iterations;
        
        if (std::fabs(x1 - x) < eps * std::max(1.0, std::fabs(x)) || 
            std::fabs(fx) < eps)
        {
            info.root = x1;
            return info.root;
        }
        
        x = x1;
    }
    
    info.root = x;
    return info.root;
}

double f(double x)
{
    return (x - 1.0) * (x - 1.0) * (x - 1.0) * std::sin(M_PI * x) * 
           (std::cos(2.0 * M_PI * x) - 1.0);
}

double df_analytical(double x)
{
    double t1 = 3.0 * (x - 1.0) * (x - 1.0) * std::sin(M_PI * x) * 
                (std::cos(2.0 * M_PI * x) - 1.0);
    
    double t2 = (x - 1.0) * (x - 1.0) * (x - 1.0) * M_PI * std::cos(M_PI * x) * 
                (std::cos(2.0 * M_PI * x) - 1.0);
    
    double t3 = (x - 1.0) * (x - 1.0) * (x - 1.0) * std::sin(M_PI * x) * 
                (-2.0 * M_PI * std::sin(2.0 * M_PI * x));
    
    return t1 + t2 + t3;
}

int main()
{
    std::vector<double> initial_guesses = {0.9, 2.1, 2.9};
    std::vector<double> eps_values = {1e-3, 1e-4, 1e-5, 1e-6};
    int maxIter = 1000;
    
    double sigma = -3.0;
    
    for (size_t i = 0; i < initial_guesses.size(); ++i)
    {
        double expected_root = (i == 0) ? 1.0 : (i == 1) ? 2.0 : 3.0;
        
        std::cout << "Корень x = " << expected_root << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        
        for (size_t j = 0; j < eps_values.size(); ++j)
        {
            double eps = eps_values[j];
            
            NewtonRootInfo standard_info;
            double standard_root = newton_method(
                f, df_analytical, initial_guesses[i], eps, maxIter, standard_info);
            
            NewtonRootInfo modified_info;
            double modified_root = modified_newton_method(
                f, df_analytical, initial_guesses[i], eps, maxIter, sigma, modified_info);
            
            if (!std::isnan(standard_root) && !std::isnan(modified_root))
            {
                std::cout << "eps = " << std::scientific << eps 
                          << ": интерации (обычный) = " << std::setw(3) << standard_info.iterations 
                          << " | итерации (модифицированный) = " << std::setw(3) << modified_info.iterations 
                          << " | итерации (отношение) = " << std::fixed << std::setprecision(2)
                          << (double)standard_info.iterations / modified_info.iterations
                          << std::endl;
            }
        }
        
        std::cout << std::endl;
    }
    
    // std::cout << "Детальный анализ для корня x = 1 (кратность 3) с eps = 1e-6:" << std::endl;
    // std::cout << "========================================" << std::endl;
    
    // RootInfo standard_detail;
    // newton_method(f, df_analytical, 0.9, 1e-6, maxIter, standard_detail);
    
    // NewtonRootInfo modified_detail;
    // modified_newton_method(f, df_analytical, 0.9, 1e-6, maxIter, sigma, modified_detail);
    
    // std::cout << "Обычный метод Ньютона (" << standard_detail.iterations << " итераций):" << std::endl;
    // int count1 = std::min(8, (int)standard_detail.approximations.size());
    // for (int k = 0; k < count1; ++k)
    // {
    //     double err = std::fabs(standard_detail.approximations[k] - 1.0);
    //     std::cout << "  x[" << k << "] = " << std::setw(12) << standard_detail.approximations[k]
    //               << ", |x-1| = " << std::scientific << err;
    //     if (k > 0)
    //     {
    //         double prev_err = std::fabs(standard_detail.approximations[k-1] - 1.0);
    //         double ratio = (prev_err > 0) ? err / prev_err : 0;
    //         std::cout << ", ratio = " << std::fixed << std::setprecision(4) << ratio;
    //     }
    //     std::cout << std::endl;
    // }
    
    // std::cout << std::endl;
    // std::cout << "Модифицированный метод Ньютона (" << modified_detail.iterations << " итераций):" << std::endl;
    // int count2 = std::min(8, (int)modified_detail.approximations.size());
    // for (int k = 0; k < count2; ++k)
    // {
    //     double err = std::fabs(modified_detail.approximations[k] - 1.0);
    //     std::cout << "  x[" << k << "] = " << std::setw(12) << modified_detail.approximations[k]
    //               << ", |x-1| = " << std::scientific << err;
    //     if (k > 0)
    //     {
    //         double prev_err = std::fabs(modified_detail.approximations[k-1] - 1.0);
    //         double ratio = (prev_err > 0) ? err / prev_err : 0;
    //         std::cout << ", ratio = " << std::fixed << std::setprecision(4) << ratio;
    //     }
    //     std::cout << std::endl;
    // }
    
    return 0;
}