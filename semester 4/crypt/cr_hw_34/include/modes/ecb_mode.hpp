#ifndef ECB_MODE_H
#define ECB_MODE_H

#include "mode_interface.hpp"
#include <thread>

class ecb_mode : public I_cipher_mode
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
            throw std::invalid_argument("Cipher pointer is null");
        }

        if(data.size() % block_size != 0)
        {
            throw std::invalid_argument("Input size must be multiple of block size");
        }

        if(threads == 0)
        {
            threads = 1;
        }

        size_t blocks = data.size() / block_size;

        threads = std::min(threads, blocks);

        std::vector<uint8_t> result(data.size());

        std::vector<std::thread> workers;

        size_t chunk = blocks / threads;
        size_t remainder = blocks % threads;

        size_t start_block = 0;

        for(size_t t = 0; t < threads; t++)
        {
            size_t count = chunk + (t < remainder ? 1 : 0);

            size_t start = start_block;
            size_t end = start + count;

            start_block = end;

            workers.emplace_back(
                [&, start, end]()
                {
                    for(size_t i = start; i < end; i++)
                    {
                        std::vector<uint8_t> block(
                            data.begin() + i * block_size,
                            data.begin() + (i + 1) * block_size
                        );

                        auto enc = cipher->encrypt_block(block);

                        std::copy(
                            enc.begin(),
                            enc.end(),
                            result.begin() + i * block_size
                        );
                    }
                }
            );
        }

        for(auto& w : workers)
        {
            w.join();
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
            throw std::invalid_argument("Cipher pointer is null");
        }

        if(data.size() % block_size != 0)
        {
            throw std::invalid_argument("Ciphertext size must be multiple of block size");
        }

        if(threads == 0)
        {
            threads = 1;
        }

        size_t blocks = data.size() / block_size;

        threads = std::min(threads, blocks);

        std::vector<uint8_t> result(data.size());

        std::vector<std::thread> workers;

        size_t chunk = blocks / threads;
        size_t remainder = blocks % threads;

        size_t start_block = 0;

        for(size_t t = 0; t < threads; t++)
        {
            size_t count = chunk + (t < remainder ? 1 : 0);

            size_t start = start_block;
            size_t end = start + count;

            start_block = end;

            workers.emplace_back(
                [&, start, end]()
                {
                    for(size_t i = start; i < end; i++)
                    {
                        std::vector<uint8_t> block(
                            data.begin() + i * block_size,
                            data.begin() + (i + 1) * block_size
                        );

                        auto dec = cipher->decrypt_block(block);

                        std::copy(
                            dec.begin(),
                            dec.end(),
                            result.begin() + i * block_size
                        );
                    }
                }
            );
        }

        for(auto& w : workers)
        {
            w.join();
        }

        return result;
    }
};

#endif