#ifndef POLYNOMIAL_UTILS_H_
#define POLYNOMIAL_UTILS_H_

#include <iostream>
#include <complex>

namespace polynomial_utils {
    
    template<typename T>
    bool isZero(const T& v) { return v == T{}; }
    
    template<typename U>
    bool isZero(const std::complex<U>& v) { return v == std::complex<U>{}; }
    
    template<typename T>
    bool isOne(const T& v) { return v == T{1}; }
    
    template<typename U>
    bool isOne(const std::complex<U>& v) { return v == std::complex<U>{1, 0}; }
    
    template<typename T>
    bool isNegative(const T& v) { return v < T{}; }
    
    template<typename U>
    bool isNegative(const std::complex<U>& v) { return v.real() < 0; }
    
    template<typename T>
    T absValue(const T& v) { return v < T{} ? -v : v; }
    
    template<typename U>
    std::complex<U> absValue(const std::complex<U>& v) { return v; }
    
    template<typename T>
    T power(const T& base, int exp) {
        if (exp == 0) return T{1};
        T res = base;
        for (int i = 1; i < exp; ++i) res = res * base;
        return res;
    }
    
    template<typename U>
    std::complex<U> power(const std::complex<U>& base, int exp) {
        if (exp == 0) return std::complex<U>{1, 0};
        std::complex<U> res = base;
        for (int i = 1; i < exp; ++i) res = res * base;
        return res;
    }
    
    template<typename T>
    void printCoeff(std::ostream& os, const T& coeff, bool needCoeff) {
        if (needCoeff) os << coeff;
    }
    
    template<typename U>
    void printCoeff(std::ostream& os, const std::complex<U>& coeff, bool needCoeff) {
        if (needCoeff) {
            os << coeff;
        }
    }
}

#endif