#ifndef RIJNDAEL_HPP
#define RIJNDAEL_HPP

#include <vector>
#include <cstdint>
#include <stdexcept>
#include "../crypto_interfaces.hpp"
#include "rijndael_sboxes.hpp"
#include "../poly_gf2.hpp"

class rijndael : public I_feistel_chipher
{
private:
    std::size_t block_size_bytes_;
    std::size_t nb_;
    std::size_t nk_;
    std::size_t nr_;
    std::vector<std::vector<std::uint8_t>> round_keys_;

    std::uint8_t gf_mul(std::uint8_t a, std::uint8_t b) const
    {
        poly_gf2 p_a(a);
        poly_gf2 p_b(b);
        poly_gf2 p_mod(0x11B);
        return static_cast<std::uint8_t>(p_a.mul_mod(p_b, p_mod).get_value());
    }

    std::uint8_t rcon(std::size_t i) const
    {
        if (i == 0)
        {
            return 0;
        }
        std::uint8_t res = 1;
        for (std::size_t k = 1; k < i; ++k)
        {
            res = gf_mul(res, 2);
        }
        return res;
    }

    std::vector<std::uint8_t> sub_word(const std::vector<std::uint8_t>& word) const
    {
        std::vector<std::uint8_t> res(4);
        for (std::size_t i = 0; i < 4; ++i)
        {
            res[i] = s_box_[word[i]];
        }
        return res;
    }

    std::vector<std::uint8_t> rot_word(const std::vector<std::uint8_t>& word) const
    {
        std::vector<std::uint8_t> res(4);
        res[0] = word[1];
        res[1] = word[2];
        res[2] = word[3];
        res[3] = word[0];
        return res;
    }

    void add_round_key(std::vector<std::vector<std::uint8_t>>& state, std::size_t round) const
    {
        for (std::size_t c = 0; c < nb_; ++c)
        {
            for (std::size_t r = 0; r < 4; ++r)
            {
                state[r][c] ^= round_keys_[round * nb_ + c][r];
            }
        }
    }

    void sub_bytes(std::vector<std::vector<std::uint8_t>>& state) const
    {
        for (std::size_t r = 0; r < 4; ++r)
        {
            for (std::size_t c = 0; c < nb_; ++c)
            {
                state[r][c] = s_box_[state[r][c]];
            }
        }
    }

    void inv_sub_bytes(std::vector<std::vector<std::uint8_t>>& state) const
    {
        for (std::size_t r = 0; r < 4; ++r)
        {
            for (std::size_t c = 0; c < nb_; ++c)
            {
                state[r][c] = inv_s_box_[state[r][c]];
            }
        }
    }

    void shift_rows(std::vector<std::vector<std::uint8_t>>& state) const
    {
        std::vector<std::vector<std::uint8_t>> temp = state;
        std::size_t c1 = 1;
        std::size_t c2 = (nb_ == 8) ? 3 : 2;
        std::size_t c3 = (nb_ == 8) ? 4 : 3;

        for (std::size_t c = 0; c < nb_; ++c)
        {
            state[1][c] = temp[1][(c + c1) % nb_];
            state[2][c] = temp[2][(c + c2) % nb_];
            state[3][c] = temp[3][(c + c3) % nb_];
        }
    }

    void inv_shift_rows(std::vector<std::vector<std::uint8_t>>& state) const
    {
        std::vector<std::vector<std::uint8_t>> temp = state;
        std::size_t c1 = 1;
        std::size_t c2 = (nb_ == 8) ? 3 : 2;
        std::size_t c3 = (nb_ == 8) ? 4 : 3;

        for (std::size_t c = 0; c < nb_; ++c)
        {
            state[1][c] = temp[1][(c + nb_ - c1) % nb_];
            state[2][c] = temp[2][(c + nb_ - c2) % nb_];
            state[3][c] = temp[3][(c + nb_ - c3) % nb_];
        }
    }

    void mix_columns(std::vector<std::vector<std::uint8_t>>& state) const
    {
        for (std::size_t c = 0; c < nb_; ++c)
        {
            std::uint8_t s0 = state[0][c];
            std::uint8_t s1 = state[1][c];
            std::uint8_t s2 = state[2][c];
            std::uint8_t s3 = state[3][c];

            state[0][c] = gf_mul(0x02, s0) ^ gf_mul(0x03, s1) ^ s2 ^ s3;
            state[1][c] = s0 ^ gf_mul(0x02, s1) ^ gf_mul(0x03, s2) ^ s3;
            state[2][c] = s0 ^ s1 ^ gf_mul(0x02, s2) ^ gf_mul(0x03, s3);
            state[3][c] = gf_mul(0x03, s0) ^ s1 ^ s2 ^ gf_mul(0x02, s3);
        }
    }

    void inv_mix_columns(std::vector<std::vector<std::uint8_t>>& state) const
    {
        for (std::size_t c = 0; c < nb_; ++c)
        {
            std::uint8_t s0 = state[0][c];
            std::uint8_t s1 = state[1][c];
            std::uint8_t s2 = state[2][c];
            std::uint8_t s3 = state[3][c];

            state[0][c] = gf_mul(0x0e, s0) ^ gf_mul(0x0b, s1) ^ gf_mul(0x0d, s2) ^ gf_mul(0x09, s3);
            state[1][c] = gf_mul(0x09, s0) ^ gf_mul(0x0e, s1) ^ gf_mul(0x0b, s2) ^ gf_mul(0x0d, s3);
            state[2][c] = gf_mul(0x0d, s0) ^ gf_mul(0x09, s1) ^ gf_mul(0x0e, s2) ^ gf_mul(0x0b, s3);
            state[3][c] = gf_mul(0x0b, s0) ^ gf_mul(0x0d, s1) ^ gf_mul(0x09, s2) ^ gf_mul(0x0e, s3);
        }
    }

public:
    rijndael(std::size_t block_size_bits = 128)
    {
        if (block_size_bits != 128 && block_size_bits != 192 && block_size_bits != 256)
        {
            throw std::invalid_argument("Invalid block size for Rijndael");
        }
        block_size_bytes_ = block_size_bits / 8;
        nb_ = block_size_bytes_ / 4;
    }

    void setup_round_keys(const std::vector<std::uint8_t>& keys) override
    {
        std::size_t key_len = keys.size();
        if (key_len != 16 && key_len != 24 && key_len != 32)
        {
            throw std::invalid_argument("Invalid key size for Rijndael");
        }
        nk_ = key_len / 4;
        nr_ = std::max(nb_, nk_) + 6;

        std::size_t total_words = nb_ * (nr_ + 1);
        round_keys_.resize(total_words, std::vector<std::uint8_t>(4));

        for (std::size_t i = 0; i < nk_; ++i)
        {
            round_keys_[i][0] = keys[4 * i];
            round_keys_[i][1] = keys[4 * i + 1];
            round_keys_[i][2] = keys[4 * i + 2];
            round_keys_[i][3] = keys[4 * i + 3];
        }

        for (std::size_t i = nk_; i < total_words; ++i)
        {
            std::vector<std::uint8_t> temp = round_keys_[i - 1];
            if (i % nk_ == 0)
            {
                temp = sub_word(rot_word(temp));
                temp[0] ^= rcon(i / nk_);
            }
            else if (nk_ > 6 && i % nk_ == 4)
            {
                temp = sub_word(temp);
            }
            for (std::size_t j = 0; j < 4; ++j)
            {
                round_keys_[i][j] = round_keys_[i - nk_][j] ^ temp[j];
            }
        }
    }

    std::vector<std::uint8_t> encrypt_block(const std::vector<std::uint8_t>& plain_text) override
    {
        if (plain_text.size() != block_size_bytes_)
        {
            throw std::invalid_argument("Plain text size does not match block size");
        }

        std::vector<std::vector<std::uint8_t>> state(4, std::vector<std::uint8_t>(nb_));
        for (std::size_t r = 0; r < 4; ++r)
        {
            for (std::size_t c = 0; c < nb_; ++c)
            {
                state[r][c] = plain_text[r + 4 * c];
            }
        }

        add_round_key(state, 0);

        for (std::size_t round = 1; round < nr_; ++round)
        {
            sub_bytes(state);
            shift_rows(state);
            mix_columns(state);
            add_round_key(state, round);
        }

        sub_bytes(state);
        shift_rows(state);
        add_round_key(state, nr_);

        std::vector<std::uint8_t> cipher_text(block_size_bytes_);
        for (std::size_t r = 0; r < 4; ++r)
        {
            for (std::size_t c = 0; c < nb_; ++c)
            {
                cipher_text[r + 4 * c] = state[r][c];
            }
        }
        return cipher_text;
    }

    std::vector<std::uint8_t> decrypt_block(const std::vector<std::uint8_t>& chipher_text) override
    {
        if (chipher_text.size() != block_size_bytes_)
        {
            throw std::invalid_argument("Cipher text size does not match block size");
        }

        std::vector<std::vector<std::uint8_t>> state(4, std::vector<std::uint8_t>(nb_));
        for (std::size_t r = 0; r < 4; ++r)
        {
            for (std::size_t c = 0; c < nb_; ++c)
            {
                state[r][c] = chipher_text[r + 4 * c];
            }
        }

        add_round_key(state, nr_);

        for (std::size_t round = nr_ - 1; round > 0; --round)
        {
            inv_shift_rows(state);
            inv_sub_bytes(state);
            add_round_key(state, round);
            inv_mix_columns(state);
        }

        inv_shift_rows(state);
        inv_sub_bytes(state);
        add_round_key(state, 0);

        std::vector<std::uint8_t> plain_text(block_size_bytes_);
        for (std::size_t r = 0; r < 4; ++r)
        {
            for (std::size_t c = 0; c < nb_; ++c)
            {
                plain_text[r + 4 * c] = state[r][c];
            }
        }
        return plain_text;
    }
};

#endif