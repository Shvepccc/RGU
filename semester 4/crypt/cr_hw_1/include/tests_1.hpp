#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <string>
#include "../include/bit_permutation.hpp"

struct test_case
{
    std::vector<uint8_t> data;
    std::vector<int> rule;
    bit_indexing_order order;
    bit_index_base base;
    std::vector<uint8_t> expected_result;
    bool expect_exception;
    std::string description;
};

std::string bytes_to_hex(const std::vector<uint8_t>& bytes)
{
    if (bytes.empty()) return "";
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (uint8_t b : bytes)
    {
        oss << std::setw(2) << static_cast<int>(b);
    }
    return oss.str();
}

std::string bytes_to_binary(const std::vector<uint8_t>& bytes)
{
    if (bytes.empty()) return "";
    std::ostringstream oss;
    for (size_t i = 0; i < bytes.size(); ++i)
    {
        if (i > 0) oss << ' ';
        for (int b = 7; b >= 0; --b)
        {
            oss << ((bytes[i] >> b) & 1);
        }
    }
    return oss.str();
}

void print_bytes(const std::string& label, const std::vector<uint8_t>& bytes)
{
    std::cout << "  " << label << ":\n";
    std::cout << "    HEX: 0x" << bytes_to_hex(bytes) << "\n";
    std::cout << "    BIN: " << bytes_to_binary(bytes) << "\n";
}

void run_unit_tests()
{
    bit_permutation perm;

    std::vector<test_case> tests =
    {
        {
            {0x01},
            {0,1,2,3,4,5,6,7},
            bit_indexing_order::lsb0,
            bit_index_base::base0,
            {0x01},
            false,
            "LSB0, Base0, Identity permutation"
        },
        {
            {0x01},
            {7,6,5,4,3,2,1,0},
            bit_indexing_order::lsb0,
            bit_index_base::base0,
            {0x80},
            false,
            "LSB0, Base0, Bit reversal"
        },
        {
            {0x01},
            {0,1,2,3,4,5,6,7},
            bit_indexing_order::msb0,
            bit_index_base::base0,
            {0x01},
            false,
            "MSB0, Base0, Identity permutation"
        },
        {
            {0x01},
            {7,6,5,4,3,2,1,0},
            bit_indexing_order::msb0,
            bit_index_base::base0,
            {0x80},
            false,
            "MSB0, Base0, Bit reversal"
        },
        {
            {0x01},
            {1,2,3,4,5,6,7,8},
            bit_indexing_order::lsb0,
            bit_index_base::base1,
            {0x01},
            false,
            "LSB0, Base1, Identity permutation (indices 1..8)"
        },
        {
            {0x12, 0x34},
            {8,9,10,11,12,13,14,15, 0,1,2,3,4,5,6,7},
            bit_indexing_order::lsb0,
            bit_index_base::base0,
            {0x34, 0x12},
            false,
            "LSB0, Base0, Byte swap"
        },
        {
            {0xF0, 0x0F},
            {0,1,2,3,4,5,6,7, 8,9,10,11,12,13,14,15},
            bit_indexing_order::lsb0,
            bit_index_base::base0,
            {0xF0, 0x0F},
            false,
            "LSB0, Base0, Two bytes identity"
        },
        {
            {},
            {},
            bit_indexing_order::lsb0,
            bit_index_base::base0,
            {},
            false,
            "Empty data"
        },
        {
            {0x01, 0x02},
            {0,1,2,3,4,5,6,7},
            bit_indexing_order::lsb0,
            bit_index_base::base0,
            {},
            true,
            "Rule size mismatch (8 vs 16)"
        },
        {
            {0x01},
            {8,1,2,3,4,5,6,7},
            bit_indexing_order::lsb0,
            bit_index_base::base0,
            {},
            true,
            "Index out of range (8 > 7)"
        },
        {
            {0x01},
            {0,1,2,3,4,5,6,7},
            bit_indexing_order::lsb0,
            bit_index_base::base1,
            {},
            true,
            "Index 0 with base=1"
        }
    };

    size_t passed = 0;
    size_t failed = 0;

    for (size_t i = 0; i < tests.size(); ++i)
    {
        const auto& test = tests[i];
        std::cout << "\nTest " << i + 1 << ": " << test.description << "\n";

        try
        {
            std::vector<uint8_t> result = perm.permute(
                test.data, test.rule, test.order, test.base);

            if (test.expect_exception)
            {
                std::cout << "Result: FAIL (Expected exception but got none)\n";
                ++failed;
            }
            else
            {
                print_bytes("Input", test.data);
                if (result == test.expected_result)
                {
                    print_bytes("Output", result);
                    std::cout << "Result: OK\n";
                    ++passed;
                }
                else
                {
                    print_bytes("Output", result);
                    std::cout << "Result: FAIL (Unexpected result)\n";
                    ++failed;
                }
            }
        }
        catch (const std::exception& ex)
        {
            if (test.expect_exception)
            {
                std::cout << "Result: OK (Caught expected exception: " << ex.what() << ")\n";
                ++passed;
            }
            else
            {
                std::cout << "Result: FAIL (Unexpected exception: " << ex.what() << ")\n";
                ++failed;
            }
        }
    }

    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "Unit test results: PASSED: " << passed << ", FAILED: " << failed << "\n";
    std::cout << std::string(50, '=') << "\n";
}