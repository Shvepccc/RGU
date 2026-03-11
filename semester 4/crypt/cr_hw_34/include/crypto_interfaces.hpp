#ifndef CRYPTO_INTERFACES_H
#define CRYPTO_INTERFACES_H

#include <cstdint>
#include <sys/types.h>
#include <vector>

class I_key_expander
{
public:
    virtual ~I_key_expander() = default;

    virtual std::vector<std::vector<uint8_t>> expand_key(
        const std::vector<uint8_t>& input_key) const = 0;
};

class I_feistel_round_function
{
public:
    virtual ~I_feistel_round_function() = default;

    virtual std::vector<uint8_t> feistel_round(
        const std::vector<uint8_t>& input_block,
        const std::vector<uint8_t>& round_key
    ) const = 0;
};

class I_feistel_chipher
{
public:
    virtual ~I_feistel_chipher() = default;

    virtual void setup_round_keys(const std::vector<uint8_t>& keys) = 0;

    virtual std::vector<uint8_t> encrypt_block(const std::vector<uint8_t>& plain_text) = 0;

    virtual std::vector<uint8_t> decrypt_block(const std::vector<uint8_t>& chipher_text) = 0;
};

class feistel_network : public I_feistel_chipher
{
private:
    const I_key_expander* key_expander_;
    const I_feistel_round_function* feistel_round_function_;

    int num_rounds_;
    int block_size_;

    std::vector<std::vector<uint8_t>> round_keys_;

public:
    feistel_network(
        const I_key_expander* key_expander,
        const I_feistel_round_function* round_function,
        int rounds,
        int block_size = 16)
        : key_expander_(key_expander),
        feistel_round_function_(round_function),
        num_rounds_(rounds),
        block_size_(block_size)
    {
        if (!key_expander_ || !feistel_round_function_)
        {
            throw std::invalid_argument("Key expander and round function must not be null");
        }
    }

    void setup_round_keys(const std::vector<uint8_t>& keys) override
    {
        round_keys_ = key_expander_->expand_key(keys);

        if (round_keys_.size() < num_rounds_)
        {
            throw std::runtime_error("Key expander generated insufficient round keys");
        }
    }

    std::vector<uint8_t> encrypt_block(const std::vector<uint8_t>& plaintext) override
    {
        if (plaintext.size() != block_size_)
        {
            throw std::invalid_argument("Plaintext block size does not match expected block size");
        }
        if (round_keys_.empty())
        {
            throw std::runtime_error("Round keys not set up. Call setup_round_keys() first.");
        }
        size_t half_size = block_size_ / 2;
        std::vector<uint8_t> left(plaintext.begin(), plaintext.begin() + half_size);
        std::vector<uint8_t> right(plaintext.begin() + half_size, plaintext.end());

        for (int i = 0; i < num_rounds_; i++)
        {
            std::vector<uint8_t> new_left = right;

            std::vector<uint8_t> feistel_round_function_output = 
                feistel_round_function_->feistel_round(right, round_keys_[i]);
            
            if (feistel_round_function_output.size() != half_size) {
                throw std::runtime_error("Round function output size mismatch");
            }

            std::vector<uint8_t> new_right(half_size);
            for (int j = 0; j < half_size; j++)
            {
                new_right[j] = left[j] ^ feistel_round_function_output[j];
            }

            left = new_left;
            right = new_right;
        }
        std::vector<uint8_t> ciphertext;
        ciphertext.reserve(block_size_);
        ciphertext.insert(ciphertext.end(), right.begin(), right.end());
        ciphertext.insert(ciphertext.end(), left.begin(), left.end());

        return ciphertext;
    }

    std::vector<uint8_t> decrypt_block(const std::vector<uint8_t>& chipertext) override
    {
        if (chipertext.size() != block_size_)
        {   
            throw std::invalid_argument("Ciphertext block size does not match expected block size");
        }
        if (round_keys_.empty())
        {
            throw std::runtime_error("Round keys not set up. Call setup_round_keys() first.");
        }
        size_t half_size = block_size_ / 2;
        std::vector<uint8_t> right(chipertext.begin(), chipertext.begin() + half_size);
        std::vector<uint8_t> left(chipertext.begin() + half_size, chipertext.end());

        for (int i = 0; i < num_rounds_; i++)
        {
            std::vector<uint8_t> new_right = left;

            std::vector<uint8_t> feistel_round_function_output = 
                feistel_round_function_->feistel_round(left, round_keys_[num_rounds_ - 1 - i]);

            if (feistel_round_function_output.size() != half_size) {
                throw std::runtime_error("Round function output size mismatch");
            }

            std::vector<uint8_t> new_left(half_size);
            for (int j = 0; j < half_size; j++)
            {
                new_left[j] = right[j] ^ feistel_round_function_output[j];
            }

            left = new_left;
            right = new_right;
        }
        std::vector<uint8_t> plaintext;
        plaintext.reserve(block_size_);
        plaintext.insert(plaintext.end(), left.begin(), left.end());
        plaintext.insert(plaintext.end(), right.begin(), right.end());

        return plaintext;
    }

};


#endif // CRYPTO_INTERFACES_H