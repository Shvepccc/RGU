#ifndef TWOFISH_H_
#define TWOFISH_H_

#include "../crypto_interfaces.hpp"
#include "../poly_gf2.hpp"
#include "twofish_data.hpp"
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <algorithm>

std::uint8_t q0(std::uint8_t x);
std::uint8_t q1(std::uint8_t x);

class twofish : public I_feistel_chipher
{
private:
    std::vector<std::uint32_t> round_keys_;
    std::vector<std::uint32_t> s_box_keys_;

    std::uint32_t rol32(std::uint32_t x, int n) const
    {
        return (x << n) | (x >> (32 - n));
    }

    std::uint32_t ror32(std::uint32_t x, int n) const
    {
        return (x >> n) | (x << (32 - n));
    }

    std::uint8_t gf_mul_mds(std::uint8_t a, std::uint8_t b) const
    {
        poly_gf2 pa(a);
        poly_gf2 pb(b);
        poly_gf2 mod(0x169);
        return static_cast<std::uint8_t>(pa.mul_mod(pb, mod).get_value());
    }

    std::uint8_t gf_mul_rs(std::uint8_t a, std::uint8_t b) const
    {
        poly_gf2 pa(a);
        poly_gf2 pb(b);
        poly_gf2 mod(0x14D);
        return static_cast<std::uint8_t>(pa.mul_mod(pb, mod).get_value());
    }

    std::uint32_t mds_multiply(std::uint8_t b0, std::uint8_t b1, std::uint8_t b2, std::uint8_t b3) const
    {
        std::uint8_t r0 = gf_mul_mds(0x01, b0) ^ gf_mul_mds(0xEF, b1) ^ gf_mul_mds(0x5B, b2) ^ gf_mul_mds(0x5B, b3);
        std::uint8_t r1 = gf_mul_mds(0x5B, b0) ^ gf_mul_mds(0xEF, b1) ^ gf_mul_mds(0xEF, b2) ^ gf_mul_mds(0x01, b3);
        std::uint8_t r2 = gf_mul_mds(0xEF, b0) ^ gf_mul_mds(0x5B, b1) ^ gf_mul_mds(0x01, b2) ^ gf_mul_mds(0xEF, b3);
        std::uint8_t r3 = gf_mul_mds(0xEF, b0) ^ gf_mul_mds(0x01, b1) ^ gf_mul_mds(0xEF, b2) ^ gf_mul_mds(0x5B, b3);

        return (static_cast<std::uint32_t>(r3) << 24) |
               (static_cast<std::uint32_t>(r2) << 16) |
               (static_cast<std::uint32_t>(r1) << 8) |
               static_cast<std::uint32_t>(r0);
    }

    std::uint32_t rs_multiply(const std::vector<std::uint8_t>& k) const
    {
        std::uint8_t r0 = gf_mul_rs(0x01, k[0]) ^ gf_mul_rs(0xA4, k[1]) ^ gf_mul_rs(0x55, k[2]) ^ gf_mul_rs(0x87, k[3]) ^
                          gf_mul_rs(0x5A, k[4]) ^ gf_mul_rs(0x58, k[5]) ^ gf_mul_rs(0x1C, k[6]) ^ gf_mul_rs(0x97, k[7]);
        std::uint8_t r1 = gf_mul_rs(0x5A, k[0]) ^ gf_mul_rs(0x58, k[1]) ^ gf_mul_rs(0x1C, k[2]) ^ gf_mul_rs(0x97, k[3]) ^
                          gf_mul_rs(0x01, k[4]) ^ gf_mul_rs(0xA4, k[5]) ^ gf_mul_rs(0x55, k[6]) ^ gf_mul_rs(0x87, k[7]);
        std::uint8_t r2 = gf_mul_rs(0xA3, k[0]) ^ gf_mul_rs(0xD9, k[1]) ^ gf_mul_rs(0x59, k[2]) ^ gf_mul_rs(0xEA, k[3]) ^
                          gf_mul_rs(0x4E, k[4]) ^ gf_mul_rs(0x38, k[5]) ^ gf_mul_rs(0x10, k[6]) ^ gf_mul_rs(0x14, k[7]);
        std::uint8_t r3 = gf_mul_rs(0x4E, k[0]) ^ gf_mul_rs(0x38, k[1]) ^ gf_mul_rs(0x10, k[2]) ^ gf_mul_rs(0x14, k[3]) ^
                          gf_mul_rs(0xA3, k[4]) ^ gf_mul_rs(0xD9, k[5]) ^ gf_mul_rs(0x59, k[6]) ^ gf_mul_rs(0xEA, k[7]);

        return (static_cast<std::uint32_t>(r3) << 24) |
               (static_cast<std::uint32_t>(r2) << 16) |
               (static_cast<std::uint32_t>(r1) << 8) |
               static_cast<std::uint32_t>(r0);
    }

    std::uint32_t h_func(std::uint32_t x, const std::vector<std::uint32_t>& l) const
    {
        std::uint8_t b0 = static_cast<std::uint8_t>(x);
        std::uint8_t b1 = static_cast<std::uint8_t>(x >> 8);
        std::uint8_t b2 = static_cast<std::uint8_t>(x >> 16);
        std::uint8_t b3 = static_cast<std::uint8_t>(x >> 24);

        size_t k = l.size();
        if (k == 4)
        {
            b0 = q1(b0) ^ static_cast<std::uint8_t>(l[3]);
            b1 = q0(b1) ^ static_cast<std::uint8_t>(l[3] >> 8);
            b2 = q0(b2) ^ static_cast<std::uint8_t>(l[3] >> 16);
            b3 = q1(b3) ^ static_cast<std::uint8_t>(l[3] >> 24);
        }
        if (k >= 3)
        {
            b0 = q1(b0) ^ static_cast<std::uint8_t>(l[2]);
            b1 = q1(b1) ^ static_cast<std::uint8_t>(l[2] >> 8);
            b2 = q0(b2) ^ static_cast<std::uint8_t>(l[2] >> 16);
            b3 = q0(b3) ^ static_cast<std::uint8_t>(l[2] >> 24);
        }
        if (k >= 2)
        {
            b0 = q1(q0(q0(b0) ^ static_cast<std::uint8_t>(l[1])) ^ static_cast<std::uint8_t>(l[0]));
            b1 = q0(q0(q1(b1) ^ static_cast<std::uint8_t>(l[1] >> 8)) ^ static_cast<std::uint8_t>(l[0] >> 8));
            b2 = q1(q1(q0(b2) ^ static_cast<std::uint8_t>(l[1] >> 16)) ^ static_cast<std::uint8_t>(l[0] >> 16));
            b3 = q0(q1(q1(b3) ^ static_cast<std::uint8_t>(l[1] >> 24)) ^ static_cast<std::uint8_t>(l[0] >> 24));
        }

        return mds_multiply(b0, b1, b2, b3);
    }

public:
    void setup_round_keys(const std::vector<std::uint8_t>& keys) override
    {
        size_t key_len = keys.size();
        if (key_len != 16 && key_len != 24 && key_len != 32)
        {
            throw std::invalid_argument("Invalid key size");
        }

        size_t k = key_len / 8;
        std::vector<std::uint32_t> m_even(k);
        std::vector<std::uint32_t> m_odd(k);
        s_box_keys_.resize(k);

        for (size_t i = 0; i < k; ++i)
        {
            std::uint32_t m0 = static_cast<std::uint32_t>(keys[8 * i]) |
                               (static_cast<std::uint32_t>(keys[8 * i + 1]) << 8) |
                               (static_cast<std::uint32_t>(keys[8 * i + 2]) << 16) |
                               (static_cast<std::uint32_t>(keys[8 * i + 3]) << 24);

            std::uint32_t m1 = static_cast<std::uint32_t>(keys[8 * i + 4]) |
                               (static_cast<std::uint32_t>(keys[8 * i + 5]) << 8) |
                               (static_cast<std::uint32_t>(keys[8 * i + 6]) << 16) |
                               (static_cast<std::uint32_t>(keys[8 * i + 7]) << 24);

            m_even[i] = m0;
            m_odd[i] = m1;

            std::vector<std::uint8_t> rs_block(keys.begin() + 8 * i, keys.begin() + 8 * i + 8);
            s_box_keys_[k - 1 - i] = rs_multiply(rs_block);
        }

        round_keys_.resize(40);
        for (size_t i = 0; i < 20; ++i)
        {
            std::uint32_t a = h_func(static_cast<std::uint32_t>(2 * i) * 0x01010101, m_even);
            std::uint32_t b = rol32(h_func(static_cast<std::uint32_t>(2 * i + 1) * 0x01010101, m_odd), 8);

            round_keys_[2 * i] = a + b;
            round_keys_[2 * i + 1] = rol32(a + 2 * b, 9);
        }
    }

    std::vector<std::uint8_t> encrypt_block(const std::vector<std::uint8_t>& plain_text) override
    {
        if (plain_text.size() != 16)
        {
            throw std::invalid_argument("Invalid block size");
        }

        std::uint32_t p[4];
        for (int i = 0; i < 4; ++i)
        {
            p[i] = static_cast<std::uint32_t>(plain_text[4 * i]) |
                   (static_cast<std::uint32_t>(plain_text[4 * i + 1]) << 8) |
                   (static_cast<std::uint32_t>(plain_text[4 * i + 2]) << 16) |
                   (static_cast<std::uint32_t>(plain_text[4 * i + 3]) << 24);
            p[i] ^= round_keys_[i];
        }

        for (int r = 0; r < 16; ++r)
        {
            std::uint32_t t0 = h_func(p[0], s_box_keys_);
            std::uint32_t t1 = h_func(rol32(p[1], 8), s_box_keys_);

            std::uint32_t f0 = t0 + t1 + round_keys_[8 + 2 * r];
            std::uint32_t f1 = t0 + 2 * t1 + round_keys_[8 + 2 * r + 1];

            p[2] = ror32(p[2] ^ f0, 1);
            p[3] = rol32(p[3], 1) ^ f1;

            if (r < 15)
            {
                std::swap(p[0], p[2]);
                std::swap(p[1], p[3]);
            }
        }

        p[2] ^= round_keys_[4];
        p[3] ^= round_keys_[5];
        p[0] ^= round_keys_[6];
        p[1] ^= round_keys_[7];

        std::vector<std::uint8_t> cipher_text(16);
        for (int i = 0; i < 2; ++i)
        {
            cipher_text[4 * i] = static_cast<std::uint8_t>(p[i + 2]);
            cipher_text[4 * i + 1] = static_cast<std::uint8_t>(p[i + 2] >> 8);
            cipher_text[4 * i + 2] = static_cast<std::uint8_t>(p[i + 2] >> 16);
            cipher_text[4 * i + 3] = static_cast<std::uint8_t>(p[i + 2] >> 24);

            cipher_text[4 * (i + 2)] = static_cast<std::uint8_t>(p[i]);
            cipher_text[4 * (i + 2) + 1] = static_cast<std::uint8_t>(p[i] >> 8);
            cipher_text[4 * (i + 2) + 2] = static_cast<std::uint8_t>(p[i] >> 16);
            cipher_text[4 * (i + 2) + 3] = static_cast<std::uint8_t>(p[i] >> 24);
        }

        return cipher_text;
    }

    std::vector<std::uint8_t> decrypt_block(const std::vector<std::uint8_t>& cipher_text) override
    {
        if (cipher_text.size() != 16)
        {
            throw std::invalid_argument("Invalid block size");
        }

        std::uint32_t tmp[4];
        for (int i = 0; i < 4; ++i)
        {
            tmp[i] = static_cast<std::uint32_t>(cipher_text[4 * i]) |
                     (static_cast<std::uint32_t>(cipher_text[4 * i + 1]) << 8) |
                     (static_cast<std::uint32_t>(cipher_text[4 * i + 2]) << 16) |
                     (static_cast<std::uint32_t>(cipher_text[4 * i + 3]) << 24);
        }

        std::uint32_t p[4];
        p[2] = tmp[0] ^ round_keys_[4];
        p[3] = tmp[1] ^ round_keys_[5];
        p[0] = tmp[2] ^ round_keys_[6];
        p[1] = tmp[3] ^ round_keys_[7];

        for (int r = 15; r >= 0; --r)
        {
            std::uint32_t t0 = h_func(p[0], s_box_keys_);
            std::uint32_t t1 = h_func(rol32(p[1], 8), s_box_keys_);

            std::uint32_t f0 = t0 + t1 + round_keys_[8 + 2 * r];
            std::uint32_t f1 = t0 + 2 * t1 + round_keys_[8 + 2 * r + 1];

            p[2] = rol32(p[2], 1) ^ f0;
            p[3] = ror32(p[3] ^ f1, 1);

            if (r > 0)
            {
                std::swap(p[0], p[2]);
                std::swap(p[1], p[3]);
            }
        }

        p[0] ^= round_keys_[0];
        p[1] ^= round_keys_[1];
        p[2] ^= round_keys_[2];
        p[3] ^= round_keys_[3];

        std::vector<std::uint8_t> plain_text(16);
        for (int i = 0; i < 4; ++i)
        {
            plain_text[4 * i] = static_cast<std::uint8_t>(p[i]);
            plain_text[4 * i + 1] = static_cast<std::uint8_t>(p[i] >> 8);
            plain_text[4 * i + 2] = static_cast<std::uint8_t>(p[i] >> 16);
            plain_text[4 * i + 3] = static_cast<std::uint8_t>(p[i] >> 24);
        }

        return plain_text;
    }
};

#endif