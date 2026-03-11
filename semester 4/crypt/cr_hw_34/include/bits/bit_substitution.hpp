#ifndef _BIT_SUBSTITUTION_H
#define _BIT_SUBSTITUTION_H

#include <vector>
#include <map>
#include <functional>
#include <string>

class bit_substitution
{
private:
    static std::vector<uint8_t> bytes_to_bits(const std::vector<uint8_t>& data);
    
    static std::vector<uint8_t> bits_to_bytes(const std::vector<uint8_t>& bits);
    
    static std::vector<uint8_t> group_bits(const std::vector<uint8_t>& bits, size_t group_size);
    
    static std::vector<uint8_t> ungroup_bits(const std::vector<uint8_t>& groups, size_t group_size);

public:
    std::vector<uint8_t> substitute(
        const std::vector<uint8_t>& data,
        const std::map<uint8_t, uint8_t>& s_box,
        size_t group_size = 4) const;
    
    std::vector<uint8_t> substitute(
        const std::vector<uint8_t>& data,
        std::function<uint8_t(uint8_t)> func,
        size_t group_size = 4) const;
    
    static std::string format_bytes(const std::vector<uint8_t>& bytes, bool show_binary = true);
};

#endif