#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stdexcept>
#include "../include/bit_substitution.hpp"

void print_test_result(const std::string& test_name, bool passed)
{
    std::cout << "  " << (passed ? "[PASS]" : "[FAIL]") << " " << test_name << "\n";
}

bool verify_vectors(const std::vector<uint8_t>& actual, const std::vector<uint8_t>& expected)
{
    if (actual.size() != expected.size()) return false;
    for (size_t i = 0; i < actual.size(); ++i)
    {
        if (actual[i] != expected[i]) return false;
    }
    return true;
}

void run_unit_tests()
{
    std::cout << "========================================\n";
    std::cout << "RUNNING UNIT TESTS FOR bit_substitution\n";
    std::cout << "========================================\n";
    
    bit_substitution sub;
    size_t passed = 0, total = 0;
    
    // -----------------------------------------------------------------
    // Test 1: identity map, group_size=4, single byte
    // -----------------------------------------------------------------
    {
        std::vector<uint8_t> data = {0xAB};
        std::map<uint8_t, uint8_t> s_box;
        for (int i = 0; i < 16; ++i) s_box[i] = i;
        std::vector<uint8_t> expected = {0xAB};
        auto result = sub.substitute(data, s_box, 4);
        bool ok = verify_vectors(result, expected);
        print_test_result("identity map (4-bit, 1 byte)", ok);
        if (ok) ++passed; ++total;
    }
    
    // -----------------------------------------------------------------
    // Test 2: simple 4-bit substitution (swap 0x0<->0xF)
    // -----------------------------------------------------------------
    {
        std::vector<uint8_t> data = {0xF0};  // 1111 0000 -> группы: 0xF, 0x0
        std::map<uint8_t, uint8_t> s_box = {{0x0, 0xF}, {0xF, 0x0}};
        std::vector<uint8_t> expected = {0x0F};  // 0000 1111
        auto result = sub.substitute(data, s_box, 4);
        bool ok = verify_vectors(result, expected);
        print_test_result("swap 0x0<->0xF (4-bit)", ok);
        if (ok) ++passed; ++total;
    }
    
    // -----------------------------------------------------------------
    // Test 3: 3-bit groups (24 bits = 3 bytes) with known mapping
    // -----------------------------------------------------------------
    {
        std::vector<uint8_t> data = {0xFF, 0xFF, 0xFF};  // 24 бита = 8 групп по 3 бита: все группы = 0x7
        std::map<uint8_t, uint8_t> s_box;
        for (int i = 0; i < 8; ++i) s_box[i] = (i + 1) % 8;  // циклический сдвиг
        // Для 0x7 -> (7+1)%8 = 0, значит все группы станут 0x0
        // Тогда биты: 8 групп по 000 -> итого 24 нулевых бита -> 3 байта 0x00
        std::vector<uint8_t> expected = {0x00, 0x00, 0x00};
        auto result = sub.substitute(data, s_box, 3);
        bool ok = verify_vectors(result, expected);
        print_test_result("3-bit cyclic shift (all 0x7 -> 0x0)", ok);
        if (ok) ++passed; ++total;
    }
    
    // -----------------------------------------------------------------
    // Test 4: 1-bit groups (invert each bit)
    // -----------------------------------------------------------------
    {
        std::vector<uint8_t> data = {0xAA};  // 10101010
        std::map<uint8_t, uint8_t> s_box = {{0,1}, {1,0}};
        std::vector<uint8_t> expected = {0x55};  // 01010101
        auto result = sub.substitute(data, s_box, 1);
        bool ok = verify_vectors(result, expected);
        print_test_result("1-bit inversion", ok);
        if (ok) ++passed; ++total;
    }
    
    // -----------------------------------------------------------------
    // Test 5: 8-bit groups (byte substitution)
    // -----------------------------------------------------------------
    {
        std::vector<uint8_t> data = {0x12, 0x34, 0x56};
        std::map<uint8_t, uint8_t> s_box = {{0x12, 0xAB}, {0x34, 0xCD}, {0x56, 0xEF}};
        std::vector<uint8_t> expected = {0xAB, 0xCD, 0xEF};
        auto result = sub.substitute(data, s_box, 8);
        bool ok = verify_vectors(result, expected);
        print_test_result("8-bit byte substitution", ok);
        if (ok) ++passed; ++total;
    }
    
    // -----------------------------------------------------------------
    // Test 6: empty data
    // -----------------------------------------------------------------
    {
        std::vector<uint8_t> data;
        std::map<uint8_t, uint8_t> s_box;
        auto result = sub.substitute(data, s_box, 4);
        bool ok = result.empty();
        print_test_result("empty data returns empty", ok);
        if (ok) ++passed; ++total;
    }
    
    // -----------------------------------------------------------------
    // Test 7: invalid group_size (<1 or >8) throws
    // -----------------------------------------------------------------
    {
        std::vector<uint8_t> data = {0x00};
        std::map<uint8_t, uint8_t> s_box;
        bool caught = false;
        try { sub.substitute(data, s_box, 0); } catch (...) { caught = true; }
        print_test_result("group_size=0 throws", caught);
        if (caught) ++passed; ++total;
        
        caught = false;
        try { sub.substitute(data, s_box, 9); } catch (...) { caught = true; }
        print_test_result("group_size=9 throws", caught);
        if (caught) ++passed; ++total;
    }
    
    // -----------------------------------------------------------------
    // Test 8: data size not multiple of group_size throws
    // -----------------------------------------------------------------
    {
        std::vector<uint8_t> data = {0x01, 0x02}; // 16 бит
        std::map<uint8_t, uint8_t> s_box;
        bool caught = false;
        try { sub.substitute(data, s_box, 3); } catch (...) { caught = true; }
        print_test_result("data size not multiple of group_size throws", caught);
        if (caught) ++passed; ++total;
    }
    
    // -----------------------------------------------------------------
    // Test 9: missing key in s_box throws
    // -----------------------------------------------------------------
    {
        std::vector<uint8_t> data = {0x12};
        std::map<uint8_t, uint8_t> s_box = {{0x1, 0x2}}; // нет ключа 0x12 (группа 4-бит: 0x1 и 0x2? но data=0x12 -> группы 0x1 и 0x2)
        // data=0x12 = 00010010, при group_size=4: первая группа 0010=2, вторая группа 0001=1? LSB first: байт 0x12 биты: 0,1,0,0,1,0,0,0 -> группы по 4 бита: (биты 0-3): 0,1,0,0 = 0010 = 2; (биты 4-7): 1,0,0,0 = 0001 = 1. Так что группы 2 и 1. В s_box есть ключ 1, но нет ключа 2 -> исключение.
        bool caught = false;
        try { sub.substitute(data, s_box, 4); } catch (const std::out_of_range&) { caught = true; }
        print_test_result("missing key throws", caught);
        if (caught) ++passed; ++total;
    }
    
    // -----------------------------------------------------------------
    // Test 10: functional object (lambda) – same as test2
    // -----------------------------------------------------------------
    {
        std::vector<uint8_t> data = {0xF0};
        auto func = [](uint8_t nibble) -> uint8_t {
            if (nibble == 0x0) return 0xF;
            if (nibble == 0xF) return 0x0;
            return nibble; // other values unchanged
        };
        std::vector<uint8_t> expected = {0x0F};
        auto result = sub.substitute(data, func, 4);
        bool ok = verify_vectors(result, expected);
        print_test_result("functional object (swap 0x0<->0xF)", ok);
        if (ok) ++passed; ++total;
    }
    
    // -----------------------------------------------------------------
    // Test 11: functional object with state (increment per group)
    // -----------------------------------------------------------------
    {
        std::vector<uint8_t> data = {0x12, 0x34}; // 2 байта = 4 группы по 4 бита
        // data: 0x12 -> 00010010 -> группы: (2,1) ; 0x34 -> 00110100 -> группы: (4,3)  (LSB first)
        // Итак группы: 2,1,4,3
        int counter = 0;
        auto func = [&counter](uint8_t nibble) -> uint8_t {
            return nibble + counter++;
        };
        // counter будет увеличиваться после каждого вызова
        // вызов 1: nibble=2, counter=0 -> результат 2
        // вызов 2: nibble=1, counter=1 -> результат 2
        // вызов 3: nibble=4, counter=2 -> результат 6
        // вызов 4: nibble=3, counter=3 -> результат 6
        // Итоговые группы: 2,2,6,6
        // В битах: группа 2 (0010), группа 2 (0010), группа 6 (0110), группа 6 (0110)
        // Объединяем: 0010 0010 0110 0110 -> биты: младшие первой группы первыми
        // 2: 0100 (биты: 0,1,0,0) -> 0010
        // В итоге первые 4 бита (LSB) = 0100? давайте распишем аккуратно:
        // Группа1 = 2 (двоично: бит0=0, бит1=1, бит2=0, бит3=0) -> 0010 (если читать как число, то бит0 младший)
        // Группа2 = 2 -> аналогично
        // Группа3 = 6 (бит0=0, бит1=1, бит2=1, бит3=0) -> 0110
        // Группа4 = 6
        // Последовательность битов: сначала группа1 бит0, бит1, бит2, бит3, затем группа2 и т.д.
        // Байт0: биты 0-7: группа1(4 бита) + группа2(первые 4 бита?) нет, байт собирается по 8 бит подряд.
        // Первые 8 бит: группа1 (биты 0-3) и группа2 (биты 4-7)
        // группа1 биты: 0,1,0,0 -> (бит0=0, бит1=1, бит2=0, бит3=0) => если записать как байт (бит0 LSB), то это 0010 = 0x2? нет, число 2 это 00000010 в байте (бит1=1). Но мы формируем байт, устанавливая биты согласно их позициям.
        // Для байта0: бит0 = группа1 бит0 = 0; бит1 = группа1 бит1 = 1; бит2 = группа1 бит2 = 0; бит3 = группа1 бит3 = 0; бит4 = группа2 бит0 = 0; бит5 = группа2 бит1 = 1; бит6 = группа2 бит2 = 0; бит7 = группа2 бит3 = 0. Получаем байт: биты: 0:0,1:1,2:0,3:0,4:0,5:1,6:0,7:0 => это 0b00100100? Давайте сложим: бит1=1 => 2, бит5=1 => 32, итого 34 = 0x22. На самом деле 2+32=34 = 0x22.
        // Байт1: группа3 и группа4: группа3 биты: 0,1,1,0; группа4 биты: 0,1,1,0. Тогда байт1: бит0=0, бит1=1, бит2=1, бит3=0, бит4=0, бит5=1, бит6=1, бит7=0 => биты: 0:0,1:1(2),2:1(4),3:0,4:0,5:1(32),6:1(64),7:0 => сумма 2+4+32+64=102 = 0x66.
        // Итог: {0x22, 0x66}
        std::vector<uint8_t> expected = {0x22, 0x66};
        auto result = sub.substitute(data, func, 4);
        bool ok = verify_vectors(result, expected);
        print_test_result("functional object with state", ok);
        if (ok) ++passed; ++total;
    }
    
    // -----------------------------------------------------------------
    // Test 12: map and function produce same result
    // -----------------------------------------------------------------
    {
        std::vector<uint8_t> data = {0xAB, 0xCD};
        std::map<uint8_t, uint8_t> s_box = {{0xA, 0x5}, {0xB, 0x6}, {0xC, 0x7}, {0xD, 0x8}};
        auto func = [&](uint8_t nibble) { return s_box.at(nibble); };
        auto result_map = sub.substitute(data, s_box, 4);
        auto result_func = sub.substitute(data, func, 4);
        bool ok = verify_vectors(result_map, result_func);
        print_test_result("map and function give same result", ok);
        if (ok) ++passed; ++total;
    }
    
    std::cout << "----------------------------------------\n";
    std::cout << "TOTAL: " << passed << " / " << total << " tests passed.\n";
    std::cout << "========================================\n";
}