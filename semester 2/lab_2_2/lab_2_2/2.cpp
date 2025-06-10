#include <iostream>
#include "../libs/bigfloat.h"

#include <iomanip>
#include <cmath>

const double PI_D = 3.14159265358979323846;
const bigfloat EPS = bigfloat(1, 1e10);

void test_trig_func(
    const std::string& name,
    bigfloat(*bf_func)(bigfloat const&, bigfloat const&),
    double(*std_func)(double))
{
    std::cout << "=== Testing " << name << " ===\n";

    bigfloat PI = bigfloat::PI();

    for (int i = -8; i <= 8; ++i)
    {
        bigfloat x_bf = PI * bigfloat(i) / 4;
        double x_d = PI_D * i / 4;

        bigfloat result;
        try
        {
            result = bf_func(x_bf, EPS);
        }
        catch (const std::exception&)
        {
            std::cout << "Argument (" << x_d << ") did't match with " << name << "\n";
            continue;
        }

        
        double expected = std_func(x_d);
        double d_result = to_double(result);
        double error = abs(d_result - expected);

        std::cout << std::fixed << std::setprecision(8)
            << name << "(" << x_d << ") = " << d_result << " | expected: " << expected << " | error: " <<  ((error > 1e-10) ? error : 0) << std::endl;
    }
    std::cout << '\n';
}

void test_arctg_value(const bigfloat& x, const bigfloat& eps, const std::string& label)
{
    bigfloat result = arctg(x, eps);
    double actual = to_double(result);
    double expected = std::atan(to_double(x));
    double delta = std::abs(actual - expected);

    std::cout << label << "\n";
    std::cout << "x = " << to_double(x) << "\n";
    std::cout << "arctg(x) = " << std::setprecision(20) << actual << "\n";
    std::cout << "std::atan(x) = " << std::setprecision(20) << expected << "\n";
    std::cout << "delta = " << std::setprecision(20) << ((delta > 1e-8) ? delta : 0) << "\n\n";
}

void test_log_function(
    const std::string& name,
    bigfloat(*log_fn)(const bigfloat&, const bigfloat&),
    double(*ref_fn)(double),
    const std::vector<double>& inputs,
    const bigfloat& eps)
{
    std::cout << "Testing: " << name << "\n";

    for (double d : inputs)
    {
        if (d <= 0) {
            std::cout << "  Skipping invalid input: " << d << " (log undefined)\n";
            continue;
        }

        bigfloat x(d);
        bigfloat result = log_fn(x, eps);
        double reference = ref_fn(d);
        double approx = to_double(result);
        double error = std::abs(approx - reference);

        std::cout << std::fixed << std::setprecision(10);
        std::cout << "  x = " << d
            << ", log = " << approx
            << ", expected = " << reference
            << ", error = " << error << "\n";
    }

    std::cout << "\n";
}


int program_2_main(int argc, char* argv[])
{
    try
    {

        const bigfloat defaultEXP(1, 1e10);

#pragma region testing log

        //std::vector<double> test_values = {
        //    0.0001, 0.01, 0.1,
        //    0.5, 1.0, 1.5, 2.0, 2.718281828,
        //    10.0, 100.0, 1024.0, 1e6
        //};
        //
        //test_log_function("ln", ln, std::log, test_values, defaultEXP);
        //test_log_function("log2", log2, std::log2, test_values, defaultEXP);
        //test_log_function("log10", log10, std::log10, test_values, defaultEXP);

#pragma endregion

#pragma region testing trigonometry

        //test_arctg_value(bigfloat(0), defaultEXP, "Test 1: arctg(0)");
        //test_arctg_value(bigfloat(1), defaultEXP, "Test 2: arctg(1) (should be pi/4)");
        //test_arctg_value(bigfloat(-1), defaultEXP, "Test 3: arctg(-1) (should be -pi/4)");
        //test_arctg_value(bigfloat(10), defaultEXP, "Test 4: arctg(10)");
        //test_arctg_value(bigfloat(-10), defaultEXP, "Test 5: arctg(-10)");
        //test_arctg_value(bigfloat(0.5), defaultEXP, "Test 6: arctg(0.5)");
        //test_arctg_value(bigfloat(-0.5), defaultEXP, "Test 7: arctg(-0.5)");
        //test_arctg_value(bigfloat(1000000), defaultEXP, "Test 8: arctg(1e6) (should be close to pi/2)");
        //test_arctg_value(bigfloat(-1000000), defaultEXP, "Test 9: arctg(-1e6) (should be close to -pi/2)");

        //std::cout << "arcsin(1/2) = " << (to_double(arcsin(bigfloat(1, 2), defaultEXP) * bigfloat(6))) << "\n";
        //std::cout << "sin(1e10) = " << to_double(sin(bigfloat(10000000000, 1), defaultEXP)) << "\n";

        //test_trig_func("sin", sin, std::sin);
        //test_trig_func("cos", cos, std::cos);
        //test_trig_func("tg", tg, std::tan); //TODO: make it better
        //test_trig_func("ctg", ctg, [](double x) { return 1.0 / std::tan(x); });
        //test_trig_func("sec", sec, [](double x) { return 1.0 / std::cos(x); });
        //test_trig_func("cosec", cosec, [](double x) { return 1.0 / std::sin(x); });
        //test_trig_func("arcsin", arcsin, std::asin);
        //test_trig_func("arccos", arccos, std::acos);
        //test_trig_func("arctg", arctg, std::atan);
        //test_trig_func("arcctg", arcctg, [](double x) { return std::atan(1.0 / x); });

#pragma endregion

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
        //std::cout << "radical(4/9, 2) = " << to_double(radical(pow(b, 2), 2, defaultEXP)) << "\n";
        //std::cout << "log2(16/1) = " << to_double(log2(bigfloat(16.0), defaultEXP)) << "\n"; // don't work
    }
    catch (const std::exception& e)
    {
        std::cout << "Exception: " << e.what();
    }
    return 0;
}