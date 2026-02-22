#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include "bit_manipulator.hpp"

std::string bytes_to_hex(const std::vector<uint8_t>& bytes)
{
    if (bytes.empty()) return "";
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (size_t i = 0; i < bytes.size(); ++i)
    {
        if (i > 0) oss << " ";
        oss << std::setw(2) << static_cast<int>(bytes[i]);
    }
    return oss.str();
}

std::string bytes_to_binary(const std::vector<uint8_t>& bytes)
{
    if (bytes.empty()) return "";
    std::ostringstream oss;
    for (size_t i = 0; i < bytes.size(); ++i)
    {
        if (i > 0) oss << " ";
        for (int b = 7; b >= 0; --b)
        {
            oss << ((bytes[i] >> b) & 1);
        }
    }
    return oss.str();
}

struct test_case
{
    std::string name;
    std::vector<uint8_t> input;
    std::vector<uint8_t> expected;
    bool expect_exception;
    std::string exception_msg;
};

void print_test_result(const std::string& test_name, bool passed, 
                        const std::vector<uint8_t>& input = {},
                        const std::vector<uint8_t>& output = {},
                        const std::vector<uint8_t>& expected = {})
{
    std::cout << "\n" << test_name << ": " << (passed ? "[PASS]" : "[FAIL]") << "\n";
    if (!input.empty())
    {
        std::cout << "  Input:  " << bytes_to_hex(input) << "  " << bytes_to_binary(input) << "\n";
    }
    if (!output.empty() && !expected.empty())
    {
        std::cout << "  Output: " << bytes_to_hex(output) << "  " << bytes_to_binary(output) << "\n";
        std::cout << "  Expected: " << bytes_to_hex(expected) << "  " << bytes_to_binary(expected) << "\n";
    }
}

void run_unit_tests()
{
    std::cout << "========================================\n";
    std::cout << "UNIT TESTS FOR bit_manipulator\n";
    std::cout << "========================================\n";
    
    bit_manipulator bm;
    size_t passed = 0;
    size_t total = 0;

    // Test 1: rotate_left with k=0
    {
        std::vector<uint8_t> data = {0x12, 0x34};
        auto result = bm.rotate_left(data, 0);
        bool ok = (result == data);
        print_test_result("rotate_left k=0", ok, data, result, data);
        if (ok) ++passed; ++total;
    }

    // Test 2: rotate_left with k=3
    {
        std::vector<uint8_t> data = {0x12, 0x34};
        std::vector<uint8_t> expected = {0x91, 0xA0};
        auto result = bm.rotate_left(data, 3);
        bool ok = (result == expected);
        print_test_result("rotate_left k=3", ok, data, result, expected);
        if (ok) ++passed; ++total;
    }

    // Test 3: rotate_left with k=16 (full cycle)
    {
        std::vector<uint8_t> data = {0x12, 0x34};
        auto result = bm.rotate_left(data, 16);
        bool ok = (result == data);
        print_test_result("rotate_left k=16", ok, data, result, data);
        if (ok) ++passed; ++total;
    }

    // Test 4: rotate_right with k=3
    {
        std::vector<uint8_t> data = {0x12, 0x34};
        std::vector<uint8_t> expected = {0x82, 0x46};
        auto result = bm.rotate_right(data, 3);
        bool ok = (result == expected);
        print_test_result("rotate_right k=3", ok, data, result, expected);
        if (ok) ++passed; ++total;
    }

    // Test 5: rotate_right with k=13 (equivalent to rotate_left 3)
    {
        std::vector<uint8_t> data = {0x12, 0x34};
        auto left3 = bm.rotate_left(data, 3);
        auto right13 = bm.rotate_right(data, 13);
        bool ok = (left3 == right13);
        print_test_result("rotate_right k=13 == rotate_left k=3", ok, data, right13, left3);
        if (ok) ++passed; ++total;
    }

    // Test 6: apply_mask with all ones
    {
        std::vector<uint8_t> data = {0x12, 0x34};
        std::vector<uint8_t> mask = {0xFF, 0xFF};
        auto result = bm.apply_mask(data, mask);
        bool ok = (result == data);
        print_test_result("apply_mask all ones", ok, data, result, data);
        if (ok) ++passed; ++total;
    }

    // Test 7: apply_mask with all zeros
    {
        std::vector<uint8_t> data = {0x12, 0x34};
        std::vector<uint8_t> mask = {0x00, 0x00};
        std::vector<uint8_t> expected = {0x00, 0x00};
        auto result = bm.apply_mask(data, mask);
        bool ok = (result == expected);
        print_test_result("apply_mask all zeros", ok, data, result, expected);
        if (ok) ++passed; ++total;
    }

    // Test 8: apply_mask with mixed mask {0xFF, 0x00}
    {
        std::vector<uint8_t> data = {0x12, 0x34};
        std::vector<uint8_t> mask = {0xFF, 0x00};
        std::vector<uint8_t> expected = {0x12, 0x00};
        auto result = bm.apply_mask(data, mask);
        bool ok = (result == expected);
        print_test_result("apply_mask {0xFF,0x00}", ok, data, result, expected);
        if (ok) ++passed; ++total;
    }

    // Test 9: extract_bits [0..7] (first byte)
    {
        std::vector<uint8_t> data = {0x12, 0x34};
        std::vector<uint8_t> expected = {0x12};
        auto result = bm.extract_bits(data, 0, 7);
        bool ok = (result == expected);
        print_test_result("extract_bits [0..7]", ok, data, result, expected);
        if (ok) ++passed; ++total;
    }

    // Test 10: extract_bits [8..15] (second byte)
    {
        std::vector<uint8_t> data = {0x12, 0x34};
        std::vector<uint8_t> expected = {0x34};
        auto result = bm.extract_bits(data, 8, 15);
        bool ok = (result == expected);
        print_test_result("extract_bits [8..15]", ok, data, result, expected);
        if (ok) ++passed; ++total;
    }

    // Test 11: extract_bits [4..11] (cross byte)
    {
        std::vector<uint8_t> data = {0x12, 0x34};
        std::vector<uint8_t> expected = {0x41};
        auto result = bm.extract_bits(data, 4, 11);
        bool ok = (result == expected);
        print_test_result("extract_bits [4..11]", ok, data, result, expected);
        if (ok) ++passed; ++total;
    }

    // Test 12: extract_bits [3..10] (cross byte)
    {
        std::vector<uint8_t> data = {0x12, 0x34};
        std::vector<uint8_t> expected = {0x82};
        auto result = bm.extract_bits(data, 3, 10);
        bool ok = (result == expected);
        print_test_result("extract_bits [3..10]", ok, data, result, expected);
        if (ok) ++passed; ++total;
    }

    // Test 13: swap_bits same indices
    {
        std::vector<uint8_t> data = {0x12, 0x34};
        auto result = bm.swap_bits(data, 5, 5);
        bool ok = (result == data);
        print_test_result("swap_bits i==j", ok, data, result, data);
        if (ok) ++passed; ++total;
    }

    // Test 14: swap_bits both zero
    {
        std::vector<uint8_t> data = {0x12, 0x34};
        auto result = bm.swap_bits(data, 0, 7);
        bool ok = (result == data);
        print_test_result("swap_bits 0 and 7 (both 0)", ok, data, result, data);
        if (ok) ++passed; ++total;
    }

    // Test 15: swap_bits both one
    {
        std::vector<uint8_t> data = {0x12, 0x34};
        auto result = bm.swap_bits(data, 1, 4);
        bool ok = (result == data);
        print_test_result("swap_bits 1 and 4 (both 1)", ok, data, result, data);
        if (ok) ++passed; ++total;
    }

    // Test 16: swap_bits different values (1 and 2)
    {
        std::vector<uint8_t> data = {0x12, 0x34};
        std::vector<uint8_t> expected = {0x14, 0x34};
        auto result = bm.swap_bits(data, 1, 2);
        bool ok = (result == expected);
        print_test_result("swap_bits 1 and 2", ok, data, result, expected);
        if (ok) ++passed; ++total;
    }

    // Test 17: swap_bits across bytes (both zero)
    {
        std::vector<uint8_t> data = {0x12, 0x34};
        auto result = bm.swap_bits(data, 7, 8);
        bool ok = (result == data);
        print_test_result("swap_bits 7 and 8 (both 0)", ok, data, result, data);
        if (ok) ++passed; ++total;
    }

    // Test 18: swap_bits across bytes (both zero again)
    {
        std::vector<uint8_t> data = {0x12, 0x34};
        auto result = bm.swap_bits(data, 6, 9);
        bool ok = (result == data);
        print_test_result("swap_bits 6 and 9 (both 0)", ok, data, result, data);
        if (ok) ++passed; ++total;
    }

    // Test 19: set_bit_value set to 1
    {
        std::vector<uint8_t> data = {0x12, 0x34};
        std::vector<uint8_t> expected = {0x16, 0x34};
        auto result = bm.set_bit_value(data, 2, true);
        bool ok = (result == expected);
        print_test_result("set_bit_value bit2=1", ok, data, result, expected);
        if (ok) ++passed; ++total;
    }

    // Test 20: set_bit_value set to 0
    {
        std::vector<uint8_t> data = {0x12, 0x34};
        std::vector<uint8_t> expected = {0x10, 0x34};
        auto result = bm.set_bit_value(data, 1, false);
        bool ok = (result == expected);
        print_test_result("set_bit_value bit1=0", ok, data, result, expected);
        if (ok) ++passed; ++total;
    }

    // Test 21: empty data
    {
        std::vector<uint8_t> data;
        auto result = bm.rotate_left(data, 5);
        bool ok = result.empty();
        print_test_result("empty data", ok);
        if (ok) ++passed; ++total;
    }

    // Test 22: apply_mask different sizes throws
    {
        std::vector<uint8_t> data = {0x12, 0x34};
        std::vector<uint8_t> mask = {0xFF};
        bool caught = false;
        try {
            bm.apply_mask(data, mask);
        } catch (const std::invalid_argument&) {
            caught = true;
        }
        print_test_result("apply_mask different sizes throws", caught);
        if (caught) ++passed; ++total;
    }

    // Test 23: extract_bits invalid range throws
    {
        std::vector<uint8_t> data = {0x12, 0x34};
        bool caught = false;
        try {
            bm.extract_bits(data, 10, 20);
        } catch (const std::out_of_range&) {
            caught = true;
        }
        print_test_result("extract_bits invalid range throws", caught);
        if (caught) ++passed; ++total;
    }

    // Test 24: extract_bits i > j throws
    {
        std::vector<uint8_t> data = {0x12, 0x34};
        bool caught = false;
        try {
            bm.extract_bits(data, 10, 5);
        } catch (const std::out_of_range&) {
            caught = true;
        }
        print_test_result("extract_bits i>j throws", caught);
        if (caught) ++passed; ++total;
    }

    // Test 25: swap_bits index out of range throws
    {
        std::vector<uint8_t> data = {0x12, 0x34};
        bool caught = false;
        try {
            bm.swap_bits(data, 5, 16);
        } catch (const std::out_of_range&) {
            caught = true;
        }
        print_test_result("swap_bits index out of range throws", caught);
        if (caught) ++passed; ++total;
    }

    // Test 26: set_bit_value index out of range throws
    {
        std::vector<uint8_t> data = {0x12, 0x34};
        bool caught = false;
        try {
            bm.set_bit_value(data, 16, true);
        } catch (const std::out_of_range&) {
            caught = true;
        }
        print_test_result("set_bit_value index out of range throws", caught);
        if (caught) ++passed; ++total;
    }

    std::cout << "\n========================================\n";
    std::cout << "TEST SUMMARY: " << passed << "/" << total << " passed\n";
    std::cout << "========================================\n";
}