#ifndef ELGAMAL_SIGNER_H_
#define ELGAMAL_SIGNER_H_

#include <fstream>
#include <vector>
#include <cstring>
#include <random>
#include <boost/multiprecision/cpp_int.hpp>
#include "../include/modular_arithmetic.hpp"
#include "../include/diffie_hellman.hpp"

struct elgamal_keys
{
    bigint p;
    bigint g;
    bigint y;
    bigint x;
};

struct elgamal_signature
{
    bigint r;
    bigint s;
};

class elgamal_signer
{
private:
    bigint p;
    bigint g;
    bigint x;
    bigint y;

    void validate_private_key(const bigint& private_key, const bigint& prime) const
    {
        if (private_key < 2)
        {
            throw std::invalid_argument("Закрытый ключ x должен быть >= 2");
        }
        if (private_key >= prime - 1)
        {
            throw std::invalid_argument("Закрытый ключ x должен быть <= p-2");
        }
    }

    bigint generate_k(const bigint& p_minus_1) const
    {
        std::random_device rd;
        std::mt19937_64 gen(rd());
        
        while (true)
        {
            bigint k = 0;
            for (int i = 0; i < 4; ++i)
            {
                k = (k << 64) | gen();
            }
            k = k % (p_minus_1 - 1) + 1;
            
            if (gcd(k, p_minus_1) == 1)
            {
                return k;
            }
        }
    }

    bigint hash_file(const std::vector<uint8_t>& data) const
    {
        bigint hash = 0;
        for (size_t i = 0; i < data.size(); ++i)
        {
            hash = (hash << 8) | data[i];
            hash = hash % p;
        }
        return hash;
    }

public:
    elgamal_signer(const bigint& private_key)
        : x(private_key)
    {
        DHParameters params = generate_parameters(512);
        p = params.p;
        g = params.g;
        validate_private_key(x, p);
        y = mod_pow(g, x, p);
    }

    elgamal_keys get_keys() const
    {
        return {p, g, y, x};
    }

    elgamal_signature sign(const std::vector<uint8_t>& data) const
    {
        bigint p_minus_1 = p - 1;
        bigint h = hash_file(data);
        
        while (true)
        {
            bigint k = generate_k(p_minus_1);
            bigint r = mod_pow(g, k, p);
            
            if (r == 0)
            {
                continue;
            }
            
            bigint k_inv = mod_inverse(k, p_minus_1);
            if (k_inv == 0)
            {
                continue;
            }
            
            bigint s = (k_inv * (h - x * r)) % p_minus_1;
            if (s < 0)
            {
                s += p_minus_1;
            }
            
            if (s != 0)
            {
                return {r, s};
            }
        }
    }

    bool verify(const std::vector<uint8_t>& data, const elgamal_signature& sig) const
    {
        if (sig.r <= 0 || sig.r >= p || sig.s <= 0 || sig.s >= p - 1)
        {
            return false;
        }
        
        bigint h = hash_file(data);
        
        bigint lhs = mod_pow(g, h, p);
        bigint rhs = (mod_pow(y, sig.r, p) * mod_pow(sig.r, sig.s, p)) % p;
        
        return lhs == rhs;
    }

    void save_signature(const std::string& filename, const elgamal_signature& sig) const
    {
        std::ofstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Не удалось открыть файл для сохранения подписи");
        }
        file << sig.r << std::endl;
        file << sig.s << std::endl;
    }

    elgamal_signature load_signature(const std::string& filename) const
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Не удалось открыть файл для чтения подписи");
        }
        elgamal_signature sig;
        std::string r_str, s_str;
        std::getline(file, r_str);
        std::getline(file, s_str);
        sig.r = bigint(r_str);
        sig.s = bigint(s_str);
        return sig;
    }

    void save_keys(const std::string& filename) const
    {
        std::ofstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Не удалось открыть файл для сохранения ключей");
        }
        file << p << std::endl;
        file << g << std::endl;
        file << y << std::endl;
        file << x << std::endl;
    }

    void load_keys(const std::string& filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Не удалось открыть файл для чтения ключей");
        }
        std::string p_str, g_str, y_str, x_str;
        std::getline(file, p_str);
        std::getline(file, g_str);
        std::getline(file, y_str);
        std::getline(file, x_str);
        p = bigint(p_str);
        g = bigint(g_str);
        y = bigint(y_str);
        x = bigint(x_str);
        validate_private_key(x, p);
    }
};

std::vector<uint8_t> read_file(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Не удалось открыть файл: " + filename);
    }
    
    std::vector<uint8_t> data;
    data.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    return data;
}

#endif