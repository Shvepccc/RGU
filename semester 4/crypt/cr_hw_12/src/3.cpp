// make run TASK=3 ARGS="-g 12345678987654321"
// make run TASK=3 ARGS="-g key.txt"
// make run TASK=3 ARGS="-s '/Users/stepanorlov/Documents/Stepan Orlov/RGU/semester 4/crypt/cr_hw_12/test.txt'"
// make run TASK=3 ARGS="-v '/Users/stepanorlov/Documents/Stepan Orlov/RGU/semester 4/crypt/cr_hw_12/test.txt'"

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <boost/multiprecision/cpp_int.hpp>
#include "../include/modular_arithmetic.hpp"
#include "../include/diffie_hellman.hpp"
#include "../include/elgamal_signer.hpp"

bool is_file_exists(const std::string& path)
{
    std::ifstream f(path.c_str());
    return f.good();
}

bigint read_private_key_from_file(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Не удалось открыть файл с закрытым ключом: " + path);
    }
    std::string line;
    std::getline(file, line);
    return bigint(line);
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Использование:" << std::endl;
        std::cerr << "  Для генерации ключа: " << argv[0] << " -g <private_key или файл>" << std::endl;
        std::cerr << "  Для подписи: " << argv[0] << " -s <путь_к_файлу>" << std::endl;
        std::cerr << "  Для проверки: " << argv[0] << " -v <путь_к_файлу>" << std::endl;
        std::cerr << std::endl;
        std::cerr << "Примеры:" << std::endl;
        std::cerr << "  " << argv[0] << " -g 123456789" << std::endl;
        std::cerr << "  " << argv[0] << " -g private.txt" << std::endl;
        std::cerr << "  " << argv[0] << " -s document.docx" << std::endl;
        std::cerr << "  " << argv[0] << " -v document.docx" << std::endl;
        return 1;
    }
    
    std::string flag = argv[1];
    std::string arg = argv[2];
    
    try
    {
        if (flag == "-g")
        {
            std::cout << "=== Режим: ГЕНЕРАЦИЯ КЛЮЧА ===" << std::endl;
            
            bigint private_key;
            
            if (is_file_exists(arg))
            {
                std::cout << "Чтение закрытого ключа из файла: " << arg << std::endl;
                private_key = read_private_key_from_file(arg);
            }
            else
            {
                private_key = bigint(arg);
            }
            
            
            std::cout << "Генерация параметров p и g..." << std::endl;
            elgamal_signer signer(private_key);
            
            elgamal_keys keys = signer.get_keys();
            
            std::cout << "Параметры сгенерированны" << std::endl;
            
            std::string key_file = "user_sign.key";
            signer.save_keys(key_file);
            std::cout << "Ключи сохранены в файл: " << key_file << std::endl;
            std::cout << std::endl;
            std::cout << "ВАЖНО: Теперь используйте этот ключевой файл для подписи файлов." << std::endl;
            std::cout << "Скопируйте " << key_file << " в ту же директорию, где будете подписывать файлы." << std::endl;
        }
        else if (flag == "-s")
        {
            std::cout << "=== Режим: ПОДПИСАНИЕ ФАЙЛА ===" << std::endl;
            std::cout << "Файл: " << arg << std::endl;
            
            if (!is_file_exists(arg))
            {
                throw std::runtime_error("Файл не найден: " + arg);
            }
            
            std::vector<uint8_t> data = read_file(arg);
            std::cout << "Размер: " << data.size() << " байт" << std::endl;
            std::cout << std::endl;
            
            std::string key_file = "user_sign.key";
            if (!is_file_exists(key_file))
            {
                throw std::runtime_error("Ключевой файл не найден");
            }
            
            std::cout << "Загрузка ключей из файла: " << key_file << std::endl;
            elgamal_signer signer(bigint(2));
            signer.load_keys(key_file);
            
            elgamal_keys keys = signer.get_keys();
            std::cout << "Ключи загружены" << std::endl;
            std::cout << std::endl;
            
            std::cout << "Подписание файла..." << std::endl;
            elgamal_signature signature = signer.sign(data);
            
            std::string sig_file = arg + ".sig";
            signer.save_signature(sig_file, signature);
            std::cout << "Подпись сохранена в файл: " << sig_file << std::endl;
            std::cout << std::endl;
            std::cout << "Готово! Файл подписан." << std::endl;
        }
        else if (flag == "-v")
        {
            std::cout << "=== Режим: ПРОВЕРКА ПОДПИСИ ===" << std::endl;
            std::cout << "Файл: " << arg << std::endl;
            
            if (!is_file_exists(arg))
            {
                throw std::runtime_error("Файл не найден: " + arg);
            }
            
            std::vector<uint8_t> data = read_file(arg);
            std::cout << "Размер: " << data.size() << " байт" << std::endl;
            std::cout << std::endl;
            
            std::string key_file = "user_sign.key";
            if (!is_file_exists(key_file))
            {
                throw std::runtime_error("Ключевой файл не найден");
            }
            
            std::string sig_file = arg + ".sig";
            if (!is_file_exists(sig_file))
            {
                throw std::runtime_error("Файл подписи не найден: " + sig_file);
            }
            
            std::cout << "Загрузка ключей из файла: " << key_file << std::endl;
            elgamal_signer verifier(bigint(2));
            verifier.load_keys(key_file);
            
            std::cout << "Загрузка подписи из файла: " << sig_file << std::endl;
            elgamal_signature signature = verifier.load_signature(sig_file);
            
            std::cout << std::endl;
            std::cout << "Проверка подписи..." << std::endl;
            bool is_valid = verifier.verify(data, signature);
            
            std::cout << std::endl;
            std::cout << "Результат проверки: " 
                      << (is_valid ? "ПОДПИСЬ ВЕРНА" : "ПОДПИСЬ НЕВЕРНА") << std::endl;
        }
        else
        {
            std::cerr << "Ошибка: неизвестный флаг '" << flag << "'." << std::endl;
            std::cerr << "Используйте -g, -s или -v" << std::endl;
            return 1;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}