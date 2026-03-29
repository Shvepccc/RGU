#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

#include "../include/polynomial.hpp"

void demonstrate_polynomial(
    const std::string& description,
    const std::vector<std::string>& vars,
    const std::vector<std::pair<std::vector<int>, double>>& terms,
    const std::vector<term_order_type>& orders,
    const std::vector<std::string>& order_names)
{
    std::cout << description << "\n\n";
    
    for (size_t i = 0; i < orders.size(); ++i)
    {
        polynomial<double> p(vars, orders[i]);
        
        for (const auto& term : terms)
        {
            p.addTerm(term.first, term.second);
        }
        
        std::cout << "Order: " << order_names[i] << "\n";
        std::cout << "  Polynomial: " << p << "\n";
        
        std::vector<int> md = p.multideg();
        std::cout << "  multideg(f) = (";
        for (size_t j = 0; j < md.size(); ++j)
        {
            std::cout << md[j];
            if (j < md.size() - 1) std::cout << ", ";
        }
        std::cout << ")\n";
        
        std::cout << "  lc(f) = " << p.lc() << "\n";
        
        auto lt = p.lt();
        std::cout << "  lt(f) = ";
        if (lt.second != 1.0) std::cout << lt.second;
        if (!lt.first.empty() && lt.second != 1.0) std::cout << "*";
        bool first = true;
        for (size_t j = 0; j < lt.first.size(); ++j)
        {
            if (lt.first[j] > 0)
            {
                if (!first) std::cout << "*";
                std::cout << vars[j];
                if (lt.first[j] > 1) std::cout << "^" << lt.first[j];
                first = false;
            }
        }
        if (first) std::cout << "1";
        std::cout << "\n";
        
        std::cout << "  lm(f) = ";
        auto lm = p.lm();
        first = true;
        for (size_t j = 0; j < lm.size(); ++j)
        {
            if (lm[j] > 0)
            {
                if (!first) std::cout << "*";
                std::cout << vars[j];
                if (lm[j] > 1) std::cout << "^" << lm[j];
                first = false;
            }
        }
        if (first) std::cout << "1";
        std::cout << "\n\n";
    }
}

std::vector<int> parseExponentVector(const std::string& term, const std::vector<std::string>& vars)
{
    std::vector<int> exps(vars.size(), 0);
    
    for (size_t i = 0; i < vars.size(); ++i)
    {
        size_t varPos = term.find(vars[i]);
        if (varPos != std::string::npos)
        {
            size_t caretPos = term.find('^', varPos);
            if (caretPos != std::string::npos && caretPos + 1 < term.size())
            {
                size_t endPos = caretPos + 1;
                while (endPos < term.size() && std::isdigit(term[endPos]))
                {
                    ++endPos;
                }
                exps[i] = std::stoi(term.substr(caretPos + 1, endPos - caretPos - 1));
            }
            else
            {
                exps[i] = 1;
            }
        }
    }
    
    return exps;
}

std::vector<std::string> splitPolynomial(const std::string& polyStr)
{
    std::vector<std::string> terms;
    std::string cleaned = polyStr;
    
    cleaned.erase(std::remove(cleaned.begin(), cleaned.end(), ' '), cleaned.end());
    
    size_t pos = 0;
    while (pos < cleaned.size())
    {
        size_t nextPos = cleaned.find_first_of("+-", pos + 1);
        std::string term = cleaned.substr(pos, nextPos - pos);
        
        if (!term.empty())
        {
            terms.push_back(term);
        }
        
        if (nextPos == std::string::npos)
        {
            break;
        }
        pos = nextPos;
    }
    
    return terms;
}

std::vector<term_order_type> determine_order(
    const std::vector<std::pair<std::vector<int>, double>>& terms,
    const std::vector<std::string>& vars,
    const std::vector<term_order_type>& orders)
{
    std::vector<std::vector<int>> originalSequence;
    
    for (const auto& term : terms)
    {
        originalSequence.push_back(term.first);
    }
    
    std::vector<term_order_type> matchingOrders;
    
    for (const auto& order : orders)
    {
        polynomial<double> p(vars, order);
        
        for (const auto& term : terms)
        {
            p.addTerm(term.first, term.second);
        }
        
        auto sortedTerms = p.supp();
        
        term_order comparator(order);
        std::sort(sortedTerms.begin(), sortedTerms.end(),
            [&comparator](const std::pair<std::vector<int>, double>& a,
                          const std::pair<std::vector<int>, double>& b)
            {
                return comparator.compare(a.first, b.first);
            });
        
        std::vector<std::vector<int>> sortedSequence;
        for (const auto& term : sortedTerms)
        {
            sortedSequence.push_back(term.first);
        }
        
        if (sortedSequence == originalSequence)
        {
            matchingOrders.push_back(order);
        }
    }
    
    return matchingOrders;
}

void printOrderDetermination(
    const std::string& description,
    const std::vector<std::string>& vars,
    const std::vector<term_order_type> orders,
    const std::vector<std::string> order_names,
    const std::vector<std::pair<std::vector<int>, double>>& terms)
{
    std::cout << description << "\n";
    
    std::vector<std::vector<std::vector<int>>> orderSequences;
    
    for (const auto& order : orders)
    {
        polynomial<double> p(vars, order);
        
        for (const auto& term : terms)
        {
            p.addTerm(term.first, term.second);
        }
        
        auto sortedTerms = p.supp();
        
        term_order comparator(order);
        std::sort(sortedTerms.begin(), sortedTerms.end(),
            [&comparator](const std::pair<std::vector<int>, double>& a,
                          const std::pair<std::vector<int>, double>& b)
            {
                return comparator.compare(a.first, b.first);
            });
        
        std::vector<std::vector<int>> exps;
        for (const auto& term : sortedTerms)
        {
            exps.push_back(term.first);
        }
        orderSequences.push_back(exps);
    }
    
    for (size_t i = 0; i < orders.size(); ++i)
    {
        std::cout << "  " << order_names[i] << ": ";
        for (size_t k = 0; k < orderSequences[i].size(); ++k)
        {
            std::cout << "(";
            for (size_t j = 0; j < orderSequences[i][k].size(); ++j)
            {
                std::cout << orderSequences[i][k][j];
                if (j < orderSequences[i][k].size() - 1) std::cout << ",";
            }
            std::cout << ")";
            if (k < orderSequences[i].size() - 1) std::cout << " > ";
        }
        std::cout << "\n";
    }
    
    std::vector<term_order_type> matchingOrders = determine_order(terms, vars, orders);
    
    std::cout << "  Determined order(s): ";
    if (matchingOrders.empty())
    {
        std::cout << "None (original order doesn't match any of the tested orders)";
    }
    else
    {
        for (size_t i = 0; i < matchingOrders.size(); ++i)
        {
            switch (matchingOrders[i])
            {
                case term_order_type::LEX:
                    std::cout << "LEX";
                    break;
                case term_order_type::GRLEX:
                    std::cout << "GRLEX";
                    break;
                case term_order_type::GREVLEX:
                    std::cout << "GREVLEX";
                    break;
                case term_order_type::INVLEX:
                    std::cout << "INVLEX";
                    break;
                case term_order_type::RINVLEX:
                    std::cout << "RINVLEX";
                    break;
                default:
                    std::cout << "UNKNOWN";
            }
            if (i < matchingOrders.size() - 1)
            {
                std::cout << " or ";
            }
        }
    }
    std::cout << "\n\n";
}


int main()
{
    try
    {
        std::vector<term_order_type> orders = {
            term_order_type::LEX,
            term_order_type::GRLEX,
            term_order_type::GREVLEX,
            term_order_type::INVLEX,
            term_order_type::RINVLEX
        };
        
        std::vector<std::string> order_names = {"LEX", "GRLEX", "GREVLEX", "INVLEX", "RINVLEX"};
        
        std::vector<std::string> vars_xyz = {"x", "y", "z"};
        std::vector<std::string> vars_zyx = {"z", "y", "x"};
        
        std::cout << "=== Exercise 1(a) ===\n";
        std::vector<std::pair<std::vector<int>, double>> terms1a = {
            {{1, 0, 0}, 2.0},
            {{0, 1, 0}, 3.0},
            {{0, 0, 1}, 1.0},
            {{2, 0, 0}, 1.0},
            {{0, 0, 2}, -1.0},
            {{3, 0, 0}, 1.0}
        };
        
        demonstrate_polynomial(
            "f(x, y, z) = 2*x + 3*y + z + x^2 - z^2 + x^3",
            vars_xyz,
            terms1a,
            orders,
            order_names);
        
        std::cout << "=== Exercise 1(b) ===\n";
        std::vector<std::pair<std::vector<int>, double>> terms1b = {
            {{2, 8, 0}, 2.0},
            {{5, 1, 4}, -3.0},
            {{1, 2, 3}, 1.0},
            {{1, 4, 0}, -1.0}
        };
        
        demonstrate_polynomial(
            "f(x, y, z) = 2*x^2*y^8 - 3*x^5*y*z^4 + x*y^2*z^3 - x*y^4",
            vars_xyz,
            terms1b,
            orders,
            order_names);
        
        std::cout << "=== Exercise 2 ===\n";
        std::cout << "Determine which order (LEX, GRLEX, GREVLEX) was used for each polynomial\n\n";
        
        std::vector<std::pair<std::string, std::vector<std::pair<std::vector<int>, double>>>> tests = {
            {"(a) f(x, y, z) = 7*x^2*y^4*z - 2*x*y^6 + x^2*y^2",
            {{{2, 4, 1}, 7.0}, {{1, 6, 0}, -2.0}, {{2, 2, 0}, 1.0}}},
            {"(b) f(x, y, z) = x*y^3*z + x*y^2*z^2 + x^2*z^3",
            {{{1, 3, 1}, 1.0}, {{1, 2, 2}, 1.0}, {{2, 0, 3}, 1.0}}},
            {"(c) f(x, y, z) = x^4*y^5*z + 2*x^3*y^2*z - 4*x*y^2*z^4",
            {{{4, 5, 1}, 1.0}, {{3, 2, 1}, 2.0}, {{1, 2, 4}, -4.0}}}
        };
        
        for (const auto& test : tests)
        {
            printOrderDetermination(test.first, vars_xyz, orders, order_names, test.second);
        }
        
        std::cout << "=== Exercise 3 ===\n";
        std::cout << "Using variable order z > y > x\n\n";

        polynomial<double> p1(vars_xyz);
        for (const auto& term : terms1a)
        {
            p1.addTerm(term.first, term.second);
        }

        polynomial<double> p1_reordered = p1.withVariableOrder(vars_zyx);

        demonstrate_polynomial(
            "f(x, y, z) = 2*x + 3*y + z + x^2 - z^2 + x^3",
            vars_zyx,
            p1_reordered.supp(),
            orders,
            order_names);

        polynomial<double> p2(vars_xyz);
        for (const auto& term : terms1b)
        {
            p2.addTerm(term.first, term.second);
        }

        polynomial<double> p2_reordered = p2.withVariableOrder(vars_zyx);

        demonstrate_polynomial(
            "f(x, y, z) = 2*x^2*y^8 - 3*x^5*y*z^4 + x*y^2*z^3 - x*y^4 (with z > y > x)",
            vars_zyx,
            p2_reordered.supp(),
            orders,
            order_names);

        std::cout << "=== Exercise 21.6 ===\n";
        std::vector<std::pair<std::vector<int>, double>> terms21_6 = {
            {{4, 2, 1}, 2.0},
            {{4, 1, 2}, -6.0},
            {{1, 4, 2}, 4.0},
            {{1, 2, 4}, -3.0},
            {{2, 4, 1}, 1.0},
            {{2, 1, 4}, -5.0}
        };

        polynomial<double> p21_6(vars_xyz);
        for (const auto& term : terms21_6)
        {
            p21_6.addTerm(term.first, term.second);
        }

        demonstrate_polynomial(
            "f(x, y, z) = 2x^4 y^2 z - 6x^4 y z^2 + 4x y^4 z^2 - 3x y^2 z^4 + x^2 y^4 z - 5x^2 y z^4",
            vars_xyz,
            p21_6.supp(),
            orders,
            order_names);
        
        std::cout << "\nAll exercises completed successfully.\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}