// triple_des_algorithm.hpp
#ifndef TRIPLE_DES_ALGORITHM_H
#define TRIPLE_DES_ALGORITHM_H

#include "des_algorithm.hpp"
#include <vector>
#include <cstdint>
#include <stdexcept>

enum class TripleDESMode
{
    DES_EEE3,  // Encrypt-Encrypt-Encrypt with three different keys
    DES_EDE3,  // Encrypt-Decrypt-Encrypt with three different keys
    DES_EEE2,  // Encrypt-Encrypt-Encrypt with first key repeated as third
    DES_EDE2   // Encrypt-Decrypt-Encrypt with first key repeated as third
};

class triple_des_algorithm : public I_feistel_chipher
{
private:
    des_algorithm des1_;
    des_algorithm des2_;
    des_algorithm des3_;
    TripleDESMode mode_;
    bool initialized_ = false;

    std::vector<uint8_t> adjust_key(const std::vector<uint8_t>& key, size_t expected_size) const
    {
        if (key.size() == expected_size)
            return key;
        if (key.size() < expected_size)
            throw std::invalid_argument("Key too short for selected 3DES mode");

        return std::vector<uint8_t>(key.begin(), key.begin() + expected_size);
    }

public:
    explicit triple_des_algorithm(TripleDESMode mode = TripleDESMode::DES_EDE3)
        : mode_(mode)
    {
    }

    void setup_round_keys(const std::vector<uint8_t>& key) override
    {
        bool is_3key = (mode_ == TripleDESMode::DES_EEE3 || mode_ == TripleDESMode::DES_EDE3);
        
        if (is_3key && key.size() != 24 && key.size() != 16)
            throw std::invalid_argument("3-key mode requires 24 or 16 bytes");
        if (!is_3key && key.size() != 16)
            throw std::invalid_argument("2-key mode requires exactly 16 bytes");
        
        auto k1 = std::vector<uint8_t>(key.begin(), key.begin() + 8);
        auto k2 = std::vector<uint8_t>(key.begin() + 8, key.begin() + 16);
        auto k3 = (key.size() == 24) ? 
                std::vector<uint8_t>(key.begin() + 16, key.begin() + 24) : 
                k1;
        
        des1_.setup_round_keys(k1);
        des2_.setup_round_keys(k2);
        des3_.setup_round_keys(k3);
        
        initialized_ = true;
    }

    std::vector<uint8_t> encrypt_block(const std::vector<uint8_t>& plaintext) override
    {
        if (!initialized_)
            throw std::runtime_error("3DES not initialized. Call setup_round_keys() first.");
        if (plaintext.size() != 8)
            throw std::invalid_argument("3DES operates on 8-byte blocks");

        std::vector<uint8_t> result;

        switch (mode_)
        {
        case TripleDESMode::DES_EEE3:
        case TripleDESMode::DES_EEE2:
            // E(K1, E(K2, E(K3, plaintext)))
            result = des3_.encrypt_block(plaintext);
            result = des2_.encrypt_block(result);
            result = des1_.encrypt_block(result);
            break;

        case TripleDESMode::DES_EDE3:
        case TripleDESMode::DES_EDE2:
            // E(K1, D(K2, E(K3, plaintext)))
            result = des3_.encrypt_block(plaintext);
            result = des2_.decrypt_block(result);
            result = des1_.encrypt_block(result);
            break;
        }

        return result;
    }

    std::vector<uint8_t> decrypt_block(const std::vector<uint8_t>& ciphertext) override
    {
        if (!initialized_)
            throw std::runtime_error("3DES not initialized. Call setup_round_keys() first.");
        if (ciphertext.size() != 8)
            throw std::invalid_argument("3DES operates on 8-byte blocks");

        std::vector<uint8_t> result;

        switch (mode_)
        {
        case TripleDESMode::DES_EEE3:
        case TripleDESMode::DES_EEE2:
            // D(K1, D(K2, D(K3, ciphertext)))
            result = des1_.decrypt_block(ciphertext);
            result = des2_.decrypt_block(result);
            result = des3_.decrypt_block(result);
            break;

        case TripleDESMode::DES_EDE3:
        case TripleDESMode::DES_EDE2:
            // D(K1, E(K2, D(K3, ciphertext)))
            result = des1_.decrypt_block(ciphertext);
            result = des2_.encrypt_block(result);
            result = des3_.decrypt_block(result);
            break;
        }

        return result;
    }
};

#endif // TRIPLE_DES_ALGORITHM_H