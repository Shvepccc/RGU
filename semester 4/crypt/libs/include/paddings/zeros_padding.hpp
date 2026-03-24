#ifndef ZEROS_PADDING_H
#define ZEROS_PADDING_H

#include "padding_interface.hpp"

class zeros_padding : public I_padding
{
public:
    std::vector<uint8_t> pad(
        const std::vector<uint8_t>& data,
        size_t block_size) const override
    {
        std::vector<uint8_t> result = data;
        size_t padding = block_size - (data.size() % block_size);
        result.insert(result.end(), padding, 0);
        return result;
    }

    std::vector<uint8_t> unpad(
        const std::vector<uint8_t>& data,
        size_t block_size) const override
    {
        std::vector<uint8_t> result = data;
        while (!result.empty() && result.back() == 0)
        {
            result.pop_back();
        }
        return result;
    }
};

#endif