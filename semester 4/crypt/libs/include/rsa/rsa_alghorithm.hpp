#ifndef RSA_ALGHORITHM_H_
#define RSA_ALGHORITHM_H_

#include <fstream>
#include <vector>
#include <stdexcept>
#include <cstring>

class rsa_cipher
{
private:
    uint64_t n;
    uint64_t e;
    uint64_t d;
    size_t block_size;

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

    size_t calculate_block_size(uint64_t modulus)
    {
        size_t bits = 0;
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
        block_size = calculate_block_size(n);
        if (block_size == 0)
        {
            throw std::runtime_error("Block size too small");
        }
    }

    rsa_cipher(uint64_t public_key, uint64_t private_key, uint64_t modulus)
        : n(modulus), e(public_key), d(private_key)
    {
        block_size = calculate_block_size(n);
        if (block_size == 0)
        {
            throw std::runtime_error("Block size too small");
        }
    }

    std::vector<uint64_t> encrypt(const std::vector<uint8_t>& data)
    {
        std::vector<uint64_t> result;
        
        for (size_t i = 0; i < data.size(); i += block_size)
        {
            uint64_t block = 0;
            size_t bytes_to_read = std::min(block_size, data.size() - i);
            
            for (size_t j = 0; j < bytes_to_read; ++j)
            {
                block = (block << 8) | data[i + j];
            }
            
            if (block >= n)
            {
                throw std::runtime_error("Block value exceeds modulus");
            }
            
            uint64_t encrypted_block = mod_pow(block, e, n);
            result.push_back(encrypted_block);
        }
        
        return result;
    }

    std::vector<uint8_t> decrypt(const std::vector<uint64_t>& encrypted_data)
    {
        if (d == 0)
        {
            throw std::runtime_error("Private key not available for decryption");
        }
        
        std::vector<uint8_t> result;
        
        for (uint64_t block : encrypted_data)
        {
            uint64_t decrypted_block = mod_pow(block, d, n);
            
            size_t bytes_to_extract = block_size;
            uint64_t mask = 0xFF;
            
            for (size_t i = 0; i < bytes_to_extract; ++i)
            {
                uint8_t byte = static_cast<uint8_t>((decrypted_block >> (8 * (bytes_to_extract - 1 - i))) & mask);
                result.push_back(byte);
            }
        }
        
        while (!result.empty() && result.back() == 0)
        {
            result.pop_back();
        }
        
        return result;
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
        
        size_t count = encrypted.size();
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
        
        size_t count;
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