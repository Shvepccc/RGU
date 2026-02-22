#ifndef _BIT_MANIPULATOR_H
#define _BIT_MANIPULATOR_H

#include <vector>
#include <cstdint>

class bit_manipulator
{
private:
    static size_t total_bits(const std::vector<uint8_t>& data);

    static void check_bit_index(const std::vector<uint8_t>& data, size_t idx);

    static void check_range(const std::vector<uint8_t>& data, size_t i, size_t j);

    static bool get_bit(const std::vector<uint8_t>& data, size_t idx);

    static void set_bit(std::vector<uint8_t>& data, size_t idx, bool value);

public:
    std::vector<uint8_t> rotate_left(const std::vector<uint8_t>& data, size_t k) const;

    std::vector<uint8_t> rotate_right(const std::vector<uint8_t>& data, size_t k) const;

    std::vector<uint8_t> apply_mask(const std::vector<uint8_t>& data, const std::vector<uint8_t>& mask) const;

    std::vector<uint8_t> extract_bits(const std::vector<uint8_t>& data, size_t i, size_t j) const;

    std::vector<uint8_t> swap_bits(const std::vector<uint8_t>& data, size_t i, size_t j) const;

    std::vector<uint8_t> set_bit_value(const std::vector<uint8_t>& data, size_t i, bool value) const;
};

#endif