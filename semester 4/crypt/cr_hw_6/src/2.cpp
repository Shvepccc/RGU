#include "../include/modular_arithmetic.hpp"
#include "../include/rsa/rsa_key_generator.hpp"
#include <iostream>


int main()
{
    try
    {
        rsa_key_generator generator(16, 0.999999);
        
        auto keys = generator.generate_keys();
        
        std::cout << "RSA Key Pair Generated:\n";
        std::cout << "Public key (e):  " << keys.public_key << "\n";
        std::cout << "Private key (d): " << keys.private_key << "\n";
        std::cout << "Modulus (n):     " << keys.modulus << "\n";
        
        uint64_t message = 12345;
        uint64_t encrypted = mod_pow(message, keys.public_key, keys.modulus);
        uint64_t decrypted = mod_pow(encrypted, keys.private_key, keys.modulus);
        
        std::cout << "\nTest encryption/decryption:\n";
        std::cout << "Original message:   " << message << "\n";
        std::cout << "Encrypted:          " << encrypted << "\n";
        std::cout << "Decrypted:          " << decrypted << "\n";
        
        if (message == decrypted)
        {
            std::cout << "\nSuccess: Encryption/decryption works correctly!\n";
        }
        else
        {
            std::cout << "\nError: Encryption/decryption failed!\n";
        }
    }
    catch (const std::exception& ex)
    {
        std::cout << "Error: " << ex.what() << "\n";
        return 1;
    }
    
    return 0;
}