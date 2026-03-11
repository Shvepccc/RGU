#ifndef RANDOM_DELTA_MODE_H
#define RANDOM_DELTA_MODE_H

#include "mode_interface.hpp"
#include <random>
#include <stdexcept>

class random_delta_mode : public I_cipher_mode
{
private:

    static std::vector<uint8_t> add_delta(
        std::vector<uint8_t> counter,
        const std::vector<uint8_t>& delta)
    {
        int carry = 0;

        for (int i = counter.size() - 1; i >= 0; i--)
        {
            int sum = counter[i] + delta[i] + carry;

            counter[i] = static_cast<uint8_t>(sum & 0xFF);

            carry = (sum >> 8);
        }

        return counter;
    }

public:

    std::vector<uint8_t> encrypt(
        const std::vector<uint8_t>& data,
        I_feistel_chipher* cipher,
        size_t block_size,
        const std::vector<uint8_t>& iv,
        size_t threads) override
    {
        if (cipher == nullptr)
        {
            throw std::invalid_argument("Cipher is null");
        }

        if (iv.size() != block_size)
        {
            throw std::invalid_argument("IV size must equal block size");
        }

        std::random_device rd;

        std::vector<uint8_t> delta(block_size);

        for (size_t i = 0; i < block_size; i++)
        {
            delta[i] = static_cast<uint8_t>(rd());
        }

        std::vector<uint8_t> result;
        result.resize(block_size + data.size());

        std::copy(delta.begin(), delta.end(), result.begin());

        std::vector<uint8_t> counter = iv;

        size_t offset = 0;

        while (offset < data.size())
        {
            auto gamma = cipher->encrypt_block(counter);

            size_t chunk = std::min(block_size, data.size() - offset);

            for (size_t i = 0; i < chunk; i++)
            {
                result[block_size + offset + i] =
                    data[offset + i] ^ gamma[i];
            }

            counter = add_delta(counter, delta);

            offset += chunk;
        }

        return result;
    }

    std::vector<uint8_t> decrypt(
        const std::vector<uint8_t>& data,
        I_feistel_chipher* cipher,
        size_t block_size,
        const std::vector<uint8_t>& iv,
        size_t threads) override
    {
        if (cipher == nullptr)
        {
            throw std::invalid_argument("Cipher is null");
        }

        if (iv.size() != block_size)
        {
            throw std::invalid_argument("IV size must equal block size");
        }

        if (data.size() < block_size)
        {
            throw std::invalid_argument("Ciphertext too short");
        }

        std::vector<uint8_t> delta(
            data.begin(),
            data.begin() + block_size
        );

        std::vector<uint8_t> result(data.size() - block_size);

        std::vector<uint8_t> counter = iv;

        size_t offset = 0;

        while (offset < result.size())
        {
            auto gamma = cipher->encrypt_block(counter);

            size_t chunk = std::min(block_size, result.size() - offset);

            for (size_t i = 0; i < chunk; i++)
            {
                result[offset + i] =
                    data[block_size + offset + i] ^ gamma[i];
            }

            counter = add_delta(counter, delta);

            offset += chunk;
        }

        return result;
    }
};

#endif