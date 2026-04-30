#include "../include/polynomial.hpp"
#include <iostream>
#include <vector>
#include <string>

template<typename T>
void print_polynomial(const polynomial<T>& p, const std::string& name)
{
    std::cout << name << " = " << p << std::endl;
}

template<typename T>
void check_membership(const polynomial<T>& f, const std::vector<polynomial<T>>& basis, const std::string& name)
{
    auto division_result = basis[0].divide(f, basis);
    if (division_result.second.isZero())
    {
        std::cout << name << " принадлежит идеалу" << std::endl;
    }
    else
    {
        std::cout << name << " НЕ принадлежит идеалу" << std::endl;
        std::cout << "Остаток: " << division_result.second << std::endl;
    }
}

void task_1()
{
    std::cout << "========== 2.8.1 ==========" << std::endl;
    std::cout << "I = < -x^3 + y, x^2y - z >" << std::endl;
    std::cout << "f = xy^3 - z^2 + y^5 - z^3" << std::endl;
    std::cout << "Порядок переменных: x > y > z (lex)" << std::endl << std::endl;
    
    std::vector<std::string> vars = {"x", "y", "z"};
    
    polynomial<int> g1(vars, term_order_type::LEX);
    g1.addTerm({3, 0, 0}, -1);  
    g1.addTerm({0, 1, 0}, 1);   
    
    polynomial<int> g2(vars, term_order_type::LEX);
    g2.addTerm({2, 1, 0}, 1);   
    g2.addTerm({0, 0, 1}, -1);  
    
    std::vector<polynomial<int>> basis = {g1, g2};
    std::vector<polynomial<int>> gbasis = g1.buchberger(basis);
    
    std::cout << "Базис Грёбнера (lex, x>y>z):" << std::endl;
    for (size_t i = 0; i < gbasis.size(); ++i)
    {
        std::cout << "g" << i + 1 << " = " << gbasis[i] << std::endl;
    }
    std::cout << std::endl;
    
    polynomial<int> f(vars, term_order_type::LEX);
    f.addTerm({1, 3, 0}, 1); 
    f.addTerm({0, 0, 2}, -1);
    f.addTerm({0, 5, 0}, 1); 
    f.addTerm({0, 0, 3}, -1);
    
    check_membership(f, gbasis, "f");
    std::cout << std::endl;
}

void task_2()
{
    std::cout << "========== 2.8.2 ==========" << std::endl;
    std::cout << "I = < xz - y, xy + 2z^2, y - z >" << std::endl;
    std::cout << "f = x^3z - 2y^2" << std::endl;
    std::cout << "Порядок переменных: x > y > z (lex)" << std::endl << std::endl;
    
    std::vector<std::string> vars = {"x", "y", "z"};
    
    polynomial<double> h1(vars, term_order_type::LEX);
    h1.addTerm({1, 0, 1}, 1); 
    h1.addTerm({0, 1, 0}, -1);
    
    polynomial<double> h2(vars, term_order_type::LEX);
    h2.addTerm({1, 1, 0}, 1);  
    h2.addTerm({0, 0, 2}, 2);  
    
    polynomial<double> h3(vars, term_order_type::LEX);
    h3.addTerm({0, 1, 0}, 1); 
    h3.addTerm({0, 0, 1}, -1);
    
    std::vector<polynomial<double>> basis = {h1, h2, h3};
    std::vector<polynomial<double>> gbasis = h1.buchberger(basis);
    
    std::cout << "Базис Грёбнера (lex, x>y>z):" << std::endl;
    for (size_t i = 0; i < gbasis.size(); ++i)
    {
        std::cout << "g" << i + 1 << " = " << gbasis[i] << std::endl;
    }
    std::cout << std::endl;
    
    polynomial<double> f(vars, term_order_type::LEX);
    f.addTerm({3, 0, 1}, 1);  
    f.addTerm({0, 2, 0}, -2); 
    
    check_membership(f, gbasis, "f");
    std::cout << std::endl;
}

std::vector<double> solve_cubic(double a, double b, double c, double d)
{
    std::vector<double> roots;
    if (std::abs(a) < 1e-12) return roots;

    // Приводим к виду x^3 + px + q = 0
    double p = (3 * a * c - b * b) / (3 * a * a);
    double q = (2 * b * b * b - 9 * a * b * c + 27 * a * a * d) / (27 * a * a * a);

    double discriminant = (q * q) / 4 + (p * p * p) / 27;

    if (std::abs(discriminant) < 1e-10)
    {
        double u = std::cbrt(-q / 2);
        double root1 = u - p / (3 * u);
        roots.push_back(root1 - b / (3 * a));
        if (std::abs(p) > 1e-10)
            roots.push_back(-2 * u - b / (3 * a));
    }
    else if (discriminant > 0)
    {
        double sqrtD = std::sqrt(discriminant);
        double u = std::cbrt(-q / 2 + sqrtD);
        double v = std::cbrt(-q / 2 - sqrtD);
        roots.push_back(u + v - b / (3 * a));
    }
    else
    {
        double r = std::sqrt(-p * p * p / 27);
        double phi = std::acos(-q / (2 * r));
        double t = 2 * std::cbrt(r);
        roots.push_back(t * std::cos(phi / 3) - b / (3 * a));
        roots.push_back(t * std::cos((phi + 2 * M_PI) / 3) - b / (3 * a));
        roots.push_back(t * std::cos((phi + 4 * M_PI) / 3) - b / (3 * a));
    }

    std::sort(roots.begin(), roots.end());
    roots.erase(std::unique(roots.begin(), roots.end(),
        [](double x, double y) { return std::abs(x - y) < 1e-10; }), roots.end());
    return roots;
}

std::vector<double> solve_polynomial(const polynomial<double>& p)
{
    std::vector<double> roots;
    auto terms = p.supp();
    
    int max_deg = 0;
    for (const auto& term : terms)
    {
        int deg = 0;
        for (int e : term.first) deg += e;
        if (deg > max_deg) max_deg = deg;
    }
    
    if (max_deg == 1)
    {
        double a = 0, b = 0;
        for (const auto& term : terms)
        {
            int deg = term.first[0] + term.first[1];
            if (deg == 1) a = term.second;
            else if (deg == 0) b = term.second;
        }
        if (std::abs(a) > 1e-12) roots.push_back(-b / a);
    }
    else if (max_deg == 2)
    {
        double a = 0, b = 0, c = 0;
        for (const auto& term : terms)
        {
            int deg = term.first[0] + term.first[1];
            if (deg == 2) a = term.second;
            else if (deg == 1) b = term.second;
            else if (deg == 0) c = term.second;
        }
        if (std::abs(a) > 1e-12)
        {
            double D = b * b - 4 * a * c;
            if (D >= 0)
            {
                roots.push_back((-b - std::sqrt(D)) / (2 * a));
                roots.push_back((-b + std::sqrt(D)) / (2 * a));
            }
        }
    }
    else if (max_deg == 3)
    {
        double a = 0, b = 0, c = 0, d = 0;
        for (const auto& term : terms)
        {
            int deg = term.first[0] + term.first[1];
            if (deg == 3) a = term.second;
            else if (deg == 2) b = term.second;
            else if (deg == 1) c = term.second;
            else if (deg == 0) d = term.second;
        }
        roots = solve_cubic(a, b, c, d);
    }
    
    return roots;
}

std::vector<double> find_roots_numeric(const polynomial<double>& p, double x_min, double x_max, double step)
{
    std::vector<double> roots;
    auto terms = p.supp();
    
    for (double x = x_min; x <= x_max; x += step)
    {
        double val = 0;
        for (const auto& term : terms)
        {
            double term_val = term.second;
            for (size_t i = 0; i < term.first.size(); ++i)
            {
                term_val *= std::pow(x, term.first[i]);
            }
            val += term_val;
        }
        if (std::abs(val) < 1e-8)
        {
            bool found = false;
            for (double r : roots)
            {
                if (std::abs(r - x) < 1e-6)
                {
                    found = true;
                    break;
                }
            }
            if (!found) roots.push_back(x);
        }
    }
    return roots;
}

void task_3()
{
    std::cout << "========== 2.8.5 ==========" << std::endl;
    std::cout << "f(x,y) = (x^2 + y^2 - 4)(x^2 + y^2 - 1) + (x - 3/2)^2 + (y - 3/2)^2" << std::endl << std::endl;

    std::vector<std::string> vars = {"x", "y"};
    
    polynomial<double> f(vars, term_order_type::LEX);
    f.addTerm({4, 0}, 1.0);
    f.addTerm({2, 2}, 2.0);
    f.addTerm({0, 4}, 1.0);
    f.addTerm({2, 0}, -4.0);
    f.addTerm({0, 2}, -4.0);
    f.addTerm({1, 0}, -3.0);
    f.addTerm({0, 1}, -3.0);
    f.addTerm({0, 0}, 8.5);
    
    print_polynomial(f, "f");
    std::cout << std::endl;
    
    polynomial<double> fx(vars, term_order_type::LEX);
    polynomial<double> fy(vars, term_order_type::LEX);
    
    auto terms_f = f.supp();
    for (const auto& term : terms_f)
    {
        const auto& exps = term.first;
        double coeff = term.second;
        
        if (exps[0] > 0)
        {
            std::vector<int> new_exps = {exps[0] - 1, exps[1]};
            fx.addTerm(new_exps, coeff * exps[0]);
        }
        
        if (exps[1] > 0)
        {
            std::vector<int> new_exps = {exps[0], exps[1] - 1};
            fy.addTerm(new_exps, coeff * exps[1]);
        }
    }
    
    print_polynomial(fx, "fx = df/dx");
    print_polynomial(fy, "fy = df/dy");
    std::cout << std::endl;
    
    std::vector<polynomial<double>> system = {fx, fy};
    std::vector<polynomial<double>> gbasis = fx.buchberger(system);
    
    std::cout << "Базис Грёбнера (lex, x>y):" << std::endl;
    for (size_t i = 0; i < gbasis.size(); ++i)
    {
        std::cout << "g" << i + 1 << " = " << gbasis[i] << std::endl;
    }
    std::cout << std::endl;
    
    polynomial<double> univariate(vars);
    for (const auto& p : gbasis)
    {
        auto terms_p = p.supp();
        bool only_y = true;
        for (const auto& term : terms_p)
        {
            if (term.first[0] > 0)
            {
                only_y = false;
                break;
            }
        }
        if (only_y && !p.isZero())
        {
            univariate = p;
            break;
        }
    }
    
    std::vector<double> y_roots;
    
    if (!univariate.isZero())
    {
        std::cout << "Уравнение для y: " << univariate << " = 0" << std::endl;
        y_roots = solve_polynomial(univariate);
        
        if (y_roots.empty())
        {
            y_roots = find_roots_numeric(univariate, -3.0, 3.0, 0.0001);
        }
        
        std::cout << "Корни по y: ";
        for (double r : y_roots) std::cout << r << " ";
        std::cout << std::endl << std::endl;
    }
    
std::vector<std::pair<double, double>> critical_points;
    
    for (double y0 : y_roots)
    {
        polynomial<double> poly_in_x(vars, term_order_type::LEX);

        auto fx_terms = fx.supp();
        for (const auto& term : fx_terms)
        {
            double coeff_at_y = term.second;
            if (term.first[1] > 0)
            {
                coeff_at_y *= std::pow(y0, term.first[1]);
            }
            std::vector<int> new_exps = {term.first[0], 0};
            poly_in_x.addTerm(new_exps, coeff_at_y);
        }
        
        std::vector<double> x_roots = solve_polynomial(poly_in_x);
        
        if (x_roots.empty())
        {
            x_roots = find_roots_numeric(fx, -3.0, 3.0, 0.0001);
            std::vector<double> filtered;
            for (double x : x_roots)
            {
                if (std::abs(fy.evaluate({x, y0})) < 1e-7)
                {
                    filtered.push_back(x);
                }
            }
            x_roots = filtered;
        }
        else
        {
            std::vector<double> filtered;
            for (double x : x_roots)
            {
                if (std::abs(fy.evaluate({x, y0})) < 1e-7)
                {
                    filtered.push_back(x);
                }
            }
            x_roots = filtered;
        }
        
        for (double x0 : x_roots)
        {
            bool found = false;
            for (const auto& p : critical_points)
            {
                if (std::abs(p.first - x0) < 1e-7 && std::abs(p.second - y0) < 1e-7)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                critical_points.push_back({x0, y0});
            }
        }
    }
    
    std::sort(critical_points.begin(), critical_points.end());
    critical_points.erase(std::unique(critical_points.begin(), critical_points.end(),
        [](const auto& a, const auto& b) {
            return std::abs(a.first - b.first) < 1e-7 && std::abs(a.second - b.second) < 1e-7;
        }), critical_points.end());
    
    std::cout << "=== (a) Особые точки ===" << std::endl;
    if (critical_points.empty())
    {
        std::cout << "Не найдено" << std::endl;
    }
    for (size_t i = 0; i < critical_points.size(); ++i)
    {
        std::cout << i + 1 << ". (" << critical_points[i].first << ", " << critical_points[i].second << ")" << std::endl;
    }
    std::cout << std::endl;
    
    polynomial<double> fxx(vars, term_order_type::LEX);
    polynomial<double> fxy(vars, term_order_type::LEX);
    polynomial<double> fyy(vars, term_order_type::LEX);
    
    auto fx_terms2 = fx.supp();
    for (const auto& term : fx_terms2)
    {
        const auto& exps = term.first;
        double coeff = term.second;
        if (exps[0] > 0)
        {
            fxx.addTerm({exps[0] - 1, exps[1]}, coeff * exps[0]);
        }
        if (exps[1] > 0)
        {
            fxy.addTerm({exps[0], exps[1] - 1}, coeff * exps[1]);
        }
    }
    
    auto fy_terms2 = fy.supp();
    for (const auto& term : fy_terms2)
    {
        const auto& exps = term.first;
        double coeff = term.second;
        if (exps[1] > 0)
        {
            fyy.addTerm({exps[0], exps[1] - 1}, coeff * exps[1]);
        }
    }
    
    std::cout << "=== (b) Классификация ===" << std::endl;
    for (const auto& p : critical_points)
    {
        double x0 = p.first;
        double y0 = p.second;
        
        double A = fxx.evaluate({x0, y0});
        double B = fxy.evaluate({x0, y0});
        double C = fyy.evaluate({x0, y0});
        double D = A * C - B * B;
        double fval = f.evaluate({x0, y0});
        
        std::cout << "Точка (" << x0 << ", " << y0 << "): f = " << fval << ", ";
        std::cout << "D = " << D << ", A = " << A << " -> ";
        
        if (D > 1e-7)
        {
            if (A > 0)
                std::cout << "локальный минимум" << std::endl;
            else if (A < 0)
                std::cout << "локальный максимум" << std::endl;
            else
                std::cout << "требуется дополнительный анализ" << std::endl;
        }
        else if (D < -1e-7)
        {
            std::cout << "седло" << std::endl;
        }
        else
        {
            std::cout << "вырожденный случай (D ≈ 0)" << std::endl;
        }
    }
}

int main()
{
    // task_1();
    // task_2();
    task_3();
    
    return 0;
}