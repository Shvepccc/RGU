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
        std::cout << name << " не принадлежит идеалу" << std::endl;
    }
}

void problem_21_20()
{
    std::cout << "=== 21.20 ===" << std::endl;
    
    std::vector<std::string> vars = {"x", "y"};
    
    polynomial<int> f1(vars, term_order_type::LEX);
    std::vector<int> exp1_1 = {2, 0};
    std::vector<int> exp1_2 = {0, 1};
    f1.addTerm(exp1_1, 1);
    f1.addTerm(exp1_2, 1);
    f1.addTerm({0, 0}, -1);
    
    polynomial<int> f2(vars, term_order_type::LEX);
    std::vector<int> exp2_1 = {1, 1};
    std::vector<int> exp2_2 = {1, 0};
    f2.addTerm(exp2_1, 1);
    f2.addTerm(exp2_2, -1);
    
    print_polynomial(f1, "f1");
    print_polynomial(f2, "f2");
    
    std::vector<polynomial<int>> basis = {f1, f2};
    std::vector<polynomial<int>> gbasis = f1.buchberger(basis);
    
    std::cout << "Базис Грёбнера (lex, x>y):" << std::endl;
    for (size_t i = 0; i < gbasis.size(); ++i)
    {
        std::cout << "g" << i + 1 << " = " << gbasis[i] << std::endl;
    }
    
    polynomial<int> f1_test(vars, term_order_type::LEX);
    f1_test.addTerm({2, 0}, 1);
    f1_test.addTerm({0, 2}, 1);
    f1_test.addTerm({0, 1}, -1);
    
    polynomial<int> f2_test(vars, term_order_type::LEX);
    f2_test.addTerm({1, 2}, 3);
    f2_test.addTerm({1, 1}, -4);
    f2_test.addTerm({1, 0}, 1);
    f2_test.addTerm({0, 0}, 1);
    
    print_polynomial(f1_test, "t1");
    print_polynomial(f2_test, "t2");
    
    check_membership(f1_test, gbasis, "t1");
    check_membership(f2_test, gbasis, "t2");
}

void problem_21_23()
{
    std::cout << "\n=== 21.23 ===" << std::endl;
    
    std::vector<std::string> vars = {"x", "y", "z"};
    
    polynomial<int> f1(vars, term_order_type::GRLEX);
    std::vector<int> exp1 = {2, 1, 0};
    std::vector<int> exp2 = {0, 1, 1};
    f1.addTerm(exp1, 1);
    f1.addTerm(exp2, -2);
    f1.addTerm({0, 0, 0}, 1);
    
    polynomial<int> f2(vars, term_order_type::GRLEX);
    std::vector<int> exp3 = {1, 2, 0};
    std::vector<int> exp4 = {0, 0, 2};
    std::vector<int> exp5 = {1, 0, 0};
    f2.addTerm(exp3, 1);
    f2.addTerm(exp4, -1);
    f2.addTerm(exp5, 2);
    
    polynomial<int> f3(vars, term_order_type::GRLEX);
    std::vector<int> exp6 = {0, 2, 1};
    std::vector<int> exp7 = {2, 0, 0};
    f3.addTerm(exp6, 1);
    f3.addTerm(exp7, -1);
    f3.addTerm({0, 0, 0}, 5);
    
    print_polynomial(f1, "f1");
    print_polynomial(f2, "f2");
    print_polynomial(f3, "f3");
    
    std::vector<polynomial<int>> basis = {f1, f2, f3};
    std::vector<polynomial<int>> gbasis = f1.buchberger(basis);
    
    std::cout << "Базис Грёбнера (grlex, x<y<z):" << std::endl;
    for (size_t i = 0; i < gbasis.size(); ++i)
    {
        std::cout << "g" << i + 1 << " = " << gbasis[i] << std::endl;
    }
    
    std::vector<polynomial<int>> reduced = f1.reduced_groebner_basis(basis);
    std::cout << "Редуцированный базис:" << std::endl;
    for (size_t i = 0; i < reduced.size(); ++i)
    {
        std::cout << "h" << i + 1 << " = " << reduced[i] << std::endl;
    }
}

void task_271()
{
    std::cout << "\n=== 2.7.1 ===" << std::endl;
    
    std::vector<std::string> vars = {"x", "y"};
    
    polynomial<int> f1(vars, term_order_type::GRLEX);
    f1.addTerm({3, 0}, 1);
    f1.addTerm({1, 1}, -2);
    
    polynomial<int> f2(vars, term_order_type::GRLEX);
    f2.addTerm({2, 1}, 1);
    f2.addTerm({0, 2}, -2);
    f2.addTerm({1, 0}, 1);
    
    polynomial<int> f3(vars, term_order_type::GRLEX);
    f3.addTerm({2, 0}, -1);
    
    polynomial<int> f4(vars, term_order_type::GRLEX);
    f4.addTerm({1, 1}, -2);
    
    polynomial<int> f5(vars, term_order_type::GRLEX);
    f5.addTerm({0, 2}, -2);
    f5.addTerm({1, 0}, 1);
    
    std::vector<polynomial<int>> F = {f1, f2, f3, f4, f5};
    
    for (int i = 0; i < 5; ++i)
    {
        for (int j = i + 1; j < 5; ++j)
        {
            polynomial<int> spoly = F[i].S_polynomial(F[j]);
            auto div_result = spoly.divide(spoly, F);
            if (div_result.second.isZero())
            {
                std::cout << "S(f" << i+1 << ", f" << j+1 << ")^F = 0" << std::endl;
            }
            else
            {
                std::cout << "S(f" << i+1 << ", f" << j+1 << ")^F != 0" << std::endl;
            }
        }
    }
}

void task_272a()
{
    std::cout << "\n=== 2.7.2(a) ===" << std::endl;
    
    std::vector<std::string> vars = {"x", "y"};
    
    polynomial<int> f1(vars, term_order_type::LEX);
    f1.addTerm({2, 1}, 1);
    f1.addTerm({0, 0}, -1);
    
    polynomial<int> f2(vars, term_order_type::LEX);
    f2.addTerm({1, 2}, 1);
    f2.addTerm({1, 0}, -1);
    
    std::cout << "LEX порядок:" << std::endl;
    print_polynomial(f1, "f1");
    print_polynomial(f2, "f2");
    
    std::vector<polynomial<int>> basis_lex = {f1, f2};
    std::vector<polynomial<int>> gbasis_lex = f1.buchberger(basis_lex);
    std::cout << "Базис Грёбнера (lex):" << std::endl;
    for (size_t i = 0; i < gbasis_lex.size(); ++i)
    {
        std::cout << "g" << i + 1 << " = " << gbasis_lex[i] << std::endl;
    }
    
    std::vector<polynomial<int>> reduced_lex = f1.reduced_groebner_basis(basis_lex);
    std::cout << "Редуцированный базис Грёбнера (lex):" << std::endl;
    for (size_t i = 0; i < reduced_lex.size(); ++i)
    {
        std::cout << "r" << i + 1 << " = " << reduced_lex[i] << std::endl;
    }
    
    polynomial<int> f1_grlex(vars, term_order_type::GRLEX);
    f1_grlex.addTerm({2, 1}, 1);
    f1_grlex.addTerm({0, 0}, -1);
    
    polynomial<int> f2_grlex(vars, term_order_type::GRLEX);
    f2_grlex.addTerm({1, 2}, 1);
    f2_grlex.addTerm({1, 0}, -1);
    
    std::cout << "\nGRLEX порядок:" << std::endl;
    print_polynomial(f1_grlex, "f1");
    print_polynomial(f2_grlex, "f2");
    
    std::vector<polynomial<int>> basis_grlex = {f1_grlex, f2_grlex};
    std::vector<polynomial<int>> gbasis_grlex = f1_grlex.buchberger(basis_grlex);
    std::cout << "Базис Грёбнера (grlex):" << std::endl;
    for (size_t i = 0; i < gbasis_grlex.size(); ++i)
    {
        std::cout << "g" << i + 1 << " = " << gbasis_grlex[i] << std::endl;
    }
    
    std::vector<polynomial<int>> reduced_grlex = f1_grlex.reduced_groebner_basis(basis_grlex);
    std::cout << "Редуцированный базис Грёбнера (grlex):" << std::endl;
    for (size_t i = 0; i < reduced_grlex.size(); ++i)
    {
        std::cout << "r" << i + 1 << " = " << reduced_grlex[i] << std::endl;
    }
}

void task_272b()
{
    std::cout << "\n=== 2.7.2(b) ===" << std::endl;
    
    std::vector<std::string> vars = {"x", "y"};
    
    polynomial<double> f1(vars, term_order_type::LEX);
    f1.addTerm({2, 0}, 1);
    f1.addTerm({0, 1}, 1);
    
    polynomial<double> f2(vars, term_order_type::LEX);
    f2.addTerm({4, 0}, 1);
    f2.addTerm({2, 1}, 2);
    f2.addTerm({0, 2}, 1);
    f2.addTerm({0, 0}, 3);
    
    std::cout << "LEX порядок:" << std::endl;
    print_polynomial(f1, "f1");
    print_polynomial(f2, "f2");
    
    std::vector<polynomial<double>> basis_lex = {f1, f2};
    std::vector<polynomial<double>> gbasis_lex = f1.buchberger(basis_lex);
    std::cout << "Базис Грёбнера (lex):" << std::endl;
    for (size_t i = 0; i < gbasis_lex.size(); ++i)
    {
        std::cout << "g" << i + 1 << " = " << gbasis_lex[i] << std::endl;
    }
    std::cout << "999\n";
    std::vector<polynomial<double>> reduced_lex = f1.reduced_groebner_basis(basis_lex);
    std::cout << "Редуцированный базис Грёбнера (lex):" << std::endl;
    for (size_t i = 0; i < reduced_lex.size(); ++i)
    {
        std::cout << "r" << i + 1 << " = " << reduced_lex[i] << std::endl;
    }
    std::cout << "999\n";
    polynomial<int> f1_grlex(vars, term_order_type::GRLEX);
    f1_grlex.addTerm({2, 0}, 1);
    f1_grlex.addTerm({0, 1}, 1);
    
    polynomial<int> f2_grlex(vars, term_order_type::GRLEX);
    f2_grlex.addTerm({4, 0}, 1);
    f2_grlex.addTerm({2, 1}, 2);
    f2_grlex.addTerm({0, 2}, 1);
    f2_grlex.addTerm({0, 0}, 3);

    
    
    std::cout << "\nGRLEX порядок:" << std::endl;
    print_polynomial(f1_grlex, "f1");
    print_polynomial(f2_grlex, "f2");
    
    std::vector<polynomial<int>> basis_grlex = {f1_grlex, f2_grlex};
    std::vector<polynomial<int>> gbasis_grlex = f1_grlex.buchberger(basis_grlex);
    std::cout << "Базис Грёбнера (grlex):" << std::endl;
    for (size_t i = 0; i < gbasis_grlex.size(); ++i)
    {
        std::cout << "g" << i + 1 << " = " << gbasis_grlex[i] << std::endl;
    }
    
    std::vector<polynomial<int>> reduced_grlex = f1_grlex.reduced_groebner_basis(basis_grlex);
    std::cout << "Редуцированный базис Грёбнера (grlex):" << std::endl;
    for (size_t i = 0; i < reduced_grlex.size(); ++i)
    {
        std::cout << "r" << i + 1 << " = " << reduced_grlex[i] << std::endl;
    }
}

void task_272c()
{
    std::cout << "\n=== 2.7.2(c) ===" << std::endl;
    
    std::vector<std::string> vars = {"x", "y", "z"};
    
    polynomial<int> f1(vars, term_order_type::LEX);
    std::vector<int> exp1 = {1, 0, 0};
    f1.addTerm(exp1, 1);
    f1.addTerm({0, 0, 4}, -1);
    
    polynomial<int> f2(vars, term_order_type::LEX);
    std::vector<int> exp2 = {0, 1, 0};
    f2.addTerm(exp2, 1);
    f2.addTerm({0, 0, 5}, -1);
    
    std::cout << "LEX порядок:" << std::endl;
    print_polynomial(f1, "f1");
    print_polynomial(f2, "f2");
    
    std::vector<polynomial<int>> basis_lex = {f1, f2};
    std::vector<polynomial<int>> gbasis_lex = f1.buchberger(basis_lex);
    std::cout << "Базис Грёбнера (lex):" << std::endl;
    for (size_t i = 0; i < gbasis_lex.size(); ++i)
    {
        std::cout << "g" << i + 1 << " = " << gbasis_lex[i] << std::endl;
    }
    
    std::vector<polynomial<int>> reduced_lex = f1.reduced_groebner_basis(basis_lex);
    std::cout << "Редуцированный базис Грёбнера (lex):" << std::endl;
    for (size_t i = 0; i < reduced_lex.size(); ++i)
    {
        std::cout << "r" << i + 1 << " = " << reduced_lex[i] << std::endl;
    }
    
    polynomial<int> f1_grlex(vars, term_order_type::GRLEX);
    f1_grlex.addTerm({1, 0, 0}, 1);
    f1_grlex.addTerm({0, 0, 4}, -1);
    
    polynomial<int> f2_grlex(vars, term_order_type::GRLEX);
    f2_grlex.addTerm({0, 1, 0}, 1);
    f2_grlex.addTerm({0, 0, 5}, -1);
    
    std::cout << "\nGRLEX порядок:" << std::endl;
    print_polynomial(f1_grlex, "f1");
    print_polynomial(f2_grlex, "f2");
    
    std::vector<polynomial<int>> basis_grlex = {f1_grlex, f2_grlex};
    std::vector<polynomial<int>> gbasis_grlex = f1_grlex.buchberger(basis_grlex);
    std::cout << "Базис Грёбнера (grlex):" << std::endl;
    for (size_t i = 0; i < gbasis_grlex.size(); ++i)
    {
        std::cout << "g" << i + 1 << " = " << gbasis_grlex[i] << std::endl;
    }
    
    std::vector<polynomial<int>> reduced_grlex = f1_grlex.reduced_groebner_basis(basis_grlex);
    std::cout << "Редуцированный базис Грёбнера (grlex):" << std::endl;
    for (size_t i = 0; i < reduced_grlex.size(); ++i)
    {
        std::cout << "r" << i + 1 << " = " << reduced_grlex[i] << std::endl;
    }
}

int main()
{
    problem_21_20();
    problem_21_23();

    task_271();

    task_272a();
    task_272b();
    task_272c();
    
    return 0;
}