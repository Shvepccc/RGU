#include <iostream>
#include <vector>
#include <chrono>
#include "../include/ntru/ntru_algorithm.hpp" 

int main(int argc, char* argv[])
{
    ntru_cipher cipher;

    std::cout << "--- NTRUEncrypt File Demonstration ---" << std::endl;
    
    auto key_gen_start = std::chrono::high_resolution_clock::now();
    cipher.generate_keys();
    auto key_gen_end = std::chrono::high_resolution_clock::now();

    std::cout << "Key Generation: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(key_gen_end - key_gen_start).count() 
              << " ms" << std::endl;

    std::cout << "\nFile Encryption Test..." << std::endl;
    std::string input_filename = "gf.mp4";
    if (argc > 1)
    {
        input_filename = argv[1];
    }

    std::size_t dot_pos = input_filename.find_last_of('.');
    std::string base_name = (dot_pos != std::string::npos) ? input_filename.substr(0, dot_pos) : input_filename;
    std::string extension = (dot_pos != std::string::npos) ? input_filename.substr(dot_pos) : "";

    std::string enc_file = base_name + "_ntru.enc" + extension;
    std::string dec_file = base_name + "_ntru_dec" + extension;

    try
    {
        auto file_enc_start = std::chrono::high_resolution_clock::now();
        cipher.encrypt_file(input_filename, enc_file);
        auto file_enc_end = std::chrono::high_resolution_clock::now();

        std::cout << "File Encryption: " 
                  << std::chrono::duration_cast<std::chrono::milliseconds>(file_enc_end - file_enc_start).count() 
                  << " ms" << std::endl;

        auto file_dec_start = std::chrono::high_resolution_clock::now();
        cipher.decrypt_file(enc_file, dec_file);
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
            std::cout << "Integrity Check: SUCCESS (Files are identical)" << std::endl;
        }
        else
        {
            std::cout << "Integrity Check: FAILURE (Files differ or empty)" << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}