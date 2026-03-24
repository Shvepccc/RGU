#ifndef MODE_INTERFACE_H
#define MODE_INTERFACE_H

#include <vector>
#include <cstdint>
#include "../crypto_interfaces.hpp"

class I_cipher_mode
{
public:
    virtual ~I_cipher_mode() = default;

    virtual std::vector<uint8_t> encrypt(
        const std::vector<uint8_t>& data,
        I_feistel_chipher* cipher,
        size_t block_size,
        const std::vector<uint8_t>& iv,
        size_t threads
    ) = 0;

    virtual std::vector<uint8_t> decrypt(
        const std::vector<uint8_t>& data,
        I_feistel_chipher* cipher,
        size_t block_size,
        const std::vector<uint8_t>& iv,
        size_t threads
    ) = 0;
};

#endif