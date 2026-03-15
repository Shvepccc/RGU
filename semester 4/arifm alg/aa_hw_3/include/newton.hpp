#ifndef NEWTON_H_
#define NEWTON_H_

#include <vector>
#include <functional>
#include <cmath>
#include <limits>

struct NewtonRootInfo
{
    double root;
    int iterations;
    std::vector<double> approximations;
};

double newton_method(
    std::function<double(double)> f,
    std::function<double(double)> df,
    double x0,
    double eps,
    int maxIter,
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
        
        double x1 = x - fx / dfx;
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

#endif