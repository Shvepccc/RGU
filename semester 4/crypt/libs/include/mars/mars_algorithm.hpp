#ifndef MARS_ALGORITHM_H_
#define MARS_ALGORITHM_H_

#include <vector>
#include <cstdint>
#include "../crypto_interfaces.hpp"
#include "mars_data.hpp"

class mars_cipher : public I_feistel_chipher
{
private:
    std::vector<std::uint32_t> ek;

    inline std::uint32_t rotl(std::uint32_t x, int n) const 
    { 
        return (x << (n & 31)) | (x >> ((32 - n) & 31)); 
    }
    inline std::uint32_t rotr(std::uint32_t x, int n) const 
    { 
        return (x >> (n & 31)) | (x << ((32 - n) & 31)); 
    }

    void e_function(std::uint32_t in, std::uint32_t k_mul, std::uint32_t k_add, 
                    std::uint32_t& out_l, std::uint32_t& out_m, std::uint32_t& out_r) const 
    {
        std::uint32_t m = in + k_add;
        std::uint32_t r = rotl(in, 13) * k_mul;
        std::uint32_t s = mars_s_box[m & 0x1FF];
        
        out_m = rotl(r, 5);
        out_l = rotl(s, r & 0x1F);
        out_r = rotl(m, out_m & 0x1F);
    }

    void forward_mixing(std::uint32_t d[4]) const 
    {
        for (int i = 0; i < 8; ++i) 
        {
            d[1] ^= mars_s_box[d[0] & 0xFF];
            d[1] += mars_s_box[((d[0] >> 8) & 0xFF) + 256];
            d[2] += mars_s_box[(d[0] >> 16) & 0xFF];
            d[3] ^= mars_s_box[((d[0] >> 24) & 0xFF) + 256];
            d[0] = rotr(d[0], 24);
            d[0] += d[3]; d[2] ^= d[0];
            std::uint32_t t = d[0]; d[0] = d[1]; d[1] = d[2]; d[2] = d[3]; d[3] = t;
        }
    }

    void inv_forward_mixing(std::uint32_t d[4]) const 
    {
        for (int i = 0; i < 8; ++i) 
        {
            std::uint32_t t = d[3]; d[3] = d[2]; d[2] = d[1]; d[1] = d[0]; d[0] = t;
            d[2] ^= d[0]; d[0] -= d[3];
            d[0] = rotl(d[0], 24);
            d[3] ^= mars_s_box[((d[0] >> 24) & 0xFF) + 256];
            d[2] -= mars_s_box[(d[0] >> 16) & 0xFF];
            d[1] -= mars_s_box[((d[0] >> 8) & 0xFF) + 256];
            d[1] ^= mars_s_box[d[0] & 0xFF];
        }
    }

    void backward_mixing(std::uint32_t d[4]) const 
    {
        for (int i = 0; i < 8; ++i) 
        {
            d[2] ^= d[0]; d[0] -= d[3]; d[0] = rotr(d[0], 24);
            d[3] ^= mars_s_box[((d[0] >> 24) & 0xFF) + 256];
            d[2] -= mars_s_box[(d[0] >> 16) & 0xFF];
            d[1] -= mars_s_box[((d[0] >> 8) & 0xFF) + 256];
            d[1] ^= mars_s_box[d[0] & 0xFF];
            std::uint32_t t = d[0]; d[0] = d[1]; d[1] = d[2]; d[2] = d[3]; d[3] = t;
        }
    }

    void inv_backward_mixing(std::uint32_t d[4]) const 
    {
        for (int i = 0; i < 8; ++i) 
        {
            std::uint32_t t = d[3]; d[3] = d[2]; d[2] = d[1]; d[1] = d[0]; d[0] = t;
            d[1] ^= mars_s_box[d[0] & 0xFF];
            d[1] += mars_s_box[((d[0] >> 8) & 0xFF) + 256];
            d[2] += mars_s_box[(d[0] >> 16) & 0xFF];
            d[3] ^= mars_s_box[((d[0] >> 24) & 0xFF) + 256];
            d[0] = rotl(d[0], 24);
            d[0] += d[3]; d[2] ^= d[0];
        }
    }

public:
    void setup_round_keys(const std::vector<std::uint8_t>& key) override 
    {
        ek.assign(40, 0);
        int n = key.size() / 4;
        std::vector<std::uint32_t> T(15, 0);
        for (int i = 0; i < n; ++i) 
            T[i] = (key[4*i]) | (key[4*i+1] << 8) | (key[4*i+2] << 16) | (key[4*i+3] << 24);
        
        for (int j = 0; j < 4; ++j) 
        {
            for (int i = 0; i < 15; ++i) 
            {
                T[i] ^= rotl(T[(i+8)%15] ^ T[(i+13)%15], 3) ^ (4*j + i);
            }
        }
        for (int i = 0; i < 40; ++i) 
        {
            ek[i] = T[i % 15] ^ mars_s_box[T[i % 15] & 0x1FF];
            if (i >= 5 && i <= 35 && (i % 2 != 0)) ek[i] |= 0x03; 
        }
    }

    std::vector<std::uint8_t> encrypt_block(const std::vector<std::uint8_t>& p) override 
    {
        std::uint32_t d[4];
        for (int i = 0; i < 4; ++i) 
        {
            d[i] = (p[4*i]) | (p[4*i+1] << 8) | (p[4*i+2] << 16) | (p[4*i+3] << 24);
            d[i] += ek[i];
        }

        forward_mixing(d);

        for (int i = 0; i < 16; ++i) 
        {
            std::uint32_t L, M, R;
            e_function(d[0], ek[2*i + 5], ek[2*i + 4], L, M, R);
            d[0] = rotl(d[0], 13);
            if (i < 8) { d[1] += L; d[2] += M; d[3] ^= R; } 
            else { d[3] += L; d[1] += M; d[2] ^= R; }
            std::uint32_t t = d[0]; d[0] = d[1]; d[1] = d[2]; d[2] = d[3]; d[3] = t;
        }

        backward_mixing(d);

        std::vector<std::uint8_t> res(16);
        for (int i = 0; i < 4; ++i) 
        {
            d[i] -= ek[36 + i];
            res[4*i] = d[i] & 0xFF; res[4*i+1] = (d[i] >> 8) & 0xFF;
            res[4*i+2] = (d[i] >> 16) & 0xFF; res[4*i+3] = (d[i] >> 24) & 0xFF;
        }
        return res;
    }

    std::vector<std::uint8_t> decrypt_block(const std::vector<std::uint8_t>& c) override 
    {
        std::uint32_t d[4];
        for (int i = 0; i < 4; ++i) 
        {
            d[i] = (c[4*i]) | (c[4*i+1] << 8) | (c[4*i+2] << 16) | (c[4*i+3] << 24);
            d[i] += ek[36 + i];
        }

        inv_backward_mixing(d);

        for (int i = 15; i >= 0; --i) 
        {
            std::uint32_t t = d[3]; d[3] = d[2]; d[2] = d[1]; d[1] = d[0]; d[0] = t;
            std::uint32_t L, M, R;
            std::uint32_t d0_orig = rotr(d[0], 13);
            e_function(d0_orig, ek[2*i + 5], ek[2*i + 4], L, M, R);
            if (i < 8) { d[1] -= L; d[2] -= M; d[3] ^= R; } 
            else { d[3] -= L; d[1] -= M; d[2] ^= R; }
            d[0] = d0_orig;
        }

        inv_forward_mixing(d);

        std::vector<std::uint8_t> res(16);
        for (int i = 0; i < 4; ++i) 
        {
            d[i] -= ek[i];
            res[4*i] = d[i] & 0xFF; res[4*i+1] = (d[i] >> 8) & 0xFF;
            res[4*i+2] = (d[i] >> 16) & 0xFF; res[4*i+3] = (d[i] >> 24) & 0xFF;
        }
        return res;
    }
};

#endif