#ifndef DES_ALGORITHM_H
#define DES_ALGORITHM_H

#include "crypto_interfaces.hpp"
#include "feistel_template.hpp"
#include "des_tables.hpp"
#include "bit_permutation.hpp"
#include "bit_manipulator.hpp"
#include <vector>
#include <cstdint>
#include <stdexcept>

class des_algorithm : public feistel_template
{
private:
    // ========================================================================
    // 1.a: Реализация расширения ключа DES
    // ========================================================================
    class DESKeyExpander : public I_key_expander
    {
    private:
        bit_manipulator bit_manip;
        bit_permutation bit_perm;

        std::vector<uint8_t> rotate_left_28(const std::vector<uint8_t>& half, size_t shifts) const
        {
            shifts %= 28;
            if (shifts == 0) return half;
            std::vector<uint8_t> res(4, 0);
            for (int i = 0; i < 28; ++i) {
                int src = (i + shifts) % 28;  // бит из позиции src переходит в i
                if (bit_manip.get_bit_msb0(half, src))
                    bit_manip.set_bit_msb0(res, i, true);
            }
            return res;
        }
        
    public:
        std::vector<std::vector<uint8_t>> expand_key(
            const std::vector<uint8_t>& input_key) const override
        {
            using namespace des_tables;
            
            if (input_key.size() != 8)
                throw std::invalid_argument("DES key must be exactly 8 bytes");
            
            // PC-1: 64 -> 56 бит
            std::vector<uint8_t> permuted_key = bit_perm.permute(
                input_key, PC1, bit_indexing_order::msb0, bit_index_base::base1
            );
            
            // C и D (28 бит)
            std::vector<uint8_t> C = bit_perm.permute(
                permuted_key, SPLIT_C, bit_indexing_order::msb0, bit_index_base::base1
            );
            std::vector<uint8_t> D = bit_perm.permute(
                permuted_key, SPLIT_D, bit_indexing_order::msb0, bit_index_base::base1
            );
            
            std::vector<std::vector<uint8_t>> round_keys;
            round_keys.reserve(16);
            
            for (int round = 0; round < 16; ++round)
            {
                size_t shift = DES_KEY_SHIFTS[round];
                C = rotate_left_28(C, shift);
                D = rotate_left_28(D, shift);
                
                std::vector<uint8_t> combined(8, 0);
                std::copy(C.begin(), C.end(), combined.begin());
                std::copy(D.begin(), D.end(), combined.begin() + 4);
                
                std::vector<uint8_t> CD = bit_perm.permute(
                    combined, COMPACT_CD, bit_indexing_order::msb0, bit_index_base::base1
                );
                
                // PC-2: 56 -> 48 бит
                std::vector<uint8_t> round_key = bit_perm.permute(
                    CD, PC2, bit_indexing_order::msb0, bit_index_base::base1
                );
                
                round_keys.push_back(round_key);
            }
            
            return round_keys;
        }
    };
    
    // ========================================================================
    // 1.b: Реализация раундовой функции DES (F-функция)
    // ========================================================================
    class DESRoundFunction : public I_feistel_round_function
    {
    private:
        bit_manipulator bit_manip;
        bit_permutation bit_perm;

        std::vector<uint8_t> apply_sboxes(const std::vector<uint8_t>& data) const
        {
            using namespace des_tables;
            
            std::vector<uint8_t> result(4, 0);
            
            for (int sbox_idx = 0; sbox_idx < 8; ++sbox_idx)
            {
                size_t start_bit = sbox_idx * 6;
                uint8_t six_bit_value = 0;
                
                for (int bit = 0; bit < 6; ++bit)
                {
                    if (bit_manip.get_bit_msb0(data, start_bit + bit))
                    {
                        six_bit_value |= (1 << (5 - bit));
                    }
                }
                
                const std::vector<uint8_t>& sbox = SBOXES[sbox_idx];
                
                uint8_t row = ((six_bit_value & 0x20) >> 4) | (six_bit_value & 0x01);
                uint8_t col = (six_bit_value >> 1) & 0x0F;
                
                if (row > 3 || col > 15) {
                    throw std::runtime_error("Invalid S-box index");
                }
                
                uint8_t sbox_output = sbox[row * 16 + col];
                
                for (int bit = 0; bit < 4; ++bit)
                {
                    if (sbox_output & (1 << (3 - bit)))
                    {
                        size_t result_bit_pos = sbox_idx * 4 + bit;
                        bit_manip.set_bit_msb0(result, result_bit_pos, true);
                    }
                }
            }
            
            return result;
        }
        
    public:
        std::vector<uint8_t> feistel_round(
            const std::vector<uint8_t>& input_block,
            const std::vector<uint8_t>& round_key) const override
        {
            using namespace des_tables;
            
            if (input_block.size() != 4)
            {
                throw std::invalid_argument("Input block must be 32 bits for DES round function");
            }
            if (round_key.size() != 6)
            {
                throw std::invalid_argument("Round key must be 48 bits for DES round function");
            }
            
            // E: 32 -> 48 бит
            std::vector<uint8_t> expanded = bit_perm.permute(
                input_block,
                std::vector<int>(E.begin(), E.end()),
                bit_indexing_order::msb0,
                bit_index_base::base1
            );
            
            if (expanded.size() != 6)
                throw std::runtime_error("E-permutation output size mismatch");
            
            std::vector<uint8_t> xored(6, 0);
            for (size_t i = 0; i < 6; ++i)
            {
                xored[i] = expanded[i] ^ round_key[i];
            }
            
            // S-блок (48 -> 32 бит)
            std::vector<uint8_t> substituted = apply_sboxes(xored);
            
            // Перестановка P
            std::vector<uint8_t> result = bit_perm.permute(
                substituted,
                std::vector<int>(P.begin(), P.end()),
                bit_indexing_order::msb0,
                bit_index_base::base1
            );
            
            if (result.size() != 4)
                throw std::runtime_error("expanded key result size mismatch");
            
            return result;
        }
    };
    
    // ========================================================================

    std::vector<uint8_t> apply_initial_permutation(const std::vector<uint8_t>& data) const
    {
        using namespace des_tables;
        
        auto result = bit_perm_.permute(
            data,
            std::vector<int>(IP.begin(), IP.end()),
            bit_indexing_order::msb0,
            bit_index_base::base1
        );
        
        return result;
    }

    std::vector<uint8_t> apply_final_permutation(const std::vector<uint8_t>& data) const
    {
        using namespace des_tables;
        
        auto result = bit_perm_.permute(
            data,
            std::vector<int>(FP.begin(), FP.end()),
            bit_indexing_order::msb0,
            bit_index_base::base1
        );
        return result;
    }
    
private:
    DESKeyExpander key_expander_;
    DESRoundFunction round_function_;
    feistel_network* feistel_network_ptr_;
    
    bit_permutation bit_perm_;
    bit_manipulator bit_manip_;

public:
    // ========================================================================
    des_algorithm()
        : feistel_template(nullptr), 
          key_expander_(),
          round_function_(),
          feistel_network_ptr_(nullptr),
          bit_perm_(),
          bit_manip_()
    {
        feistel_network_ptr_ = new feistel_network(
            &key_expander_,
            &round_function_,
            16,
            8
        );
        
        this->set_feistel_network(feistel_network_ptr_);
    }
    
    ~des_algorithm()
    {
        delete feistel_network_ptr_;
        this->feistel_network_ = nullptr;
    }
    
    des_algorithm(const des_algorithm&) = delete;
    des_algorithm& operator=(const des_algorithm&) = delete;
    
    // ========================================================================
    
    std::vector<uint8_t> pre_encrypt(const std::vector<uint8_t>& data) override
    {
        if (data.size() != 8)
        {
            throw std::invalid_argument("Data must be exactly 8 bytes for DES encryption");
        }
        return apply_initial_permutation(data);
    }
    
    std::vector<uint8_t> post_encrypt(const std::vector<uint8_t>& data) override
    {
        if (data.size() != 8)
        {
            throw std::invalid_argument("Data must be exactly 8 bytes for DES encryption");
        }
        return apply_final_permutation(data);
    }
    
    std::vector<uint8_t> pre_decrypt(const std::vector<uint8_t>& data) override
    {
        if (data.size() != 8)
        {
            throw std::invalid_argument("Data must be exactly 8 bytes for DES decryption");
        }
        return apply_initial_permutation(data);
    }

    std::vector<uint8_t> post_decrypt(const std::vector<uint8_t>& data) override
    {
        if (data.size() != 8)
        {
            throw std::invalid_argument("Data must be exactly 8 bytes for DES decryption");
        }
        return apply_final_permutation(data);
    }
    
    std::vector<uint8_t> pre_key_setup(const std::vector<uint8_t>& key) override
    {
        return key;
    }
};

#endif // DES_ALGORITHM_H