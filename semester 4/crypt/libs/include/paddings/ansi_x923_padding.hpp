#ifndef ANSI_X923_PADDING_H
#define ANSI_X923_PADDING_H

#include "padding_interface.hpp"

class ansi_x923_padding : public I_padding
{
public:

    std::vector<uint8_t> pad(
        const std::vector<uint8_t>& data,
        size_t block_size) const override
    {
        std::vector<uint8_t> result = data;

        size_t padding = block_size - (data.size() % block_size);

        if (padding == 0)
        {
            padding = block_size;
        }

        result.insert(result.end(), padding - 1, 0);
        result.push_back(static_cast<uint8_t>(padding));

        return result;
    }

    std::vector<uint8_t> unpad(
        const std::vector<uint8_t>& data,
        size_t block_size) const override
    {
        if (data.empty())
        {
            return data;
        }

        uint8_t padding = data.back();

        return std::vector<uint8_t>(
            data.begin(),
            data.end() - padding
        );
    }
};

#endif