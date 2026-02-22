#ifndef FEISTEL_TEMPLATE_H
#define FEISTEL_TEMPLATE_H

#include "crypto_interfaces.hpp"
#include <vector>
#include <cstdint>
#include <stdexcept>

class feistel_template : public I_feistel_chipher
{
protected:
    feistel_network* feistel_network_;
    
    bool is_initialized_;

public:

    feistel_template(feistel_network* network)
        : feistel_network_(network), is_initialized_(false)
    {
        // if (!feistel_network_)
        // {
        //     throw std::invalid_argument("feistel_network pointer must not be null");
        // }
    }

    virtual ~feistel_template() = default;

    std::vector<uint8_t> encrypt_block(const std::vector<uint8_t>& plaintext) override final
    {
        if (!is_initialized_)
        {
            throw std::runtime_error("Round keys not set up. Call setup_round_keys() first.");
        }

        std::vector<uint8_t> processed_input = pre_encrypt(plaintext);

        std::vector<uint8_t> encrypted = feistel_network_->encrypt_block(processed_input);

        std::vector<uint8_t> final_output = post_encrypt(encrypted);

        return final_output;
    }

    std::vector<uint8_t> decrypt_block(const std::vector<uint8_t>& ciphertext) override final
    {
        if (!is_initialized_)
        {
            throw std::runtime_error("Round keys not set up. Call setup_round_keys() first.");
        }

        std::vector<uint8_t> processed_input = pre_decrypt(ciphertext);

        std::vector<uint8_t> decrypted = feistel_network_->decrypt_block(processed_input);

        std::vector<uint8_t> final_output = post_decrypt(decrypted);

        return final_output;
    }

    void setup_round_keys(const std::vector<uint8_t>& key) override final
    {
        std::vector<uint8_t> processed_key = pre_key_setup(key);

        feistel_network_->setup_round_keys(processed_key);

        post_key_setup();

        is_initialized_ = true;
    }

    virtual std::vector<uint8_t> pre_encrypt(const std::vector<uint8_t>& data)
    {
        return data;
    }

    virtual std::vector<uint8_t> post_encrypt(const std::vector<uint8_t>& data)
    {
        return data;
    }

    virtual std::vector<uint8_t> pre_decrypt(const std::vector<uint8_t>& data)
    {
        return data;
    }

    virtual std::vector<uint8_t> post_decrypt(const std::vector<uint8_t>& data)
    {
        return data;
    }

    virtual std::vector<uint8_t> pre_key_setup(const std::vector<uint8_t>& key)
    {
        return key;
    }

    virtual void post_key_setup()
    {
    }

protected:
    void set_feistel_network(feistel_network* network) 
    {
        feistel_network_ = network;
    }
};

#endif