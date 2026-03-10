#ifndef CRYPTO_ENUMS_H
#define CRYPTO_ENUMS_H

enum class cipher_mode_type
{
    ECB,
    CBC,
    PCBC
};

enum class padding_mode_type
{
    Zeros,
    ANSI_X923
};

#endif