#include <iostream>
#include <vector>
#include <string>
#include <complex>
#include <iomanip>
#include "../include/modular.hpp"
#include "../include/polynomial.hpp"

struct quadratic_element
{
    double a;
    double b;
    static inline double d_val = -1.0;

    quadratic_element(double _a = 0, double _b = 0) : a(_a), b(_b) {}

    quadratic_element operator+(const quadratic_element& other) const { return {a + other.a, b + other.b}; }
    quadratic_element operator-(const quadratic_element& other) const { return {a - other.a, b - other.b}; }
    quadratic_element operator-() const { return {-a, -b}; }
    quadratic_element operator*(const quadratic_element& other) const 
    { 
        return {a * other.a + b * other.b * d_val, a * other.b + b * other.a}; 
    }
    quadratic_element operator/(const quadratic_element& other) const 
    {
        double norm = other.a * other.a - d_val * other.b * other.b;
        return {(a * other.a - b * other.b * d_val) / norm, (b * other.a - a * other.b) / norm};
    }
    bool operator==(const quadratic_element& other) const { return a == other.a && b == other.b; }
    bool operator!=(const quadratic_element& other) const { return !(*this == other); }
    bool operator<(const quadratic_element& other) const { return a != other.a ? a < other.a : b < other.b; }
};

std::ostream& operator<<(std::ostream& os, const quadratic_element& q)
{
    os << "(" << q.a << (q.b >= 0 ? "+" : "") << q.b << "i)";
    return os;
}

namespace polynomial_utils
{
    template<> bool isZero(const quadratic_element& v) { return v.a == 0 && v.b == 0; }
    template<> bool isOne(const quadratic_element& v) { return v.a == 1 && v.b == 0; }
    template<> bool isNegative(const quadratic_element& v) { return v.a < 0; }
    template<> quadratic_element absValue(const quadratic_element& v) { return v.a < 0 ? -v : v; }
}

template<typename T>
void run_demo(const std::string& ring_name, const std::vector<std::string>& vars, term_order_type order, 
              const std::vector<std::vector<std::pair<std::vector<int>, T>>>& input_polys)
{
    std::cout << ">>> TEST CASE: " << ring_name << " <<<\n";
    std::cout << "Variables: ";
    for (const auto& v : vars) std::cout << v << " ";
    std::cout << "\nInput Basis:\n";

    std::vector<polynomial<T>> basis;
    for (const auto& p_data : input_polys)
    {
        polynomial<T> p(vars, order);
        for (const auto& term : p_data) p.addTerm(term.first, term.second);
        std::cout << "  f_" << basis.size() + 1 << " = " << p << "\n";
        basis.push_back(p);
    }

    auto gb = basis[0].reduced_groebner_basis(basis);

    std::cout << "Reduced Groebner Basis:\n";
    if (gb.empty()) std::cout << "  { 0 }\n";
    else for (size_t i = 0; i < gb.size(); ++i) std::cout << "  g_" << i + 1 << " = " << gb[i] << "\n";
    std::cout << std::string(40, '-') << "\n\n";
}

int main()
{
    std::vector<std::string> vars = {"x", "y"};

    run_demo<double>("Z (Rational via Double)", vars, term_order_type::LEX, {
        {{{2, 0}, 1.0}, {{0, 1}, -1.0}}, // x^2 - y
        {{{1, 1}, 1.0}, {{0, 0}, -1.0}}  // xy - 1
    });

    run_demo<modular<13>>("Z13", vars, term_order_type::LEX, {
        {{{2, 0}, 1}, {{0, 1}, 12}},    // x^2 + 12y
        {{{1, 1}, 1}, {{0, 0}, 12}}     // xy + 12
    });

    quadratic_element::d_val = -1.0;
    run_demo<quadratic_element>("O_Q(D), D=-1", vars, term_order_type::LEX, {
        {{{1, 1}, {1, 0}}, {{0, 0}, {0, 1}}}, // xy + i
        {{{2, 0}, {1, 0}}, {{0, 2}, {1, 0}}}  // x^2 + y^2
    });

    run_demo<double>("R (GrLex Order)", vars, term_order_type::GRLEX, {
        {{{2, 1}, 1.0}, {{0, 2}, -1.0}}, // x^2y - y^2
        {{{1, 2}, 1.0}, {{0, 1}, 1.0}}   // xy^2 + y
    });

    run_demo<std::complex<double>>("C", vars, term_order_type::LEX, {
        {{{1, 0}, {0, 1}}, {{0, 1}, {1, 0}}}, // ix + y
        {{{0, 2}, {1, 0}}, {{0, 0}, {-1, 0}}} // y^2 - 1
    });

    return 0;
}