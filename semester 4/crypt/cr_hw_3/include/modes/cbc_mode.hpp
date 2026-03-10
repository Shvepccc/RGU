#ifndef CBC_MODE_H
#define CBC_MODE_H

#include "mode_interface.hpp"

class cbc_mode : public I_cipher_mode
{
public:

    std::vector<uint8_t> encrypt(
    const std::vector<uint8_t>& data,
    I_feistel_chipher* cipher,
    size_t block_size,
    const std::vector<uint8_t>& iv,
    size_t threads
    ) override
    {
        if(iv.size() != block_size)
        {
            throw std::invalid_argument("IV size must equal block size");
        }

        if(data.size() % block_size != 0)
        {
            throw std::invalid_argument("Input size must be multiple of block size");
        }

        std::vector<uint8_t> result(data.size());

        std::vector<uint8_t> prev = iv;

        size_t blocks = data.size() / block_size;

        for(size_t i = 0; i < blocks; i++)
        {
            std::vector<uint8_t> block(
                data.begin() + i * block_size,
                data.begin() + (i + 1) * block_size
            );

            for(size_t j = 0; j < block_size; j++)
            {
                block[j] ^= prev[j];
            }

            auto enc = cipher->encrypt_block(block);

            prev = enc;

            std::copy(
                enc.begin(),
                enc.end(),
                result.begin() + i * block_size
            );
        }

        return result;
    }

    std::vector<uint8_t> decrypt(
    const std::vector<uint8_t>& data,
    I_feistel_chipher* cipher,
    size_t block_size,
    const std::vector<uint8_t>& iv,
    size_t threads
    ) override
    {
        if(iv.size() != block_size)
        {
            throw std::invalid_argument("IV size must equal block size");
        }

        if(data.size() % block_size != 0)
        {
            throw std::invalid_argument("Ciphertext size must be multiple of block size");
        }

        std::vector<uint8_t> result(data.size());

        std::vector<uint8_t> prev = iv;

        size_t blocks = data.size() / block_size;

        for(size_t i = 0; i < blocks; i++)
        {
            std::vector<uint8_t> block(
                data.begin() + i * block_size,
                data.begin() + (i + 1) * block_size
            );

            auto dec = cipher->decrypt_block(block);

            for(size_t j = 0; j < block_size; j++)
            {
                dec[j] ^= prev[j];
            }

            prev = block;

            std::copy(
                dec.begin(),
                dec.end(),
                result.begin() + i * block_size
            );
        }

        return result;
    }
};

#endif