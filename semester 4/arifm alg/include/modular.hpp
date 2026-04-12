#ifndef MODULAR_H_
#define MODULAR_H_

#include <iostream>

template<int MOD>
class modular {
private:
    int value;
    int normalize(int v) const {
        v %= MOD;
        if (v < 0) v += MOD;
        return v;
    }
public:
    modular(int v = 0) : value(normalize(v)) {}
    modular(long long v) : value(normalize(static_cast<int>(v % MOD))) {}
    
    int getValue() const { return value; }
    
    modular operator+(const modular& other) const { return modular(value + other.value); }
    modular operator-(const modular& other) const { return modular(value - other.value); }
    modular operator*(const modular& other) const { return modular(static_cast<long long>(value) * other.value); }
    modular operator-() const { return modular(-value); }
    modular& operator+=(const modular& other) { value = normalize(value + other.value); return *this; }
    modular& operator-=(const modular& other) { value = normalize(value - other.value); return *this; }
    modular& operator*=(const modular& other) { value = normalize(static_cast<long long>(value) * other.value); return *this; }
    modular operator/(const modular& other) const {
        int inv = 1, a = other.value, b = MOD, x0 = 1, x1 = 0;
        while (b) {
            int q = a / b;
            int tmp = a % b; a = b; b = tmp;
            tmp = x0 - q * x1; x0 = x1; x1 = tmp;
        }
        inv = x0;
        if (inv < 0) inv += MOD;
        return modular(static_cast<long long>(value) * inv % MOD);
    }
    
    bool operator==(const modular& other) const { return value == other.value; }
    bool operator!=(const modular& other) const { return value != other.value; }
    bool operator<(const modular& other) const { return value < other.value; }
    bool operator>(const modular& other) const { return value > other.value; }
    
    friend std::ostream& operator<<(std::ostream& os, const modular& m) {
        os << m.value;
        return os;
    }
};

#endif