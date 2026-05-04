#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <random>
#include <boost/multiprecision/cpp_int.hpp>
#include "../include/modular_arithmetic.hpp"
#include "../include/diffie_hellman.hpp"

struct elgamal_keys
{
    bigint p;
    bigint g;
    bigint y;
    bigint x;
};

struct elgamal_signature
{
    bigint r;
    bigint s;
};

class elgamal_signer
{
private:
    bigint p;
    bigint g;
    bigint x;
    bigint y;

    bigint generate_k(const bigint& p_minus_1) const
    {
        std::random_device rd;
        std::mt19937_64 gen(rd());
        
        while (true)
        {
            bigint k = 0;
            for (int i = 0; i < 8; ++i)
            {
                k = (k << 64) | gen();
            }
            k = k % (p_minus_1 - 1) + 1;
            
            if (gcd(k, p_minus_1) == 1)
            {
                return k;
            }
        }
    }

    bigint hash_message(const std::vector<uint8_t>& message) const
    {
        bigint hash = 0;
        for (size_t i = 0; i < message.size(); ++i)
        {
            hash = (hash << 8) | message[i];
        }
        return hash % p;
    }

public:
    elgamal_signer(const bigint& prime, const bigint& generator, const bigint& private_key)
        : p(prime)
        , g(generator)
        , x(private_key)
    {
        y = mod_pow(g, x, p);
    }

    elgamal_keys get_keys() const
    {
        return {p, g, y, x};
    }

    elgamal_signature sign(const std::vector<uint8_t>& message) const
    {
        bigint p_minus_1 = p - 1;
        bigint h = hash_message(message);
        
        while (true)
        {
            bigint k = generate_k(p_minus_1);
            bigint r = mod_pow(g, k, p);
            
            if (r == 0)
            {
                continue;
            }
            
            bigint k_inv = mod_inverse(k, p_minus_1);
            if (k_inv == 0)
            {
                continue;
            }
            
            bigint s = (k_inv * (h - x * r)) % p_minus_1;
            if (s < 0)
            {
                s += p_minus_1;
            }
            
            if (s != 0)
            {
                return {r, s};
            }
        }
    }

    bool verify(const std::vector<uint8_t>& message, const elgamal_signature& sig) const
    {
        if (sig.r <= 0 || sig.r >= p || sig.s <= 0 || sig.s >= p - 1)
        {
            return false;
        }
        
        bigint h = hash_message(message);
        bigint p_minus_1 = p - 1;
        
        bigint lhs = (mod_pow(g, h, p));
        
        bigint rhs = (mod_pow(y, sig.r, p) * mod_pow(sig.r, sig.s, p)) % p;
        
        return lhs == rhs;
    }
};

std::vector<uint8_t> string_to_bytes(const std::string& str)
{
    std::vector<uint8_t> result(str.begin(), str.end());
    return result;
}

std::string bytes_to_string(const std::vector<uint8_t>& bytes)
{
    return std::string(bytes.begin(), bytes.end());
}

void print_bigint(const std::string& label, const bigint& value)
{
    std::cout << label << ": " << value << std::endl;
}

void print_signature(const elgamal_signature& sig)
{
    std::cout << "Подпись:" << std::endl;
    std::cout << "  r = " << sig.r << std::endl;
    std::cout << "  s = " << sig.s << std::endl;
}

void demonstrate_elgamal_signature()
{
    std::cout << "========================================" << std::endl;
    std::cout << "Демонстрация схемы подписи Эль-Гамаля" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    bigint p("797984847998432998784379843279843279843279843279843279843279843279843279843279843279843279843279843279");
    bigint g("5");
    bigint x("12345678987654321");
    
    print_bigint("Используемое простое число p", p);
    print_bigint("Генератор g", g);
    print_bigint("Закрытый ключ x", x);
    std::cout << std::endl;
    
    elgamal_signer signer(p, g, x);
    elgamal_keys keys = signer.get_keys();
    
    print_bigint("Открытый ключ y = g^x mod p", keys.y);
    std::cout << std::endl;
    
    std::string message_text = "Hello World! Это тестовое сообщение для подписи Эль-Гамаля.";
    std::vector<uint8_t> message = string_to_bytes(message_text);
    
    std::cout << "Исходное сообщение: " << message_text << std::endl;
    std::cout << "Размер сообщения: " << message.size() << " байт" << std::endl;
    std::cout << std::endl;
    
    elgamal_signature signature = signer.sign(message);
    
    print_signature(signature);
    std::cout << std::endl;
    
    bool is_valid = signer.verify(message, signature);
    
    std::cout << "Результат проверки подписи: " << (is_valid ? "ПОДПИСЬ ВЕРНА" : "ПОДПИСЬ НЕВЕРНА") << std::endl;
    std::cout << std::endl;
    
    std::vector<uint8_t> tampered_message = message;
    if (!tampered_message.empty())
    {
        tampered_message[10] ^= 0xFF;
    }
    
    std::cout << "Изменённое сообщение (повреждён байт):" << std::endl;
    std::string tampered_str = bytes_to_string(tampered_message);
    std::cout << "  " << tampered_str << std::endl;
    std::cout << std::endl;
    
    bool is_valid_tampered = signer.verify(tampered_message, signature);
    
    std::cout << "Результат проверки изменённого сообщения: " 
              << (is_valid_tampered ? "ПОДПИСЬ ВЕРНА" : "ПОДПИСЬ НЕВЕРНА") << std::endl;
    std::cout << std::endl;
}

int main()
{
    
    demonstrate_elgamal_signature();
    
    return 0;
}