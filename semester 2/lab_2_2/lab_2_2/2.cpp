#include <iostream>
#include "../libs/bigfloat.h"

#include <iomanip>
#include <cmath>

const double PI_D = 3.14159265358979323846;
const bigfloat EPS = bigfloat(1, 1e8);

void test_trig_func(
    const std::string& name,
    bigfloat(*bf_func)(bigfloat const&, bigfloat const&),
    double(*std_func)(double))
{
    std::cout << "=== Testing " << name << " ===\n";

    bigfloat PI = bigfloat::PI();

    for (int i = -16; i <= 16; ++i)
    {
        bigfloat x_bf = PI * bigfloat(i) / 4;
        double x_d = PI_D * i / 4;

        bigfloat result = bf_func(x_bf, EPS);
        double expected = std_func(x_d);
        double d_result = to_double(result);
        double error = d_result - expected;

        std::cout << std::fixed << std::setprecision(8)
            << name << "(" << x_d << ") = " << d_result << " | expected: " << expected << " | error: " <<  ((error > 1e-6) ? error : 0) << std::endl;
    }
    std::cout << '\n';
}


int program_2_main(int argc, char* argv[])
{
    //std::cout << "sin(22/7) = " << sin(bigfloat(22, 7), bigfloat(1, 1e6)) << "\n";

    test_trig_func("sin", sin, std::sin);
    test_trig_func("cos", cos, std::cos);
    test_trig_func("tg", tg, std::tan);
    test_trig_func("ctg", ctg, [](double x) { return 1.0 / std::tan(x); });
    test_trig_func("sec", sec, [](double x) { return 1.0 / std::cos(x); });
    test_trig_func("cosec", cosec, [](double x) { return 1.0 / std::sin(x); });

    return 0;

    //const bigfloat defaultEXP(1, 1e8);
    //bigfloat a(1, 4);
    //bigfloat b(2, 3);
    //bigfloat c(-12, 4);
    //bigfloat d;
    //bigfloat one(1.0);
    //
    //std::cout << "Values" << std::endl;
    //std::cout << "a = " << a << "\n";
    //std::cout << "b = " << b << "\n";
    //std::cout << "c = " << c << "\n\n";
    //
    ////std::cout << "Enter value (<sign><numerator>/<denominator>) " << std::endl;
    ////std::cin >> d;
    ////std::cout << "Entererd value:" << d << std::endl;
    //
    //std::cout << "Main arithmetic" << std::endl;
    //std::cout << "a + b = " << (a + b) << "\n";
    //std::cout << "a - b = " << (a - b) << "\n";
    //std::cout << "a * b = " << (a * b) << "\n";
    //std::cout << "a * c = " << (a * c) << "\n";
    //std::cout << "a / b = " << (a / b) << "\n\n";
    //
    //std::cout << "Compare functions" << std::endl;
    //std::cout << "a == b: " << (a == b) << "\n";
    //std::cout << "a > b: " << (a > b) << "\n";
    //std::cout << "a <= b: " << (a <= b) << "\n\n";
    //
    //std::cout << "Trigonometry functions:" << std::endl;
    //std::cout << "sin(1/4) = " << sin(a, defaultEXP) << "\n";
    //std::cout << "cos(1/4) = " << cos(a, defaultEXP) << "\n";
    //std::cout << "tg(1/4) = " << tg(a, defaultEXP) << "\n";
    //std::cout << "ctg(1/4) = " << ctg(a, defaultEXP) << "\n";
    //std::cout << "\n";
    //std::cout << "sec(1/4) = " << sec(a, defaultEXP) << "\n";
    //std::cout << "cosec(1/4) = " << cosec(a, defaultEXP) << "\n\n";
    //
    //std::cout << "Inverse trigonometric functions:" << std::endl;
    //std::cout << "arcsin(1/4) = " << arcsin(a, defaultEXP) << "\n";
    //std::cout << "arccos(1/4) = " << arccos(a, defaultEXP) << "\n";
    //std::cout << "arctg(1/4) = " << arctg(a, defaultEXP) << "\n";
    //std::cout << "arcctg(1/4) = " << arcctg(a, defaultEXP) << "\n";
    //
    //std::cout <<  "Inverse trigonometric functions (1):" << std::endl;
    //std::cout << "arcsin(1) = " << arcsin(one, defaultEXP) << "\n"; //Large margin of error (0.14)
    //std::cout << "arccos(1) = " << arccos(one, defaultEXP) << "\n"; //Large margin of error (0.15)
    //std::cout << "arctg(1) = " << arctg(one, defaultEXP) << "\n"; //Margin of error (0.0024)
    //std::cout << "arcctg(1) = " << arcctg(one, defaultEXP) << "\n"; //Margin of error (0.001)
    //
    //std::cout << "Another functions:" << std::endl;
    //std::cout << "(2/3)^3 = " << pow(b, 3) << "\n";
    //std::cout << "(2/3)^-1 = " << pow(b, -1) << "\n";
    //std::cout << "radical(4/9, 2) = " << radical(pow(b, 2), 2, defaultEXP) << "\n";
    //std::cout << "log2(16/1) = " << log2(bigfloat(16.0), defaultEXP) << "\n"; // don't work
    //
    //std::cout << "sin(22/7) = " << sin(bigfloat(1, 2), defaultEXP) << "\n";
    //
    //return 0;
}