#include <iostream>
#include <functional>
#include <vector>
#include <cmath>
#include <iomanip>
#include <limits>

const double PI = std::acos(-1.0);

double dichotomy(
    std::function<double(double)> f,
    double a,
    double b,
    double eps,
    int& iter)
{
    double fa = f(a);
    double fb = f(b);
    if (fa * fb > 0)
    {
        iter = -1;
        return std::numeric_limits<double>::quiet_NaN();
    }
    iter = 0;
    while ((b - a) > 2.0 * eps)
    {
        double c = (a + b) / 2.0;
        double fc = f(c);
        if (fc == 0.0)
        {
            a = b = c;
            break;
        }
        if (fa * fc < 0)
        {
            b = c;
            fb = fc;
        }
        else
        {
            a = c;
            fa = fc;
        }
        iter++;
    }
    return (a + b) / 2.0;
}

double newton(
    std::function<double(double)> f,
    std::function<double(double)> df,
    double x0,
    double eps,
    int& iter,
    int maxIter = 1000)
{
    double x = x0;
    iter = 0;
    for (int i = 0; i < maxIter; ++i)
    {
        double fx = f(x);
        double dfx = df(x);
        if (std::fabs(dfx) < 1e-12)
        {
            iter = -1;
            return std::numeric_limits<double>::quiet_NaN();
        }
        double x1 = x - fx / dfx;
        ++iter;
        if (std::fabs(x1 - x) < eps)
            return x1;
        x = x1;
    }
    return x;
}

std::vector<double> localize(
    std::function<double(double)> f,
    double left,
    double right,
    double step)
{
    std::vector<double> roots;
    double a = left;
    double fa = f(a);
    for (double b = a + step; b <= right + step/2; b += step)
    {
        double fb = f(b);
        if (fa * fb < 0)
        {
            int iterDummy;
            double root = dichotomy(f, a, b, 1e-6, iterDummy);
            if (!std::isnan(root))
                roots.push_back(root);
        }
        a = b;
        fa = fb;
    }
    return roots;
}

void process_equation(
    const std::string& name,
    std::function<double(double)> f,
    std::function<double(double)> df,
    std::vector<double> roots,
    double eps,
    double delta = 0.1)
{
    std::cout << "\n=== " << name << " ===\n";
    std::cout << "Количество корней: " << roots.size() << "\n";
    
    for (size_t i = 0; i < roots.size() && i < 3; ++i)
    {
        std::cout << "Корень " << i+1 << " ~ " << roots[i] << "\n";
        
        int iterDich = 0, iterNewt = 0;
        
        double left = roots[i] - delta;
        double right = roots[i] + delta;
        
        double rootDich = dichotomy(f, left, right, eps, iterDich);
        std::cout << "  Дихотомия: итераций " << iterDich << ", корень " << rootDich << "\n";
        
        double rootNewt = newton(f, df, roots[i], eps, iterNewt);
        std::cout << "  Ньютон: итераций " << iterNewt << ", корень " << rootNewt << "\n";
    }
}

void process_equation_no_dichotomy(
    const std::string& name,
    std::function<double(double)> f,
    std::function<double(double)> df,
    std::vector<double> roots,
    double eps)
{
    std::cout << "\n=== " << name << " ===\n";
    std::cout << "Первые 3 положительных корня:\n";
    
    for (size_t i = 0; i < roots.size() && i < 3; ++i)
    {
        std::cout << "Корень " << i+1 << " ~ " << roots[i] << "\n";
        
        int iterNewt = 0;
        std::cout << "  Дихотомия не применима (нет смены знака)\n";
        
        double rootNewt = newton(f, df, roots[i], eps, iterNewt);
        std::cout << "  Ньютон: итераций " << iterNewt << ", корень " << rootNewt << "\n";
    }
}

double fa(double x) { return std::sin(x) - 2.0 * x * x + 0.5; }
double dfa(double x) { return std::cos(x) - 4.0 * x; }

int n_b = 3;
double a_b = 8.0;
double fb(double x) { return std::pow(x, n_b) - a_b; }
double dfb(double x) { return n_b * std::pow(x, n_b - 1); }

double fc(double x) { return std::sqrt(1.0 - x * x) - std::exp(x) + 0.1; }
double dfc(double x)
{
    return -x / std::sqrt(1.0 - x * x) - std::exp(x);
}

double fd(double x) { return std::pow(x, 6) - 5.0 * std::pow(x, 3) - 2.0; }
double dfd(double x) { return 6.0 * std::pow(x, 5) - 15.0 * x * x; }

double fe(double x) { return std::log2(x) - 1.0 / (1.0 + x * x); }
double dfe(double x) { return 1.0 / (x * std::log(2.0)) + (2.0 * x) / ((1.0 + x * x) * (1.0 + x * x)); }

double ff(double x) { return std::sin(x * x) - 1.0; }
double dff(double x) { return 2.0 * x * std::cos(x * x); }

double fg(double x) { return std::log(x) - 1.0; }
double dfg(double x) { return 1.0 / x; }

int main()
{
    std::cout << "Введите точность n (10^(-n)): ";
    int n;
    std::cin >> n;
    double eps = std::pow(10.0, -n);
    
    std::cout << std::fixed << std::setprecision(10);
    
    process_equation("Уравнение a: sin x - 2x^2 + 0.5 = 0", 
        fa, dfa, localize(fa, -2.0, 2.0, 0.1), eps);
    
    double left_b = 0.0, right_b = a_b + 1.0;
    if (n_b % 2 == 0) left_b = -a_b - 1.0;
    process_equation("Уравнение b: x^" + std::to_string(n_b) + " = " + std::to_string(a_b), 
        fb, dfb, localize(fb, left_b, right_b, 0.1), eps);
    
    process_equation("Уравнение c: sqrt(1-x^2) - e^x + 0.1 = 0", 
        fc, dfc, localize(fc, -1.0, 1.0, 0.05), eps, 0.05);
    
    process_equation("Уравнение d: x^6 = 5x^3 + 2", 
        fd, dfd, localize(fd, -2.0, 2.0, 0.1), eps);
    
    process_equation("Уравнение e: log2 x = 1/(1+x^2)", 
        fe, dfe, localize(fe, 0.1, 10.0, 0.1), eps);
    
    std::vector<double> f_roots;
    for (int k = 0; k < 3; ++k)
    {
        f_roots.push_back(std::sqrt(PI/2.0 + 2.0 * PI * k));
    }
    process_equation_no_dichotomy("Уравнение f: sin(x^2) = 1", 
        ff, dff, f_roots, eps);
    
    process_equation("Уравнение g: ln x = 1", 
        fg, dfg, localize(fg, 1.0, 3.0, 0.1), eps);
    
    return 0;
}