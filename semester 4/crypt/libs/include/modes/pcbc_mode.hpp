#ifndef PCBC_MODE_H
#define PCBC_MODE_H

#include "mode_interface.hpp"

class pcbc_mode : public I_cipher_mode
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
        if(cipher == nullptr)
        {
            throw std::invalid_argument("Cipher is null");
        }

        if(iv.size() != block_size)
        {
            throw std::invalid_argument("IV size must equal block size");
        }

        if(data.size() % block_size != 0)
        {
            throw std::invalid_argument("Data size must be multiple of block size");
        }

        std::vector<uint8_t> result(data.size());

        std::vector<uint8_t> prev_plain(block_size);
        std::vector<uint8_t> prev_cipher = iv;

        size_t blocks = data.size() / block_size;

        for(size_t i = 0; i < blocks; i++)
        {
            std::vector<uint8_t> curr_plain(
                data.begin() + i * block_size,
                data.begin() + (i + 1) * block_size
            );

            std::vector<uint8_t> xored(block_size);
            for(size_t j = 0; j < block_size; j++)
            {
                xored[j] = curr_plain[j] ^ prev_plain[j] ^ prev_cipher[j];
            }

            std::vector<uint8_t> curr_cipher = cipher->encrypt_block(xored);

            std::copy(curr_cipher.begin(), curr_cipher.end(),
                    result.begin() + i * block_size);

            prev_plain = curr_plain;
            prev_cipher = curr_cipher;
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
        if(cipher == nullptr)
        {
            throw std::invalid_argument("Cipher is null");
        }

        if(iv.size() != block_size)
        {
            throw std::invalid_argument("IV size must equal block size");
        }

        if(data.size() % block_size != 0)
        {
            throw std::invalid_argument("Ciphertext size must be multiple of block size");
        }

        std::vector<uint8_t> result(data.size());

        std::vector<uint8_t> prev_plain(block_size);
        std::vector<uint8_t> prev_cipher = iv;

        size_t blocks = data.size() / block_size;

        for(size_t i = 0; i < blocks; i++)
        {
            std::vector<uint8_t> curr_cipher(
                data.begin() + i * block_size,
                data.begin() + (i + 1) * block_size
            );

            std::vector<uint8_t> decrypted = cipher->decrypt_block(curr_cipher);

            std::vector<uint8_t> curr_plain(block_size);
            for(size_t j = 0; j < block_size; j++)
            {
                curr_plain[j] = decrypted[j] ^ prev_plain[j] ^ prev_cipher[j];
            }

            std::copy(curr_plain.begin(), curr_plain.end(),
                    result.begin() + i * block_size);

            prev_plain = curr_plain;
            prev_cipher = curr_cipher;
        }

        return result;
    }
};

#endif