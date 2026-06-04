#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <boost/multiprecision/cpp_int.hpp>
#include "../include/modular_arithmetic.hpp"
#include "../include/diffie_hellman.hpp"
#include "../include/elgamal_signer.hpp"

std::vector<uint8_t> string_to_bytes(const std::string& str)
{
    std::vector<uint8_t> result(str.begin(), str.end());
    return result;
}

void print_bigint(const std::string& label, const bigint& value)
{
    std::cout << label << ": " << value << std::endl;
}

int main()
{
    bigint x("12345678987654321");
    
    print_bigint("Закрытый ключ x", x);
    std::cout << std::endl;
    
    elgamal_signer signer(x);
    elgamal_keys keys = signer.get_keys();
    
    print_bigint("p", keys.p);
    print_bigint("g", keys.g);
    std::cout << std::endl;
    
    std::string message_text = "Hello World! Это тестовое сообщение для подписи Эль-Гамаля.";
    std::vector<uint8_t> message = string_to_bytes(message_text);
    
    std::cout << "Исходное сообщение: " << message_text << std::endl;
    std::cout << std::endl;
    
    elgamal_signature signature = signer.sign(message);
    
    bool is_valid = signer.verify(message, signature);
    
    std::cout << "Результат проверки подписи: " << (is_valid ? "ПОДПИСЬ ВЕРНА" : "ПОДПИСЬ НЕВЕРНА") << std::endl;
    std::cout << std::endl;
    
    std::vector<uint8_t> tampered_message = message;
    if (!tampered_message.empty())
    {
        tampered_message[10] ^= 0xFF;
    }
    
    std::cout << "Изменённое сообщение:" << std::string(tampered_message.begin(), tampered_message.end()) << std::endl;
    std::cout << std::endl;
    
    bool is_valid_tampered = signer.verify(tampered_message, signature);
    
    std::cout << "Результат проверки изменённого сообщения: " 
              << (is_valid_tampered ? "ПОДПИСЬ ВЕРНА" : "ПОДПИСЬ НЕВЕРНА") << std::endl;
    
    return 0;
}