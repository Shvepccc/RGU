#ifndef CFB_MODE_H
#define CFB_MODE_H

#include "mode_interface.hpp"
#include <stdexcept>

class cfb_mode : public I_cipher_mode
{
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

        std::vector<uint8_t> result(data.size());

        std::vector<uint8_t> feedback = iv;

        size_t offset = 0;

        while (offset < data.size())
        {
            auto gamma = cipher->encrypt_block(feedback);

            size_t chunk = std::min(block_size, data.size() - offset);

            std::vector<uint8_t> ciphertext(chunk);

            for (size_t i = 0; i < chunk; i++)
            {
                ciphertext[i] = data[offset + i] ^ gamma[i];
                result[offset + i] = ciphertext[i];
            }

            if (chunk == block_size)
            {
                feedback = ciphertext;
            }
            else
            {
                for (size_t i = 0; i < chunk; i++)
                {
                    feedback[i] = ciphertext[i];
                }
            }

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

        std::vector<uint8_t> result(data.size());

        std::vector<uint8_t> feedback = iv;

        size_t offset = 0;

        while (offset < data.size())
        {
            auto gamma = cipher->encrypt_block(feedback);

            size_t chunk = std::min(block_size, data.size() - offset);

            std::vector<uint8_t> plaintext(chunk);

            for (size_t i = 0; i < chunk; i++)
            {
                plaintext[i] = data[offset + i] ^ gamma[i];
                result[offset + i] = plaintext[i];
            }

            if (chunk == block_size)
            {
                feedback.assign(
                    data.begin() + offset,
                    data.begin() + offset + block_size
                );
            }
            else
            {
                for (size_t i = 0; i < chunk; i++)
                {
                    feedback[i] = data[offset + i];
                }
            }

            offset += chunk;
        }

        return result;
    }
};

#endif