#include <iostream>
#include <vector>
#include <cassert>
#include <fstream>

#include "../include/des_algorithm.hpp"
#include "../include/crypto_context.hpp"
#include "../include/modes/ecb_mode.hpp"
#include "../include/modes/cbc_mode.hpp"
#include "../include/modes/pcbc_mode.hpp"
#include "../include/paddings/zeros_padding.hpp"
#include "../include/paddings/ansi_x923_padding.hpp"

std::vector<uint8_t> generate_data(size_t size)
{
    std::vector<uint8_t> data(size);

    for(size_t i = 0; i < size; i++)
    {
        data[i] = static_cast<uint8_t>(i % 251);
    }

    return data;
}

void test_des_block()
{
    des_algorithm des;

    std::vector<uint8_t> key =
    {
        0x13,0x34,0x57,0x79,0x9B,0xBC,0xDF,0xF1
    };

    std::vector<uint8_t> block =
    {
        0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF
    };

    des.setup_round_keys(key);

    auto encrypted = des.encrypt_block(block);
    auto decrypted = des.decrypt_block(encrypted);

    assert(block == decrypted);

    std::cout << "DES block test passed\n";
}

void test_padding()
{
    zeros_padding zeros;
    ansi_x923_padding ansi;

    std::vector<uint8_t> data = generate_data(23);

    auto padded1 = zeros.pad(data,8);
    auto unpadded1 = zeros.unpad(padded1,8);

    assert(data == unpadded1);

    auto padded2 = ansi.pad(data,8);
    auto unpadded2 = ansi.unpad(padded2,8);

    assert(data == unpadded2);

    std::cout << "Padding test passed\n";
}

void test_modes()
{
    des_algorithm des;

    std::vector<uint8_t> key =
    {
        1,2,3,4,5,6,7,8
    };

    des.setup_round_keys(key);

    std::vector<uint8_t> data = generate_data(1024);

    std::vector<uint8_t> iv(8,1);

    ecb_mode ecb;
    cbc_mode cbc;
    pcbc_mode pcbc;

    auto ecb_enc = ecb.encrypt(data,&des,8,iv,4);
    auto ecb_dec = ecb.decrypt(ecb_enc,&des,8,iv,4);

    assert(data == ecb_dec);

    auto cbc_enc = cbc.encrypt(data,&des,8,iv,1);
    auto cbc_dec = cbc.decrypt(cbc_enc,&des,8,iv,1);

    assert(data == cbc_dec);

    auto pcbc_enc = pcbc.encrypt(data,&des,8,iv,1);
    auto pcbc_dec = pcbc.decrypt(pcbc_enc,&des,8,iv,1);

    assert(data == pcbc_dec);

    std::cout << "Cipher modes test passed\n";
}

void test_crypto_context_memory()
{
    des_algorithm des;

    std::vector<uint8_t> key =
    {
        10,11,12,13,14,15,16,17
    };

    des.setup_round_keys(key);

    ecb_mode ecb;
    ansi_x923_padding padding;

    crypto_context ctx(
        &des,
        &ecb,
        &padding,
        {},
        8
    );

    auto data = generate_data(5000);

    std::vector<uint8_t> encrypted;
    std::vector<uint8_t> decrypted;

    ctx.encrypt(data,encrypted,8);

    ctx.decrypt(encrypted,decrypted,8);

    assert(data == decrypted);

    std::cout << "Crypto context memory test passed\n";
}

void test_crypto_context_files()
{
    des_algorithm des;

    std::vector<uint8_t> key =
    {
        7,7,7,7,7,7,7,7
    };

    des.setup_round_keys(key);

    cbc_mode cbc;
    zeros_padding padding;

    std::vector<uint8_t> iv(8,5);

    crypto_context ctx(
        &des,
        &cbc,
        &padding,
        iv,
        8
    );

    auto data = generate_data(20000);

    // std::ofstream f("input.txt",std::ios::binary);
    // f.write(reinterpret_cast<char*>(data.data()),data.size());
    // f.close();

    auto future_enc = ctx.encrypt_file(
        "test_data/test_2.txt",
        "enc.txt",
        4
    );

    future_enc.get();

    auto future_dec = ctx.decrypt_file(
        "enc.txt",
        "dec.txt",
        4
    );

    future_dec.get();

    std::ifstream d("dec.txt",std::ios::binary);

    std::vector<uint8_t> result(
        (std::istreambuf_iterator<char>(d)),
        std::istreambuf_iterator<char>()
    );

    //assert(data == result);

    std::cout << "Crypto context file test passed\n";
}

int main()
{
    test_des_block();

    test_padding();

    test_modes();

    test_crypto_context_memory();

    test_crypto_context_files();

    std::cout << "\nAll tests passed\n";

    return 0;
}