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
    // Вложенный класс 1.a: Реализация расширения ключа DES
    // ========================================================================
    class DESKeyExpander : public I_key_expander
    {
    private:
        bit_manipulator bit_manip;
        bit_permutation bit_perm;

    public:
        bool test_bit_permutation()
        {
            // Тест: перестановка одного байта с сохранением порядка
            std::vector<uint8_t> data = {0x0F}; // 00001111 в MSB0: биты 0-3=0, 4-7=1
            
            // Правило: взять биты в обратном порядке
            std::vector<int> rule = {8,7,6,5,4,3,2,1}; // MSB0, base 1
            
            bit_permutation perm;
            auto result = perm.permute(data, rule, 
                                    bit_indexing_order::msb0, 
                                    bit_index_base::base1);
            
            // Ожидаем: 11110000 = 0xF0
            return (result.size() == 1 && result[0] == 0xF0);
        }
    private:
        // std::vector<uint8_t> rotate_left_28(const std::vector<uint8_t>& half, size_t shifts) const
        // {
        //     uint32_t val = 0;
        //     for (int i = 0; i < 28; ++i) {
        //         if (bit_manip.get_bit_msb0(half, i))
        //             val |= (1 << (27 - i)); // бит i (MSB0) становится битом (27-i) числа (LSB0 числа)
        //     }
        //     val = ((val << shifts) | (val >> (28 - shifts))) & 0x0FFFFFFF;
        //     // Обратно в вектор
        //     std::vector<uint8_t> res(4, 0);
        //     for (int i = 0; i < 28; ++i) {
        //         if (val & (1 << (27 - i)))
        //             bit_manip.set_bit_msb0(res, i, true);
        //     }
        //     return res;
        // }

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
            
            // Разделяем на C и D (по 28 бит) с помощью SPLIT таблиц
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
                // Циклический сдвиг влево
                size_t shift = DES_KEY_SHIFTS[round];
                C = rotate_left_28(C, shift);
                D = rotate_left_28(D, shift);
                
                // Объединяем C и D через COMPACT_CD
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
    // Вложенный класс 1.b: Реализация раундовой функции DES (F-функция)
    // ========================================================================
    class DESRoundFunction : public I_feistel_round_function
    {
    private:
        bit_manipulator bit_manip;
        bit_permutation bit_perm;
        
        /**
         * @brief Преобразует 48-битный вход в 32-битный выход через S-блоки
         */
        // std::vector<uint8_t> apply_sboxes(const std::vector<uint8_t>& data) const
        // {
        //     using namespace des_tables;
            
        //     std::vector<uint8_t> result(4, 0);
            
        //     for (int sbox_idx = 0; sbox_idx < 8; ++sbox_idx)
        //     {
        //         // Извлекаем 6 бит для текущего S-блока
        //         size_t start_bit = sbox_idx * 6;
        //         uint8_t six_bit_value = 0;
                
        //         // Формируем 6-битное значение в правильном порядке для S-блока
        //         // В DES: биты идут как b1 b2 b3 b4 b5 b6, где:
        //         // b1 и b6 образуют номер строки (0-3)
        //         // b2 b3 b4 b5 образуют номер столбца (0-15)
        //         for (int bit = 0; bit < 6; ++bit)
        //         {
        //             if (bit_manip.get_bit_msb0(data, start_bit + bit))
        //             {
        //                 six_bit_value |= (1 << (5 - bit));
        //             }
        //         }
                
        //         // Получаем S-блок
        //         const std::vector<uint8_t>& sbox = SBOXES[sbox_idx];
                
        //         // Вычисляем строку (биты 0 и 5) и столбец (биты 1-4)
        //         uint8_t row = ((six_bit_value & 0x20) >> 4) | (six_bit_value & 0x01);
        //         uint8_t col = (six_bit_value >> 1) & 0x0F;
        //         uint8_t sbox_output = sbox[row * 16 + col];
                
        //         // Записываем 4 бита результата
        //         for (int bit = 0; bit < 4; ++bit)
        //         {
        //             if (sbox_output & (1 << (3 - bit)))
        //             {
        //                 size_t result_bit_pos = sbox_idx * 4 + bit;
        //                 bit_manip.set_bit_msb0(result, result_bit_pos, true);
        //             }
        //         }
        //     }
            
        //     return result;
        // }

        std::vector<uint8_t> apply_sboxes(const std::vector<uint8_t>& data) const
{
    using namespace des_tables;
    
    std::vector<uint8_t> result(4, 0);
    
    for (int sbox_idx = 0; sbox_idx < 8; ++sbox_idx)
    {
        // Извлекаем 6 бит для текущего S-блока
        size_t start_bit = sbox_idx * 6;
        uint8_t six_bit_value = 0;
        
        // Собираем 6 бит (они уже в правильном порядке MSB0)
        for (int bit = 0; bit < 6; ++bit)
        {
            if (bit_manip.get_bit_msb0(data, start_bit + bit))
            {
                six_bit_value |= (1 << (5 - bit));
            }
        }
        
        // Получаем S-блок
        const std::vector<uint8_t>& sbox = SBOXES[sbox_idx];
        
        // В DES: 
        // - биты 0 и 5 (первый и последний) образуют номер строки
        // - биты 1-4 образуют номер столбца
        uint8_t row = ((six_bit_value & 0x20) >> 4) | (six_bit_value & 0x01);
        uint8_t col = (six_bit_value >> 1) & 0x0F;
        
        // Проверка границ
        if (row > 3 || col > 15) {
            throw std::runtime_error("Invalid S-box index");
        }
        
        uint8_t sbox_output = sbox[row * 16 + col];
        
        // Записываем 4 бита результата (MSB0)
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
            
            // 1. Расширение E: 32 -> 48 бит
            std::vector<uint8_t> expanded = bit_perm.permute(
                input_block,
                std::vector<int>(E.begin(), E.end()),
                bit_indexing_order::msb0,
                bit_index_base::base1
            );
            
            // Убеждаемся, что размер правильный
            if (expanded.size() != 6)
                throw std::runtime_error("E-permutation output size mismatch");
            
            // 2. XOR с раундовым ключом
            std::vector<uint8_t> xored(6, 0);
            for (size_t i = 0; i < 6; ++i)
            {
                xored[i] = expanded[i] ^ round_key[i];
            }
            
            // 3. Применяем S-блоки (48 -> 32 бит)
            std::vector<uint8_t> substituted = apply_sboxes(xored);
            
            // 4. Перестановка P
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
    // Вспомогательные методы для перестановок
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
    // Конструктор
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