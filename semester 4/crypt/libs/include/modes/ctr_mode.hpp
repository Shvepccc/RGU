#ifndef CTR_MODE_H
#define CTR_MODE_H

#include "mode_interface.hpp"
#include <thread>
#include <stdexcept>

class ctr_mode : public I_cipher_mode
{
private:

    static std::vector<uint8_t> add_counter(
        std::vector<uint8_t> counter,
        uint64_t value)
    {
        int i = counter.size() - 1;

        while (value > 0 && i >= 0)
        {
            uint64_t sum = counter[i] + (value & 0xFF);
            counter[i] = static_cast<uint8_t>(sum & 0xFF);
            value = (value >> 8) + (sum >> 8);
            i--;
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

        if (threads == 0)
        {
            threads = 1;
        }

        size_t blocks = (data.size() + block_size - 1) / block_size;

        std::vector<uint8_t> result(data.size());

        threads = std::min(threads, blocks);

        size_t chunk = blocks / threads;
        size_t remainder = blocks % threads;

        std::vector<std::thread> workers;

        size_t start_block = 0;

        for (size_t t = 0; t < threads; t++)
        {
            size_t work = chunk + (t < remainder ? 1 : 0);
            size_t end_block = start_block + work;

            workers.emplace_back(
                [&, start_block, end_block]()
                {
                    auto counter = add_counter(iv, start_block);

                    for (size_t block = start_block; block < end_block; block++)
                    {
                        auto gamma = cipher->encrypt_block(counter);

                        size_t offset = block * block_size;
                        size_t chunk_size =
                            std::min(block_size, data.size() - offset);

                        for (size_t i = 0; i < chunk_size; i++)
                        {
                            result[offset + i] =
                                data[offset + i] ^ gamma[i];
                        }

                        counter = add_counter(counter, 1);
                    }
                }
            );

            start_block = end_block;
        }

        for (auto& w : workers)
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
        size_t threads) override
    {
        return encrypt(data, cipher, block_size, iv, threads);
    }
};

#endif