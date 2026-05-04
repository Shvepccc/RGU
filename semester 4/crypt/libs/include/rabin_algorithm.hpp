#ifndef RABIN_ALGORITHM_HPP
#define RABIN_ALGORITHM_HPP

#include <vector>
#include <cstdint>
#include <random>
#include <algorithm>
#include <stdexcept>
#include <boost/multiprecision/cpp_int.hpp>
#include "modular_arithmetic.hpp"
#include "probability_test/probabilistic_primality_tests_realisations.hpp"
#include "paddings/padding_interface.hpp"

class rabin_cryptosystem
{
private:
    bigint p_factor;
    bigint q_factor;
    bigint n_public;
    std::size_t key_bytes;
    bigint B;
    const I_padding& padding_handler;

    bigint generate_safe_prime(std::size_t bits)
    {
        miller_rabin_test mr_test;
        std::mt19937_64 gen(std::random_device{}());
        
        bigint lower = bigint(1) << (bits - 1);
        bigint upper = (bigint(1) << bits) - 1;
        boost::random::uniform_int_distribution<bigint> dist(lower, upper);

        while (true)
        {
            bigint candidate = dist(gen);
            if (candidate % 4 == 3 && mr_test.is_prime(candidate, 0.999))
            {
                return candidate;
            }
        }
    }

    std::vector<uint8_t> bigint_to_bytes(const bigint& num, std::size_t target_size)
    {
        std::vector<uint8_t> bytes;
        boost::multiprecision::export_bits(num, std::back_inserter(bytes), 8, true);
        
        if (bytes.size() < target_size)
        {
            bytes.insert(bytes.begin(), target_size - bytes.size(), 0);
        }
        else if (bytes.size() > target_size)
        {
            throw std::runtime_error("overflow in conversion");
        }
        
        return bytes;
    }
    
    bigint bytes_to_bigint(const std::vector<uint8_t>& bytes)
    {
        bigint result;
        boost::multiprecision::import_bits(result, bytes.begin(), bytes.end(), 8, true);
        return result;
    }

public:
    rabin_cryptosystem(std::size_t key_bits, const I_padding& padding, const bigint& b = 0)
        : padding_handler(padding), B(b)
    {
        if (key_bits % 8 != 0)
        {
            throw std::invalid_argument("key_bits must be multiple of 8");
        }
        
        p_factor = generate_safe_prime(key_bits / 2);
        q_factor = generate_safe_prime(key_bits / 2);

        while (p_factor == q_factor)
        {
            q_factor = generate_safe_prime(key_bits / 2);
        }

        n_public = p_factor * q_factor;
        key_bytes = key_bits / 8;
    }

    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data)
{
    std::vector<uint8_t> encrypted_result;
    std::size_t target_padded_size = key_bytes - 1;
    // 2 байта длины + минимум 1 байт паддинга
    std::size_t max_chunk_size = target_padded_size - 3;
    if (max_chunk_size < 1)
        throw std::runtime_error("key too small for required overhead");

    for (std::size_t i = 0; i < data.size(); i += max_chunk_size)
    {
        std::size_t current_len = std::min(max_chunk_size, data.size() - i);
        std::vector<uint8_t> chunk(data.begin() + i, data.begin() + i + current_len);

        // Добавляем два байта длины (big-endian)
        std::vector<uint8_t> enhanced_chunk;
        enhanced_chunk.push_back(static_cast<uint8_t>((current_len >> 8) & 0xFF));
        enhanced_chunk.push_back(static_cast<uint8_t>(current_len & 0xFF));
        enhanced_chunk.insert(enhanced_chunk.end(), chunk.begin(), chunk.end());

        std::vector<uint8_t> padded_block = padding_handler.pad(enhanced_chunk, target_padded_size);
        bigint m = bytes_to_bigint(padded_block);
        bigint c = (m * (m + B)) % n_public;
        std::vector<uint8_t> c_bytes = bigint_to_bytes(c, key_bytes);
        encrypted_result.insert(encrypted_result.end(), c_bytes.begin(), c_bytes.end());
    }
    return encrypted_result;
}

std::vector<uint8_t> decrypt(const std::vector<uint8_t>& encrypted_data)
{
    if (encrypted_data.size() % key_bytes != 0)
        throw std::runtime_error("invalid ciphertext size");

    std::vector<uint8_t> decrypted_result;
    std::size_t target_padded_size = key_bytes - 1;

    bigint inv_q_mod_p = mod_inverse(q_factor, p_factor);
    bigint inv_p_mod_q = mod_inverse(p_factor, q_factor);
    bigint inv_2 = mod_inverse(2, n_public);   // вычисляем один раз

    for (std::size_t i = 0; i < encrypted_data.size(); i += key_bytes)
    {
        std::vector<uint8_t> block(encrypted_data.begin() + i, encrypted_data.begin() + i + key_bytes);
        bigint c = bytes_to_bigint(block);

        bigint D = (B * B + 4 * c) % n_public;

        bigint D_mod_p = D % p_factor;
        bigint D_mod_q = D % q_factor;

        bigint sqrt_D_p = mod_pow(D_mod_p, (p_factor + 1) / 4, p_factor);
        bigint sqrt_D_q = mod_pow(D_mod_q, (q_factor + 1) / 4, q_factor);

        bigint sqrt1 = (sqrt_D_p * q_factor % n_public * inv_q_mod_p +
                        sqrt_D_q * p_factor % n_public * inv_p_mod_q) % n_public;
        bigint sqrt2 = n_public - sqrt1;
        bigint sqrt3 = (sqrt_D_p * q_factor % n_public * inv_q_mod_p -
                        sqrt_D_q * p_factor % n_public * inv_p_mod_q) % n_public;
        if (sqrt3 < 0) sqrt3 += n_public;
        bigint sqrt4 = n_public - sqrt3;

        bigint sqrt_D_values[4] = {sqrt1, sqrt2, sqrt3, sqrt4};

        bool found = false;
        for (int j = 0; j < 4 && !found; ++j)
        {
            bigint r = sqrt_D_values[j];
            bigint m1 = ((-B + r) % n_public);
            if (m1 < 0) m1 += n_public;
            m1 = (m1 * inv_2) % n_public;

            bigint m2 = ((-B - r) % n_public);
            if (m2 < 0) m2 += n_public;
            m2 = (m2 * inv_2) % n_public;

            bigint candidates[2] = {m1, m2};

            for (int k = 0; k < 2 && !found; ++k)
            {
                if (boost::multiprecision::msb(candidates[k]) >= target_padded_size * 8)
                    continue;

                try
                {
                    std::vector<uint8_t> candidate_bytes = bigint_to_bytes(candidates[k], target_padded_size);
                    std::vector<uint8_t> unpadded = padding_handler.unpad(candidate_bytes, target_padded_size);

                    // Проверяем двухбайтовую метку длины
                    if (unpadded.size() < 2) continue;
                    uint16_t expected_len = (static_cast<uint16_t>(unpadded[0]) << 8) | unpadded[1];
                    if (expected_len != unpadded.size() - 2) continue;

                    // Успех – извлекаем исходные данные без метки
                    decrypted_result.insert(decrypted_result.end(),
                                            unpadded.begin() + 2, unpadded.end());
                    found = true;
                }
                catch (...)
                {
                    continue;
                }
            }
        }

        if (!found)
            throw std::runtime_error("decryption failed: no valid root found");
    }
    return decrypted_result;
}
};

#endif