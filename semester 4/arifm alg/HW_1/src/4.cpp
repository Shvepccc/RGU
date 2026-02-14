#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>
#include <vector>

enum class type_of_lim_polynom
{
    plus_inf,
    minus_inf,
    point
};

double evaluatePolynom(const std::vector<double>& p, double x)
{
    double res = 0.0;
    for (int i = p.size() - 1; i >= 0; i--)
        res = res * x + p[i];
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

bool is_zero_polynomial(const std::vector<double>& p, double eps = 1e-12)
{
    for (double coeff : p)
    {
        if (std::abs(coeff) > eps)
            return false;
    }
    return true;
}

int zero_order(const std::vector<double>& p, double A, double eps = 1e-12)
{
    if (is_zero_polynomial(p, eps))
        return -1;
        
    std::vector<double> taylor = taylor_at_point(p, A);
    for (int i = 0; i < taylor.size(); i++)
    {
        if (std::abs(taylor[i]) > eps)
        {
            return i;
        }
    }
    return taylor.size();
}

double lim_polynom(std::vector<double>& f, std::vector<double>& g, 
    type_of_lim_polynom type_lim, double point = 0)
{
    const double eps = 1e-12;
    
    if (is_zero_polynomial(f, eps) && is_zero_polynomial(g, eps))
    {
        return std::numeric_limits<double>::quiet_NaN();
    }

    int size_f = f.size();
    int size_g = g.size();

    if (type_lim == type_of_lim_polynom::point) {
        double f_value = evaluatePolynom(f, point);
        double g_value = evaluatePolynom(g, point);

        if (std::abs(g_value) > eps)
        {
            return f_value / g_value;
        }
        else
        {
            if (is_zero_polynomial(f, eps))
                return 0.0;
                
            int order_f = zero_order(f, point, eps);
            int order_g = zero_order(g, point, eps);
            
            if (order_f == -1) return 0.0;
            if (order_g == -1) return std::numeric_limits<double>::quiet_NaN();
            
            if (order_f > order_g) {
                return 0.0;
            }
            else if (order_f == order_g) {
                std::vector<double> f_taylor = taylor_at_point(f, point);
                std::vector<double> g_taylor = taylor_at_point(g, point);
                
                double f_coeff = f_taylor[order_f];
                double g_coeff = g_taylor[order_g];
                
                return f_coeff / g_coeff;
            }
            else { 
                std::vector<double> f_taylor = taylor_at_point(f, point);
                std::vector<double> g_taylor = taylor_at_point(g, point);
                
                double f_coeff = f_taylor[order_f];  
                double g_coeff = g_taylor[order_g];
                
                int d = order_g - order_f; 
                bool positive = (f_coeff * g_coeff > 0);
                
                if (d % 2 == 1)
                {
                    return std::numeric_limits<double>::quiet_NaN();
                }
                
                return positive ? 
                    std::numeric_limits<double>::infinity() : 
                    -std::numeric_limits<double>::infinity();
            }
        }
    }
    else
    {
        if (size_f == size_g)
        {
            return f[size_f-1]/g[size_g-1];
        }
        else if (size_f < size_g)
        {
            return 0;
        }
        else
        {
            double sign = (f[size_f-1] * g[size_g-1] > 0) ? 1.0 : -1.0;
            int d = size_f - size_g;
            
            if (type_lim == type_of_lim_polynom::minus_inf && (d % 2 == 1))
            {
                sign = -sign;
            }

            return (sign > 0) ? 
                std::numeric_limits<double>::infinity() : 
                -std::numeric_limits<double>::infinity();
        }
    }
}

void printResult(const std::string& testName, double result) {
    std::cout << std::left << std::setw(30) << testName << " -> ";
    if (std::isinf(result)) {
        if (result > 0)
            std::cout << "+∞";
        else
            std::cout << "-∞";
    } else if (std::isnan(result)) {
        std::cout << "не существует (NaN)";
    } else {
        std::cout << result;
    }
    std::cout << std::endl;
}

int main() {
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "=== lim_polynom ===\n\n";

    std::cout << "--- Пределы в конечных точках ---\n";
    
    std::vector<double> f1 = {3.0, 2.0};
    std::vector<double> g1 = {-1.0, 1.0};
    printResult("lim x->2 (2x+3)/(x-1)", 
        lim_polynom(f1, g1, type_of_lim_polynom::point, 2.0));
    
    std::vector<double> f2 = {0.0, 1.0};
    std::vector<double> g2 = {-2.0, 1.0};
    printResult("lim x->2 x/(x-2)", 
        lim_polynom(f2, g2, type_of_lim_polynom::point, 2.0));
    
    std::vector<double> f3 = {-4.0, 1.0};
    std::vector<double> g3 = {-8.0, 2.0};
    printResult("lim x->4 (x-4)/(2x-8)", 
        lim_polynom(f3, g3, type_of_lim_polynom::point, 4.0));
    
    std::vector<double> f4 = {16.0, -8.0, 1.0};
    std::vector<double> g4 = {-4.0, 1.0};
    printResult("lim x->4 (x-4)^2/(x-4)", 
        lim_polynom(f4, g4, type_of_lim_polynom::point, 4.0));
    
    std::vector<double> f5 = {-4.0, 1.0};
    std::vector<double> g5 = {16.0, -8.0, 1.0};
    printResult("lim x->4 (x-4)/(x-4)^2", 
        lim_polynom(f5, g5, type_of_lim_polynom::point, 4.0));
    
    std::vector<double> f6 = {-4.0, 1.0};
    std::vector<double> g6 = {-64.0, 48.0, -12.0, 1.0};
    printResult("lim x->4 (x-4)/(x-4)^3", 
        lim_polynom(f6, g6, type_of_lim_polynom::point, 4.0));
    
    std::vector<double> f7 = {4.0, -1.0};
    std::vector<double> g7 = {16.0, -8.0, 1.0};
    printResult("lim x->4 -(x-4)/(x-4)^2", 
        lim_polynom(f7, g7, type_of_lim_polynom::point, 4.0));
    
    std::vector<double> f_zero = {0.0};
    std::vector<double> g_zero_denom = {-2.0, 1.0};
    printResult("lim x->2 0/(x-2)", 
        lim_polynom(f_zero, g_zero_denom, type_of_lim_polynom::point, 2.0));

    std::cout << "\n";

    std::cout << "--- Пределы на бесконечности ---\n";
    
    std::vector<double> f8 = {1.0, 2.0};
    std::vector<double> g8 = {5.0, 2.0, 3.0};
    printResult("lim x->+∞ (2x+1)/(3x^2+2x+5)", 
        lim_polynom(f8, g8, type_of_lim_polynom::plus_inf));
    printResult("lim x->-∞ (2x+1)/(3x^2+2x+5)", 
        lim_polynom(f8, g8, type_of_lim_polynom::minus_inf));
    
    std::vector<double> f9 = {2.0, 3.0, 4.0};
    std::vector<double> g9 = {5.0, -1.0, 2.0};
    printResult("lim x->+∞ (4x^2+3x+2)/(2x^2-x+5)", 
        lim_polynom(f9, g9, type_of_lim_polynom::plus_inf));
    
    std::vector<double> f10 = {1.0, 0.0, 0.0, 3.0};
    std::vector<double> g10 = {5.0, -2.0};
    printResult("lim x->+∞ (3x^3+1)/(-2x+5)", 
        lim_polynom(f10, g10, type_of_lim_polynom::plus_inf));
    printResult("lim x->-∞ (3x^3+1)/(-2x+5)", 
        lim_polynom(f10, g10, type_of_lim_polynom::minus_inf));
    
    std::vector<double> f11 = {0.0, 0.0, 0.0, 2.0};
    std::vector<double> g11 = {0.0, 1.0};
    printResult("lim x->+∞ 2x^3/x", 
        lim_polynom(f11, g11, type_of_lim_polynom::plus_inf));
    printResult("lim x->-∞ 2x^3/x", 
        lim_polynom(f11, g11, type_of_lim_polynom::minus_inf));
    
    std::vector<double> f12 = {0.0, 0.0, 2.0};
    std::vector<double> g12 = {0.0, 1.0};
    printResult("lim x->-∞ 2x^2/x", 
        lim_polynom(f12, g12, type_of_lim_polynom::minus_inf));

    std::cout << "\n";

    std::cout << "--- Граничные случаи ---\n";
    
    std::vector<double> f13 = {5.0};
    std::vector<double> g13 = {2.0};
    printResult("lim x->10 5/2", 
        lim_polynom(f13, g13, type_of_lim_polynom::point, 10.0));
    
    std::vector<double> f14 = {0.0};
    std::vector<double> g14 = {2.0, 1.0};
    printResult("lim x->1 0/(x+2)", 
        lim_polynom(f14, g14, type_of_lim_polynom::point, 1.0));
    
    std::vector<double> f15 = {1.0, 3.0};
    std::vector<double> g15 = {2.0};
    printResult("lim x->∞ (3x+1)/2", 
        lim_polynom(f15, g15, type_of_lim_polynom::plus_inf));
    
    std::vector<double> f_zero_both = {0.0};
    std::vector<double> g_zero_both = {0.0};
    printResult("lim x->0 0/0", 
        lim_polynom(f_zero_both, g_zero_both, type_of_lim_polynom::point, 0.0));

    std::cout << "\n";

    return 0;
}