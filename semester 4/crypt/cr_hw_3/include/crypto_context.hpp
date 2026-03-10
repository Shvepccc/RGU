#ifndef CRYPTO_CONTEXT_H
#define CRYPTO_CONTEXT_H

#include <future>
#include <fstream>
#include "../include/modes/mode_interface.hpp"
#include "../include/paddings/padding_interface.hpp"

class crypto_context
{
private:

    I_feistel_chipher* cipher_;
    I_cipher_mode* mode_;
    I_padding* padding_;

    std::vector<uint8_t> iv_;
    size_t block_size_;

public:

    crypto_context(
        I_feistel_chipher* cipher,
        I_cipher_mode* mode,
        I_padding* padding,
        const std::vector<uint8_t>& iv = {},
        size_t block_size = 16)
        : cipher_(cipher),
          mode_(mode),
          padding_(padding),
          iv_(iv),
          block_size_(block_size)
    {
    }

    void encrypt(
        const std::vector<uint8_t>& input,
        std::vector<uint8_t>& output,
        size_t threads
    )
    {
        auto padded = padding_->pad(input, block_size_);

        output = mode_->encrypt(
            padded,
            cipher_,
            block_size_,
            iv_,
            threads
        );
    }

    void decrypt(
        const std::vector<uint8_t>& input,
        std::vector<uint8_t>& output,
        size_t threads
    )
    {
        auto decrypted = mode_->decrypt(
            input,
            cipher_,
            block_size_,
            iv_,
            threads
        );

        output = padding_->unpad(decrypted, block_size_);
    }

    std::future<void> encrypt_file(
        const std::string& in,
        const std::string& out,
        size_t threads
    )
    {
        return std::async(
            std::launch::async,
            [=]()
            {
                std::ifstream fin(in, std::ios::binary);
                std::ofstream fout(out, std::ios::binary);

                std::vector<uint8_t> data(
                    (std::istreambuf_iterator<char>(fin)),
                    std::istreambuf_iterator<char>()
                );

                std::vector<uint8_t> result;

                encrypt(data, result, threads);

                fout.write(
                    reinterpret_cast<char*>(result.data()),
                    result.size()
                );
            }
        );
    }

    std::future<void> decrypt_file(
        const std::string& in,
        const std::string& out,
        size_t threads
    )
    {
        return std::async(
            std::launch::async,
            [=]()
            {
                std::ifstream fin(in, std::ios::binary);
                std::ofstream fout(out, std::ios::binary);

                std::vector<uint8_t> data(
                    (std::istreambuf_iterator<char>(fin)),
                    std::istreambuf_iterator<char>()
                );

                std::vector<uint8_t> result;

                decrypt(data, result, threads);

                fout.write(
                    reinterpret_cast<char*>(result.data()),
                    result.size()
                );
            }
        );
    }
};

#endif