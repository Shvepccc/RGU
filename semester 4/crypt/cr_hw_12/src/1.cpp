#include <vector>
#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <fstream>
#include <chrono>
#include "../include/rabin_algorithm.hpp"
#include "../include/paddings/ansi_x923_padding.hpp"

int main(int argc, char* argv[])
{
    std::size_t key_bits = 1024;
    
    ansi_x923_padding padding;
    
    auto key_gen_start = std::chrono::high_resolution_clock::now();
    rabin_cryptosystem rabin(key_bits, padding);
    auto key_gen_end = std::chrono::high_resolution_clock::now();
    
    std::cout << "--- Rabin Cryptosystem Demonstration ---" << std::endl;
    std::cout << "Key Generation (" << key_bits << " bits): "
              << std::chrono::duration_cast<std::chrono::milliseconds>(key_gen_end - key_gen_start).count()
              << " ms" << std::endl;
    
    std::cout << "\n[1] Array Data Test..." << std::endl;
    std::vector<uint8_t> data = {0xDE, 0xAD, 0xBE, 0xEF, 0x01, 0x02, 0x03, 0x04};
    
    auto array_enc_start = std::chrono::high_resolution_clock::now();
    std::vector<uint8_t> cipher = rabin.encrypt(data);
    auto array_enc_end = std::chrono::high_resolution_clock::now();
    
    auto array_dec_start = std::chrono::high_resolution_clock::now();
    std::vector<uint8_t> decrypted_data = rabin.decrypt(cipher);
    auto array_dec_end = std::chrono::high_resolution_clock::now();
    
    if (data == decrypted_data)
    {
        std::cout << "Array Status: SUCCESS" << std::endl;
    }
    else
    {
        std::cout << "Array Status: FAILURE" << std::endl;
    }
    std::cout << "Array Enc/Dec Time: "
              << std::chrono::duration_cast<std::chrono::microseconds>(array_dec_end - array_enc_start).count()
              << " ms" << std::endl;
    
    std::cout << "\n[2] File Encryption Test..." << std::endl;
    std::string input_filename = "gf.mp4";
    if (argc > 1)
    {
        input_filename = argv[1];
    }
    
    std::size_t dot_pos = input_filename.find_last_of('.');
    std::string base_name = (dot_pos != std::string::npos) ? input_filename.substr(0, dot_pos) : input_filename;
    std::string extension = (dot_pos != std::string::npos) ? input_filename.substr(dot_pos) : "";
    
    std::string enc_file = base_name + "_rabin.enc";
    std::string dec_file = base_name + "_rabin_dec" + extension;
    
    try
    {
        auto file_enc_start = std::chrono::high_resolution_clock::now();
        
        std::ifstream in_file(input_filename, std::ios::binary);
        if (!in_file)
        {
            throw std::runtime_error("Cannot open input file");
        }
        
        std::vector<uint8_t> file_data((std::istreambuf_iterator<char>(in_file)),
                                        std::istreambuf_iterator<char>());
        in_file.close();
        
        std::vector<uint8_t> encrypted_data = rabin.encrypt(file_data);
        
        std::ofstream out_enc(enc_file, std::ios::binary);
        out_enc.write(reinterpret_cast<const char*>(encrypted_data.data()), encrypted_data.size());
        out_enc.close();
        
        auto file_enc_end = std::chrono::high_resolution_clock::now();
        
        std::cout << "File Encryption: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(file_enc_end - file_enc_start).count()
                  << " ms" << std::endl;
        
        auto file_dec_start = std::chrono::high_resolution_clock::now();
        
        std::ifstream in_enc(enc_file, std::ios::binary);
        std::vector<uint8_t> cipher_data((std::istreambuf_iterator<char>(in_enc)),
                                          std::istreambuf_iterator<char>());
        in_enc.close();
        
        std::vector<uint8_t> decrypted_file_data = rabin.decrypt(cipher_data);
        
        std::ofstream out_dec(dec_file, std::ios::binary);
        out_dec.write(reinterpret_cast<const char*>(decrypted_file_data.data()), decrypted_file_data.size());
        out_dec.close();
        
        auto file_dec_end = std::chrono::high_resolution_clock::now();
        
        std::cout << "File Decryption: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(file_dec_end - file_dec_start).count()
                  << " ms" << std::endl;

        std::cout << "Original size: " << file_data.size() << std::endl;
        std::cout << "Decrypted size: " << decrypted_file_data.size() << std::endl;
        if (file_data == decrypted_file_data)
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