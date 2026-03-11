#ifndef PADDING_INTERFACE_H
#define PADDING_INTERFACE_H

#include <vector>
#include <cstdint>

class I_padding
{
public:
    virtual ~I_padding() = default;

    virtual std::vector<uint8_t> pad(
        const std::vector<uint8_t>& data,
        size_t block_size) const = 0;

    virtual std::vector<uint8_t> unpad(
        const std::vector<uint8_t>& data,
        size_t block_size) const = 0;
};

#endif