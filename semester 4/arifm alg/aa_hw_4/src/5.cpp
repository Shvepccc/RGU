#include <iostream>
#include <vector>
#include <string>

#include "../include/polynomial.hpp"
#include "../include/modular.hpp"

template<typename T>
void printDivisionResult(
    const std::string& title,
    const polynomial<T>& f,
    const std::vector<polynomial<T>>& divisors,
    term_order_type order,
    const std::vector<std::string>& vars)
{
    polynomial<T> f_copy(vars, order);
    for (const auto& term : f.supp())
    {
        f_copy.addTerm(term.first, term.second);
    }
    
    std::vector<polynomial<T>> divisors_copy;
    for (const auto& div : divisors)
    {
        polynomial<T> div_copy(vars, order);
        for (const auto& term : div.supp())
        {
            div_copy.addTerm(term.first, term.second);
        }
        divisors_copy.push_back(div_copy);
    }
    
    auto result = f_copy.divide(f_copy, divisors_copy);
    
    std::cout << title << "\n";
    std::cout << "Order: " << (order == term_order_type::LEX ? "LEX" : "GREVLEX") << "\n";
    std::cout << "f = " << f_copy << "\n";
    std::cout << "F = (";
    for (size_t i = 0; i < divisors_copy.size(); ++i)
    {
        std::cout << divisors_copy[i];
        if (i < divisors_copy.size() - 1) std::cout << ", ";
    }
    std::cout << ")\n";
    
    std::cout << "Quotients:\n";
    for (size_t i = 0; i < result.first.size(); ++i)
    {
        std::cout << "  q" << i+1 << " = " << result.first[i] << "\n";
    }
    std::cout << "Remainder: " << result.second << "\n\n";
}

void exercise1()
{
    std::vector<std::string> vars = {"x", "y"};
    
    polynomial<int> f(vars);
    f.addTerm({7, 2}, 1);
    f.addTerm({3, 2}, 1);
    f.addTerm({0, 1}, -1);
    f.addTerm({0, 0}, 1);
    
    polynomial<int> g1(vars);
    g1.addTerm({1, 2}, 1);
    g1.addTerm({1, 0}, -1);
    
    polynomial<int> g2(vars);
    g2.addTerm({1, 0}, 1);
    g2.addTerm({0, 3}, -1);
    
    std::vector<term_order_type> orders = {term_order_type::GREVLEX, term_order_type::LEX};
    
    std::cout << "=== Exercise 1(a) ===\n\n";
    for (const auto& order : orders)
    {
        printDivisionResult("Division with F = (xy^2 - x, x - y^3)", f, {g1, g2}, order, vars);
    }
    
    std::cout << "=== Exercise 1(b) ===\n\n";
    for (const auto& order : orders)
    {
        printDivisionResult("Division with F = (x - y^3, xy^2 - x)", f, {g2, g1}, order, vars);
    }
}

void exercise2()
{
    std::vector<std::string> vars = {"x", "y", "z"};
    
    polynomial<int> f(vars);
    f.addTerm({1, 1, 2}, 1);
    f.addTerm({1, 1, 0}, 1);
    f.addTerm({0, 1, 1}, -1);
    
    polynomial<int> g1(vars);
    g1.addTerm({1, 0, 0}, 1);
    g1.addTerm({0, 2, 0}, -1);
    
    polynomial<int> g2(vars);
    g2.addTerm({0, 1, 0}, 1);
    g2.addTerm({0, 0, 3}, -1);
    
    polynomial<int> g3(vars);
    g3.addTerm({0, 0, 2}, 1);
    g3.addTerm({0, 0, 0}, -1);
    
    std::vector<term_order_type> orders = {term_order_type::GREVLEX, term_order_type::LEX};
    
    std::cout << "\n=== Exercise 2(a) ===\n\n";
    for (const auto& order : orders)
    {
        printDivisionResult("Division with F = (x - y^2, y - z^3, z^2 - 1)", f, {g1, g2, g3}, order, vars);
    }
    
    std::cout << "=== Exercise 2(b) ===\n\n";
    for (const auto& order : orders)
    {
        printDivisionResult("Division with F = (y - z^3, z^2 - 1, x - y^2)", f, {g2, g3, g1}, order, vars);
    }
}

void demonstrateDivision()
{
    using namespace std;
    vector<string> vars = {"x", "y"};

    // ---------- 1. Деление с остатком (не точное) ----------
    polynomial<int> p(vars);
    p.addTerm({4,2}, 6);
    p.addTerm({3,1}, 3);
    p.addTerm({2,0}, 9);

    polynomial<int> q(vars);
    q.addTerm({2,1}, 2);
    q.addTerm({1,0}, 1);

    cout << "p = " << p << "\n";
    cout << "q = " << q << "\n";
    auto [quotients, remainder] = p.divide(p, {q});
    cout << "Division of p by q (not exact):\n";
    cout << "  Quotient: " << quotients[0] << "\n";
    cout << "  Remainder: " << remainder << "\n\n";

    // ---------- 2. Точное деление ----------
    polynomial<int> r(vars);
    r.addTerm({2,2}, 4);
    r.addTerm({1,1}, 2);

    polynomial<int> s(vars);
    s.addTerm({1,1}, 2);

    cout << "r = " << r << "\n";
    cout << "s = " << s << "\n";
    cout << "r / s = " << (r / s) << " (exact division)\n\n";

    // ---------- 3. Деление на скаляр (целый) ----------
    polynomial<int> t(vars);
    t.addTerm({2,0}, 10);
    t.addTerm({1,1}, 6);
    t.addTerm({0,2}, 2);

    cout << "t = " << t << "\n";
    cout << "t / 2 = " << (t / 2) << "\n";
    t /= 2;
    cout << "After t /= 2, t = " << t << "\n\n";

    // ---------- 4. Деление на скаляр, не делящийся нацело (исключение) ----------
    polynomial<int> u(vars);
    u.addTerm({1,0}, 3);
    cout << "u = " << u << "\n";
    try {
        cout << "u / 2 = ";
        auto result = u / 2;
        cout << result << "\n";
    } catch (const exception& e) {
        cout << "Caught: " << e.what() << "\n";
    }
    cout << "\n";

    // ---------- 5. Деление с разными переменными (исключение) ----------
    vector<string> vars2 = {"a", "b"};
    polynomial<int> w(vars2);
    w.addTerm({1,0}, 5);
    try {
        cout << "Attempting division with different variable sets:\n";
        auto res = p / w;
    } catch (const exception& e) {
        cout << "Caught: " << e.what() << "\n\n";
    }

    // ---------- 6. Деление на ноль ----------
    polynomial<int> zero(vars);
    try {
        cout << "Division by zero polynomial:\n";
        auto res = p / zero;
    } catch (const exception& e) {
        cout << "Caught: " << e.what() << "\n";
    }
    try {
        cout << "Division by zero scalar:\n";
        auto res = p / 0;
    } catch (const exception& e) {
        cout << "Caught: " << e.what() << "\n\n";
    }

    // ---------- 7. Пример с modular (полем) ----------
    using mod5 = modular<5>;
    polynomial<mod5> p_mod(vars);
    p_mod.addTerm({2,1}, mod5(3)); // 3
    p_mod.addTerm({1,2}, mod5(2)); // 2

    polynomial<mod5> q_mod(vars);
    q_mod.addTerm({1,0}, mod5(2)); // 2

    cout << "p_mod (mod 5) = " << p_mod << "\n";
    cout << "q_mod = " << q_mod << "\n";
    try {
        auto res_mod = p_mod / q_mod;
        cout << "p_mod / q_mod = " << res_mod << "\n";
    } catch (const exception& e) {
        cout << "Division error: " << e.what() << "\n";
    }

    // Пример с точным делением в поле
    polynomial<mod5> r_mod(vars);
    r_mod.addTerm({2,0}, mod5(4)); // 4
    r_mod.addTerm({0,2}, mod5(1)); // 1

    polynomial<mod5> s_mod(vars);
    s_mod.addTerm({1,0}, mod5(2)); // 2
    s_mod.addTerm({0,1}, mod5(3)); // 3

    cout << "\nr_mod = " << r_mod << "\n";
    cout << "s_mod = " << s_mod << "\n";
    
    // Показываем, что r_mod делится на s_mod
    auto [mod_quot, mod_rem] = r_mod.divide(r_mod, {s_mod});
    cout << "Division of r_mod by s_mod:\n";
    cout << "  Quotient: " << mod_quot[0] << "\n";
    cout << "  Remainder: " << mod_rem << "\n";
    
    if (mod_rem.isZero()) {
        cout << "  This is exact division!\n";
        cout << "  r_mod / s_mod = " << mod_quot[0] << "\n";
    } else {
        cout << "  Not exact division (as expected)\n";
    }
    cout << "\n";

    // ---------- 8. Пример с комплексными числами ----------
    using cd = complex<double>;
    polynomial<cd> p_cplx(vars);
    p_cplx.addTerm({2,0}, cd(1,1));
    p_cplx.addTerm({0,1}, cd(2,-1));

    polynomial<cd> q_cplx(vars);
    q_cplx.addTerm({1,0}, cd(1,0));

    cout << "p_cplx = " << p_cplx << "\n";
    cout << "q_cplx = " << q_cplx << "\n";
    try {
        auto res_cplx = p_cplx / q_cplx;
        cout << "p_cplx / q_cplx = " << res_cplx << "\n";
    } catch (const exception& e) {
        cout << "Division error: " << e.what() << "\n";
    }
    cout << "\n";

    // ---------- 9. Проверка делимости (метод divides) ----------
    polynomial<int> a(vars);
    a.addTerm({2,2}, 4);
    polynomial<int> b(vars);
    b.addTerm({1,1}, 2);
    cout << "a = " << a << ", b = " << b << "\n";
    cout << "Does b divide a? " << (b.divides(b, a) ? "Yes" : "No") << "\n";
    cout << "Does a divide b? " << (a.divides(a, b) ? "Yes" : "No") << "\n";
    
    // ---------- 10. Дополнительный пример: работа с полиномами в поле ----------
    cout << "\n=== Working in GF(7) ===\n";
    using mod7 = modular<7>;
    polynomial<mod7> f_mod(vars);
    f_mod.addTerm({2,1}, mod7(2));
    f_mod.addTerm({1,2}, mod7(3));
    f_mod.addTerm({0,0}, mod7(1));
    
    polynomial<mod7> g_mod(vars);
    g_mod.addTerm({1,0}, mod7(2));
    g_mod.addTerm({0,1}, mod7(1));
    
    cout << "f = " << f_mod << "\n";
    cout << "g = " << g_mod << "\n";
    
    auto [f_quot, f_rem] = f_mod.divide(f_mod, {g_mod});
    cout << "f divided by g:\n";
    cout << "  Quotient: " << f_quot[0] << "\n";
    cout << "  Remainder: " << f_rem << "\n";
}

int main()
{
    try
    {
        demonstrateDivision();
        exercise1();
        exercise2();
        
        std::cout << "All exercises completed successfully.\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}