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
}

void task_272b()
{
    std::cout << "\n=== 2.7.2(b) ===" << std::endl;
    
    std::vector<std::string> vars = {"x", "y"};
    
    polynomial<int> f1(vars, term_order_type::LEX);
    f1.addTerm({2, 0}, 1);
    f1.addTerm({0, 1}, 1);
    
    polynomial<int> f2(vars, term_order_type::LEX);
    f2.addTerm({4, 0}, 1);
    f2.addTerm({2, 1}, 2);
    f2.addTerm({0, 2}, 1);
    f2.addTerm({0, 0}, 3);
    
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
}