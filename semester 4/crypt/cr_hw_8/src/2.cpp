#include <iostream>
#include <vector>
#include <cstdint>
#include <iomanip>
#include "../include/poly_gf2.hpp"

std::uint8_t apply_affine_transformation(std::uint8_t b)
{
    std::uint8_t res = 0;
    std::uint8_t constant = 0x63;
    for (int i = 0; i < 8; ++i)
    {
        std::uint8_t bit = (b >> i) & 1;
        bit ^= (b >> ((i + 4) % 8)) & 1;
        bit ^= (b >> ((i + 5) % 8)) & 1;
        bit ^= (b >> ((i + 6) % 8)) & 1;
        bit ^= (b >> ((i + 7) % 8)) & 1;
        bit ^= (constant >> i) & 1;
        res |= (bit << i);
    }
    return res;
}

std::vector<std::uint8_t> construct_s_box(std::uint64_t irreducible_poly)
{
    std::vector<std::uint8_t> s_box(256);
    poly_gf2 mod(irreducible_poly);

    for (int i = 0; i < 256; ++i)
    {
        poly_gf2 val(static_cast<std::uint64_t>(i));
        poly_gf2 inv = val.inverse(mod);
        s_box[i] = apply_affine_transformation(static_cast<std::uint8_t>(inv.get_value()));
    }
    return s_box;
}

std::vector<std::uint8_t> construct_inv_s_box(std::uint64_t irreducible_poly)
{
    auto s_box = construct_s_box(irreducible_poly);
    std::vector<std::uint8_t> inv_s_box(256);
    for (int i = 0; i < 256; ++i)
    {
        inv_s_box[s_box[i]] = static_cast<std::uint8_t>(i);
    }
    return inv_s_box;
}

void print_box(const std::string& title, const std::vector<std::uint8_t>& box)
{
    std::cout << title << ":" << std::endl;
    for (int i = 0; i < 256; ++i)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
            << static_cast<int>(box[i]) << (i % 16 == 15 ? "\n" : " ");
    }
    std::cout << std::dec << std::endl;
}

int main()
{
    std::uint64_t rijndael_poly = 0x11B;

    std::vector<std::uint8_t> s_box = 
        construct_s_box(rijndael_poly);
    print_box("S-Box", s_box);

    std::vector<std::uint8_t> inv_s_box = 
        construct_inv_s_box(rijndael_poly);
    print_box("Inverse S-Box", inv_s_box);

    return 0;
}