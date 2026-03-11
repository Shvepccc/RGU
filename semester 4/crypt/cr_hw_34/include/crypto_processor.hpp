#ifndef CRYPTO_PROCESSOR_H
#define CRYPTO_PROCESSOR_H

#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <stdexcept>

#include "crypto_interfaces.hpp"

class crypto_processor
{
private:
    std::unique_ptr<I_feistel_chipher> cipher_;
    size_t block_size_;
    
    std::vector<uint8_t> pad(const std::vector<uint8_t>& data) const
    {
        size_t pad_len = block_size_ - (data.size() % block_size_);
        std::vector<uint8_t> result = data;
        result.insert(result.end(), pad_len, static_cast<uint8_t>(pad_len));
        return result;
    }
    
    std::vector<uint8_t> unpad(const std::vector<uint8_t>& data) const
    {
        if (data.empty()) return data;
        uint8_t pad_len = data.back();
        return std::vector<uint8_t>(data.begin(), data.end() - pad_len);
    }
    
public:
    explicit crypto_processor(I_feistel_chipher* cipher, size_t block_size = 8)
        : cipher_(cipher), block_size_(block_size)
    {
        if (!cipher_) throw std::invalid_argument("Cipher cannot be null");
    }
    
    void set_key(const std::vector<uint8_t>& key)
    {
        cipher_->setup_round_keys(key);
    }
    
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data)
    {
        std::vector<uint8_t> padded = pad(data);
        std::vector<uint8_t> result;
        result.reserve(padded.size());
        
        for (size_t i = 0; i < padded.size(); i += block_size_)
        {
            std::vector<uint8_t> block(padded.begin() + i, padded.begin() + i + block_size_);
            std::vector<uint8_t> encrypted = cipher_->encrypt_block(block);
            result.insert(result.end(), encrypted.begin(), encrypted.end());
        }
        return result;
    }
    
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& data)
    {
        if (data.size() % block_size_ != 0)
            throw std::invalid_argument("Invalid ciphertext size");
            
        std::vector<uint8_t> result;
        result.reserve(data.size());
        
        for (size_t i = 0; i < data.size(); i += block_size_)
        {
            std::vector<uint8_t> block(data.begin() + i, data.begin() + i + block_size_);
            std::vector<uint8_t> decrypted = cipher_->decrypt_block(block);
            result.insert(result.end(), decrypted.begin(), decrypted.end());
        }
        return unpad(result);
    }
    
    void encrypt_file(const std::string& in_path, const std::string& out_path)
    {
        std::ifstream in(in_path, std::ios::binary);
        std::ofstream out(out_path, std::ios::binary);
        
        std::vector<uint8_t> data((std::istreambuf_iterator<char>(in)), {});
        std::vector<uint8_t> encrypted = encrypt(data);
        out.write(reinterpret_cast<char*>(encrypted.data()), encrypted.size());
    }
    
    void decrypt_file(const std::string& in_path, const std::string& out_path)
    {
        std::ifstream in(in_path, std::ios::binary);
        std::ofstream out(out_path, std::ios::binary);
        
        std::vector<uint8_t> data((std::istreambuf_iterator<char>(in)), {});
        std::vector<uint8_t> decrypted = decrypt(data);
        out.write(reinterpret_cast<char*>(decrypted.data()), decrypted.size());
    }
};

#endif