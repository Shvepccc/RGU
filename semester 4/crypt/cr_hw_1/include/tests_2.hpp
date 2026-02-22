#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stdexcept>
#include "../include/bit_substitution.hpp"

inline void print_test_result(const std::string& test_name, bool passed)
{
    std::cout << "  " << (passed ? "[PASS]" : "[FAIL]") << " " << test_name << "\n";
}

inline bool verify_vectors(const std::vector<uint8_t>& actual, const std::vector<uint8_t>& expected)
{
    if (actual.size() != expected.size()) return false;
    for (size_t i = 0; i < actual.size(); ++i)
    {
        if (actual[i] != expected[i]) return false;
    }
    return true;
}

inline void run_unit_tests()
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
        std::vector<uint8_t> data = {0x12, 0x34};
        int counter = 0;
        auto func = [&counter](uint8_t nibble) -> uint8_t {
            return nibble + counter++;
        };
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