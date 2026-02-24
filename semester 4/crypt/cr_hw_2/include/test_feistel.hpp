#ifndef FEISTEL_NETWORK_TEST_H
#define FEISTEL_NETWORK_TEST_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdint>
#include <random>
#include <chrono>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <string>
#include <sstream>

#include "crypto_interfaces.hpp"

// ============================================================================
class test_logger
{
private:
    std::ostream& output_;
    size_t total_tests_;
    size_t passed_tests_;
    std::string current_suite_;
    
    void print_separator(char c = '=', size_t length = 70) const
    {
        output_ << std::string(length, c) << std::endl;
    }
    
public:
    test_logger(std::ostream& out = std::cout)
        : output_(out)
        , total_tests_(0)
        , passed_tests_(0)
        , current_suite_("")
    {
    }
    
    void start_suite(const std::string& suite_name)
    {
        current_suite_ = suite_name;
        print_separator();
        output_ << "▶ " << suite_name << std::endl;
        print_separator('-');
    }
    
    void end_suite()
    {
        output_ << std::endl;
    }
    
    bool check_result(bool condition, const std::string& test_name, 
                      const std::string& details = "")
    {
        total_tests_++;
        
        output_ << "  " << std::setw(50) << std::left << test_name << " : ";
        
        if (condition)
        {
            passed_tests_++;
            output_ << "\033[32mПРОЙДЕН\033[0m";
        }
        else
        {
            output_ << "\033[31mПРОВАЛЕН\033[0m";
        }
        
        if (!details.empty())
        {
            output_ << " [" << details << "]";
        }
        output_ << std::endl;
        
        return condition;
    }
    
    void print_summary() const
    {
        print_separator('=');
        output_ << "ИТОГИ ТЕСТИРОВАНИЯ:" << std::endl;
        output_ << "  Всего тестов: " << total_tests_ << std::endl;
        output_ << "  Пройдено:     " << passed_tests_ << std::endl;
        output_ << "  Провалено:    " << (total_tests_ - passed_tests_) << std::endl;
        
        double success_rate = (total_tests_ > 0) 
            ? (static_cast<double>(passed_tests_) / total_tests_ * 100.0) 
            : 0.0;
        
        output_ << "  Успешность:   " << std::fixed << std::setprecision(2) 
                << success_rate << "%" << std::endl;
        print_separator('=');
    }
    
    size_t get_passed() const { return passed_tests_; }
    size_t get_total() const { return total_tests_; }
};

// ============================================================================

namespace test_utils
{
    inline std::string bytes_to_hex(const std::vector<uint8_t>& data, size_t max_bytes = 0)
    {
        if (data.empty())
        {
            return "[пусто]";
        }
        
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        
        size_t display_bytes = (max_bytes > 0) ? std::min(data.size(), max_bytes) : data.size();
        
        for (size_t i = 0; i < display_bytes; ++i)
        {
            if (i > 0 && i % 16 == 0)
            {
                ss << "\n          ";
            }
            else if (i > 0 && i % 8 == 0)
            {
                ss << " ";
            }
            ss << std::setw(2) << static_cast<int>(data[i]);
        }
        
        if (data.size() > display_bytes)
        {
            ss << "... [" << (data.size() - display_bytes) << " байт пропущено]";
        }
        
        return ss.str();
    }
    
    inline std::vector<uint8_t> random_bytes(size_t size, uint32_t seed = 0)
    {
        std::vector<uint8_t> result(size);
        
        if (seed == 0)
        {
            seed = static_cast<uint32_t>(std::chrono::system_clock::now().time_since_epoch().count());
        }
        
        std::mt19937 rng(seed);
        std::uniform_int_distribution<int> dist(0, 255);
        
        for (size_t i = 0; i < size; ++i)
        {
            result[i] = static_cast<uint8_t>(dist(rng));
        }
        
        return result;
    }
    
    inline bool bytes_equal(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b)
    {
        if (a.size() != b.size())
        {
            return false;
        }
        
        for (size_t i = 0; i < a.size(); ++i)
        {
            if (a[i] != b[i])
            {
                return false;
            }
        }
        
        return true;
    }
    
    inline std::vector<uint8_t> bytes_xor(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b)
    {
        if (a.size() != b.size())
        {
            throw std::invalid_argument("Arrays must have the same size for XOR");
        }
        
        std::vector<uint8_t> result(a.size());
        for (size_t i = 0; i < a.size(); ++i)
        {
            result[i] = a[i] ^ b[i];
        }
        
        return result;
    }
    
    inline size_t count_different_bits(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b)
    {
        if (a.size() != b.size())
        {
            throw std::invalid_argument("Arrays must have the same size");
        }
        
        size_t diff_bits = 0;
        for (size_t i = 0; i < a.size(); ++i)
        {
            uint8_t x = a[i] ^ b[i];
            while (x)
            {
                diff_bits += (x & 1);
                x >>= 1;
            }
        }
        
        return diff_bits;
    }

    inline std::vector<uint8_t> key_from_uint(uint64_t value, size_t size)
    {
        std::vector<uint8_t> key(size);
        for (size_t i = 0; i < size; ++i)
        {
            key[i] = static_cast<uint8_t>((value >> (8 * (size - 1 - i))) & 0xFF);
        }
        return key;
    }
}

// ============================================================================

class simple_key_expander_mock : public I_key_expander
{
private:
    size_t round_keys_count_;
    size_t round_key_size_;
    
public:
    simple_key_expander_mock(size_t rounds = 16, size_t key_size = 8)
        : round_keys_count_(rounds)
        , round_key_size_(key_size)
    {
    }
    
    std::vector<std::vector<uint8_t>> expand_key(
        const std::vector<uint8_t>& input_key) const override
    {
        std::vector<std::vector<uint8_t>> round_keys;
        round_keys.reserve(round_keys_count_);
        
        for (size_t round = 0; round < round_keys_count_; ++round)
        {
            std::vector<uint8_t> round_key(round_key_size_);
            for (size_t i = 0; i < round_key_size_; ++i)
            {
                round_key[i] = input_key[i % input_key.size()] ^ static_cast<uint8_t>(round + i);
            }
            round_keys.push_back(round_key);
        }
        
        return round_keys;
    }
};

class simple_xor_round_mock : public I_feistel_round_function
{
public:
    std::vector<uint8_t> feistel_round(
        const std::vector<uint8_t>& input_block,
        const std::vector<uint8_t>& round_key) const override
    {
        if (input_block.size() != round_key.size())
        {
            throw std::invalid_argument("Block and key must have the same size");
        }
        
        size_t n = input_block.size();
        std::vector<uint8_t> result(n);
        
        uint32_t state = 0;
        for (size_t i = 0; i < n; ++i)
        {
            state = (state << 8) | input_block[i];
        }
        
        uint32_t key_val = 0;
        for (size_t i = 0; i < n; ++i)
        {
            key_val = (key_val << 8) | round_key[i];
        }
        
        state = state ^ key_val;
        state = state * 0x9e3779b9;
        state = state ^ (state >> 16);
        state = state * 0x85ebca6b;
        state = state ^ (state >> 13);
        
        for (size_t i = 0; i < n; ++i)
        {
            result[i] = (state >> (8 * (n - 1 - i))) & 0xFF;
        }
        
        return result;
    }
};

class zero_round_mock : public I_feistel_round_function
{
public:
    std::vector<uint8_t> feistel_round(
        const std::vector<uint8_t>& input_block,
        const std::vector<uint8_t>& round_key) const override
    {
        return std::vector<uint8_t>(input_block.size(), 0);
    }
};

class constant_round_mock : public I_feistel_round_function
{
private:
    uint8_t constant_;
    
public:
    constant_round_mock(uint8_t constant = 0xAA)
        : constant_(constant)
    {
    }
    
    std::vector<uint8_t> feistel_round(
        const std::vector<uint8_t>& input_block,
        const std::vector<uint8_t>& round_key) const override
    {
        return std::vector<uint8_t>(input_block.size(), constant_);
    }
};

class fixed_key_expander_mock : public I_key_expander
{
private:
    std::vector<std::vector<uint8_t>> fixed_keys_;
    
public:
    fixed_key_expander_mock(const std::vector<std::vector<uint8_t>>& keys)
        : fixed_keys_(keys)
    {
    }
    
    std::vector<std::vector<uint8_t>> expand_key(
        const std::vector<uint8_t>& input_key) const override
    {
        return fixed_keys_;
    }
};


class counting_round_mock : public I_feistel_round_function
{
private:
    mutable size_t call_count_;
    std::function<std::vector<uint8_t>(const std::vector<uint8_t>&, const std::vector<uint8_t>&)> impl_;
    
public:
    counting_round_mock()
        : call_count_(0)
        , impl_([](const auto& block, const auto& key) { return block; })
    {
    }
    
    counting_round_mock(std::function<std::vector<uint8_t>(const std::vector<uint8_t>&, const std::vector<uint8_t>&)> impl)
        : call_count_(0)
        , impl_(impl)
    {
    }
    
    std::vector<uint8_t> feistel_round(
        const std::vector<uint8_t>& input_block,
        const std::vector<uint8_t>& round_key) const override
    {
        call_count_++;
        return impl_(input_block, round_key);
    }
    
    size_t get_call_count() const
    {
        return call_count_;
    }
    
    void reset_count()
    {
        call_count_ = 0;
    }
};

// ============================================================================
// Основной класс для тестирования feistel_network
// ============================================================================

class feistel_network_test
{
private:
    test_logger logger_;
    
public:
    feistel_network_test()
        : logger_()
    {
    }
    
    bool run_all_tests()
    {
        bool all_passed = true;
        
        logger_.start_suite("Базовые тесты конструктора");
        all_passed &= test_constructor() && all_passed;
        logger_.end_suite();
        
        logger_.start_suite("Тесты шифрования/дешифрования");
        all_passed &= test_encryption_decryption() && all_passed;
        logger_.end_suite();
        
        logger_.start_suite("Тесты с разными размерами блоков");
        all_passed &= test_block_sizes() && all_passed;
        logger_.end_suite();
        
        logger_.start_suite("Тесты с разным количеством раундов");
        all_passed &= test_round_counts() && all_passed;
        logger_.end_suite();
        
        logger_.start_suite("Тесты свойств сети Фейстеля");
        all_passed &= test_feistel_properties() && all_passed;
        logger_.end_suite();
        
        logger_.start_suite("Тесты обработки ошибок");
        all_passed &= test_error_handling() && all_passed;
        logger_.end_suite();
        
        logger_.start_suite("Тесты производительности");
        all_passed &= test_performance() && all_passed;
        logger_.end_suite();
        
        logger_.start_suite("Тесты детерминизма");
        all_passed &= test_determinism() && all_passed;
        logger_.end_suite();
        
        logger_.start_suite("Тесты лавинного эффекта");
        all_passed &= test_avalanche_effect() && all_passed;
        logger_.end_suite();
        
        logger_.print_summary();
        
        return all_passed;
    }
    
private:
    // ========================================================================
    // Тест 1: Проверка конструктора
    // ========================================================================
    bool test_constructor()
    {
        bool all_passed = true;
        
        try
        {
            simple_key_expander_mock key_expander;
            simple_xor_round_mock round_func;
            
            // Тест 1.1: Нормальное создание
            {
                feistel_network network(&key_expander, &round_func, 16, 8);
                all_passed &= logger_.check_result(true, "Создание с валидными параметрами");
            }
            
            // Тест 1.3: Проверка на нулевые указатели
            {
                bool exception_caught = false;
                try
                {
                    feistel_network network(nullptr, &round_func, 16, 8);
                }
                catch (const std::invalid_argument&)
                {
                    exception_caught = true;
                }
                all_passed &= logger_.check_result(exception_caught, 
                    "Исключение при key_expander = nullptr");
            }
            
            {
                bool exception_caught = false;
                try
                {
                    feistel_network network(&key_expander, nullptr, 16, 8);
                }
                catch (const std::invalid_argument&)
                {
                    exception_caught = true;
                }
                all_passed &= logger_.check_result(exception_caught, 
                    "Исключение при round_function = nullptr");
            }
        }
        catch (const std::exception& e)
        {
            logger_.check_result(false, "Неожиданное исключение", e.what());
            all_passed = false;
        }
        
        return all_passed;
    }
    
    // ========================================================================
    // Тест 2: Проверка шифрования и дешифрования
    // ========================================================================
    bool test_encryption_decryption()
    {
        bool all_passed = true;
        
        try
        {
            simple_key_expander_mock key_expander(16, 4);  // 16 раундов, ключи по 4 байта
            simple_xor_round_mock round_func;
            
            feistel_network network(&key_expander, &round_func, 16, 8);  // блок 8 байт
            
            // Тест 2.1: Простое шифрование/дешифрование
            {
                std::vector<uint8_t> key = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
                std::vector<uint8_t> plaintext = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
                
                network.setup_round_keys(key);
                
                auto ciphertext = network.encrypt_block(plaintext);
                auto decrypted = network.decrypt_block(ciphertext);
                
                bool success = test_utils::bytes_equal(plaintext, decrypted);
                all_passed &= logger_.check_result(success, "Шифрование->дешифрование возвращает оригинал");
            }
            
            // Тест 2.2: Многократное шифрование разных блоков
            {
                std::vector<uint8_t> key = test_utils::random_bytes(8, 12345);
                network.setup_round_keys(key);
                
                bool all_success = true;
                for (int i = 0; i < 100; ++i)
                {
                    auto plaintext = test_utils::random_bytes(8, 1000 + i);
                    auto ciphertext = network.encrypt_block(plaintext);
                    auto decrypted = network.decrypt_block(ciphertext);
                    
                    if (!test_utils::bytes_equal(plaintext, decrypted))
                    {
                        all_success = false;
                        break;
                    }
                }
                
                all_passed &= logger_.check_result(all_success, "100 разных блоков успешно обработаны");
            }
            
            // Тест 2.3: Проверка что шифрование изменяет данные
            {
                std::vector<uint8_t> key = test_utils::random_bytes(8, 42);
                std::vector<uint8_t> plaintext = test_utils::random_bytes(8, 43);
                
                network.setup_round_keys(key);
                auto ciphertext = network.encrypt_block(plaintext);
                
                bool changed = !test_utils::bytes_equal(plaintext, ciphertext);
                all_passed &= logger_.check_result(changed, "Шифрование изменяет данные");
            }
            
            // Тест 2.4: Разные ключи дают разные результаты
            {
                std::vector<uint8_t> plaintext = test_utils::random_bytes(8, 100);
                
                std::vector<uint8_t> key1 = test_utils::random_bytes(8, 1);
                std::vector<uint8_t> key2 = test_utils::random_bytes(8, 2);
                
                network.setup_round_keys(key1);
                auto cipher1 = network.encrypt_block(plaintext);
                
                network.setup_round_keys(key2);
                auto cipher2 = network.encrypt_block(plaintext);
                
                bool different = !test_utils::bytes_equal(cipher1, cipher2);
                all_passed &= logger_.check_result(different, "Разные ключи дают разные шифротексты");
            }
        }
        catch (const std::exception& e)
        {
            logger_.check_result(false, "Неожиданное исключение", e.what());
            all_passed = false;
        }
        
        return all_passed;
    }
    
    // ========================================================================
    // Тест 3: Проверка разных размеров блоков
    // ========================================================================
    bool test_block_sizes()
    {
        bool all_passed = true;
        
        try
        {
            simple_xor_round_mock round_func;
            
            std::vector<size_t> test_sizes = {2, 4, 8, 16, 32};
            
            for (size_t block_size : test_sizes)
            {
                size_t half_size = block_size / 2;
                simple_key_expander_mock key_expander(16, half_size);
                feistel_network network(&key_expander, &round_func, 16, block_size);
                
                std::vector<uint8_t> key = test_utils::random_bytes(8, 42);
                std::vector<uint8_t> plaintext = test_utils::random_bytes(block_size, 43);
                
                network.setup_round_keys(key);
                
                auto ciphertext = network.encrypt_block(plaintext);
                auto decrypted = network.decrypt_block(ciphertext);
                
                bool success = test_utils::bytes_equal(plaintext, decrypted);
                std::string details = "блок " + std::to_string(block_size) + " байт";
                
                all_passed &= logger_.check_result(success, 
                    "Работа с размером блока", details);
            }
        }
        catch (const std::exception& e)
        {
            logger_.check_result(false, "Неожиданное исключение", e.what());
            all_passed = false;
        }
        
        return all_passed;
    }
    
    // ========================================================================
    // Тест 4: Проверка разного количества раундов
    // ========================================================================
    bool test_round_counts()
    {
        bool all_passed = true;
        
        try
        {
            simple_xor_round_mock round_func;
            
            std::vector<size_t> round_counts = {1, 2, 4, 8, 16, 32};
            
            for (size_t rounds : round_counts)
            {
                simple_key_expander_mock key_expander(rounds, 4);
                feistel_network network(&key_expander, &round_func, rounds, 8);
                
                std::vector<uint8_t> key = test_utils::random_bytes(8, 42);
                std::vector<uint8_t> plaintext = test_utils::random_bytes(8, 43);
                
                network.setup_round_keys(key);
                
                auto ciphertext = network.encrypt_block(plaintext);
                auto decrypted = network.decrypt_block(ciphertext);
                
                bool success = test_utils::bytes_equal(plaintext, decrypted);
                std::string details = std::to_string(rounds) + " раундов";
                
                all_passed &= logger_.check_result(success, 
                    "Работа с количеством раундов", details);
            }
            
            // Проверка что количество вызовов раундовой функции совпадает
            {
                counting_round_mock counting_round;
                simple_key_expander_mock key_expander(10, 4);
                feistel_network network(&key_expander, &counting_round, 10, 8);
                
                std::vector<uint8_t> key = test_utils::random_bytes(8, 100);
                std::vector<uint8_t> plaintext = test_utils::random_bytes(8, 101);
                
                network.setup_round_keys(key);
                counting_round.reset_count();
                
                auto ciphertext = network.encrypt_block(plaintext);
                
                bool correct_calls = (counting_round.get_call_count() == 10);
                all_passed &= logger_.check_result(correct_calls, 
                    "Раундовая функция вызывается правильное количество раз",
                    "ожидалось 10, получено " + std::to_string(counting_round.get_call_count()));
            }
        }
        catch (const std::exception& e)
        {
            logger_.check_result(false, "Неожиданное исключение", e.what());
            all_passed = false;
        }
        
        return all_passed;
    }
    
    // ========================================================================
    // Тест 5: Проверка свойств сети Фейстеля
    // ========================================================================
    bool test_feistel_properties()
    {
        bool all_passed = true;
        
        try
        {
            // Тест 5.1: Свойство обратимости для нулевой раундовой функции
            {
                zero_round_mock zero_round;
                simple_key_expander_mock key_expander(16, 4);
                feistel_network network(&key_expander, &zero_round, 16, 8);
                
                std::vector<uint8_t> key(8, 0);
                std::vector<uint8_t> plaintext = test_utils::random_bytes(8, 100);
                
                network.setup_round_keys(key);
                
                auto ciphertext = network.encrypt_block(plaintext);
                auto decrypted = network.decrypt_block(ciphertext);
                
                // Для нулевой функции должно выполняться свойство
                bool reversible = test_utils::bytes_equal(plaintext, decrypted);
                all_passed &= logger_.check_result(reversible, 
                    "Обратимость при нулевой раундовой функции");
            }
            
            // Тест 5.2: Проверка что левая и правая половины меняются местами
            {
                class tracking_round_mock : public I_feistel_round_function
                {
                public:
                    std::vector<uint8_t> feistel_round(
                        const std::vector<uint8_t>& input_block,
                        const std::vector<uint8_t>& round_key) const override
                    {
                        return std::vector<uint8_t>(input_block.size(), 0xFF);
                    }
                };
                
                tracking_round_mock track_round;
                simple_key_expander_mock key_expander(1, 4); // Только 1 раунд
                feistel_network network(&key_expander, &track_round, 1, 8);
                
                std::vector<uint8_t> key(8, 0);
                std::vector<uint8_t> plaintext(8, 0);

                for (size_t i = 4; i < 8; ++i)
                {
                    plaintext[i] = 0xFF;
                }
                
                network.setup_round_keys(key);
                auto ciphertext = network.encrypt_block(plaintext);
                
                bool left_is_old_right = (ciphertext[4] == 0xFF && ciphertext[5] == 0xFF && 
                                          ciphertext[6] == 0xFF && ciphertext[7] == 0xFF);
                
                all_passed &= logger_.check_result(left_is_old_right, 
                    "Проверка обмена половинами в сети Фейстеля");
            }
        }
        catch (const std::exception& e)
        {
            logger_.check_result(false, "Неожиданное исключение", e.what());
            all_passed = false;
        }
        
        return all_passed;
    }
    
    // ========================================================================
    // Тест 6: Проверка обработки ошибок
    // ========================================================================
    bool test_error_handling()
    {
        bool all_passed = true;
        
        try
        {
            simple_key_expander_mock key_expander(16, 4);
            simple_xor_round_mock round_func;
            feistel_network network(&key_expander, &round_func, 16, 8);
            
            // Тест 6.1: Шифрование без установки ключей
            {
                bool exception_caught = false;
                std::vector<uint8_t> data(8, 0);
                
                try
                {
                    auto result = network.encrypt_block(data);
                }
                catch (const std::runtime_error&)
                {
                    exception_caught = true;
                }
                
                all_passed &= logger_.check_result(exception_caught, 
                    "Исключение при encrypt_block без setup_round_keys");
            }
            
            // Тест 6.2: Дешифрование без установки ключей
            {
                bool exception_caught = false;
                std::vector<uint8_t> data(8, 0);
                
                try
                {
                    auto result = network.decrypt_block(data);
                }
                catch (const std::runtime_error&)
                {
                    exception_caught = true;
                }
                
                all_passed &= logger_.check_result(exception_caught, 
                    "Исключение при decrypt_block без setup_round_keys");
            }
            
            // Тест 6.3: Неправильный размер блока при шифровании
            {
                std::vector<uint8_t> key(8, 0);
                network.setup_round_keys(key);
                
                bool exception_caught = false;
                std::vector<uint8_t> wrong_size_data(7, 0); // 7 байт вместо 8
                
                try
                {
                    auto result = network.encrypt_block(wrong_size_data);
                }
                catch (const std::invalid_argument&)
                {
                    exception_caught = true;
                }
                
                all_passed &= logger_.check_result(exception_caught, 
                    "Исключение при неправильном размере блока (encrypt)");
            }
            
            // Тест 6.4: Неправильный размер блока при дешифровании
            {
                std::vector<uint8_t> key(8, 0);
                network.setup_round_keys(key);
                
                bool exception_caught = false;
                std::vector<uint8_t> wrong_size_data(9, 0); // 9 байт вместо 8
                
                try
                {
                    auto result = network.decrypt_block(wrong_size_data);
                }
                catch (const std::invalid_argument&)
                {
                    exception_caught = true;
                }
                
                all_passed &= logger_.check_result(exception_caught, 
                    "Исключение при неправильном размере блока (decrypt)");
            }
            
            // Тест 6.5: Неправильное количество раундовых ключей
            {
                class bad_key_expander : public I_key_expander
                {
                public:
                    std::vector<std::vector<uint8_t>> expand_key(
                        const std::vector<uint8_t>& input_key) const override
                    {
                        // Возвращаем только 8 ключей для 16 раундов
                        return std::vector<std::vector<uint8_t>>(8, std::vector<uint8_t>(4, 0));
                    }
                };
                
                bad_key_expander bad_expander;
                feistel_network bad_network(&bad_expander, &round_func, 16, 8);
                
                bool exception_caught = false;
                std::vector<uint8_t> key(8, 0);
                
                try
                {
                    bad_network.setup_round_keys(key);
                    auto result = bad_network.encrypt_block(std::vector<uint8_t>(8, 0));
                }
                catch (const std::runtime_error&)
                {
                    exception_caught = true;
                }
                
                all_passed &= logger_.check_result(exception_caught, 
                    "Исключение при недостаточном количестве раундовых ключей");
            }
        }
        catch (const std::exception& e)
        {
            logger_.check_result(false, "Неожиданное исключение", e.what());
            all_passed = false;
        }
        
        return all_passed;
    }
    
    // ========================================================================
    // Тест 7: Проверка производительности
    // ========================================================================
    bool test_performance()
    {
        bool all_passed = true;
        
        try
        {
            simple_key_expander_mock key_expander(16, 4);
            simple_xor_round_mock round_func;
            feistel_network network(&key_expander, &round_func, 16, 8);
            
            std::vector<uint8_t> key(8, 0);
            std::vector<uint8_t> plaintext(8, 0);
            
            network.setup_round_keys(key);
            
            // Тест 7.1: Измерение времени шифрования
            const size_t iterations = 10000;
            
            auto start = std::chrono::high_resolution_clock::now();
            
            for (size_t i = 0; i < iterations; ++i)
            {
                auto ciphertext = network.encrypt_block(plaintext);
            }
            
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            
            double avg_time = static_cast<double>(duration.count()) / iterations;
            
            std::stringstream ss;
            ss << std::fixed << std::setprecision(2) << avg_time << " мкс/блок";
            
            all_passed &= logger_.check_result(true, "Измерение производительности", ss.str());
            //all_passed &= true; // Не проваливаем тест из-за производительности
            
            // Тест 7.2: Проверка что шифрование не слишком медленное
            bool performance_ok = (avg_time < 1000);
            all_passed &= logger_.check_result(performance_ok, 
                "Производительность в допустимых пределах", 
                std::to_string(avg_time) + " мкс");
        }
        catch (const std::exception& e)
        {
            logger_.check_result(false, "Неожиданное исключение", e.what());
            all_passed = false;
        }
        
        return all_passed;
    }
    
    // ========================================================================
    // Тест 8: Проверка детерминизма
    // ========================================================================
    bool test_determinism()
    {
        bool all_passed = true;
        
        try
        {
            simple_key_expander_mock key_expander(16, 4);
            simple_xor_round_mock round_func;
            feistel_network network(&key_expander, &round_func, 16, 8);
            
            std::vector<uint8_t> key = test_utils::random_bytes(8, 123);
            std::vector<uint8_t> plaintext = test_utils::random_bytes(8, 456);
            
            network.setup_round_keys(key);
            
            // Шифруем несколько раз
            auto cipher1 = network.encrypt_block(plaintext);
            auto cipher2 = network.encrypt_block(plaintext);
            auto cipher3 = network.encrypt_block(plaintext);
            
            bool deterministic = test_utils::bytes_equal(cipher1, cipher2) && 
                                 test_utils::bytes_equal(cipher2, cipher3);
            
            all_passed &= logger_.check_result(deterministic, 
                "Шифрование детерминировано (одинаковые результаты)");
            
            // Дешифрование тоже детерминировано
            auto dec1 = network.decrypt_block(cipher1);
            auto dec2 = network.decrypt_block(cipher1);
            
            bool decrypt_deterministic = test_utils::bytes_equal(dec1, dec2);
            all_passed &= logger_.check_result(decrypt_deterministic, 
                "Дешифрование детерминировано");
        }
        catch (const std::exception& e)
        {
            logger_.check_result(false, "Неожиданное исключение", e.what());
            all_passed = false;
        }
        
        return all_passed;
    }
    
    // ========================================================================
    // Тест 9: Проверка лавинного эффекта
    // ========================================================================
    bool test_avalanche_effect()
    {
        bool all_passed = true;
        
        try
        {
            simple_key_expander_mock key_expander(16, 4);
            simple_xor_round_mock round_func;
            feistel_network network(&key_expander, &round_func, 16, 8);
            
            // Тест 9.1: Изменение одного бита в plaintext
            {
                std::vector<uint8_t> key(8, 0x55);
                std::vector<uint8_t> plaintext1(8, 0xAA);
                std::vector<uint8_t> plaintext2 = plaintext1;
                
                // Меняем один бит
                plaintext2[0] ^= 0x01;
                
                network.setup_round_keys(key);
                
                auto cipher1 = network.encrypt_block(plaintext1);
                auto cipher2 = network.encrypt_block(plaintext2);
                
                size_t diff_bits = test_utils::count_different_bits(cipher1, cipher2);
                size_t total_bits = 8 * 8; // 64 бита
                
                double avalanche = static_cast<double>(diff_bits) / total_bits * 100.0;
                
                std::stringstream ss;
                ss << std::fixed << std::setprecision(2) << avalanche << "% (" 
                   << diff_bits << "/" << total_bits << " бит)";
                
                bool good_avalanche = (diff_bits > total_bits * 0.3) && 
                                      (diff_bits < total_bits * 0.7);
                
                all_passed &= logger_.check_result(good_avalanche, 
                    "Лавинный эффект при изменении plaintext", ss.str());
            }
            
            // Тест 9.2: Изменение одного бита в ключе
            {
                std::vector<uint8_t> key1(8, 0x55);
                std::vector<uint8_t> key2 = key1;
                std::vector<uint8_t> plaintext(8, 0xAA);
                
                key2[0] ^= 0x01;
                
                network.setup_round_keys(key1);
                auto cipher1 = network.encrypt_block(plaintext);
                
                network.setup_round_keys(key2);
                auto cipher2 = network.encrypt_block(plaintext);
                
                size_t diff_bits = test_utils::count_different_bits(cipher1, cipher2);
                size_t total_bits = 8 * 8; // 64 бита
                
                double avalanche = static_cast<double>(diff_bits) / total_bits * 100.0;
                
                std::stringstream ss;
                ss << std::fixed << std::setprecision(2) << avalanche << "% (" 
                   << diff_bits << "/" << total_bits << " бит)";
                
                bool good_avalanche = (diff_bits > total_bits * 0.3) && 
                                      (diff_bits < total_bits * 0.7);
                
                all_passed &= logger_.check_result(good_avalanche, 
                    "Лавинный эффект при изменении ключа", ss.str());
            }
        }
        catch (const std::exception& e)
        {
            logger_.check_result(false, "Неожиданное исключение", e.what());
            all_passed = false;
        }
        
        return all_passed;
    }
};

inline bool run_feistel_network_tests()
{
    feistel_network_test tester;
    return tester.run_all_tests();
}

#endif // FEISTEL_NETWORK_TEST_H