#include <string>
#include <vector>
#include <stdexcept>
#include "../include/bit_permutation.hpp"

std::vector<uint8_t> bit_permutation::permute(
    const std::vector<uint8_t>& data,
    const std::vector<int>& rule,
    bit_indexing_order order,
    bit_index_base base) const
{
    size_t input_bits_size = data.size() * 8;
    size_t output_bits_size = rule.size();

    if (data.empty())
    {
        return {};
    }

    std::vector<uint8_t> input_bits(input_bits_size);
    for (size_t i = 0; i < input_bits_size; ++i)
    {
        size_t byte_idx = i / 8;
        size_t bit_in_byte;
        if (order == bit_indexing_order::lsb0)
        {
            bit_in_byte = i % 8;
        }
        else
        {
            bit_in_byte = 7 - (i % 8);
        }
        input_bits[i] = (data[byte_idx] >> bit_in_byte) & 1;
    }

    
    std::vector<uint8_t> output_bits(output_bits_size);
    for (size_t j = 0; j < output_bits_size; ++j)
    {
        int src_index = rule[j];

        if (base == bit_index_base::base1)
        {
            if (src_index < 1)
            {
                throw std::out_of_range("Index (" + std::to_string(src_index) + 
                    ") in rule must be >= 1 when base = 1 on iteration " + std::to_string(j));
            }
            --src_index;
        }

        if (src_index < 0 || static_cast<size_t>(src_index) >= input_bits_size)
        {
            throw std::out_of_range("Index in rule is out of range");
        }

        output_bits[j] = input_bits[static_cast<size_t>(src_index)];
    }

    std::vector<uint8_t> result(rule.size() / 8, 0);
    for (size_t j = 0; j < output_bits_size; ++j)
    {
        if (output_bits[j] == 0) continue;

        size_t byte_idx = j / 8;
        size_t bit_in_byte;
        if (order == bit_indexing_order::lsb0)
        {
            bit_in_byte = j % 8;
        }
        else
        {
            bit_in_byte = 7 - (j % 8);
        }
        result[byte_idx] |= (1 << bit_in_byte);
    }

    return result;
}