#include <cstddef>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cctype>

#include "../include/boolean_expression.hpp"
#include "../include/truth_table.hpp"

int main() {
    
    std::vector<std::string> expressions = {
        "((((x_1 & x_2) + (x_3 & x_4)) & (((x_1 @ x_2) @ x_3) > x_4)) @ (((x_1 & x_2) & (x_3 > x_4)) + (x_3 & x_4)))",
        "(x_1 & y_2)",
        // "(-x_1 | y_2)",
        // "((x_1 + y_2) & z_3)",
        // "(x_1 @ (y_2 > z_3))",
        // "((x_1 + y_2) & [-z_3 + x_123])",
        // "(x_1 & (y_2 + -y_2))",
        // "", 
        // "{       -([{x_123} & y_2])}",
        // "(x_1 & (y_2 + -y_2))",
        // "((x_2 > x_1) & (x_2 ! x_2))",
        "((x_1 > ((x_2 > x_3) > x_4)) ~ ((-x_1) & (x_2 > x_3) & (-x_4)))",
        // "(x_1 + -x_1) & ( y_2+ -y_2) & (z_3 + -z_3)",
        // "(x_1&-x_1)+(y_1&-y_1)+(z_1&-z_1)"
    };
    
    for (const auto& expr : expressions) {
        try {
            std::cout << "Parsing: " << expr << std::endl;
            
            BooleanExpression bool_expr(expr);
            
            std::unordered_map<std::string, bool> vars = {
                {"x_1", true},
                {"x_123", true},
                {"y_2", false},
                {"z_3", true}
            };
            
            TruthTable table(bool_expr.getVariables(), bool_expr.generateTruthTable());

            std::cout << "Truth Table:" << std::endl;
            table.printTruthTable();

            std::cout << "Delete Fictive Variables:" << std::endl;
            if (table.deleteFectiveVariables()) {
                std::cout << std::endl;
                std::cout << "NEW Truth Table:" << std::endl;
                table.printTruthTable();
            }
            else {
                std::cout << "No fective variables" << std::endl;
            }
            
            std::cout << "SDNF: " << table.getSDNF() << std::endl;
            std::cout << "SKNF: " << table.getSKNF() << std::endl;
            std::cout << "ANF:  " << table.getANF() << std::endl;

            std::cout << "\n----- Dual function -----" << std::endl;
            auto dual_function_table = table.getDualFunction();
            dual_function_table.printTruthTable();
            std::cout << "SDNF: " << dual_function_table.getSDNF() << std::endl;
            std::cout << "SKNF: " << dual_function_table.getSKNF() << std::endl;
            std::cout << "\n\n";
        } 
        catch (const std::exception& e) {
            std::cerr << "*** Error: " << e.what() << std::endl;
        }
    }
    
    return 0;
}