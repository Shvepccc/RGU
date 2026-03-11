#ifndef _BIT_PERMUTATION_H
#define _BIT_PERMUTATION_H
#include <vector>

enum class bit_indexing_order
{
    lsb0,
    msb0
};

enum class bit_index_base
{
    base0,
    base1
};

class bit_permutation
{
public:
    std::vector<uint8_t> permute(
        const std::vector<uint8_t>& data,
        const std::vector<int>& rule,
        bit_indexing_order order,
        bit_index_base base) const;
};

#endif