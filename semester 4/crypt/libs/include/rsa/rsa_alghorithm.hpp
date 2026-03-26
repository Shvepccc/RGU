#ifndef RSA_ALGHORITHM_H_
#define RSA_ALGHORITHM_H_

#include <fstream>
#include <vector>
#include <stdexcept>
#include <cstring>
#include "../paddings/pkcs7_padding.hpp"

class rsa_cipher
{
private:
    uint64_t n;
    uint64_t e;
    uint64_t d;
    std::size_t block_size;

    uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod)
    {
        return static_cast<uint64_t>((static_cast<__uint128_t>(a) * b) % mod);
    }

    uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
    {
        uint64_t result = 1;
        base %= mod;
        while (exp > 0)
        {
            if (exp & 1)
            {
                result = mod_mul(result, base, mod);
            }
            base = mod_mul(base, base, mod);
            exp >>= 1;
        }
        return result;
    }

    std::size_t calculate_rsa_block_size(uint64_t modulus)
    {
        std::size_t bits = 0;
        uint64_t temp = modulus;
        while (temp > 0)
        {
            ++bits;
            temp >>= 1;
        }
        return (bits - 1) / 8;
    }

public:
    rsa_cipher(uint64_t public_key, uint64_t modulus)
        : n(modulus), e(public_key), d(0)
    {
        block_size = calculate_rsa_block_size(n);
        if (block_size == 0)
        {
            throw std::runtime_error("Block size too small");
        }
    }

    rsa_cipher(uint64_t public_key, uint64_t private_key, uint64_t modulus)
        : n(modulus), e(public_key), d(private_key)
    {
        block_size = calculate_rsa_block_size(n);
        if (block_size == 0)
        {
            throw std::runtime_error("Block size too small");
        }
    }

    std::vector<uint64_t> encrypt(const std::vector<uint8_t>& data)
    {
        pkcs7_padding rsa_pad;
        std::vector<uint8_t> padded_data = rsa_pad.pad(data, block_size);
        std::vector<uint64_t> result;

        for (std::size_t i = 0; i < padded_data.size(); i += block_size)
        {
            uint64_t block_val = 0;
            for (std::size_t j = 0; j < block_size; ++j)
            {
                block_val = (block_val << 8) | padded_data[i + j];
            }

            result.push_back(mod_pow(block_val, e, n));
        }

        return result;
    }

    std::vector<uint8_t> decrypt(const std::vector<uint64_t>& encrypted_data)
    {
        if (d == 0)
        {
            throw std::runtime_error("Private key not available for decryption");
        }

        std::vector<uint8_t> padded_data;
        for (uint64_t encrypted_val : encrypted_data)
        {
            uint64_t decrypted_val = mod_pow(encrypted_val, d, n);

            for (std::size_t i = 0; i < block_size; ++i)
            {
                uint8_t byte = static_cast<uint8_t>((decrypted_val >> (8 * (block_size - 1 - i))) & 0xFF);
                padded_data.push_back(byte);
            }
        }

        pkcs7_padding rsa_pad;
        return rsa_pad.unpad(padded_data, block_size);
    }

    void encrypt_file(const std::string& input_file, const std::string& output_file)
    {
        std::ifstream in(input_file, std::ios::binary);
        if (!in.is_open())
        {
            throw std::runtime_error("Cannot open input file");
        }

        std::vector<uint8_t> data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        in.close();

        std::vector<uint64_t> encrypted = encrypt(data);

        std::ofstream out(output_file, std::ios::binary);
        if (!out.is_open())
        {
            throw std::runtime_error("Cannot open output file");
        }

        std::size_t count = encrypted.size();
        out.write(reinterpret_cast<const char*>(&count), sizeof(count));
        out.write(reinterpret_cast<const char*>(encrypted.data()), encrypted.size() * sizeof(uint64_t));
        out.close();
    }

    void decrypt_file(const std::string& input_file, const std::string& output_file)
    {
        if (d == 0)
        {
            throw std::runtime_error("Private key not available for decryption");
        }

        std::ifstream in(input_file, std::ios::binary);
        if (!in.is_open())
        {
            throw std::runtime_error("Cannot open input file");
        }

        std::size_t count;
        in.read(reinterpret_cast<char*>(&count), sizeof(count));

        std::vector<uint64_t> encrypted(count);
        in.read(reinterpret_cast<char*>(encrypted.data()), count * sizeof(uint64_t));
        in.close();

        std::vector<uint8_t> decrypted = decrypt(encrypted);

        std::ofstream out(output_file, std::ios::binary);
        if (!out.is_open())
        {
            throw std::runtime_error("Cannot open output file");
        }

        out.write(reinterpret_cast<const char*>(decrypted.data()), decrypted.size());
        out.close();
    }
};

#endif