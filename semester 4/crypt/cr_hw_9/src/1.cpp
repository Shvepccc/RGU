#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <fstream>
#include <chrono>
#include "../include/crypto_context.hpp"
#include "../include/crypto_enums.hpp"
#include "../include/twofish/twofish_algorithm.hpp"

int main(int argc, char* argv[])
{
    cipher_mode_type selected_mode = cipher_mode_type::CTR;      
    padding_mode_type selected_padding = padding_mode_type::PKCS7; 
    std::size_t block_size_bits = 128; // fixed!
    std::size_t key_size_bits = 256;   // 128, 192, 256
    std::size_t num_threads = 20;

    std::size_t block_size_bytes = block_size_bits / 8;
    
    std::vector<std::uint8_t> key(key_size_bits / 8, 0x42);
    std::vector<std::uint8_t> iv(block_size_bytes, 0x11);

    twofish cipher_instance; 
    try {
        cipher_instance.setup_round_keys(key);
    } catch (const std::exception& e) {
        std::cerr << "Key setup error: " << e.what() << std::endl;
        return 1;
    }

    crypto_context ctx(
        &cipher_instance,
        selected_mode,
        selected_padding,
        iv,
        block_size_bytes
    );

    std::cout << "--- Twofish Algorithm Test ---" << std::endl;
    std::cout << "Mode: " << (int)selected_mode << " | Key size: " << key_size_bits << " bits" << std::endl;

    // --- Тест на массиве байт ---
    std::cout << "\n[1] Synchronous Byte Array Test..." << std::endl;
    std::vector<std::uint8_t> data_to_encrypt = {0x54, 0x77, 0x6f, 0x66, 0x69, 0x73, 0x68, 0x20, 0x54, 0x65, 0x73, 0x74};
    std::vector<std::uint8_t> encrypted_data;
    std::vector<std::uint8_t> decrypted_data;

    ctx.encrypt(data_to_encrypt, encrypted_data, num_threads);
    ctx.decrypt(encrypted_data, decrypted_data, num_threads);

    if (data_to_encrypt == decrypted_data) {
        std::cout << "Array Status: SUCCESS" << std::endl;
    } else {
        std::cout << "Array Status: FAILURE" << std::endl;
    }

    // --- Тест на файле ---
    std::cout << "\n[2] Asynchronous File Test..." << std::endl;
    std::string input_filename = "gf.mp4";
    if (argc > 1) input_filename = argv[1];

    std::ifstream check_file(input_filename, std::ios::binary);
    if (!check_file.good()) {
        std::cerr << "Skip file test: File '" << input_filename << "' not found." << std::endl;
        return 0;
    }
    check_file.close();

    std::size_t dot_pos = input_filename.find_last_of('.');
    std::string base_name = (dot_pos != std::string::npos) ? input_filename.substr(0, dot_pos) : input_filename;
    std::string extension = (dot_pos != std::string::npos) ? input_filename.substr(dot_pos) : "";

    std::string enc_file = base_name + "_tf.enc" + extension;
    std::string dec_file = base_name + "_tf_dec" + extension;

    try {
        auto enc_start = std::chrono::high_resolution_clock::now();
        auto f_enc = ctx.encrypt_file(input_filename, enc_file, num_threads);
        f_enc.get(); 
        auto enc_end = std::chrono::high_resolution_clock::now();
        std::cout << "Encryption: " << std::chrono::duration_cast<std::chrono::milliseconds>(enc_end - enc_start).count() << " ms" << std::endl;

        auto dec_start = std::chrono::high_resolution_clock::now();
        auto f_dec = ctx.decrypt_file(enc_file, dec_file, num_threads);
        f_dec.get();
        auto dec_end = std::chrono::high_resolution_clock::now();
        std::cout << "Decryption: " << std::chrono::duration_cast<std::chrono::milliseconds>(dec_end - dec_start).count() << " ms" << std::endl;

        std::ifstream f1(input_filename, std::ios::binary);
        std::ifstream f2(dec_file, std::ios::binary);
        std::vector<uint8_t> v1((std::istreambuf_iterator<char>(f1)), {});
        std::vector<uint8_t> v2((std::istreambuf_iterator<char>(f2)), {});

        if (v1 == v2 && !v1.empty()) {
            std::cout << "SUCCESS: Files are identical." << std::endl;
        } else {
            std::cout << "FAILURE: Decrypted file differs!" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Critical Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}