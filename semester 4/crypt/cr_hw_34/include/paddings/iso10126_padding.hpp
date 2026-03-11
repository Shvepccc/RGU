#ifndef ISO10126_PADDING_H
#define ISO10126_PADDING_H

#include <random>
#include "padding_interface.hpp"

class iso10126_padding : public I_padding
{
public:

    std::vector<uint8_t> pad(
        const std::vector<uint8_t>& data,
        size_t block_size) const override
    {
        size_t pad = block_size - (data.size() % block_size);
        if (pad == 0) pad = block_size;

        std::vector<uint8_t> result = data;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0,255);

        for(size_t i = 0; i < pad-1; i++)
            result.push_back(dis(gen));

        result.push_back(pad);

        return result;
    }

    std::vector<uint8_t> unpad(
        const std::vector<uint8_t>& data,
        size_t block_size) const override
    {
        if (data.empty())
            throw std::runtime_error("Invalid ISO10126 padding");

        uint8_t pad = data.back();

        if(pad == 0 || pad > data.size())
            throw std::runtime_error("Invalid ISO10126 padding");

        return std::vector<uint8_t>(data.begin(), data.end() - pad);
    }
};

#endif