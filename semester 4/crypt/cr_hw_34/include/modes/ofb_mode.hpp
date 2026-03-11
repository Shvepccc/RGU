#ifndef OFB_MODE_H
#define OFB_MODE_H

#include "mode_interface.hpp"
#include <stdexcept>

class ofb_mode : public I_cipher_mode
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
            feedback = cipher->encrypt_block(feedback);

            size_t chunk = std::min(block_size, data.size() - offset);

            for (size_t i = 0; i < chunk; i++)
            {
                result[offset + i] =
                    data[offset + i] ^ feedback[i];
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
        return encrypt(data, cipher, block_size, iv, threads);
    }
};

#endif