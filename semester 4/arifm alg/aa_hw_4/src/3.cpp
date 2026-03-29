#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <complex>

#include "../include/modular.hpp"
#include "../include/polynomial.hpp"

int main() {
    try {
        // -------------------------------------------------------------
        // 1. Многочлены от двух переменных "x", "y" с коэффициентами double
        // -------------------------------------------------------------
        std::vector<std::string> vars2 = {"x", "y"};
        polynomial<double> p(vars2), q(vars2);

        p.addTerm({2, 1}, 3.5);
        p.addTerm({0, 1}, 2.3);
        p.addTerm({1, 0}, 1.7);

        q.addTerm({1, 0}, 4.0);
        q.addTerm({0, 1}, -2.0);
        q.addTerm({0, 0}, 5.0);

        std::cout << "=== Демонстрация для double (2 переменные) ===\n";
        std::cout << "p = " << p << "\n";
        std::cout << "q = " << q << "\n";
        std::cout << "p + q = " << p + q << "\n";
        std::cout << "p - q = " << p - q << "\n";
        std::cout << "p * q = " << p * q << "\n";
        std::cout << "2.0 * p = " << 2.0 * p << "\n";
        std::cout << "p * 2.0 = " << p * 2.0 << "\n";

        std::vector<double> point = {2.0, 3.0};
        std::cout << "p(2,3) = " << p.evaluate(point) << "\n";

        auto supp_p = p.supp();
        std::cout << "supp(p):\n";
        for (const auto& term : supp_p) {
            std::cout << "  ";
            for (size_t i = 0; i < term.first.size(); ++i)
                if (term.first[i]) std::cout << vars2[i] << "^" << term.first[i] << " ";
            std::cout << ": " << term.second << "\n";
        }

        std::cout << "p == p? " << (p == p ? "да" : "нет") << "\n";
        std::cout << "p == q? " << (p == q ? "да" : "нет") << "\n";

        // -------------------------------------------------------------
        // 2. Однородность и выделение компонент
        // -------------------------------------------------------------
        polynomial<double> homogeneous(vars2);
        homogeneous.addTerm({3, 0}, 1.0);
        homogeneous.addTerm({2, 1}, 2.0);
        homogeneous.addTerm({1, 2}, 3.0);
        homogeneous.addTerm({0, 3}, 4.0);

        polynomial<double> inhomogeneous(vars2);
        inhomogeneous.addTerm({2, 0}, 1.0);
        inhomogeneous.addTerm({1, 1}, 2.0);
        inhomogeneous.addTerm({0, 2}, 3.0);
        inhomogeneous.addTerm({0, 0}, 5.0);

        std::cout << "\n=== Однородность ===\n";
        std::cout << "h = " << homogeneous << "\n";
        std::cout << "h однороден? " << (homogeneous.isHomogeneous() ? "да" : "нет") << "\n";
        std::cout << "степень однородности h: " << homogeneous.homogeneousDegree() << "\n";
        std::cout << "g = " << inhomogeneous << "\n";
        std::cout << "g однороден? " << (inhomogeneous.isHomogeneous() ? "да" : "нет") << "\n";
        std::cout << "степень однородности g: " << inhomogeneous.homogeneousDegree() << "\n";

        std::cout << "\n=== Выделение однородных компонент ===\n";
        for (int deg = 0; deg <= 3; ++deg) {
            auto comp = homogeneous.homogeneousComponent(deg);
            std::cout << "Компонента степени " << deg << ": " << comp << "\n";
        }

        // -------------------------------------------------------------
        // 3. Многочлены от трёх переменных (int)
        // -------------------------------------------------------------
        std::vector<std::string> vars3 = {"a", "b", "c"};
        polynomial<int> r(vars3);
        r.addTerm({1, 0, 2}, 3);
        r.addTerm({0, 2, 1}, -2);
        r.addTerm({0, 0, 0}, 7);
        r.addTerm({2, 1, 0}, 5);

        polynomial<int> s(vars3);
        s.addTerm({1, 0, 2}, 3);
        s.addTerm({0, 2, 1}, 1);
        s.addTerm({0, 0, 1}, 4);

        std::cout << "\n=== Многочлены от трёх переменных (int) ===\n";
        std::cout << "r = " << r << "\n";
        std::cout << "s = " << s << "\n";
        std::cout << "r + s = " << r + s << "\n";
        std::cout << "r - s = " << r - s << "\n";
        std::cout << "r * s = " << r * s << "\n";

        std::vector<int> pt3 = {2, 1, 3};
        std::cout << "r(2,1,3) = " << r.evaluate(pt3) << "\n";

        // -------------------------------------------------------------
        // 4. Многочлены от одной переменной (complex<double>)
        // -------------------------------------------------------------
        std::vector<std::string> vars1 = {"z"};
        polynomial<std::complex<double>> f(vars1);
        f.addTerm({2}, std::complex<double>{1.0, 1.0});
        f.addTerm({1}, std::complex<double>{2.0, -1.0});
        f.addTerm({0}, std::complex<double>{0.0, 3.0});

        std::cout << "\n=== Многочлен от одной переменной (комплексные) ===\n";
        std::cout << "f(z) = " << f << "\n";
        std::complex<double> z0 = {1.0, 2.0};
        std::cout << "f(" << z0 << ") = " << f.evaluate({z0}) << "\n";

        // -------------------------------------------------------------
        // 5. Многочлены с кольцом вычетов modular<7>
        // -------------------------------------------------------------
        std::vector<std::string> vars_mod = {"uppi", "vappi"};
        polynomial<modular<7>> A(vars_mod), B(vars_mod);

        A.addTerm({2, 1}, modular<7>(3));
        A.addTerm({0, 1}, modular<7>(2));
        A.addTerm({1, 0}, modular<7>(1));

        B.addTerm({1, 0}, modular<7>(4));
        B.addTerm({0, 1}, modular<7>(5));
        B.addTerm({0, 0}, modular<7>(5));

        std::cout << "\n=== Многочлены над Z/7Z ===\n";
        std::cout << "A = " << A << "\n";
        std::cout << "B = " << B << "\n";
        std::cout << "A + B = " << A + B << "\n";
        std::cout << "A * B = " << A * B << "\n";

        std::vector<modular<7>> pt_mod = {modular<7>(2), modular<7>(3)};
        std::cout << "A(2,3) mod 7 = " << A.evaluate(pt_mod) << "\n";

        // -------------------------------------------------------------
        // 6. Граничные случаи: нулевой многочлен, отрицательные степени,
        //    несовпадение размерностей (исключения)
        // -------------------------------------------------------------
        std::cout << "\n=== Граничные случаи ===\n";
        polynomial<double> zero(vars2);
        std::cout << "Нулевой многочлен: " << zero << " (isZero? " << zero.isZero() << ")\n";
        std::cout << "supp(zero) размер: " << zero.supp().size() << "\n";

        std::cout << "p + zero = " << p + zero << "\n";

        std::cout << "zero.isHomogeneous() = " << zero.isHomogeneous() << "\n";
        std::cout << "zero.homogeneousDegree() = " << zero.homogeneousDegree() << "\n";

        try {
            zero.addTerm({1, 0, 1}, 5.0);
        } catch (const std::exception& e) {
            std::cout << "Ожидаемое исключение при addTerm: " << e.what() << "\n";
        }

        try {
            p.addTerm({-1, 2}, 1.0);
        } catch (const std::exception& e) {
            std::cout << "Ожидаемое исключение (отрицательная степень): " << e.what() << "\n";
        }

        polynomial<double> oneVar(vars1);
        try {
            p + oneVar;
        } catch (const std::exception& e) {
            std::cout << "Ожидаемое исключение (разное число переменных): " << e.what() << "\n";
        }

        // -------------------------------------------------------------
        // 7. Умножение на скаляр и проверка равенства после преобразований
        // -------------------------------------------------------------
        polynomial<double> p2 = p;
        p2 *= 2.0;
        std::cout << "\n=== Умножение на скаляр с присваиванием ===\n";
        std::cout << "p = " << p << "\n";
        std::cout << "p2 (p*2) = " << p2 << "\n";
        std::cout << "p2 == p*2 ? " << (p2 == p * 2.0 ? "да" : "нет") << "\n";

        // -------------------------------------------------------------
        // 8. Многочлен от 7 переменных с длинными названиями
        //    и различными степенями (4 различные степени)
        // -------------------------------------------------------------
        std::cout << "\n=== Многочлен от 7 переменных ===\n";
        std::vector<std::string> many_vars = {
            "temperature", "pressure", "volume", 
            "entropy", "enthalpy", "density", "viscosity"
        };

        polynomial<double> complexPoly(many_vars);

        complexPoly.addTerm({0, 0, 0, 0, 0, 0, 0}, 3.14159);
        complexPoly.addTerm({2, 0, 1, 0, 0, 0, 0}, 2.71828);
        complexPoly.addTerm({0, 1, 0, 1, 0, 0, 0}, 1.41421);
        complexPoly.addTerm({1, 0, 2, 0, 1, 0, 0}, 0.57721);
        complexPoly.addTerm({0, 0, 0, 2, 0, 1, 0}, 1.61803);
        complexPoly.addTerm({3, 0, 0, 0, 0, 0, 2}, 0.69314);
        complexPoly.addTerm({0, 2, 0, 0, 2, 0, 1}, 0.30102);
        complexPoly.addTerm({1, 1, 1, 1, 1, 0, 0}, 0.47712);
        complexPoly.addTerm({0, 0, 3, 0, 0, 2, 1}, 0.84510);
        complexPoly.addTerm({2, 0, 0, 2, 0, 0, 2}, 0.12345);
        complexPoly.addTerm({0, 0, 0, 0, 0, 0, 4}, 0.98765);
        complexPoly.addTerm({4, 0, 0, 0, 0, 0, 0}, 0.45678);
        complexPoly.addTerm({0, 0, 0, 0, 0, 3, 0}, 0.78901);
        complexPoly.addTerm({0, 0, 0, 0, 0, 0, 0}, 2.71828);

        std::cout << "Многочлен от 7 переменных:\n";
        std::cout << complexPoly << "\n\n";

        std::cout << "Носитель (supp) с группировкой по суммарной степени:\n";
        auto supp_complex = complexPoly.supp();
        std::map<int, std::vector<std::pair<std::vector<int>, double>>> byDegree;
        for (const auto& term : supp_complex) {
            int totalDegree = 0;
            for (int exp : term.first) totalDegree += exp;
            byDegree[totalDegree].push_back(term);
        }

        for (const auto& [deg, terms] : byDegree) {
            std::cout << "  Степень " << deg << " (" << terms.size() << " членов):\n";
            for (const auto& term : terms) {
                std::cout << "    ";
                bool first = true;
                for (size_t i = 0; i < term.first.size(); ++i) {
                    if (term.first[i] > 0) {
                        if (!first) std::cout << " * ";
                        std::cout << many_vars[i];
                        if (term.first[i] > 1) std::cout << "^" << term.first[i];
                        first = false;
                    }
                }
                if (first) std::cout << "1";
                std::cout << " : " << term.second << "\n";
            }
        }
        std::cout << "\n";

        std::cout << "Многочлен однороден? " << (complexPoly.isHomogeneous() ? "да" : "нет") << "\n";
        std::cout << "Степень однородности: " << complexPoly.homogeneousDegree() << "\n";

        std::cout << "\nОднородные компоненты:\n";
        for (const auto& [deg, _] : byDegree) {
            auto component = complexPoly.homogeneousComponent(deg);
            std::cout << "  Компонента степени " << deg << ": " << component << "\n";
        }

        polynomial<double> sumComponents(many_vars);
        for (const auto& [deg, _] : byDegree) {
            sumComponents = sumComponents + complexPoly.homogeneousComponent(deg);
        }
        std::cout << "\nСумма всех однородных компонент:\n" << sumComponents << "\n";
        std::cout << "Сумма компонент == исходный многочлен? " 
                << (sumComponents == complexPoly ? "ДА" : "НЕТ") << "\n";

        std::vector<double> point7(many_vars.size(), 1.0);
        std::cout << "\nЗначение в точке (1,1,1,1,1,1,1): " 
                << complexPoly.evaluate(point7) << "\n";
        std::cout << "Ожидаемое значение (сумма всех коэффициентов): ";
        double expected = 0;
        for (const auto& term : supp_complex) expected += term.second;
        std::cout << expected << "\n";

        std::vector<double> point7b = {2.0, 3.0, 1.5, 0.5, 2.5, 1.0, 4.0};
        std::cout << "Значение в точке (2,3,1.5,0.5,2.5,1,4): " 
                << complexPoly.evaluate(point7b) << "\n";

        std::cout << "\nПроверка операций с самим собой:\n";
        auto doubled = complexPoly + complexPoly;
        std::cout << "f + f = " << doubled << "\n";
        auto squared = complexPoly * complexPoly;
        std::cout << "Количество членов в f*f: " << squared.supp().size() << "\n";

        auto scaled = complexPoly * 2.5;
        std::cout << "f * 2.5 = " << scaled << "\n";

        std::cout << "\nВсе тесты успешно завершены.\n";

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}