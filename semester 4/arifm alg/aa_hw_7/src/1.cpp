#include <iostream>
#include "../include/modular.hpp"
#include "../include/polynomial.hpp"

int main()
{
    // Example 1: Polynomials over integers with LEX order
    std::vector<std::string> vars = {"x", "y"};
    
    polynomial<int> f(vars, term_order_type::LEX);
    polynomial<int> g(vars, term_order_type::LEX);
    
    // f = x^2 + y
    f.addTerm({2, 0}, 1);
    f.addTerm({0, 1}, 1);
    
    // g = x*y + 1
    g.addTerm({1, 1}, 1);
    g.addTerm({0, 0}, 1);
    
    std::cout << "=== Example 1: Integer polynomials ===" << std::endl;
    std::cout << "f = " << f << std::endl;
    std::cout << "g = " << g << std::endl;
    std::cout << "multideg(f) = (" << f.multideg()[0] << ", " << f.multideg()[1] << ")" << std::endl;
    std::cout << "multideg(g) = (" << g.multideg()[0] << ", " << g.multideg()[1] << ")" << std::endl;
    
    polynomial<int> spoly = S_polynomial(f, g);
    std::cout << "S(f, g) = " << spoly << std::endl;
    std::cout << "multideg S(f,g) = (" << S_multideg(f, g)[0] << ", " << S_multideg(f, g)[1] << ")" << std::endl;
    std::cout << std::endl;
    
    // Example 2: Polynomials with GRLEX order
    polynomial<int> h(vars, term_order_type::GRLEX);
    polynomial<int> k(vars, term_order_type::GRLEX);
    
    // h = x^2*y + x*y^2
    h.addTerm({2, 1}, 1);
    h.addTerm({1, 2}, 1);
    
    // k = x*y + y^2
    k.addTerm({1, 1}, 1);
    k.addTerm({0, 2}, 1);
    
    std::cout << "=== Example 2: GRLEX order ===" << std::endl;
    std::cout << "h = " << h << std::endl;
    std::cout << "k = " << k << std::endl;
    std::cout << "multideg(h) = (" << h.multideg()[0] << ", " << h.multideg()[1] << ")" << std::endl;
    std::cout << "multideg(k) = (" << k.multideg()[0] << ", " << k.multideg()[1] << ")" << std::endl;
    
    polynomial<int> spoly2 = S_polynomial(h, k);
    std::cout << "S(h, k) = " << spoly2 << std::endl;
    std::cout << "multideg S(h,k) = (" << S_multideg(h, k)[0] << ", " << S_multideg(h, k)[1] << ")" << std::endl;
    std::cout << std::endl;
    
    // Example 3: Modular arithmetic with mod 7
    using mod7 = modular<7>;
    
    std::vector<std::string> vars2 = {"x", "y", "z"};
    polynomial<mod7> p(vars2, term_order_type::GREVLEX);
    polynomial<mod7> q(vars2, term_order_type::GREVLEX);
    
    // p = x^2*y + z^2
    p.addTerm({2, 1, 0}, mod7(1));
    p.addTerm({0, 0, 2}, mod7(1));
    
    // q = y*z + 2*x
    q.addTerm({0, 1, 1}, mod7(1));
    q.addTerm({1, 0, 0}, mod7(2));
    
    std::cout << "=== Example 3: Modular arithmetic (mod 7) ===" << std::endl;
    std::cout << "p = " << p << std::endl;
    std::cout << "q = " << q << std::endl;
    std::cout << "multideg(p) = (" << p.multideg()[0] << ", " << p.multideg()[1] << ", " << p.multideg()[2] << ")" << std::endl;
    std::cout << "multideg(q) = (" << q.multideg()[0] << ", " << q.multideg()[1] << ", " << q.multideg()[2] << ")" << std::endl;
    
    polynomial<mod7> spoly3 = S_polynomial(p, q);
    std::cout << "S(p, q) = " << spoly3 << std::endl;
    
    std::vector<int> spoly_multideg = S_multideg(p, q);
    std::cout << "multideg S(p,q) = (" << spoly_multideg[0] << ", " << spoly_multideg[1] << ", " << spoly_multideg[2] << ")" << std::endl;
    std::cout << std::endl;
    
    // Example 4: Groebner basis criterion check
    std::cout << "=== Example 4: Groebner basis criterion ===" << std::endl;
    
    std::vector<std::string> vars3 = {"x", "y"};
    polynomial<int> b1(vars3, term_order_type::LEX);
    polynomial<int> b2(vars3, term_order_type::LEX);
    
    // Groebner basis for ideal <x^2 + y, x*y + 1>
    b1.addTerm({2, 0}, 1);
    b1.addTerm({0, 1}, 1);
    b2.addTerm({1, 1}, 1);
    b2.addTerm({0, 0}, 1);
    
    std::vector<polynomial<int>> basis = {b1, b2};
    
    std::cout << "Basis: " << std::endl;
    std::cout << "  g1 = " << b1 << std::endl;
    std::cout << "  g2 = " << b2 << std::endl;
    
    bool isGB = isGroebnerBasis(basis);
    std::cout << "Is this a Groebner basis? " << (isGB ? "Yes" : "No") << std::endl;
    
    // Example 5: Non-Groebner basis
    std::cout << std::endl;
    std::cout << "=== Example 5: Non-Groebner basis ===" << std::endl;
    
    polynomial<int> c1(vars3, term_order_type::LEX);
    polynomial<int> c2(vars3, term_order_type::LEX);
    
    // Not a Groebner basis for ideal <x^2, x*y + y^2>
    c1.addTerm({2, 0}, 1);
    c2.addTerm({1, 1}, 1);
    c2.addTerm({0, 2}, 1);
    
    std::vector<polynomial<int>> bad_basis = {c1, c2};
    std::cout << "Basis: " << std::endl;
    std::cout << "  h1 = " << c1 << std::endl;
    std::cout << "  h2 = " << c2 << std::endl;
    
    polynomial<int> spoly_test = S_polynomial(c1, c2);
    std::cout << "S(h1, h2) = " << spoly_test << std::endl;
    
    auto division_result = spoly_test.divide(spoly_test, bad_basis);
    std::cout << "Remainder after division by basis: " << division_result.second << std::endl;
    
    bool isGB2 = isGroebnerBasis(bad_basis);
    std::cout << "Is this a Groebner basis? " << (isGB2 ? "Yes" : "No") << std::endl;
    
    return 0;
}