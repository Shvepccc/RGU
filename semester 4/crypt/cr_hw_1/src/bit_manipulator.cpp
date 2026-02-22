#include <vector>
#include <stdexcept>
#include <cstdint>
#include "../include/bit_manipulator.hpp"


size_t bit_manipulator::total_bits(const std::vector<uint8_t>& data)
{
    return data.size() * 8;
}

void bit_manipulator::check_bit_index(const std::vector<uint8_t>& data, size_t idx)
{
    if (idx >= total_bits(data))
    {
        throw std::out_of_range("bit index out of range");
    }
}

void bit_manipulator::check_range(const std::vector<uint8_t>& data, size_t i, size_t j)
{
    if (i > j || j >= total_bits(data))
    {
        throw std::out_of_range("invalid bit range");
    }
}

bool bit_manipulator::get_bit(const std::vector<uint8_t>& data, size_t idx)
{
    size_t byte_idx = idx / 8;
    size_t bit_in_byte = idx % 8;
    return (data[byte_idx] >> bit_in_byte) & 1;
}

void bit_manipulator::set_bit(std::vector<uint8_t>& data, size_t idx, bool value)
{
    size_t byte_idx = idx / 8;
    size_t bit_in_byte = idx % 8;
    if (value)
        data[byte_idx] |= (1 << bit_in_byte);
    else
        data[byte_idx] &= ~(1 << bit_in_byte);
}

std::vector<uint8_t> bit_manipulator::rotate_left(const std::vector<uint8_t>& data, size_t k) const
{
    if (data.empty()) return {};
    size_t n = total_bits(data);
    k %= n;
    if (k == 0) return data;

    std::vector<uint8_t> result(data.size(), 0);
    for (size_t i = 0; i < n; ++i)
    {
        bool bit = get_bit(data, i);
        size_t new_pos = (i + k) % n;
        set_bit(result, new_pos, bit);
    }
    return result;
}

std::vector<uint8_t> bit_manipulator::rotate_right(const std::vector<uint8_t>& data, size_t k) const
{
    if (data.empty()) return {};
    size_t n = total_bits(data);
    k %= n;
    if (k == 0) return data;
    return rotate_left(data, n - k);
}

std::vector<uint8_t> bit_manipulator::apply_mask(const std::vector<uint8_t>& data, const std::vector<uint8_t>& mask) const
{
    if (data.size() != mask.size())
    {
        throw std::invalid_argument("data and mask must have same size");
    }
    std::vector<uint8_t> result(data.size());
    for (size_t i = 0; i < data.size(); ++i)
    {
        result[i] = data[i] & mask[i];
    }
    return result;
}

std::vector<uint8_t> bit_manipulator::extract_bits(const std::vector<uint8_t>& data,
                                       size_t i, size_t j) const
    {
        check_range(data, i, j);
        size_t len = j - i + 1;
        size_t bytes_needed = (len + 7) / 8;
        std::vector<uint8_t> result(bytes_needed, 0);

        for (size_t pos = 0; pos < len; ++pos)
        {
            bool bit = get_bit(data, i + pos);
            if (bit)
            {
                size_t byte_idx = pos / 8;
                size_t bit_in_byte = pos % 8;
                result[byte_idx] |= (1 << bit_in_byte);
            }
        }
        return result;
    }

std::vector<uint8_t> bit_manipulator::swap_bits(const std::vector<uint8_t>& data, size_t i, size_t j) const
{
    check_bit_index(data, i);
    check_bit_index(data, j);
    if (i == j) return data;

    bool bi = get_bit(data, i);
    bool bj = get_bit(data, j);

    std::vector<uint8_t> result = data;
    set_bit(result, i, bj);
    set_bit(result, j, bi);
    return result;
}

std::vector<uint8_t> bit_manipulator::set_bit_value(const std::vector<uint8_t>& data, size_t i, bool value) const
{
    check_bit_index(data, i);
    std::vector<uint8_t> result = data;
    set_bit(result, i, value);
    return result;
}
