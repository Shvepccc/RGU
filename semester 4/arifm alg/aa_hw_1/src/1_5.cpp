#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>

#define EPS 1e-12

struct RationalFunction
{
    std::vector<double> f1, s1, f2, s2;
    int k, l;
};

void normalize(std::vector<double>& p)
{
    while (!p.empty() && std::abs(p.back()) < EPS)
        p.pop_back();
    if (p.empty()) p = {0.0};
}

int deg(const std::vector<double>& p)
{
    return p.size() - 1;
}

double eval(const std::vector<double>& p, double x)
{
    double res = 0.0;
    for (int i = p.size() - 1; i >= 0; i--)
        res = res * x + p[i];
    return res;
}

std::vector<double> add(const std::vector<double>& a, const std::vector<double>& b)
{
    std::vector<double> res(std::max(a.size(), b.size()), 0.0);
    for (int i = 0; i < a.size(); i++) res[i] += a[i];
    for (int i = 0; i < b.size(); i++) res[i] += b[i];
    normalize(res);
    return res;
}

std::vector<double> mul(const std::vector<double>& a, const std::vector<double>& b)
{
    if (a.empty() || b.empty()) return {0.0};
    std::vector<double> res(a.size() + b.size() - 1, 0.0);
    for (int i = 0; i < a.size(); i++)
    {
        for (int j = 0; j < b.size(); j++)
        {
            res[i + j] += a[i] * b[j];
        }
    }
    normalize(res);
    return res;
}

std::vector<double> pow_poly(const std::vector<double>& p, int n)
{
    if (n == 0) return {1.0};
    std::vector<double> res = p;
    for (int i = 1; i < n; i++)
    {
        res = mul(res, p);
    }
    return res;
}

std::vector<double> compose(const std::vector<double>& f, const std::vector<double>& g)
{
    std::vector<double> res = {0.0};
    std::vector<double> g_pow = {1.0};
    
    for (int i = 0; i < f.size(); i++)
    {
        if (std::abs(f[i]) > EPS)
        {
            std::vector<double> term = mul(g_pow, {f[i]});
            res = add(res, term);
        }
        if (i < f.size() - 1)
        {
            g_pow = mul(g_pow, g);
        }
    }
    return res;
}

std::vector<double> taylor_at_point(const std::vector<double>& coeffs, double a)
{
    std::vector<double> b = coeffs;
    int n = (int)b.size() - 1;
    for (int i = n - 1; i >= 0; --i)
    {
        for (int j = i; j < n; ++j)
        {
            b[j] += a * b[j + 1];
        }
    }
    return b;
}

int zero_order(const std::vector<double>& p, double A)
{
    std::vector<double> taylor = taylor_at_point(p, A);
    for (int i = 0; i < taylor.size(); i++)
    {
        if (std::abs(taylor[i]) > EPS)
        {
            return i;
        }
    }
    return taylor.size();
}


double limit_at_point(const RationalFunction& func, double A)
{
    std::vector<double> numerator_poly = pow_poly(compose(func.f1, func.s1), func.k);
    std::vector<double> denominator_poly = pow_poly(compose(func.f2, func.s2), func.l);
    
    double num_val = eval(numerator_poly, A);
    double den_val = eval(denominator_poly, A);
    
    if (std::abs(den_val) > EPS)
    {
        return num_val / den_val;
    }
    
    if (std::abs(den_val) < EPS && std::abs(num_val) > EPS)
    {
        double eps = 1e-7;
        double left_val = eval(numerator_poly, A - eps) / eval(denominator_poly, A - eps);
        double right_val = eval(numerator_poly, A + eps) / eval(denominator_poly, A + eps);
        
        if (left_val > 0 && right_val > 0)
        {
            return std::numeric_limits<double>::infinity();
        }
        else if (left_val < 0 && right_val < 0)
        {
            return -std::numeric_limits<double>::infinity();
        }
        else
        {
            return std::numeric_limits<double>::quiet_NaN();
        }
    }
    
    if (std::abs(num_val) < EPS && std::abs(den_val) < EPS)
    {
        int num_order = zero_order(numerator_poly, A);
        int den_order = zero_order(denominator_poly, A);
        
        if (num_order > den_order)
        {
            return 0.0;
        }
        else if (num_order < den_order)
        {
            double eps = 1e-7;
            double left_val = eval(numerator_poly, A - eps) / eval(denominator_poly, A - eps);
            double right_val = eval(numerator_poly, A + eps) / eval(denominator_poly, A + eps);
            
            if (left_val > 0 && right_val > 0)
            {
                return std::numeric_limits<double>::infinity();
            }
            else if (left_val < 0 && right_val < 0)
            {
                return -std::numeric_limits<double>::infinity();
            }
            else
            {
                return std::numeric_limits<double>::quiet_NaN();
            }
        }
        else
        {
            std::vector<double> num_taylor = taylor_at_point(numerator_poly, A);
            std::vector<double> den_taylor = taylor_at_point(denominator_poly, A);
            
            double num_coeff = num_taylor[num_order];
            double den_coeff = den_taylor[den_order];
            
            return num_coeff / den_coeff;
        }
    }
    
    return std::numeric_limits<double>::quiet_NaN();
}

double limit_at_infinity(const RationalFunction& func, bool positive)
{
    std::vector<double> num_poly = pow_poly(compose(func.f1, func.s1), func.k);
    std::vector<double> den_poly = pow_poly(compose(func.f2, func.s2), func.l);
    
    int deg_num = deg(num_poly);
    int deg_den = deg(den_poly);
    
    if (deg_num > deg_den)
    {
        double sign = 1.0;
        
        if ((deg_num - deg_den) % 2 == 1 && !positive)
            sign = -1.0;
        
        double lc_num = num_poly.back();
        double lc_den = den_poly.back();
        if (lc_num < 0) sign = -sign;
        if (lc_den < 0) sign = -sign;
        
        return sign > 0 ? 
            std::numeric_limits<double>::infinity() : 
            -std::numeric_limits<double>::infinity();
    }
    else if (deg_num < deg_den)
    {
        return 0.0;
    }
    else
    {
        double lc_num = num_poly.back();
        double lc_den = den_poly.back();
        return lc_num / lc_den;
    }
}

void print_limit(double result, double point, bool is_inf_point)
{
    if (is_inf_point)
    {
        if (point > 0)
            std::cout << "lim x->+inf T(x) = ";
        else
            std::cout << "lim x->-inf T(x) = ";
    }
    else
    {
        std::cout << "lim x->" << point << " T(x) = ";
    }
    
    if (std::isnan(result))
    {
        std::cout << "undefined (does not exist)" << std::endl;
    }
    else if (std::isinf(result))
    {
        if (result > 0)
            std::cout << "+infinity" << std::endl;
        else
            std::cout << "-infinity" << std::endl;
    }
    else
    {
        std::cout << result << std::endl;
    }
}

// int main()
// {
//     // T(x) = ( (x+1)^2 )^3 / ( (x-1) )^2
//     // f1(t) = t^2, s1(x) = x+1, k=3
//     // f2(t) = t, s2(x) = x-1, l=2
    
//     RationalFunction func;

//     func.f1 = {0, 0, 1};
//     func.s1 = {1, 1};

//     func.f2 = {0, 1};
//     func.s2 = {-1, 1};

//     func.k = 3;
//     func.l = 2;
    
//     print_limit(limit_at_point(func, 3.0), 3.0, false);
//     print_limit(limit_at_point(func, 1.0), 1.0, false);
//     print_limit(limit_at_infinity(func, true), 1.0, true);
//     print_limit(limit_at_infinity(func, false), -1.0, true);
    
//     return 0;
// }

int main()
{
    // Тест 1: T(x) = (x^2) / (x) = x
    // f1(t) = t, s1(x) = x^2, k = 1
    // f2(t) = t, s2(x) = x, l = 1
    RationalFunction func1;
    func1.f1 = {0, 1};        // t
    func1.s1 = {0, 0, 1};     // x^2
    func1.f2 = {0, 1};        // t
    func1.s2 = {0, 1};        // x
    func1.k = 1;
    func1.l = 1;
    
    std::cout << "Тест 1: x^2 / x = x" << std::endl;
    print_limit(limit_at_point(func1, 0.0), 0.0, false);
    print_limit(limit_at_infinity(func1, true), 1.0, true);
    print_limit(limit_at_infinity(func1, false), -1.0, true);
    std::cout << std::endl;
    
    // Тест 2: T(x) = (x-1)^2 / (x-1) = x-1
    // f1(t) = t^2, s1(x) = x-1, k = 1
    // f2(t) = t, s2(x) = x-1, l = 1
    RationalFunction func2;
    func2.f1 = {0, 0, 1};     // t^2
    func2.s1 = {-1, 1};       // x-1
    func2.f2 = {0, 1};        // t
    func2.s2 = {-1, 1};       // x-1
    func2.k = 1;
    func2.l = 1;
    
    std::cout << "Тест 2: (x-1)^2 / (x-1) = x-1" << std::endl;
    print_limit(limit_at_point(func2, 1.0), 1.0, false);
    print_limit(limit_at_point(func2, 2.0), 2.0, false);
    print_limit(limit_at_infinity(func2, true), 1.0, true);
    std::cout << std::endl;
    
    // Тест 3: T(x) = 1 / (x-2)
    // f1(t) = 1, s1(x) = x, k = 1
    // f2(t) = t, s2(x) = x-2, l = 1
    RationalFunction func3;
    func3.f1 = {1};           // 1
    func3.s1 = {0, 1};        // x
    func3.f2 = {0, 1};        // t
    func3.s2 = {-2, 1};       // x-2
    func3.k = 1;
    func3.l = 1;
    
    std::cout << "Тест 3: 1/(x-2)" << std::endl;
    print_limit(limit_at_point(func3, 2.0), 2.0, false);
    print_limit(limit_at_point(func3, 3.0), 3.0, false);
    print_limit(limit_at_infinity(func3, true), 1.0, true);
    std::cout << std::endl;
    
    // Тест 4: T(x) = ((x+1)^2)^3 / (x-1)^2
    // f1(t) = t^2, s1(x) = x+1, k = 3
    // f2(t) = t, s2(x) = x-1, l = 2
    RationalFunction func4;
    func4.f1 = {0, 0, 1};     // t^2
    func4.s1 = {1, 1};        // x+1
    func4.f2 = {0, 1};        // t
    func4.s2 = {-1, 1};       // x-1
    func4.k = 3;
    func4.l = 2;
    
    std::cout << "Тест 4: ((x+1)^2)^3 / (x-1)^2" << std::endl;
    print_limit(limit_at_point(func4, 1.0), 1.0, false);
    print_limit(limit_at_point(func4, -1.0), -1.0, false);
    print_limit(limit_at_infinity(func4, true), 1.0, true);
    print_limit(limit_at_infinity(func4, false), -1.0, true);
    std::cout << std::endl;
    
    // Тест 5: T(x) = (x^2 - 1) / (x - 1) = x + 1
    // f1(t) = t^2 - 1, s1(x) = x, k = 1
    // f2(t) = t - 1, s2(x) = x, l = 1
    RationalFunction func5;
    func5.f1 = {-1, 0, 1};    // t^2 - 1
    func5.s1 = {0, 1};        // x
    func5.f2 = {-1, 1};       // t - 1
    func5.s2 = {0, 1};        // x
    func5.k = 1;
    func5.l = 1;
    
    std::cout << "Тест 5: (x^2 - 1)/(x - 1) = x + 1" << std::endl;
    print_limit(limit_at_point(func5, 1.0), 1.0, false);
    print_limit(limit_at_point(func5, 0.0), 0.0, false);
    print_limit(limit_at_infinity(func5, true), 1.0, true);
    std::cout << std::endl;
    
    // Тест 6: T(x) = (x^3)/(x^2) = x
    // f1(t) = t^3, s1(x) = x, k = 1
    // f2(t) = t^2, s2(x) = x, l = 1
    RationalFunction func6;
    func6.f1 = {0, 0, 0, 1};  // t^3
    func6.s1 = {0, 1};        // x
    func6.f2 = {0, 0, 1};     // t^2
    func6.s2 = {0, 1};        // x
    func6.k = 1;
    func6.l = 1;
    
    std::cout << "Тест 6: x^3/x^2 = x" << std::endl;
    print_limit(limit_at_point(func6, 0.0), 0.0, false);
    print_limit(limit_at_infinity(func6, true), 1.0, true);
    print_limit(limit_at_infinity(func6, false), -1.0, true);
    std::cout << std::endl;
    
    // Тест 7: T(x) = 5
    // f1(t) = 5, s1(x) = x, k = 1
    // f2(t) = 1, s2(x) = x, l = 1
    RationalFunction func7;
    func7.f1 = {5};           // 5
    func7.s1 = {0, 1};        // x
    func7.f2 = {1};           // 1
    func7.s2 = {0, 1};        // x
    func7.k = 1;
    func7.l = 1;
    
    std::cout << "Тест 7: 5" << std::endl;
    print_limit(limit_at_point(func7, 10.0), 10.0, false);
    print_limit(limit_at_infinity(func7, true), 1.0, true);
    print_limit(limit_at_infinity(func7, false), -1.0, true);
    std::cout << std::endl;
    
    return 0;
}