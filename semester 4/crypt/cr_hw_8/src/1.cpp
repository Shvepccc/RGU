#include <iostream>
#include "../include/poly_gf2.hpp"
#include <cassert>

std::string to_string(const poly_gf2& p) {
    uint64_t val = p.get_value();
    if (val == 0) return "0";
    std::string s;
    bool first = true;
    for (int i = 63; i >= 0; --i) {
        if ((val >> i) & 1) {
            if (!first) s += " + ";
            if (i == 0) s += "1";
            else if (i == 1) s += "x";
            else s += "x^" + std::to_string(i);
            first = false;
        }
    }
    return s;
}

void test_basic() {
    std::cout << "test_basic...\n";
    poly_gf2 p0(0);
    assert(p0.get_value() == 0);
    assert(p0.add(p0).get_value() == 0);

    poly_gf2 p1(1);
    poly_gf2 px(2);
    poly_gf2 px1(3);
    assert(p1.get_value() == 1);
    assert(px.get_value() == 2);
    assert(px1.get_value() == 3);
    std::cout << "  OK\n";
}

void test_add() {
    std::cout << "test_add...\n";
    poly_gf2 a(0b101);
    poly_gf2 b(0b110);
    poly_gf2 c = a.add(b);
    assert(c.get_value() == 3);
    assert(a.add(poly_gf2(0)).get_value() == a.get_value());
    assert(b.add(a).get_value() == c.get_value());
    std::cout << "  OK\n";
}

void test_mul_mod() {
    std::cout << "test_mul_mod...\n";
    poly_gf2 px1(3);
    poly_gf2 mod(0b111);
    poly_gf2 prod = px1.mul_mod(px1, mod);
    assert(prod.get_value() == 2);

    poly_gf2 one(1);
    prod = px1.mul_mod(one, mod);
    assert(prod.get_value() == px1.get_value());

    poly_gf2 zero(0);
    prod = px1.mul_mod(zero, mod);
    assert(prod.get_value() == 0);

    poly_gf2 mod1(1);
    prod = px1.mul_mod(px1, mod1);
    assert(prod.get_value() == 0);

    poly_gf2 mod0(0);
    prod = px1.mul_mod(px1, mod0);
    assert(prod.get_value() == 0);

    poly_gf2 a(0b101011);
    poly_gf2 b(0b11001);
    poly_gf2 m(0b1000111);
    poly_gf2 ab1 = a.mul_mod(b, m);
    poly_gf2 a_mod = a.mul_mod(poly_gf2(1), m);
    poly_gf2 b_mod = b.mul_mod(poly_gf2(1), m);
    poly_gf2 ab2 = a_mod.mul_mod(b_mod, m);
    assert(ab1.get_value() == ab2.get_value());

    std::cout << "  OK\n";
}

void test_inverse() {
    std::cout << "test_inverse...\n";
    poly_gf2 mod(0b111);
    poly_gf2 x(2);
    poly_gf2 inv = x.inverse(mod);
    poly_gf2 check = x.mul_mod(inv, mod);
    assert(check.get_value() == 1);

    poly_gf2 x1(3);
    inv = x1.inverse(mod);
    check = x1.mul_mod(inv, mod);
    assert(check.get_value() == 1);

    poly_gf2 mod16(0b10011);
    poly_gf2 a(0b1010);
    inv = a.inverse(mod16);
    check = a.mul_mod(inv, mod16);
    assert(check.get_value() == 1);

    poly_gf2 zero(0);
    inv = zero.inverse(mod16);
    assert(inv.get_value() == 0);

    poly_gf2 bad_mod(0b101);
    poly_gf2 a2(0b11);
    inv = a2.inverse(bad_mod);
    assert(inv.get_value() == 0);

    poly_gf2 mod1(1);
    inv = a.inverse(mod1);
    assert(inv.get_value() == 0);

    std::cout << "  OK\n";
}

void test_irreducible() {
    std::cout << "test_irreducible...\n";
    assert(poly_gf2(2).is_irreducible());        // x
    assert(poly_gf2(3).is_irreducible());        // x+1
    assert(poly_gf2(0b111).is_irreducible());    // x^2+x+1
    assert(poly_gf2(0b1011).is_irreducible());   // x^3+x+1
    assert(poly_gf2(0b1101).is_irreducible());   // x^3+x^2+1
    assert(poly_gf2(0b10011).is_irreducible());  // x^4+x+1
    assert(poly_gf2(0b11001).is_irreducible());  // x^4+x^3+1
    assert(!poly_gf2(0).is_irreducible());
    assert(!poly_gf2(1).is_irreducible());
    assert(!poly_gf2(0b101).is_irreducible());   // x^2+1 = (x+1)^2
    assert(!poly_gf2(0b1001).is_irreducible());  // x^3+1 = (x+1)(x^2+x+1)
    assert(!poly_gf2(0b1111).is_irreducible());  // (x+1)^3
    assert(!poly_gf2(0b10101).is_irreducible()); // (x^2+x+1)^2
    std::cout << "  OK\n";
}



void test_gcd() {
    std::cout << "test_gcd...\n";
    poly_gf2 mod(0b111);
    poly_gf2 a(0b10);
    poly_gf2 b(0b11);
    poly_gf2 inv = a.inverse(b);
    assert(inv.get_value() != 0);
    inv = a.inverse(a);
    assert(inv.get_value() == 0);
    std::cout << "  OK\n";
}

void test_edge_cases() {
    std::cout << "test_edge_cases...\n";
    uint64_t big = 1ULL << 62;
    poly_gf2 big_poly(big);
    poly_gf2 mod(0b1000000000000000000000000000000000000000000000000000000000000111ULL);
    poly_gf2 inv = big_poly.inverse(mod);
    poly_gf2 check = big_poly.mul_mod(inv, mod);
    assert(check.get_value() == 1);

    uint64_t mod63 = (1ULL << 63) | 0b101;
    poly_gf2 p(0b1001);
    poly_gf2 m63(mod63);
    poly_gf2 p_inv = p.inverse(m63);
    if (p_inv.get_value() != 0) {
        poly_gf2 prod = p.mul_mod(p_inv, m63);
        assert(prod.get_value() == 1);
    }

    poly_gf2 a(0b101);
    poly_gf2 b(0b111);
    poly_gf2 m(0b101);
    poly_gf2 prod = a.mul_mod(b, m);
    assert(prod.get_value() == 0);

    std::cout << "  OK\n";
}

int main() {
    std::cout << "=== Testing poly_gf2 class ===\n";
    test_basic();
    test_add();
    test_mul_mod();
    test_inverse();
    test_irreducible();
    test_gcd();
    test_edge_cases();
    std::cout << "All tests passed!\n";
    return 0;
}