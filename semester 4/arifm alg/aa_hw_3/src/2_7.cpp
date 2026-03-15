#include <iostream>
#include <vector>
#include <functional>
#include <cmath>
#include <algorithm>
#include <iomanip>

#include "../include/newton.hpp"


double f(double x)
{
    return (x - 1.0) * (x - 1.0) * (x - 1.0) * std::sin(M_PI * x) * 
           (std::cos(2.0 * M_PI * x) - 1.0);
}

double df(double x)
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
    std::vector<RootInfo> results;
    double eps = 1e-12;
    int maxIter = 1000;
    
    std::cout << "Поиск первых трех положительных корней уравнения" << std::endl;
    std::cout << "(x-1)^3 * sin(pi*x) * (cos(2pi*x) - 1) = 0" << std::endl;
    std::cout << std::endl;
    std::cout << "Аналитически корни должны быть: x = 1, 2, 3" << std::endl;
    std::cout << std::endl;
    
    for (size_t i = 0; i < initial_guesses.size(); ++i)
    {
        RootInfo info;
        double root = newton_method(
            f, df, initial_guesses[i], eps, maxIter, info);
        
        if (!std::isnan(root))
        {
            results.push_back(info);
        }
    }
    
    std::sort(results.begin(), results.end(), 
        [](const RootInfo& a, const RootInfo& b) { return a.root < b.root; });
    
    for (size_t i = 0; i < results.size() && i < 3; ++i)
    {
        double expected_root = (i == 0) ? 1.0 : (i == 1) ? 2.0 : 3.0;
        
        std::cout << "Корень " << i + 1 << ": x = " << std::setprecision(6) 
                  << results[i].root << std::endl;
        std::cout << "Итераций: " << results[i].iterations << std::endl;
        std::cout << "Значение функции: " << f(results[i].root) << std::endl;
        std::cout << "Начальное приближение: " << results[i].approximations[0] << std::endl;
        
        std::cout << "Первые 5 приближений: ";
        int count = std::min(5, (int)results[i].approximations.size());
        for (int j = 0; j < count; ++j)
        {
            std::cout << results[i].approximations[j];
            if (j < count - 1) std::cout << ", ";
        }
        std::cout << std::endl;
        
        std::cout << "Анализ сходимости (|x_k - " << expected_root << "|):" << std::endl;
        double prev_error = std::fabs(results[i].approximations[0] - expected_root);
        for (size_t j = 1; j < std::min((size_t)8, results[i].approximations.size()); ++j)
        {
            double curr_error = std::fabs(results[i].approximations[j] - expected_root);
            double ratio = (prev_error > 1e-15) ? curr_error / prev_error : 0.0;
            
            std::cout << "  step " << j << ": " 
                      << curr_error << ", ratio = " << std::fixed 
                      << std::setprecision(6) << ratio;
            
            if (i == 0 && j > 1 && ratio > 0.66 && ratio < 0.67)
            {
                std::cout << " (близко к 2/3 ≈ 0.666667)";
            }
            else if (i > 0 && ratio < 0.1)
            {
                std::cout << " (быстрая сходимость для простого корня)";
            }
            
            std::cout << std::endl;
            prev_error = curr_error;
        }
        
        std::cout << std::endl;
    }
    
    return 0;
}