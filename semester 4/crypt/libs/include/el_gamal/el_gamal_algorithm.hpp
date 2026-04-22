#ifndef EL_GAMAL_H_
#define EL_GAMAL_H_

#include <random>
#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <boost/random.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include "../modular_arithmetic.hpp"
#include "../probability_test/probabilistic_primality_tests_realisations.hpp"

struct el_gamal_public_key
{
    bigint p;
    bigint g;
    bigint y;
};

struct el_gamal_private_key
{
    bigint x;
};

struct el_gamal_ciphertext
{
    bigint a;
    bigint b;
};

class el_gamal
{
private:
    static std::size_t calculate_block_size(const bigint& p)
    {
        std::size_t bits = boost::multiprecision::msb(p) + 1;
        return (bits - 1) / 8;
    }

public:
    static void generate_keys(bigint min_p, bigint max_p, el_gamal_public_key& pub, el_gamal_private_key& priv)
    {
        std::random_device rd;
        boost::random::mt19937 gen(rd());
        miller_rabin_test tester;

        bigint p;
        while (true)
        {
            boost_dist dist(min_p, max_p);
            p = dist(gen);
            if (p % 2 != 0 && tester.is_prime(p, 0.999))
            {
                break;
            }
        }

        boost_dist g_dist(2, p - 1);
        bigint g = g_dist(gen);

        boost_dist x_dist(1, p - 2);
        bigint x = x_dist(gen);

        bigint y = mod_pow(g, x, p);

        pub.p = p;
        pub.g = g;
        pub.y = y;
        priv.x = x;
    }

    static el_gamal_ciphertext encrypt(const bigint& message, const el_gamal_public_key& pub)
    {
        std::random_device rd;
        boost::random::mt19937 gen(rd());
        boost_dist k_dist(1, pub.p - 2);
        
        bigint k = k_dist(gen);
        bigint a = mod_pow(pub.g, k, pub.p);
        bigint s = mod_pow(pub.y, k, pub.p);
        bigint b = (message % pub.p * s) % pub.p;

        return {a, b};
    }

    static bigint decrypt(const el_gamal_ciphertext& cipher, const el_gamal_public_key& pub, const el_gamal_private_key& priv)
    {
        bigint s = mod_pow(cipher.a, priv.x, pub.p);
        bigint s_inv = mod_inverse(s, pub.p);
        
        if (s_inv == 0)
        {
            throw std::runtime_error("Modular inverse does not exist");
        }

        return (cipher.b * s_inv) % pub.p;
    }

    static void encrypt_file(const std::string& input_path, const std::string& output_path, const el_gamal_public_key& pub)
    {
        std::ifstream in(input_path, std::ios::binary);
        if (!in)
        {
            throw std::runtime_error("Cannot open input file");
        }

        std::ofstream out(output_path, std::ios::binary);
        if (!out)
        {
            throw std::runtime_error("Cannot open output file");
        }

        std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        in.close();

        std::size_t block_size = calculate_block_size(pub.p);
        if (block_size == 0)
        {
            block_size = 1;
        }

        std::size_t total_size = buffer.size();
        out.write(reinterpret_cast<const char*>(&total_size), sizeof(total_size));

        for (std::size_t i = 0; i < buffer.size(); i += block_size)
        {
            std::size_t current_chunk_size = std::min(block_size, buffer.size() - i);
            bigint m = 0;
            for (std::size_t j = 0; j < current_chunk_size; ++j)
            {
                m = (m << 8) | buffer[i + j];
            }

            el_gamal_ciphertext cipher = encrypt(m, pub);

            auto write_bigint = [&](const bigint& val)
            {
                std::vector<uint8_t> bytes;
                boost::multiprecision::export_bits(val, std::back_inserter(bytes), 8);
                std::size_t len = bytes.size();
                out.write(reinterpret_cast<const char*>(&len), sizeof(len));
                out.write(reinterpret_cast<const char*>(bytes.data()), len);
            };

            write_bigint(cipher.a);
            write_bigint(cipher.b);
        }
    }

    static void decrypt_file(const std::string& input_path, const std::string& output_path, const el_gamal_public_key& pub, const el_gamal_private_key& priv)
    {
        std::ifstream in(input_path, std::ios::binary);
        if (!in)
        {
            throw std::runtime_error("Cannot open input file");
        }

        std::ofstream out(output_path, std::ios::binary);
        if (!out)
        {
            throw std::runtime_error("Cannot open output file");
        }

        std::size_t original_size;
        in.read(reinterpret_cast<char*>(&original_size), sizeof(original_size));

        std::size_t block_size = calculate_block_size(pub.p);
        if (block_size == 0)
        {
            block_size = 1;
        }

        std::size_t bytes_written = 0;

        auto read_bigint = [&]() -> bigint
        {
            std::size_t len;
            in.read(reinterpret_cast<char*>(&len), sizeof(len));
            std::vector<uint8_t> bytes(len);
            in.read(reinterpret_cast<char*>(bytes.data()), len);
            bigint val;
            boost::multiprecision::import_bits(val, bytes.begin(), bytes.end());
            return val;
        };

        while (bytes_written < original_size)
        {
            el_gamal_ciphertext cipher;
            cipher.a = read_bigint();
            cipher.b = read_bigint();

            bigint m = decrypt(cipher, pub, priv);
            
            std::size_t current_block_size = std::min(block_size, original_size - bytes_written);
            std::vector<uint8_t> decrypted_bytes(current_block_size);
            
            for (std::size_t j = 0; j < current_block_size; ++j)
            {
                decrypted_bytes[current_block_size - 1 - j] = static_cast<uint8_t>(m & 0xFF);
                m >>= 8;
            }

            out.write(reinterpret_cast<const char*>(decrypted_bytes.data()), current_block_size);
            bytes_written += current_block_size;
        }
    }
};

#endif