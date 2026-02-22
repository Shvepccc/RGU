#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "../include/bit_substitution.hpp"

std::vector<uint8_t> bit_substitution::bytes_to_bits(const std::vector<uint8_t>& data)
{
    std::vector<uint8_t> bits;
    bits.reserve(data.size() * 8);
    for (uint8_t byte : data)
    {
        for (int i = 0; i < 8; ++i)
        {
            bits.push_back((byte >> i) & 1);
        }
    }
    return bits;
}

std::vector<uint8_t> bit_substitution::bits_to_bytes(const std::vector<uint8_t>& bits)
{
    if (bits.size() % 8 != 0)
    {
        throw std::invalid_argument("bits_to_bytes: number of bits must be multiple of 8");
    }
    std::vector<uint8_t> bytes(bits.size() / 8, 0);
    for (size_t i = 0; i < bits.size(); ++i)
    {
        if (bits[i])
        {
            size_t byte_idx = i / 8;
            size_t bit_idx = i % 8;
            bytes[byte_idx] |= (1 << bit_idx);
        }
    }
    return bytes;
}

std::vector<uint8_t> bit_substitution::group_bits(const std::vector<uint8_t>& bits, size_t group_size)
{
    if (bits.size() % group_size != 0)
    {
        throw std::invalid_argument("group_bits: bits size must be multiple of group size");
    }
    std::vector<uint8_t> groups;
    groups.reserve(bits.size() / group_size);
    for (size_t i = 0; i < bits.size(); i += group_size)
    {
        uint8_t value = 0;
        for (size_t j = 0; j < group_size; ++j)
        {
            if (bits[i + j])
            {
                value |= (1 << j);
            }
        }
        groups.push_back(value);
    }
    return groups;
}

std::vector<uint8_t> bit_substitution::ungroup_bits(const std::vector<uint8_t>& groups, size_t group_size)
{
    std::vector<uint8_t> bits;
    bits.reserve(groups.size() * group_size);
    for (uint8_t group : groups)
    {
        for (size_t j = 0; j < group_size; ++j)
        {
            bits.push_back((group >> j) & 1);
        }
    }
    return bits;
}

std::vector<uint8_t> bit_substitution::substitute(
    const std::vector<uint8_t>& data,
    const std::map<uint8_t, uint8_t>& s_box,
    size_t group_size) const
{
    if (data.empty())
    {
        return {};
    }
    if (group_size < 1 || group_size > 8)
    {
        throw std::invalid_argument("group_size must be between 1 and 8");
    }
    if ((data.size() * 8) % group_size != 0)
    {
        throw std::invalid_argument("data size in bits must be multiple of group_size");
    }
    
    std::vector<uint8_t> bits = bytes_to_bits(data);
    std::vector<uint8_t> groups = group_bits(bits, group_size);
    std::vector<uint8_t> result_groups;
    result_groups.reserve(groups.size());
    
    for (uint8_t group : groups)
    {
        auto it = s_box.find(group);
        if (it == s_box.end())
        {
            std::stringstream ss;
            ss << "S-box missing value for input " << static_cast<int>(group);
            throw std::out_of_range(ss.str());
        }
        result_groups.push_back(it->second);
    }
    
    std::vector<uint8_t> result_bits = ungroup_bits(result_groups, group_size);
    return bits_to_bytes(result_bits);
}

std::vector<uint8_t> bit_substitution::substitute(
    const std::vector<uint8_t>& data,
    std::function<uint8_t(uint8_t)> func,
    size_t group_size) const
{
    if (data.empty())
    {
        return {};
    }
    if (group_size < 1 || group_size > 8)
    {
        throw std::invalid_argument("group_size must be between 1 and 8");
    }
    if ((data.size() * 8) % group_size != 0)
    {
        throw std::invalid_argument("data size in bits must be multiple of group_size");
    }
    
    std::vector<uint8_t> bits = bytes_to_bits(data);
    std::vector<uint8_t> groups = group_bits(bits, group_size);
    std::vector<uint8_t> result_groups;
    result_groups.reserve(groups.size());
    
    for (uint8_t group : groups)
    {
        result_groups.push_back(func(group));
    }
    
    std::vector<uint8_t> result_bits = ungroup_bits(result_groups, group_size);
    return bits_to_bytes(result_bits);
}

std::string bit_substitution::format_bytes(const std::vector<uint8_t>& bytes, bool show_binary)
{
    if (bytes.empty()) return "(empty)";
    std::stringstream ss;
    ss << "HEX: ";
    for (size_t i = 0; i < bytes.size(); ++i)
    {
        if (i > 0) ss << " ";
        ss << "0x" << std::hex << std::setw(2) << std::setfill('0') 
            << static_cast<int>(bytes[i]);
    }
    if (show_binary)
    {
        ss << "\n     BIN: ";
        for (size_t i = 0; i < bytes.size(); ++i)
        {
            if (i > 0) ss << " ";
            for (int b = 7; b >= 0; --b)
            {
                ss << ((bytes[i] >> b) & 1);
            }
        }
    }
    return ss.str();
}