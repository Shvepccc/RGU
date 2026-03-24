#ifndef CRYPTO_CONTEXT_H
#define CRYPTO_CONTEXT_H

#include <future>
#include <fstream>
#include <stdexcept>
#include "../include/modes/mode_interface.hpp"
#include "../include/paddings/padding_interface.hpp"
#include "../include/crypto_enums.hpp"
#include "../include/modes/ecb_mode.hpp"
#include "../include/modes/cbc_mode.hpp"
#include "../include/modes/pcbc_mode.hpp"
#include "../include/paddings/zeros_padding.hpp"
#include "../include/paddings/ansi_x923_padding.hpp"
#include "modes/cfb_mode.hpp"
#include "modes/ctr_mode.hpp"
#include "modes/ofb_mode.hpp"
#include "modes/random_delta_mode.hpp"
#include "paddings/iso10126_padding.hpp"
#include "paddings/pkcs7_padding.hpp"

class crypto_context
{
private:
    I_feistel_chipher* cipher_;
    I_cipher_mode* mode_;
    I_padding* padding_;
    
    std::vector<uint8_t> iv_;
    size_t block_size_;
    
    static I_cipher_mode* create_mode(cipher_mode_type mode, va_list args)
    {
        switch (mode)
        {
            case cipher_mode_type::ECB:
                return new ecb_mode();
            case cipher_mode_type::CBC:
                return new cbc_mode();
            case cipher_mode_type::PCBC:
                return new pcbc_mode();
            case cipher_mode_type::CFB:
                return new cfb_mode();
            case cipher_mode_type::CTR:
                return new ctr_mode();
            case cipher_mode_type::OFB:
                return new ofb_mode();
            case cipher_mode_type::RANDOM_DELTA:
                return new random_delta_mode();
            default:
                throw std::invalid_argument("Unsupported cipher mode");
        }
    }
    
    static I_padding* create_padding(padding_mode_type padding)
    {
        switch (padding)
        {
            case padding_mode_type::Zeros:
                return new zeros_padding();
            case padding_mode_type::ANSI_X923:
                return new ansi_x923_padding();
            case padding_mode_type::ISO10126:
                return new iso10126_padding();
            case padding_mode_type::PKCS7:
                return new pkcs7_padding();
            default:
                throw std::invalid_argument("Unsupported padding mode");
        }
    }

public:
    crypto_context(
        I_feistel_chipher* cipher,
        cipher_mode_type mode,
        padding_mode_type padding,
        const std::vector<uint8_t>& iv = {},
        size_t block_size = 16,
        ...)
        : cipher_(cipher),
          mode_(nullptr),
          padding_(nullptr),
          iv_(iv),
          block_size_(block_size)
    {
        if (!cipher_)
            throw std::invalid_argument("Cipher pointer cannot be null");
        
        va_list args;
        va_start(args, block_size);
        
        try
        {
            mode_ = create_mode(mode, args);
            padding_ = create_padding(padding);
        }
        catch (...)
        {
            va_end(args);
            delete mode_;
            delete padding_;
            throw;
        }
        
        va_end(args);
    }
    
    ~crypto_context()
    {
        delete mode_;
        delete padding_;
    }
    
    crypto_context(const crypto_context&) = delete;
    crypto_context& operator=(const crypto_context&) = delete;
    
    void encrypt(
        const std::vector<uint8_t>& input,
        std::vector<uint8_t>& output,
        size_t threads)
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
        size_t threads)
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
        size_t threads)
    {
        return std::async(
            std::launch::async,
            [this, in, out, threads]()
            {
                std::ifstream fin(in, std::ios::binary);
                if (!fin)
                    throw std::runtime_error("Cannot open input file");
                
                std::ofstream fout(out, std::ios::binary);
                if (!fout)
                    throw std::runtime_error("Cannot open output file");
                
                std::vector<uint8_t> data(
                    (std::istreambuf_iterator<char>(fin)),
                    std::istreambuf_iterator<char>()
                );
                
                std::vector<uint8_t> result;
                encrypt(data, result, threads);
                
                fout.write(
                    reinterpret_cast<const char*>(result.data()),
                    result.size()
                );
            }
        );
    }
    
    std::future<void> decrypt_file(
        const std::string& in,
        const std::string& out,
        size_t threads)
    {
        return std::async(
            std::launch::async,
            [this, in, out, threads]()
            {
                std::ifstream fin(in, std::ios::binary);
                if (!fin)
                    throw std::runtime_error("Cannot open input file");
                
                std::ofstream fout(out, std::ios::binary);
                if (!fout)
                    throw std::runtime_error("Cannot open output file");
                
                std::vector<uint8_t> data(
                    (std::istreambuf_iterator<char>(fin)),
                    std::istreambuf_iterator<char>()
                );
                
                std::vector<uint8_t> result;
                decrypt(data, result, threads);
                
                fout.write(
                    reinterpret_cast<const char*>(result.data()),
                    result.size()
                );
            }
        );
    }
};

#endif