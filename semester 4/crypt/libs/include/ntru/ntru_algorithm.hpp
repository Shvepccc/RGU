#ifndef NTRU_ENCRYPT_H_
#define NTRU_ENCRYPT_H_

#include <vector>
#include <random>
#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <cstdint>
#include <iostream>

class ntru_poly
{
public:
    std::size_t n;
    std::vector<int64_t> coeffs;

    ntru_poly(std::size_t degree) : n(degree), coeffs(degree, 0) {}

    static ntru_poly multiply(const ntru_poly& a, const ntru_poly& b, int64_t mod)
    {
        std::size_t n = a.n;
        ntru_poly result(n);
        for (std::size_t i = 0; i < n; ++i) {
            if (a.coeffs[i] == 0) continue;
            for (std::size_t j = 0; j < n; ++j) {
                if (b.coeffs[j] == 0) continue;
                std::size_t k = (i + j) % n;
                result.coeffs[k] = (result.coeffs[k] + a.coeffs[i] * b.coeffs[j]) % mod;
            }
        }
        for (auto& c : result.coeffs) {
            if (c < 0) c += mod;
        }
        return result;
    }

    void mod_poly(int64_t mod)
    {
        for (auto& c : coeffs) {
            c %= mod;
            if (c < 0) c += mod;
        }
    }

    void center_lift(int64_t mod)
    {
        int64_t half = mod / 2;
        for (auto& c : coeffs) {
            c %= mod;
            if (c < 0) c += mod;
            if (c > half) c -= mod;
        }
    }
};

class ntru_cipher
{
private:
    std::size_t n;
    int64_t p;
    int64_t q;
    int df, dg;
    
    ntru_poly h; 
    ntru_poly f; 
    ntru_poly fp;
    ntru_poly fq;
    
    std::mt19937_64 gen;

    static int64_t mod_inverse(int64_t a, int64_t m) {
        a = a % m;
        for (int64_t x = 1; x < m; x++)
            if ((a * x) % m == 1) return x;
        return 1;
    }

    bool invert_modp(const ntru_poly& poly, ntru_poly& inv, int64_t mod)
    {
        std::size_t N = poly.n;
        std::vector<std::vector<int64_t>> mat(N, std::vector<int64_t>(2 * N, 0));
        
        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < N; ++j) {
                mat[i][j] = poly.coeffs[(N + i - j) % N] % mod;
                if (mat[i][j] < 0) mat[i][j] += mod;
            }
            mat[i][N + i] = 1;
        }

        for (std::size_t i = 0; i < N; ++i) {
            if (mat[i][i] == 0) {
                std::size_t swap_row = i;
                for (std::size_t k = i + 1; k < N; ++k) {
                    if (mat[k][i] != 0) { swap_row = k; break; }
                }
                if (swap_row == i) return false;
                std::swap(mat[i], mat[swap_row]);
            }
            
            int64_t inv_val = mod_inverse(mat[i][i], mod);
            for (std::size_t j = i; j < 2 * N; ++j) {
                mat[i][j] = (mat[i][j] * inv_val) % mod;
            }
            
            for (std::size_t k = 0; k < N; ++k) {
                if (k != i && mat[k][i] != 0) {
                    int64_t factor = mat[k][i];
                    for (std::size_t j = i; j < 2 * N; ++j) {
                        mat[k][j] = (mat[k][j] - factor * mat[i][j]) % mod;
                        if (mat[k][j] < 0) mat[k][j] += mod;
                    }
                }
            }
        }
        for (std::size_t i = 0; i < N; ++i) inv.coeffs[i] = mat[i][N];
        return true;
    }

    bool invert_modq(const ntru_poly& poly, ntru_poly& inv, int64_t q_target)
    {
        if (!invert_modp(poly, inv, 2)) return false;
        
        int64_t current_q = 2;
        while (current_q < q_target) {
            current_q *= 2;
            ntru_poly temp = ntru_poly::multiply(poly, inv, current_q);
            for (auto& c : temp.coeffs) {
                c = (2 - c) % current_q;
                if (c < 0) c += current_q;
            }
            inv = ntru_poly::multiply(inv, temp, current_q);
        }
        inv.mod_poly(q_target);
        return true;
    }

    ntru_poly generate_ternary_poly(int num_ones, int num_minus_ones)
    {
        ntru_poly res(n);
        std::vector<int> positions(n);
        std::iota(positions.begin(), positions.end(), 0);
        std::shuffle(positions.begin(), positions.end(), gen);

        for (int i = 0; i < num_ones; ++i) res.coeffs[positions[i]] = 1;
        for (int i = num_ones; i < num_ones + num_minus_ones; ++i) res.coeffs[positions[i]] = -1;
        return res;
    }

public:
    ntru_cipher(std::size_t n_val = 107, int64_t p_val = 3, int64_t q_val = 128, int d_val = 15)
        : n(n_val), p(p_val), q(q_val), df(d_val), dg(d_val),
          h(n_val), f(n_val), fp(n_val), fq(n_val), gen(std::random_device{}()) {}

    void generate_keys()
    {
        while (true) {
            f = generate_ternary_poly(df, df - 1);
            if (!invert_modp(f, fp, p)) continue;
            if (!invert_modq(f, fq, q)) continue;
            
            ntru_poly g = generate_ternary_poly(dg, dg);
            h = ntru_poly::multiply(fq, g, q);
            for (auto& c : h.coeffs) c = (c * p) % q;
            break;
        }
    }

    ntru_poly encrypt_block(const ntru_poly& m)
    {
        ntru_poly r = generate_ternary_poly(dg, dg);
        ntru_poly e = ntru_poly::multiply(r, h, q);
        for (std::size_t i = 0; i < n; ++i) {
            e.coeffs[i] = (e.coeffs[i] + m.coeffs[i]) % q;
            if (e.coeffs[i] < 0) e.coeffs[i] += q;
        }
        return e;
    }

    ntru_poly decrypt_block(const ntru_poly& e)
    {
        ntru_poly a = ntru_poly::multiply(f, e, q);
        a.center_lift(q);
        ntru_poly m = ntru_poly::multiply(a, fp, p);
        m.mod_poly(p);
        return m;
    }

    void encrypt_file(const std::string& in_path, const std::string& out_path)
    {
        std::ifstream in(in_path, std::ios::binary);
        if (!in) throw std::runtime_error("Cannot open input file");
        std::vector<uint8_t> data((std::istreambuf_iterator<char>(in)), {});
        in.close();

        std::vector<int> bits;
        for (uint8_t b : data) {
            for (int i = 0; i < 8; ++i) bits.push_back((b >> i) & 1);
        }

        std::ofstream out(out_path, std::ios::binary);
        if (!out) throw std::runtime_error("Cannot open output file");
        
        uint64_t data_size = data.size();
        out.write(reinterpret_cast<const char*>(&data_size), sizeof(data_size));

        for (std::size_t i = 0; i < bits.size(); i += n) {
            ntru_poly m(n);
            for (std::size_t j = 0; j < n && i + j < bits.size(); ++j) {
                m.coeffs[j] = bits[i + j];
            }
            ntru_poly e = encrypt_block(m);
            for (std::size_t j = 0; j < n; ++j) {
                int8_t val = static_cast<int8_t>(e.coeffs[j]);
                out.write(reinterpret_cast<const char*>(&val), 1);
            }
        }
    }

    void decrypt_file(const std::string& in_path, const std::string& out_path)
    {
        std::ifstream in(in_path, std::ios::binary);
        if (!in) throw std::runtime_error("Cannot open input file");

        uint64_t data_size;
        in.read(reinterpret_cast<char*>(&data_size), sizeof(data_size));

        std::vector<int> bits;
        while (in.peek() != EOF) {
            ntru_poly e(n);
            bool read_success = false;
            for (std::size_t j = 0; j < n; ++j) {
                int8_t val = 0;
                if (in.read(reinterpret_cast<char*>(&val), 1)) {
                    e.coeffs[j] = val;
                    if (e.coeffs[j] < 0) e.coeffs[j] += q;
                    read_success = true;
                }
            }
            if (!read_success) break;

            ntru_poly m = decrypt_block(e);
            for (std::size_t j = 0; j < n; ++j) {
                bits.push_back(m.coeffs[j]);
            }
        }

        std::ofstream out(out_path, std::ios::binary);
        if (!out) throw std::runtime_error("Cannot open output file");

        std::size_t total_bytes = 0;
        for (std::size_t i = 0; i < bits.size() && total_bytes < data_size; i += 8) {
            uint8_t b = 0;
            for (int j = 0; j < 8 && i + j < bits.size(); ++j) {
                b |= (bits[i + j] << j);
            }
            out.write(reinterpret_cast<const char*>(&b), 1);
            total_bytes++;
        }
    }
};

#endif