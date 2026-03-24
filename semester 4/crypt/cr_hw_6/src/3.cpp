#include <iostream>
#include "../include/rsa/rsa_alghorithm.hpp"
#include "../include/rsa/rsa_key_generator.hpp"
#include <string>

int main(int argc, char* argv[])
{
    try
    {
        std::string input_filename = "2_test.txt";
        
        if (argc > 1)
        {
            input_filename = argv[1];
        }
        
        size_t dot_pos = input_filename.find_last_of('.');
        std::string base_name = (dot_pos != std::string::npos) ? input_filename.substr(0, dot_pos) : input_filename;
        std::string extension = (dot_pos != std::string::npos) ? input_filename.substr(dot_pos) : "";
        
        std::string encrypted_filename = base_name + ".enc" + extension;
        std::string decrypted_filename = base_name + ".dec" + extension;
        
        rsa_key_generator generator(16, 0.999999);
        auto keys = generator.generate_keys();
        
        std::cout << "RSA keys generated:\n";
        std::cout << "Public key (e): " << keys.public_key << "\n";
        std::cout << "Private key (d): " << keys.private_key << "\n";
        std::cout << "Modulus (n): " << keys.modulus << "\n\n";
        
        std::cout << "Input file: " << input_filename << "\n";
        std::cout << "Encrypted file: " << encrypted_filename << "\n";
        std::cout << "Decrypted file: " << decrypted_filename << "\n\n";
        
        rsa_cipher cipher_encrypt(keys.public_key, keys.modulus);
        rsa_cipher cipher_decrypt(keys.public_key, keys.private_key, keys.modulus);
        
        std::cout << "Encrypting file...\n";
        cipher_encrypt.encrypt_file(input_filename, encrypted_filename);
        std::cout << "Encryption completed.\n\n";
        
        std::cout << "Decrypting file...\n";
        cipher_decrypt.decrypt_file(encrypted_filename, decrypted_filename);
        std::cout << "Decryption completed.\n\n";
        
        std::ifstream original(input_filename, std::ios::binary);
        std::ifstream decrypted(decrypted_filename, std::ios::binary);
        
        if (original.is_open() && decrypted.is_open())
        {
            std::vector<uint8_t> orig_data((std::istreambuf_iterator<char>(original)), std::istreambuf_iterator<char>());
            std::vector<uint8_t> dec_data((std::istreambuf_iterator<char>(decrypted)), std::istreambuf_iterator<char>());
            
            if (orig_data == dec_data)
            {
                std::cout << "SUCCESS: Decrypted file matches original file!\n";
            }
            else
            {
                std::cout << "ERROR: Decrypted file does not match original file!\n";
            }
            
            original.close();
            decrypted.close();
        }
        else
        {
            std::cout << "Warning: Could not verify decryption result\n";
        }
    }
    catch (const std::exception& ex)
    {
        std::cout << "Error: " << ex.what() << "\n";
        return 1;
    }
    
    return 0;
}