#ifndef CBC_MODE_H
#define CBC_MODE_H

#include "mode_interface.hpp"
#include <thread>
#include <vector>

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

            std::copy(enc.begin(), enc.end(), result.begin() + i * block_size);
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

        size_t blocks = data.size() / block_size;
        std::vector<uint8_t> result(data.size());

        if(threads <= 1 || blocks < threads)
        {
            std::vector<uint8_t> prev = iv;
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
                std::copy(dec.begin(), dec.end(), result.begin() + i * block_size);
            }
        }
        else
        {
            std::vector<std::thread> workers;
            size_t blocks_per_thread = blocks / threads;
            size_t remainder = blocks % threads;
            
            std::vector<std::vector<uint8_t>> prev_blocks(blocks);
            
            for(size_t i = 0; i < blocks; i++)
            {
                if(i == 0)
                    prev_blocks[i] = iv;
                else
                    prev_blocks[i].assign(data.begin() + (i-1) * block_size, 
                                          data.begin() + i * block_size);
            }
            
            size_t start = 0;
            for(size_t t = 0; t < threads; t++)
            {
                size_t count = blocks_per_thread + (t < remainder ? 1 : 0);
                size_t end = start + count;
                
                workers.emplace_back([&, start, end]() {
                    for(size_t i = start; i < end; i++)
                    {
                        std::vector<uint8_t> block(
                            data.begin() + i * block_size,
                            data.begin() + (i + 1) * block_size
                        );
                        
                        auto dec = cipher->decrypt_block(block);
                        
                        for(size_t j = 0; j < block_size; j++)
                        {
                            dec[j] ^= prev_blocks[i][j];
                        }
                        
                        std::copy(dec.begin(), dec.end(), result.begin() + i * block_size);
                    }
                });
                
                start = end;
            }
            
            for(auto& w : workers)
            {
                w.join();
            }
        }

        return result;
    }
};

#endif