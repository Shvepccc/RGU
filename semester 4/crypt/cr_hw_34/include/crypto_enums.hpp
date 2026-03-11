#ifndef CRYPTO_ENUMS_H
#define CRYPTO_ENUMS_H

enum class cipher_mode_type
{
    ECB,
    CBC,
    PCBC,
    CFB,
    OFB,
    CTR,
    RANDOM_DELTA
};

enum class padding_mode_type
{
    Zeros,
    ANSI_X923,
    PKCS7,
    ISO10126
};

#endif