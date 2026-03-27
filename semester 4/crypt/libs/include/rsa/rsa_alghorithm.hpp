#ifndef RSA_ALGHORITHM_H_
#define RSA_ALGHORITHM_H_

#include <fstream>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <iterator>
#include <algorithm>
#include "../paddings/pkcs7_padding.hpp"
#include <boost/multiprecision/cpp_int.hpp>

using bigint = boost::multiprecision::cpp_int;

class rsa_cipher
{
private:
    bigint n;
    bigint e;
    bigint d;
    std::size_t block_size;

    bigint mod_mul(bigint a, bigint b, bigint mod)
    {
        return static_cast<bigint>((a * b) % mod);
    }

    bigint mod_pow(bigint base, bigint exp, bigint mod)
    {
        bigint result = 1;
        base %= mod;
        while (exp > 0)
        {
            if (exp & 1) result = mod_mul(result, base, mod);
            base = mod_mul(base, base, mod);
            exp >>= 1;
        }
        return result;
    }

    std::size_t calculate_rsa_block_size(bigint modulus)
    {
        std::size_t bits = boost::multiprecision::msb(modulus) + 1;
        return (bits - 1) >> 3;
    }

public:
    rsa_cipher(bigint public_key, bigint modulus)
        : n(modulus), e(public_key), d(0)
    {
        block_size = calculate_rsa_block_size(n);
        if (block_size == 0) throw std::runtime_error("Block size too small");
        std::cout << "block size: " << block_size << std::endl;
    }

    rsa_cipher(bigint public_key, bigint private_key, bigint modulus)
        : n(modulus), e(public_key), d(private_key)
    {
        block_size = calculate_rsa_block_size(n);
        if (block_size == 0) throw std::runtime_error("Block size too small");
        std::cout << "block size: " << block_size << std::endl;
    }

    std::vector<bigint> encrypt(const std::vector<uint8_t>& data)
    {
        pkcs7_padding rsa_pad;
        std::vector<uint8_t> padded_data = rsa_pad.pad(data, block_size);
        std::vector<bigint> result;

        for (std::size_t i = 0; i < padded_data.size(); i += block_size)
        {
            bigint block_val;
            boost::multiprecision::import_bits(block_val, 
                padded_data.begin() + i, padded_data.begin() + i + block_size);
            result.push_back(mod_pow(block_val, e, n));
        }
        return result;
    }

    std::vector<uint8_t> decrypt(const std::vector<bigint>& encrypted_data)
    {
        if (d == 0) throw std::runtime_error("Private key not available");

        std::vector<uint8_t> padded_data;
        for (const auto& encrypted_val : encrypted_data)
        {
            bigint decrypted_val = mod_pow(encrypted_val, d, n);
            std::vector<uint8_t> temp;
            boost::multiprecision::export_bits(decrypted_val, std::back_inserter(temp), 8);
            
            std::vector<uint8_t> block(block_size, 0);
            if (temp.size() > block_size) throw std::runtime_error("Decryption error: block overflow");
            std::copy(temp.rbegin(), temp.rend(), block.rbegin());
            
            padded_data.insert(padded_data.end(), block.begin(), block.end());
        }

        pkcs7_padding rsa_pad;
        return rsa_pad.unpad(padded_data, block_size);
    }

    void encrypt_file(const std::string& input_file, const std::string& output_file)
    {
        std::ifstream in(input_file, std::ios::binary);
        if (!in) throw std::runtime_error("Cannot open input file");
        std::vector<uint8_t> data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        in.close();

        auto encrypted = encrypt(data);
        std::ofstream out(output_file, std::ios::binary);
        if (!out) throw std::runtime_error("Cannot open output file");

        std::size_t count = encrypted.size();
        out.write(reinterpret_cast<const char*>(&count), sizeof(count));

        for (const auto& val : encrypted)
        {
            std::vector<uint8_t> bytes;
            boost::multiprecision::export_bits(val, std::back_inserter(bytes), 8);
            std::size_t len = bytes.size();
            out.write(reinterpret_cast<const char*>(&len), sizeof(len));
            out.write(reinterpret_cast<const char*>(bytes.data()), len);
        }
    }

    void decrypt_file(const std::string& input_file, const std::string& output_file)
    {
        std::ifstream in(input_file, std::ios::binary);
        if (!in) throw std::runtime_error("Cannot open input file");

        std::size_t count;
        in.read(reinterpret_cast<char*>(&count), sizeof(count));

        std::vector<bigint> encrypted;
        for (std::size_t i = 0; i < count; ++i)
        {
            std::size_t len;
            in.read(reinterpret_cast<char*>(&len), sizeof(len));
            std::vector<uint8_t> bytes(len);
            in.read(reinterpret_cast<char*>(bytes.data()), len);
            
            bigint val;
            boost::multiprecision::import_bits(val, bytes.begin(), bytes.end());
            encrypted.push_back(val);
        }

        auto decrypted = decrypt(encrypted);
        std::ofstream out(output_file, std::ios::binary);
        out.write(reinterpret_cast<const char*>(decrypted.data()), decrypted.size());
    }
};

#endif