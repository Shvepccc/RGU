#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <fstream>
#include <chrono>
#include "../include/el_gamal/el_gamal_algorithm.hpp"
#include "../include/modular_arithmetic.hpp"

int main(int argc, char* argv[])
{
    el_gamal_public_key pub;
    el_gamal_private_key priv;

    std::size_t key_strength_bytes = 16; 
    bigint min_p = bigint(1) << (key_strength_bytes * 8 - 1);
    bigint max_p = (bigint(1) << (key_strength_bytes * 8)) - 1;

    auto key_gen_start = std::chrono::high_resolution_clock::now();
    el_gamal::generate_keys(min_p, max_p, pub, priv);
    auto key_gen_end = std::chrono::high_resolution_clock::now();

    std::cout << "--- ElGamal Demonstration ---" << std::endl;
    std::cout << "Key Generation: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(key_gen_end - key_gen_start).count() 
              << " ms" << std::endl;

    std::cout << "\n[1] Array Data Test..." << std::endl;
    std::vector<uint8_t> data = {0xDE, 0xAD, 0xBE, 0xEF, 0x01, 0x02, 0x03, 0x04};
    bigint m = 0;
    for (uint8_t b : data)
    {
        m = (m << 8) | b;
    }

    auto array_enc_start = std::chrono::high_resolution_clock::now();
    el_gamal_ciphertext cipher = el_gamal::encrypt(m, pub);
    auto array_enc_end = std::chrono::high_resolution_clock::now();

    auto array_dec_start = std::chrono::high_resolution_clock::now();
    bigint decrypted_m = el_gamal::decrypt(cipher, pub, priv);
    auto array_dec_end = std::chrono::high_resolution_clock::now();

    if (m == decrypted_m)
    {
        std::cout << "Array Status: SUCCESS" << std::endl;
    }
    else
    {
        std::cout << "Array Status: FAILURE" << std::endl;
    }
    std::cout << "Array Enc/Dec Time: " 
              << std::chrono::duration_cast<std::chrono::microseconds>(array_dec_end - array_enc_start).count() 
              << " us" << std::endl;

    std::cout << "\n[2] File Encryption Test..." << std::endl;
    std::string input_filename = "gf.mp4";
    if (argc > 1)
    {
        input_filename = argv[1];
    }

    std::size_t dot_pos = input_filename.find_last_of('.');
    std::string base_name = (dot_pos != std::string::npos) ? input_filename.substr(0, dot_pos) : input_filename;
    std::string extension = (dot_pos != std::string::npos) ? input_filename.substr(dot_pos) : "";

    std::string enc_file = base_name + "_elgamal.enc" + extension;
    std::string dec_file = base_name + "_elgamal_dec" + extension;

    try
    {
        auto file_enc_start = std::chrono::high_resolution_clock::now();
        el_gamal::encrypt_file(input_filename, enc_file, pub);
        auto file_enc_end = std::chrono::high_resolution_clock::now();

        std::cout << "File Encryption: " 
                  << std::chrono::duration_cast<std::chrono::milliseconds>(file_enc_end - file_enc_start).count() 
                  << " ms" << std::endl;

        auto file_dec_start = std::chrono::high_resolution_clock::now();
        el_gamal::decrypt_file(enc_file, dec_file, pub, priv);
        auto file_dec_end = std::chrono::high_resolution_clock::now();

        std::cout << "File Decryption: " 
                  << std::chrono::duration_cast<std::chrono::milliseconds>(file_dec_end - file_dec_start).count() 
                  << " ms" << std::endl;

        std::ifstream f1(input_filename, std::ios::binary);
        std::ifstream f2(dec_file, std::ios::binary);
        std::vector<uint8_t> v1((std::istreambuf_iterator<char>(f1)), std::istreambuf_iterator<char>());
        std::vector<uint8_t> v2((std::istreambuf_iterator<char>(f2)), std::istreambuf_iterator<char>());

        if (v1 == v2 && !v1.empty())
        {
            std::cout << "Integrity Check: SUCCESS" << std::endl;
        }
        else
        {
            std::cout << "Integrity Check: FAILURE" << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}