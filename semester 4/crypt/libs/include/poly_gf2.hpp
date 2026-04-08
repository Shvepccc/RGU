#ifndef POLY_GF_H_
#define POLY_GF_H_

#include <cstdint>
#include <algorithm>

class poly_gf2 {
private:
    std::uint64_t data;   // всегда приведён по модулю

public:
    poly_gf2(std::uint64_t value = 0) : data(value) {}

    static int get_degree128(__uint128_t p) {
        if (p == 0) return -1;
        int deg = 0;
        while (p >>= 1) ++deg;
        return deg;
    }

    static __uint128_t poly_mod128(__uint128_t a, __uint128_t mod) {
        if (mod == 0) return 0;
        int deg_mod = get_degree128(mod);
        int deg_a = get_degree128(a);
        while (deg_a >= deg_mod) {
            a ^= (mod << (deg_a - deg_mod));
            deg_a = get_degree128(a);
        }
        return a;
    }

    static __uint128_t poly_gcd128(__uint128_t a, __uint128_t b) {
        while (b != 0) {
            a = poly_mod128(a, b);
            std::swap(a, b);
        }
        return a;
    }

    std::uint64_t get_value() const { return data; }

    poly_gf2 add(const poly_gf2& other) const {
        return poly_gf2(data ^ other.data);
    }

    poly_gf2 mul_mod(const poly_gf2& other, const poly_gf2& mod) const {
        if (mod.data == 0) return poly_gf2(0);
        int deg_mod = get_degree128(mod.data);
        if (deg_mod == 0) return poly_gf2(0);

        __uint128_t a = poly_mod128(this->data, mod.data);
        __uint128_t b = other.data;
        __uint128_t res = 0;

        for (int i = 0; i <= get_degree128(b); ++i) {
            if ((b >> i) & 1)
                res ^= a;
            bool overflow = (a >> (deg_mod - 1)) & 1;
            a <<= 1;
            if (overflow)
                a ^= mod.data;
        }
        return poly_gf2(static_cast<uint64_t>(poly_mod128(res, mod.data)));
    }

    poly_gf2 inverse(const poly_gf2& mod) const {
        if (data == 0 || mod.data == 0) return poly_gf2(0);

        __uint128_t u = data;
        __uint128_t v = mod.data;
        __uint128_t g1 = 1;
        __uint128_t g2 = 0;

        while (u != 0) {
            int deg_u = get_degree128(u);
            int deg_v = get_degree128(v);
            if (deg_v == -1) break;

            if (deg_u < deg_v) {
                std::swap(u, v);
                std::swap(g1, g2);
                std::swap(deg_u, deg_v);
            }

            int shift = deg_u - deg_v;
            u ^= (v << shift);
            g1 ^= (g2 << shift);

            g1 = poly_mod128(g1, mod.data);
            g2 = poly_mod128(g2, mod.data);
        }

        if (v != 1) return poly_gf2(0);
        return poly_gf2(static_cast<uint64_t>(poly_mod128(g2, mod.data)));
    }

    bool is_irreducible() const {
        int n = get_degree128(data);
        if (n <= 0) return false;
        if (n == 1) return true;

        poly_gf2 p = *this;
        poly_gf2 x(2);
        poly_gf2 x_mod = poly_gf2(static_cast<uint64_t>(poly_mod128(x.get_value(), data)));

        poly_gf2 x_pow = x_mod;
        for (int i = 0; i < n; ++i)
            x_pow = x_pow.mul_mod(x_pow, p);
        if (!(x_pow.add(x_mod).get_value() == 0))
            return false;

        int m = n;
        for (int d = 2; d * d <= m; ++d) {
            if (m % d == 0) {
                int exp = n / d;
                poly_gf2 h = x_mod;
                for (int i = 0; i < exp; ++i)
                    h = h.mul_mod(h, p);
                h = h.add(x_mod);
                __uint128_t g = poly_gcd128(data, h.get_value());
                if (g != 1)
                    return false;
                while (m % d == 0) m /= d;
            }
        }
        if (m > 1) {
            int exp = n / m;
            poly_gf2 h = x_mod;
            for (int i = 0; i < exp; ++i)
                h = h.mul_mod(h, p);
            h = h.add(x_mod);
            __uint128_t g = poly_gcd128(data, h.get_value());
            if (g != 1)
                return false;
        }
        return true;
    }
};

#endif