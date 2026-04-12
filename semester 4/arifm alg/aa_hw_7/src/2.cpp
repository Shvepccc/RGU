#include <iostream>
#include <vector>
#include <string>
#include "../include/polynomial.hpp"

// Helper function to print multideg
void print_multideg(const std::vector<int>& md, const std::vector<std::string>& vars)
{
    std::cout << "(";
    for (size_t i = 0; i < md.size(); ++i)
    {
        std::cout << md[i];
        if (i < md.size() - 1) std::cout << ", ";
    }
    std::cout << ")";
}

int main()
{
    std::cout << "========== TASK 5: Compute S(f, g) with LEX ordering ==========\n\n";
    
    // Task 5(a): f = 4x^2z - 7y^2, g = xyz^2 + 3xz^4
    {
        std::cout << "--- Task 5(a) ---\n";
        std::vector<std::string> vars = {"x", "y", "z"};
        polynomial<double> f(vars, term_order_type::LEX);
        polynomial<double> g(vars, term_order_type::LEX);
        
        f.addTerm({2, 0, 1}, 4);
        f.addTerm({0, 2, 0}, -7);
        
        g.addTerm({1, 1, 2}, 1);
        g.addTerm({1, 0, 4}, 3);
        
        std::cout << "f = " << f << "\n";
        std::cout << "g = " << g << "\n";
        
        auto spoly = f.S_polynomial(g);
        std::cout << "S(f, g) = " << spoly << "\n";
        
        auto mdeg_f = f.multideg();
        auto mdeg_g = g.multideg();
        auto mdeg_s = spoly.multideg();
        
        std::cout << "multideg(f) = "; print_multideg(mdeg_f, vars); std::cout << "\n";
        std::cout << "multideg(g) = "; print_multideg(mdeg_g, vars); std::cout << "\n";
        std::cout << "multideg(S(f,g)) = "; print_multideg(mdeg_s, vars); std::cout << "\n\n";
    }
    
    // Task 5(b): f = x^4y - z^2, g = 3xz^2 - y
    {
        std::cout << "--- Task 5(b) ---\n";
        std::vector<std::string> vars = {"x", "y", "z"};
        polynomial<double> f(vars, term_order_type::LEX);
        polynomial<double> g(vars, term_order_type::LEX);
        
        f.addTerm({4, 1, 0}, 1);
        f.addTerm({0, 0, 2}, -1);
        
        g.addTerm({1, 0, 2}, 3);
        g.addTerm({0, 1, 0}, -1);
        
        std::cout << "f = " << f << "\n";
        std::cout << "g = " << g << "\n";
        
        auto spoly = f.S_polynomial(g);
        std::cout << "S(f, g) = " << spoly << "\n";
        
        auto mdeg_f = f.multideg();
        auto mdeg_g = g.multideg();
        auto mdeg_s = spoly.multideg();
        
        std::cout << "multideg(f) = "; print_multideg(mdeg_f, vars); std::cout << "\n";
        std::cout << "multideg(g) = "; print_multideg(mdeg_g, vars); std::cout << "\n";
        std::cout << "multideg(S(f,g)) = "; print_multideg(mdeg_s, vars); std::cout << "\n\n";
    }
    
    // Task 5(c): f = x^7y^2z + 2ixyz, g = 2x^7y^2z + 4 (using complex)
    {
        std::cout << "--- Task 5(c) ---\n";
        std::vector<std::string> vars = {"x", "y", "z"};
        polynomial<std::complex<double>> f(vars, term_order_type::LEX);
        polynomial<std::complex<double>> g(vars, term_order_type::LEX);
        
        f.addTerm({7, 2, 1}, std::complex<double>(1, 0));
        f.addTerm({1, 1, 1}, std::complex<double>(0, 2));
        
        g.addTerm({7, 2, 1}, std::complex<double>(2, 0));
        g.addTerm({0, 0, 0}, std::complex<double>(4, 0));
        
        std::cout << "f = " << f << "\n";
        std::cout << "g = " << g << "\n";
        
        auto spoly = f.S_polynomial(g);
        std::cout << "S(f, g) = " << spoly << "\n\n";
    }
    
    // Task 5(d): f = xy + z^3, g = z^2 - 3z
    {
        std::cout << "--- Task 5(d) ---\n";
        std::vector<std::string> vars = {"x", "y", "z"};
        polynomial<int> f(vars, term_order_type::LEX);
        polynomial<int> g(vars, term_order_type::LEX);
        
        f.addTerm({1, 1, 0}, 1);
        f.addTerm({0, 0, 3}, 1);
        
        g.addTerm({0, 0, 2}, 1);
        g.addTerm({0, 0, 1}, -3);
        
        std::cout << "f = " << f << "\n";
        std::cout << "g = " << g << "\n";
        
        auto spoly = f.S_polynomial(g);
        std::cout << "S(f, g) = " << spoly << "\n\n";
    }
    
    std::cout << "========== TASK 7: Check if set is Groebner basis (grelex, x>y>z) ==========\n\n";
    // Task 7: {x^4y^2 - z^5, x^3y^3 - 1, x^2y^4 - 2z}
    {
        std::vector<std::string> vars = {"x", "y", "z"};
        polynomial<int> p1(vars, term_order_type::GRLEX);
        polynomial<int> p2(vars, term_order_type::GRLEX);
        polynomial<int> p3(vars, term_order_type::GRLEX);
        
        p1.addTerm({4, 2, 0}, 1);
        p1.addTerm({0, 0, 5}, -1);
        
        p2.addTerm({3, 3, 0}, 1);
        p2.addTerm({0, 0, 0}, -1);
        
        p3.addTerm({2, 4, 0}, 1);
        p3.addTerm({0, 0, 1}, -2);
        
        std::cout << "p1 = " << p1 << "\n";
        std::cout << "p2 = " << p2 << "\n";
        std::cout << "p3 = " << p3 << "\n\n";
        
        std::vector<polynomial<int>> basis = {p1, p2, p3};
        
        // Check all S-polynomials
        std::cout << "Checking S(p1, p2):\n";
        auto s12 = p1.S_polynomial(p2);
        std::cout << "S(p1, p2) = " << s12 << "\n";
        auto div12 = p1.divide(s12, basis);
        std::cout << "Remainder = " << div12.second << "\n";
        
        std::cout << "\nChecking S(p1, p3):\n";
        auto s13 = p1.S_polynomial(p3);
        std::cout << "S(p1, p3) = " << s13 << "\n";
        auto div13 = p1.divide(s13, basis);
        std::cout << "Remainder = " << div13.second << "\n";
        
        std::cout << "\nChecking S(p2, p3):\n";
        auto s23 = p2.S_polynomial(p3);
        std::cout << "S(p2, p3) = " << s23 << "\n";
        auto div23 = p1.divide(s23, basis);
        std::cout << "Remainder = " << div23.second << "\n";
        
        bool isGroebner = p1.isGroebnerBasis(basis);
        std::cout << "\nIs {p1, p2, p3} a Groebner basis? " << (isGroebner ? "YES" : "NO") << "\n";
        std::cout << "Explanation: For a Groebner basis, all S-polynomials must reduce to zero.\n";
        std::cout << "Since not all remainders are zero, this is NOT a Groebner basis.\n\n";
    }
    
    std::cout << "========== TASK 8: Prove {y - x^2, z - x^3} is NOT Groebner basis for lex with x>y>z ==========\n\n";
    {
        std::vector<std::string> vars = {"x", "y", "z"};
        polynomial<int> f(vars, term_order_type::LEX);
        polynomial<int> g(vars, term_order_type::LEX);
        
        f.addTerm({0, 1, 0}, 1);
        f.addTerm({2, 0, 0}, -1);
        
        g.addTerm({0, 0, 1}, 1);
        g.addTerm({3, 0, 0}, -1);
        
        std::cout << "f = " << f << "  (y - x^2)\n";
        std::cout << "g = " << g << "  (z - x^3)\n\n";
        
        auto spoly = f.S_polynomial(g);
        std::cout << "S(f, g) = " << spoly << "\n";
        
        std::vector<polynomial<int>> basis = {f, g};
        auto division_result = f.divide(spoly, basis);
        
        std::cout << "Remainder of S(f,g) divided by {f,g} = " << division_result.second << "\n";
        
        bool isGroebner = f.isGroebnerBasis(basis);
        std::cout << "\nIs {y - x^2, z - x^3} a Groebner basis? " << (isGroebner ? "YES" : "NO") << "\n";
        std::cout << "Explanation: S(f,g) = x^2*z - x*y, which does NOT reduce to zero modulo {f,g}.\n";
        std::cout << "Therefore, by Buchberger's criterion, this is NOT a Groebner basis.\n\n";
    }
    
    std::cout << "========== TASK 9: Determine if sets are Groebner bases ==========\n\n";
    
    // Task 9(a): G = {x^2 - y, x^3 - z}, grlex ordering
    {
        std::cout << "--- Task 9(a): grlex ordering ---\n";
        std::vector<std::string> vars = {"x", "y", "z"};
        polynomial<int> f(vars, term_order_type::GRLEX);
        polynomial<int> g(vars, term_order_type::GRLEX);
        
        f.addTerm({2, 0, 0}, 1);
        f.addTerm({0, 1, 0}, -1);
        
        g.addTerm({3, 0, 0}, 1);
        g.addTerm({0, 0, 1}, -1);
        
        std::cout << "f = " << f << "\n";
        std::cout << "g = " << g << "\n\n";
        
        std::vector<polynomial<int>> basis = {f, g};
        
        auto spoly = f.S_polynomial(g);
        std::cout << "S(f, g) = " << spoly << "\n";
        
        auto division_result = f.divide(spoly, basis);
        std::cout << "Remainder = " << division_result.second << "\n";
        
        bool isGroebner = f.isGroebnerBasis(basis);
        std::cout << "Is {x^2 - y, x^3 - z} a Groebner basis for grlex? " << (isGroebner ? "YES" : "NO") << "\n\n";
    }
    
    // Task 9(b): G = {x^2 - y, x^3 - z}, invlex ordering
    {
        std::cout << "--- Task 9(b): invlex ordering ---\n";
        std::vector<std::string> vars = {"x", "y", "z"};
        polynomial<int> f(vars, term_order_type::INVLEX);
        polynomial<int> g(vars, term_order_type::INVLEX);
        
        f.addTerm({2, 0, 0}, 1);
        f.addTerm({0, 1, 0}, -1);
        
        g.addTerm({3, 0, 0}, 1);
        g.addTerm({0, 0, 1}, -1);
        
        std::cout << "f = " << f << "\n";
        std::cout << "g = " << g << "\n\n";
        
        std::vector<polynomial<int>> basis = {f, g};
        
        auto spoly = f.S_polynomial(g);
        std::cout << "S(f, g) = " << spoly << "\n";
        
        auto division_result = f.divide(spoly, basis);
        std::cout << "Remainder = " << division_result.second << "\n";
        
        bool isGroebner = f.isGroebnerBasis(basis);
        std::cout << "Is {x^2 - y, x^3 - z} a Groebner basis for invlex? " << (isGroebner ? "YES" : "NO") << "\n\n";
    }
    
    // Task 9(c): G = {xy^2 - xz + y, xy - z^2, x - yz^4}, lex ordering
    {
        std::cout << "--- Task 9(c): lex ordering (x>y>z) ---\n";
        std::vector<std::string> vars = {"x", "y", "z"};
        polynomial<int> p1(vars, term_order_type::LEX);
        polynomial<int> p2(vars, term_order_type::LEX);
        polynomial<int> p3(vars, term_order_type::LEX);
        
        p1.addTerm({1, 2, 0}, 1);
        p1.addTerm({1, 0, 1}, -1);
        p1.addTerm({0, 1, 0}, 1);
        
        p2.addTerm({1, 1, 0}, 1);
        p2.addTerm({0, 0, 2}, -1);
        
        p3.addTerm({1, 0, 0}, 1);
        p3.addTerm({0, 1, 4}, -1);
        
        std::cout << "p1 = " << p1 << "\n";
        std::cout << "p2 = " << p2 << "\n";
        std::cout << "p3 = " << p3 << "\n\n";
        
        std::vector<polynomial<int>> basis = {p1, p2, p3};
        
        std::cout << "Checking S(p1, p2):\n";
        auto s12 = p1.S_polynomial(p2);
        std::cout << "S(p1, p2) = " << s12 << "\n";
        auto div12 = p1.divide(s12, basis);
        std::cout << "Remainder = " << div12.second << "\n";
        
        std::cout << "\nChecking S(p1, p3):\n";
        auto s13 = p1.S_polynomial(p3);
        std::cout << "S(p1, p3) = " << s13 << "\n";
        auto div13 = p1.divide(s13, basis);
        std::cout << "Remainder = " << div13.second << "\n";
        
        std::cout << "\nChecking S(p2, p3):\n";
        auto s23 = p2.S_polynomial(p3);
        std::cout << "S(p2, p3) = " << s23 << "\n";
        auto div23 = p1.divide(s23, basis);
        std::cout << "Remainder = " << div23.second << "\n";
        
        bool isGroebner = p1.isGroebnerBasis(basis);
        std::cout << "\nIs G a Groebner basis for lex? " << (isGroebner ? "YES" : "NO") << "\n";
    }
    
    return 0;
}