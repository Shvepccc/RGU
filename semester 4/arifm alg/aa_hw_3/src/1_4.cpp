#include <iostream>
#include <functional>
#include <cmath>
#include <iomanip>
#include <string>
#include <limits>
#include <sstream>

//TODO: check this number

class FunctionA {
    double c, a, b;
public:
    FunctionA(double c_, double a_, double b_) : c(c_), a(a_), b(b_) {}
    double operator()(double x) const { return c + a * sin(x)*sin(x) + b * cos(x)*cos(x); }
    double derivative(double x) const { return (a - b) * sin(2*x); }
    bool converges() const { return std::fabs(a - b) < 1.0; }
    std::string info() const { 
        return "|a-b| = " + std::to_string(std::fabs(a-b)) + (converges() ? " < 1 => СХОДИТСЯ" : " >= 1 => НЕ СХОДИТСЯ");
    }
};

class FunctionB {
    double c, a, b;
public:
    FunctionB(double c_, double a_, double b_) : c(c_), a(a_), b(b_) {}
    double operator()(double x) const { return c + a * exp(-b * x * x); }
    double derivative(double x) const { return -2 * a * b * x * exp(-b * x * x); }
    
    double maxDerivative() const { 
        if (b <= 0) return std::numeric_limits<double>::infinity();
        return std::fabs(a) * 2 * b * (1.0 / sqrt(2 * b * exp(1)));
    }
    
    bool converges() const { 
        if (b <= 0) return false;
        return maxDerivative() < 1.0; 
    }
    
    std::string info() const {
        if (b <= 0) return "b <= 0 => НЕ СХОДИТСЯ";
        double maxd = maxDerivative();
        std::stringstream ss;
        ss << "max|φ'| = " << std::fixed << std::setprecision(4) << maxd;
        ss << (maxd < 1.0 ? " < 1 => СХОДИТСЯ" : " >= 1 => НЕ СХОДИТСЯ");
        return ss.str();
    }
};

void demo(std::function<double(double)> f, double x0, int n) {
    double x = x0;
    for (int i = 1; i <= n; i++) {
        double xn = f(x);
        std::cout << "x" << i << " = " << std::fixed << std::setprecision(4) << xn;
        std::cout << " | delta = " << std::fabs(xn - x) << "\n";
        x = xn;
    }
}

int main() {
    //system("chcp 65001 > nul");
    
    std::cout << "1. φ(x) = c + a*sin^2(x) + b*cos^2(x)\n";
    std::cout << "--------------------------------\n";
    
    FunctionA f1(0, 2, 1.5);
    std::cout << "c=0, a=2, b=1.5: " << f1.info() << "\n";
    demo(f1, 1.0, 5);
    
    FunctionA f2(1, 5, 2);
    std::cout << "\nc=1, a=5, b=2: " << f2.info() << "\n";
    demo(f2, 1.0, 5);
    
    std::cout << "\n2. φ(x) = c + a·e^(-b·x^2)\n";
    std::cout << "---------------------------\n";
    
    FunctionB f3(0, 1.5, 0.5);
    std::cout << "c=0, a=1.5, b=0.5: " << f3.info() << "\n";
    demo(f3, 1.0, 10);
    
    FunctionB f4(0, 2.0, 0.5); 
    std::cout << "\nc=0, a=2.0, b=0.5: " << f4.info() << "\n";
    demo(f4, 1.0, 10);
    
    FunctionB f5(0, 1, -0.1);
    std::cout << "\nc=0, a=1, b=-0.1: " << f5.info() << "\n";
    demo(f5, 0.5, 10);
    
    return 0;
}