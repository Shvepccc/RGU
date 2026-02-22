// test_triple_des.hpp
#ifndef TRIPLE_DES_TEST_H
#define TRIPLE_DES_TEST_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdint>
#include <chrono>
#include <stdexcept>
#include <string>
#include <sstream>

// Подключаем тестируемый класс
#include "triple_des_algorithm.hpp"

// Подключаем существующие утилиты для тестирования (test_logger, test_utils)
#include "test_feistel.hpp"

// ============================================================================
// Класс для тестирования triple_des_algorithm
// ============================================================================
class triple_des_test
{
private:
    test_logger logger_;
    
    // Вспомогательная функция для генерации ключей разной длины
    std::vector<uint8_t> make_key(size_t length, uint8_t base = 0x11)
    {
        std::vector<uint8_t> key(length);
        for (size_t i = 0; i < length; ++i)
            key[i] = static_cast<uint8_t>(base + i);
        return key;
    }
    
public:
    bool run_all_tests()
    {
        bool all_passed = true;
        
        logger_.start_suite("Конструктор triple_des_algorithm");
        all_passed &= test_constructor() && all_passed;
        logger_.end_suite();
        
        logger_.start_suite("Установка ключей (setup_round_keys)");
        all_passed &= test_key_setup() && all_passed;
        logger_.end_suite();
        
        logger_.start_suite("Шифрование/дешифрование для всех режимов");
        all_passed &= test_encrypt_decrypt_all_modes() && all_passed;
        logger_.end_suite();
        
        // logger_.start_suite("Совместимость с одинарным DES");
        // all_passed &= test_des_compatibility() && all_passed;
        // logger_.end_suite();
        
        logger_.start_suite("Обработка длины ключа");
        all_passed &= test_key_length_handling() && all_passed;
        logger_.end_suite();
        
        logger_.start_suite("Обработка ошибок");
        all_passed &= test_error_handling() && all_passed;
        logger_.end_suite();
        
        logger_.start_suite("Детерминизм");
        all_passed &= test_determinism() && all_passed;
        logger_.end_suite();
        
        logger_.start_suite("Лавинный эффект");
        all_passed &= test_avalanche_effect() && all_passed;
        logger_.end_suite();
        
        logger_.start_suite("Производительность");
        all_passed &= test_performance() && all_passed;
        logger_.end_suite();
        
        logger_.start_suite("Специфичные проверки режимов");
        all_passed &= test_mode_specific() && all_passed;
        logger_.end_suite();
        
        logger_.print_summary();
        return all_passed;
    }
    
private:
    // ------------------------------------------------------------------------
    // Тест 1: Конструктор
    // ------------------------------------------------------------------------
    bool test_constructor()
    {
        bool all_passed = true;
        
        try
        {
            triple_des_algorithm tdes_ede3(TripleDESMode::DES_EDE3);
            triple_des_algorithm tdes_eee3(TripleDESMode::DES_EEE3);
            triple_des_algorithm tdes_ede2(TripleDESMode::DES_EDE2);
            triple_des_algorithm tdes_eee2(TripleDESMode::DES_EEE2);
            
            all_passed &= logger_.check_result(true, "Создание объектов для всех режимов");
        }
        catch (const std::exception& e)
        {
            logger_.check_result(false, "Неожиданное исключение", e.what());
            all_passed = false;
        }
        
        return all_passed;
    }
    
    // ------------------------------------------------------------------------
    // Тест 2: Установка ключей
    // ------------------------------------------------------------------------
    bool test_key_setup()
    {
        bool all_passed = true;
        
        triple_des_algorithm tdes_ede3(TripleDESMode::DES_EDE3);
        triple_des_algorithm tdes_eee3(TripleDESMode::DES_EEE3);
        triple_des_algorithm tdes_ede2(TripleDESMode::DES_EDE2);
        triple_des_algorithm tdes_eee2(TripleDESMode::DES_EEE2);
        
        // Корректные ключи
        auto key24 = make_key(24, 0x10);
        auto key16 = make_key(16, 0x20);
        
        try
        {
            tdes_ede3.setup_round_keys(key24);
            tdes_eee3.setup_round_keys(key24);
            tdes_ede2.setup_round_keys(key16);
            tdes_eee2.setup_round_keys(key16);
            
            all_passed &= logger_.check_result(true, "Установка корректных ключей");
        }
        catch (const std::exception& e)
        {
            logger_.check_result(false, "Ошибка при установке корректных ключей", e.what());
            all_passed = false;
        }
        
        return all_passed;
    }
    
    // ------------------------------------------------------------------------
    // Тест 3: Шифрование/дешифрование для всех режимов
    // ------------------------------------------------------------------------
    bool test_encrypt_decrypt_all_modes()
    {
        bool all_passed = true;
        std::vector<uint8_t> plain = test_utils::random_bytes(8, 12345);
        
        struct TestMode
        {
            TripleDESMode mode;
            std::string name;
            std::vector<uint8_t> key;
        };
        
        std::vector<TestMode> modes = {
            {TripleDESMode::DES_EDE3, "EDE3", make_key(24, 0x11)},
            {TripleDESMode::DES_EEE3, "EEE3", make_key(24, 0x22)},
            {TripleDESMode::DES_EDE2, "EDE2", make_key(16, 0x33)},
            {TripleDESMode::DES_EEE2, "EEE2", make_key(16, 0x44)}
        };
        
        for (const auto& m : modes)
        {
            try
            {
                triple_des_algorithm tdes(m.mode);
                tdes.setup_round_keys(m.key);
                
                auto cipher = tdes.encrypt_block(plain);
                auto decrypted = tdes.decrypt_block(cipher);
                
                bool ok = test_utils::bytes_equal(plain, decrypted);
                all_passed &= logger_.check_result(ok, "Обратимость для режима " + m.name);
                
                if (!ok)
                {
                    std::cout << "      Plain : " << test_utils::bytes_to_hex(plain) << std::endl;
                    std::cout << "      Cipher: " << test_utils::bytes_to_hex(cipher) << std::endl;
                    std::cout << "      Decr  : " << test_utils::bytes_to_hex(decrypted) << std::endl;
                }
            }
            catch (const std::exception& e)
            {
                logger_.check_result(false, "Исключение для режима " + m.name, e.what());
                all_passed = false;
            }
        }
        
        return all_passed;
    }
    
    // ------------------------------------------------------------------------
    // Тест 4: Совместимость с одинарным DES (для режима EDE3 с тремя одинаковыми ключами)
    // ------------------------------------------------------------------------
    // bool test_des_compatibility()
    // {
    //     bool all_passed = true;
        
    //     // Известный тестовый вектор DES
    //     std::vector<uint8_t> des_key = test_utils::hex_to_bytes("133457799BBCDFF1");
    //     std::vector<uint8_t> plain = test_utils::hex_to_bytes("0123456789ABCDEF");
    //     std::vector<uint8_t> expected_cipher = test_utils::hex_to_bytes("85E813540F0AB405");
        
    //     // Собираем 24-байтовый ключ из трёх одинаковых
    //     std::vector<uint8_t> triple_key;
    //     triple_key.insert(triple_key.end(), des_key.begin(), des_key.end());
    //     triple_key.insert(triple_key.end(), des_key.begin(), des_key.end());
    //     triple_key.insert(triple_key.end(), des_key.begin(), des_key.end());
        
    //     try
    //     {
    //         // Одинарный DES
    //         des_algorithm des;
    //         des.setup_round_keys(des_key);
    //         auto des_cipher = des.encrypt_block(plain);
            
    //         // 3DES EDE3
    //         triple_des_algorithm tdes(TripleDESMode::DES_EDE3);
    //         tdes.setup_round_keys(triple_key);
    //         auto tdes_cipher = tdes.encrypt_block(plain);
            
    //         bool match = test_utils::bytes_equal(des_cipher, tdes_cipher) &&
    //                      test_utils::bytes_equal(tdes_cipher, expected_cipher);
            
    //         all_passed &= logger_.check_result(match, "EDE3 с тремя одинаковыми ключами = DES");
            
    //         if (!match)
    //         {
    //             std::cout << "      DES cipher : " << test_utils::bytes_to_hex(des_cipher) << std::endl;
    //             std::cout << "      3DES cipher: " << test_utils::bytes_to_hex(tdes_cipher) << std::endl;
    //             std::cout << "      Expected   : " << test_utils::bytes_to_hex(expected_cipher) << std::endl;
    //         }
    //     }
    //     catch (const std::exception& e)
    //     {
    //         logger_.check_result(false, "Исключение в тесте совместимости", e.what());
    //         all_passed = false;
    //     }
        
    //     return all_passed;
    // }
    
    // ------------------------------------------------------------------------
    // Тест 5: Обработка длины ключа
    // ------------------------------------------------------------------------
    bool test_key_length_handling()
    {
        bool all_passed = true;
        
        triple_des_algorithm tdes_ede3(TripleDESMode::DES_EDE3);
        triple_des_algorithm tdes_eee3(TripleDESMode::DES_EEE3);
        triple_des_algorithm tdes_ede2(TripleDESMode::DES_EDE2);
        triple_des_algorithm tdes_eee2(TripleDESMode::DES_EEE2);
        
        // Корректные длины
        auto key24 = make_key(24);
        auto key16 = make_key(16);
        
        // Некорректные длины
        auto key8  = make_key(8);
        auto key10 = make_key(10);
        auto key32 = make_key(32);
        
        // Для 3-ключевых режимов должны принимать 24 и 16 (тогда K3=K1)
        bool ok_ede3_24 = true, ok_ede3_16 = true, ok_eee3_24 = true, ok_eee3_16 = true;
        try { tdes_ede3.setup_round_keys(key24); } catch (...) { ok_ede3_24 = false; }
        try { tdes_ede3.setup_round_keys(key16); } catch (...) { ok_ede3_16 = false; }
        try { tdes_eee3.setup_round_keys(key24); } catch (...) { ok_eee3_24 = false; }
        try { tdes_eee3.setup_round_keys(key16); } catch (...) { ok_eee3_16 = false; }
        
        all_passed &= logger_.check_result(ok_ede3_24, "EDE3 принимает 24-байтовый ключ");
        all_passed &= logger_.check_result(ok_ede3_16, "EDE3 принимает 16-байтовый ключ (K3=K1)");
        all_passed &= logger_.check_result(ok_eee3_24, "EEE3 принимает 24-байтовый ключ");
        all_passed &= logger_.check_result(ok_eee3_16, "EEE3 принимает 16-байтовый ключ (K3=K1)");
        
        // Для 2-ключевых режимов должны принимать только 16 байт
        bool ok_ede2_16 = true, ok_ede2_24 = false, ok_eee2_16 = true, ok_eee2_24 = false;
        try { tdes_ede2.setup_round_keys(key16); } catch (...) { ok_ede2_16 = false; }
        try { tdes_ede2.setup_round_keys(key24); ok_ede2_24 = true; } catch (...) { }
        try { tdes_eee2.setup_round_keys(key16); } catch (...) { ok_eee2_16 = false; }
        try { tdes_eee2.setup_round_keys(key24); ok_eee2_24 = true; } catch (...) { }
        
        all_passed &= logger_.check_result(ok_ede2_16, "EDE2 принимает 16-байтовый ключ");
        all_passed &= logger_.check_result(!ok_ede2_24, "EDE2 отвергает 24-байтовый ключ");
        all_passed &= logger_.check_result(ok_eee2_16, "EEE2 принимает 16-байтовый ключ");
        all_passed &= logger_.check_result(!ok_eee2_24, "EEE2 отвергает 24-байтовый ключ");
        
        // Все режимы должны отвергать ключи другой длины (8,10,32)
        auto check_invalid = [&](triple_des_algorithm& tdes, const std::vector<uint8_t>& key, const std::string& desc) {
            bool threw = false;
            try { tdes.setup_round_keys(key); } catch (const std::invalid_argument&) { threw = true; }
            catch (...) { /* другие исключения не ожидаются */ }
            return logger_.check_result(threw, desc);
        };
        
        all_passed &= check_invalid(tdes_ede3, key8,  "EDE3 отвергает 8-байтовый ключ");
        all_passed &= check_invalid(tdes_ede3, key10, "EDE3 отвергает 10-байтовый ключ");
        all_passed &= check_invalid(tdes_ede3, key32, "EDE3 отвергает 32-байтовый ключ");
        all_passed &= check_invalid(tdes_ede2, key8,  "EDE2 отвергает 8-байтовый ключ");
        all_passed &= check_invalid(tdes_ede2, key10, "EDE2 отвергает 10-байтовый ключ");
        all_passed &= check_invalid(tdes_ede2, key32, "EDE2 отвергает 32-байтовый ключ");
        
        return all_passed;
    }
    
    // ------------------------------------------------------------------------
    // Тест 6: Обработка ошибок (неинициализирован, неправильный размер блока)
    // ------------------------------------------------------------------------
    bool test_error_handling()
    {
        bool all_passed = true;
        
        triple_des_algorithm tdes(TripleDESMode::DES_EDE3);
        auto key = make_key(24);
        std::vector<uint8_t> plain8(8, 0xAA);
        std::vector<uint8_t> bad7(7, 0xBB);
        std::vector<uint8_t> bad9(9, 0xCC);
        
        // Без ключа
        try { tdes.encrypt_block(plain8); all_passed &= logger_.check_result(false, "encrypt без ключа должно кидать исключение"); }
        catch (const std::runtime_error&) { all_passed &= logger_.check_result(true, "encrypt без ключа кидает runtime_error"); }
        catch (...) { all_passed &= logger_.check_result(false, "encrypt без ключа кидает неверное исключение"); }
        
        try { tdes.decrypt_block(plain8); all_passed &= logger_.check_result(false, "decrypt без ключа должно кидать исключение"); }
        catch (const std::runtime_error&) { all_passed &= logger_.check_result(true, "decrypt без ключа кидает runtime_error"); }
        catch (...) { all_passed &= logger_.check_result(false, "decrypt без ключа кидает неверное исключение"); }
        
        // После установки ключа
        tdes.setup_round_keys(key);
        
        // Неправильный размер блока
        try { tdes.encrypt_block(bad7); all_passed &= logger_.check_result(false, "encrypt с блоком 7 байт должно кидать исключение"); }
        catch (const std::invalid_argument&) { all_passed &= logger_.check_result(true, "encrypt с блоком 7 байт кидает invalid_argument"); }
        catch (...) { all_passed &= logger_.check_result(false, "encrypt с блоком 7 байт кидает неверное исключение"); }
        
        try { tdes.decrypt_block(bad9); all_passed &= logger_.check_result(false, "decrypt с блоком 9 байт должно кидать исключение"); }
        catch (const std::invalid_argument&) { all_passed &= logger_.check_result(true, "decrypt с блоком 9 байт кидает invalid_argument"); }
        catch (...) { all_passed &= logger_.check_result(false, "decrypt с блоком 9 байт кидает неверное исключение"); }
        
        return all_passed;
    }
    
    // ------------------------------------------------------------------------
    // Тест 7: Детерминизм
    // ------------------------------------------------------------------------
    bool test_determinism()
    {
        bool all_passed = true;
        
        triple_des_algorithm tdes(TripleDESMode::DES_EDE3);
        auto key = make_key(24);
        std::vector<uint8_t> plain = test_utils::random_bytes(8, 999);
        tdes.setup_round_keys(key);
        
        std::vector<std::vector<uint8_t>> ciphers;
        const int N = 5;
        for (int i = 0; i < N; ++i)
            ciphers.push_back(tdes.encrypt_block(plain));
        
        bool all_same = true;
        for (int i = 1; i < N; ++i)
            if (!test_utils::bytes_equal(ciphers[0], ciphers[i]))
                all_same = false;
        
        all_passed &= logger_.check_result(all_same, "Шифрование детерминировано (5 раз)");
        
        // Дешифрование
        auto cipher = ciphers[0];
        std::vector<std::vector<uint8_t>> plains;
        for (int i = 0; i < N; ++i)
            plains.push_back(tdes.decrypt_block(cipher));
        
        all_same = true;
        for (int i = 1; i < N; ++i)
            if (!test_utils::bytes_equal(plains[0], plains[i]))
                all_same = false;
        
        all_passed &= logger_.check_result(all_same, "Дешифрование детерминировано (5 раз)");
        
        return all_passed;
    }
    
    // ------------------------------------------------------------------------
    // Тест 8: Лавинный эффект
    // ------------------------------------------------------------------------
    bool test_avalanche_effect()
    {
        bool all_passed = true;
        
        triple_des_algorithm tdes(TripleDESMode::DES_EDE3);
        auto key = make_key(24, 0x55);
        std::vector<uint8_t> plain = test_utils::random_bytes(8, 1234);
        tdes.setup_round_keys(key);
        
        // Изменение одного бита в plaintext
        auto plain2 = plain;
        plain2[0] ^= 0x01;  // меняем младший бит первого байта
        
        auto c1 = tdes.encrypt_block(plain);
        auto c2 = tdes.encrypt_block(plain2);
        
        size_t diff_bits = test_utils::count_different_bits(c1, c2);
        double avalanche = (diff_bits * 100.0) / 64.0;
        
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << avalanche << "% (" << diff_bits << "/64 бит)";
        
        // Ожидаем около 50% (допустим 30-70%)
        bool good = (diff_bits >= 19 && diff_bits <= 45);
        all_passed &= logger_.check_result(good, "Лавинный эффект при изменении plaintext", ss.str());
        
        // Изменение одного бита в ключе
        auto key2 = key;
        key2[5] ^= 0x80;  // меняем старший бит шестого байта
        
        triple_des_algorithm tdes2(TripleDESMode::DES_EDE3);
        tdes2.setup_round_keys(key2);
        auto c3 = tdes2.encrypt_block(plain);
        
        diff_bits = test_utils::count_different_bits(c1, c3);
        avalanche = (diff_bits * 100.0) / 64.0;
        ss.str(""); ss << std::fixed << std::setprecision(2) << avalanche << "% (" << diff_bits << "/64 бит)";
        
        good = (diff_bits >= 19 && diff_bits <= 45);
        all_passed &= logger_.check_result(good, "Лавинный эффект при изменении ключа", ss.str());
        
        return all_passed;
    }
    
    // ------------------------------------------------------------------------
    // Тест 9: Производительность (просто замер)
    // ------------------------------------------------------------------------
    bool test_performance()
    {
        bool all_passed = true;
        
        triple_des_algorithm tdes(TripleDESMode::DES_EDE3);
        auto key = make_key(24);
        std::vector<uint8_t> plain(8, 0x42);
        tdes.setup_round_keys(key);
        
        const size_t iterations = 10000;
        auto start = std::chrono::high_resolution_clock::now();
        
        for (size_t i = 0; i < iterations; ++i)
            tdes.encrypt_block(plain);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        double avg = static_cast<double>(duration.count()) / iterations;
        
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << avg << " мкс/блок";
        logger_.check_result(true, "Замер производительности", ss.str());
        
        // Не проваливаем тест из-за производительности, просто информируем
        return all_passed;
    }
    
    // ------------------------------------------------------------------------
    // Тест 10: Специфичные проверки режимов (например, что EDE2 и EEE2 отличаются)
    // ------------------------------------------------------------------------
    bool test_mode_specific()
    {
        bool all_passed = true;
        
        auto key16 = make_key(16, 0x77);
        std::vector<uint8_t> plain = test_utils::random_bytes(8, 5555);
        
        triple_des_algorithm tdes_ede2(TripleDESMode::DES_EDE2);
        triple_des_algorithm tdes_eee2(TripleDESMode::DES_EEE2);
        
        tdes_ede2.setup_round_keys(key16);
        tdes_eee2.setup_round_keys(key16);
        
        auto c_ede2 = tdes_ede2.encrypt_block(plain);
        auto c_eee2 = tdes_eee2.encrypt_block(plain);
        
        bool different = !test_utils::bytes_equal(c_ede2, c_eee2);
        all_passed &= logger_.check_result(different, "EDE2 и EEE2 дают разные результаты для одного ключа");
        
        // Проверим, что EDE3 с 16-байтовым ключом (K3=K1) работает так же, как EDE2?
        // Не обязательно, потому что в EDE3 раундовые ключи для третьего DES генерируются из K1,
        // а в EDE2 тоже K3=K1, но количество раундов может отличаться? Нет, в обоих 16 раундов.
        // Но внутренняя реализация DES одинакова. Поэтому результат должен совпадать.
        // Проверим это свойство.
        triple_des_algorithm tdes_ede3_16(TripleDESMode::DES_EDE3);
        tdes_ede3_16.setup_round_keys(key16);  // key16 интерпретируется как K1||K2, K3=K1
        
        auto c_ede3_16 = tdes_ede3_16.encrypt_block(plain);
        
        bool ede3_16_equals_ede2 = test_utils::bytes_equal(c_ede3_16, c_ede2);
        all_passed &= logger_.check_result(ede3_16_equals_ede2, 
            "EDE3 с 16-байтовым ключом (K3=K1) даёт тот же результат, что и EDE2");
        
        return all_passed;
    }
};

// ============================================================================
// Функция для запуска всех тестов
// ============================================================================
inline bool run_triple_des_tests()
{
    triple_des_test tester;
    return tester.run_all_tests();
}

#endif // TRIPLE_DES_TEST_H