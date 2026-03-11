#ifndef PKCS7_PADDING_H
#define PKCS7_PADDING_H

#include "padding_interface.hpp"

class pkcs7_padding : public I_padding
{
public:
    std::vector<uint8_t> pad(
        const std::vector<uint8_t>& data,
        size_t block_size) const override
    {
        size_t pad = block_size - (data.size() % block_size);
        if (pad == 0) pad = block_size;

        std::vector<uint8_t> result = data;
        result.insert(result.end(), pad, static_cast<uint8_t>(pad));
        return result;
    }

    std::vector<uint8_t> unpad(
        const std::vector<uint8_t>& data,
        size_t block_size) const override
    {
        if (data.empty())
            throw std::runtime_error("Invalid PKCS7 padding: empty data");
        
        if (data.size() % block_size != 0)
            throw std::runtime_error("Invalid PKCS7 padding: data size not multiple of block size");
        
        uint8_t pad = data.back();
        
        if (pad == 0 || pad > block_size)
            throw std::runtime_error("Invalid PKCS7 padding: pad value out of range");
        
        if (pad > data.size())
            throw std::runtime_error("Invalid PKCS7 padding: pad > data size");
        
        for (size_t i = data.size() - pad; i < data.size(); i++)
        {
            if (data[i] != pad)
                throw std::runtime_error("Invalid PKCS7 padding: inconsistent pad bytes");
        }
        
        return std::vector<uint8_t>(data.begin(), data.end() - pad);
    }
};

#endif