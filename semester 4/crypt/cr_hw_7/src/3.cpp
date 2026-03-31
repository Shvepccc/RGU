#include "../include/continued_fractions.hpp"
#include <iostream>
#include <vector>

using cf = continued_fractions;
void print_fraction(const cf::fraction& f)
{
    std::cout << f.numerator << "/" << f.denominator;
}

void print_path(const std::vector<bool>& path)
{
    for (bool bit : path)
    {
        std::cout << (bit ? "R" : "L");
    }
}

int main()
{
    bigint a = 117;
    bigint b = 38;

    std::cout << "Original fraction: ";
    std::cout << a << "/" << b << std::endl;

    std::vector<bigint> quotients = cf::get_continued_fraction(a, b);
    std::cout << "a. Partial quotients: [";
    for (std::size_t i = 0; i < quotients.size(); ++i)
    {
        std::cout << quotients[i] << (i == quotients.size() - 1 ? "" : ", ");
    }
    std::cout << "]" << std::endl;

    cf::fraction from_quotients = cf::get_fraction_from_continued(quotients);
    std::cout << "b. Fraction from quotients: ";
    print_fraction(from_quotients);
    std::cout << std::endl;

    std::vector<cf::fraction> convergents = cf::get_convergents(a, b);
    std::cout << "c. All convergents: ";
    for (std::size_t i = 0; i < convergents.size(); ++i)
    {
        print_fraction(convergents[i]);
        std::cout << (i == convergents.size() - 1 ? "" : ", ");
    }
    std::cout << std::endl;

    std::vector<bool> cw_path = cf::get_calkin_wilf_path(a, b);
    std::vector<bool> sb_path = cf::get_stern_brocot_path(a, b);
    std::cout << "d. Path in Calkin-Wilf tree: ";
    print_path(cw_path);
    std::cout << std::endl;
    std::cout << "d. Path in Stern-Brocot tree: ";
    print_path(sb_path);
    std::cout << std::endl;

    cf::fraction from_cw = cf::get_fraction_from_calkin_wilf(cw_path);
    cf::fraction from_sb = cf::get_fraction_from_stern_brocot(sb_path);
    std::cout << "e. Fraction from Calkin-Wilf path: ";
    print_fraction(from_cw);
    std::cout << std::endl;
    std::cout << "e. Fraction from Stern-Brocot path: ";
    print_fraction(from_sb);
    std::cout << std::endl;

    std::vector<cf::fraction> convergents_from_sb = cf::get_convergents_from_stern_brocot(sb_path);
    std::cout << "f. Convergents from Stern-Brocot path: ";
    for (std::size_t i = 0; i < convergents_from_sb.size(); ++i)
    {
        print_fraction(convergents_from_sb[i]);
        std::cout << (i == convergents_from_sb.size() - 1 ? "" : ", ");
    }
    std::cout << std::endl;

    return 0;
}