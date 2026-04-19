#include <iostream>
#include <vector>
#include <string>
#include "../include/polynomial.hpp"

void print_basis(const std::string& title, const std::vector<polynomial<double>>& basis)
{
    std::cout << title << ":" << std::endl;
    if (basis.empty())
    {
        std::cout << "  {0}" << std::endl;
    }
    else
    {
        for (const auto& p : basis)
        {
            std::cout << "  " << p << std::endl;
        }
    }
    std::cout << std::endl;
}

int main()
{
    std::vector<std::string> vars_xyz = {"x", "y", "z"};
    std::vector<std::string> vars_zxy = {"z", "x", "y"};

    {
        std::cout << ">>> Ideal I1 = <x^2 - y, x^3 - z> <<<" << std::endl;

        polynomial<double> p_kzx(vars_xyz, term_order_type::LEX);
        polynomial<double> f1(vars_xyz, term_order_type::LEX);
        f1.addTerm({2, 0, 0}, 1.0);
        f1.addTerm({0, 1, 0}, -1.0);
        polynomial<double> f2(vars_xyz, term_order_type::LEX);
        f2.addTerm({3, 0, 0}, 1.0);
        f2.addTerm({0, 0, 1}, -1.0);

        std::vector<polynomial<double>> gb_kzx = p_kzx.reduced_groebner_basis({f1, f2});
        print_basis("Basis in k[z][x,y] (LEX order x > y > z)", gb_kzx);

        polynomial<double> p_xyz(vars_xyz, term_order_type::GRLEX);
        polynomial<double> h1(vars_xyz, term_order_type::GRLEX);
        h1.addTerm({2, 0, 0}, 1.0);
        h1.addTerm({0, 1, 0}, -1.0);
        polynomial<double> h2(vars_xyz, term_order_type::GRLEX);
        h2.addTerm({3, 0, 0}, 1.0);
        h2.addTerm({0, 0, 1}, -1.0);

        std::vector<polynomial<double>> gb_xyz = p_xyz.reduced_groebner_basis({h1, h2});
        print_basis("Basis in k[x,y,z] (GRLEX order)", gb_xyz);
    }

    {
        std::cout << ">>> Ideal I2 = <x + y + z, x*y + y*z + z*x, x*y*z - 1> <<<" << std::endl;

        polynomial<double> p_kzx(vars_xyz, term_order_type::LEX);
        polynomial<double> f1(vars_xyz, term_order_type::LEX);
        f1.addTerm({1, 0, 0}, 1.0); 
        f1.addTerm({0, 1, 0}, 1.0); 
        f1.addTerm({0, 0, 1}, 1.0);
        polynomial<double> f2(vars_xyz, term_order_type::LEX);
        f2.addTerm({1, 1, 0}, 1.0); 
        f2.addTerm({0, 1, 1}, 1.0); 
        f2.addTerm({1, 0, 1}, 1.0);
        polynomial<double> f3(vars_xyz, term_order_type::LEX);
        f3.addTerm({1, 1, 1}, 1.0); 
        f3.addTerm({0, 0, 0}, -1.0);

        std::vector<polynomial<double>> gb_kzx = p_kzx.reduced_groebner_basis({f1, f2, f3});
        print_basis("Basis in k[z][x,y] (LEX order x > y > z)", gb_kzx);

        polynomial<double> p_kxz(vars_zxy, term_order_type::LEX);
        polynomial<double> g1(vars_zxy, term_order_type::LEX);
        g1.addTerm({1, 0, 0}, 1.0); 
        g1.addTerm({0, 1, 0}, 1.0); 
        g1.addTerm({0, 0, 1}, 1.0);
        polynomial<double> g2(vars_zxy, term_order_type::LEX);
        g2.addTerm({0, 1, 1}, 1.0); 
        g2.addTerm({1, 0, 1}, 1.0); 
        g2.addTerm({1, 1, 0}, 1.0);
        polynomial<double> g3(vars_zxy, term_order_type::LEX);
        g3.addTerm({1, 1, 1}, 1.0); 
        g3.addTerm({0, 0, 0}, -1.0);

        std::vector<polynomial<double>> gb_kxz = p_kxz.reduced_groebner_basis({g1, g2, g3});
        print_basis("Basis in k[x,y][z] (LEX order z > x > y)", gb_kxz);
    }

    return 0;
}