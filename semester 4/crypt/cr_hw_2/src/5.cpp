#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <random>
#include <chrono>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <algorithm>
#include <sys/types.h>
#include <unistd.h>

#include "../include/crypto_processor.hpp"
#include "../include/des_algorithm.hpp"
#include "../include/triple_des_algorithm.hpp"

#define PATH "/Users/stepanorlov/Documents/Stepan Orlov/RGU/semester 4/crypt/cr_hw_2/src/test_paths.txt"

bool file_exists(const std::string& path)
{
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

size_t file_size(const std::string& path)
{
    struct stat buffer;
    if (stat(path.c_str(), &buffer) != 0)
        return 0;
    return buffer.st_size;
}

std::string get_file_extension(const std::string& path)
{
    size_t pos = path.find_last_of('.');
    if (pos != std::string::npos)
        return path.substr(pos);
    return "";
}

std::string get_filename(const std::string& path)
{
    size_t pos = path.find_last_of('/');
    if (pos != std::string::npos)
        return path.substr(pos + 1);
    return path;
}

std::string get_directory(const std::string& path)
{
    size_t pos = path.find_last_of('/');
    if (pos != std::string::npos)
        return path.substr(0, pos);
    return ".";
}

bool create_directory(const std::string& path)
{
    std::string cmd = "mkdir -p \"" + path + "\"";
    return (system(cmd.c_str()) == 0);
}

std::string format_file_size(size_t bytes)
{
    const char* units[] = {"B", "KB", "MB", "GB"};
    int unit = 0;
    double size = bytes;
    
    while (size >= 1024 && unit < 3)
    {
        size /= 1024;
        unit++;
    }
    
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << size << " " << units[unit];
    return ss.str();
}

std::vector<uint8_t> generate_random_bytes(size_t size, uint32_t seed = 0)
{
    std::vector<uint8_t> data(size);
    
    if (seed == 0)
        seed = static_cast<uint32_t>(std::chrono::system_clock::now().time_since_epoch().count());
    
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0, 255);
    
    for (size_t i = 0; i < size; ++i)
        data[i] = static_cast<uint8_t>(dist(rng));
    
    return data;
}

std::string bytes_to_hex(const std::vector<uint8_t>& data, size_t max = 32)
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    
    size_t display = std::min(data.size(), max);
    for (size_t i = 0; i < display; ++i)
        ss << std::setw(2) << static_cast<int>(data[i]);
    
    if (data.size() > max)
        ss << "... (" << data.size() << " bytes total)";
    
    return ss.str();
}

std::vector<std::string> read_test_paths(const std::string& filename)
{
    std::vector<std::string> paths;
    std::ifstream file(filename.c_str());
    
    if (!file.is_open())
    {
        std::cerr << "Warning: Could not open " << filename << std::endl;
        return paths;
    }
    
    std::string line;
    while (std::getline(file, line))
    {
        if (!line.empty() && line[0] != '#')
        {
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);
            
            if (!line.empty())
                paths.push_back(line);
        }
    }
    
    return paths;
}

void test_random_data(crypto_processor& processor, const std::string& algo_name, size_t data_size = 256)
{
    std::cout << "\n  Testing random data (" << data_size << " bytes):\n";
    
    auto plain = generate_random_bytes(data_size);
    std::cout << "    Plaintext : " << bytes_to_hex(plain) << "\n";
    
    auto cipher = processor.encrypt(plain);
    std::cout << "    Ciphertext: " << bytes_to_hex(cipher) << "\n";
    
    auto decrypted = processor.decrypt(cipher);
    std::cout << "    Decrypted : " << bytes_to_hex(decrypted) << "\n";
    
    bool success = (plain == decrypted);
    std::cout << "    Result    : " << (success ? "PASS" : "FAIL") << "\n";
}

void test_file_operation(crypto_processor& processor, const std::string& algo_name, 
                         const std::string& input_path)
{
    if (!file_exists(input_path))
    {
        std::cout << "File not found: " << input_path << "\n";
        return;
    }
    
    std::string filename = get_filename(input_path);
    std::string file_dir = get_directory(input_path);
    std::string ext = get_file_extension(filename);
    std::string name_without_ext = filename.substr(0, filename.size() - ext.size());
    
    std::string results_dir = file_dir + "/results";
    if (!file_exists(results_dir))
    {
        if (!create_directory(results_dir))
        {
            std::cout << "Cannot create results directory: " << results_dir << "\n";
            return;
        }
    }
    
    std::string file_results_dir = results_dir + "/" + name_without_ext;
    if (!file_exists(file_results_dir))
    {
        if (!create_directory(file_results_dir))
        {
            std::cout << "Cannot create file results directory: " << file_results_dir << "\n";
            return;
        }
    }
    
    size_t fsize = file_size(input_path);
    
    std::string encrypted_path = file_results_dir + "/" + filename + "." + algo_name + ".enc";
    std::string decrypted_path = file_results_dir + "/" + name_without_ext + "." + algo_name + ".dec" + ext;
    
    std::cout << "  Input : " << input_path << "\n";
    std::cout << "    Size: " << format_file_size(fsize) << "\n";
    std::cout << "    Results dir: " << file_results_dir << "\n";
    
    // Шифрование
    auto start = std::chrono::high_resolution_clock::now();
    processor.encrypt_file(input_path, encrypted_path);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto enc_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "    Encrypted to : " << encrypted_path << "\n";
    std::cout << "    Enc time     : " << enc_duration.count() << " ms\n";
    
    // Дешифрование
    start = std::chrono::high_resolution_clock::now();
    processor.decrypt_file(encrypted_path, decrypted_path);
    end = std::chrono::high_resolution_clock::now();
    
    auto dec_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "    Decrypted to : " << decrypted_path << "\n";
    std::cout << "    Dec time     : " << dec_duration.count() << " ms\n";
    
    // Проверка
    std::ifstream orig(input_path.c_str(), std::ios::binary);
    std::ifstream dec(decrypted_path.c_str(), std::ios::binary);
    
    std::vector<uint8_t> orig_data((std::istreambuf_iterator<char>(orig)), {});
    std::vector<uint8_t> dec_data((std::istreambuf_iterator<char>(dec)), {});
    
    bool success = (orig_data == dec_data);
    std::cout << "    Verification: " << (success ? "PASS" : "FAIL") << "\n";
    
    if (!success)
    {
        std::cout << "      Original size: " << orig_data.size() << "\n";
        std::cout << "      Decrypted size: " << dec_data.size() << "\n";
    }
}

// ============================================================================

void demonstrate_des()
{
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "DEMONSTRATION: DES ALGORITHM\n";
    std::cout << std::string(60, '=') << "\n";
    
    des_algorithm* des = new des_algorithm();
    crypto_processor processor(des, 8);
    
    std::vector<uint8_t> key = {0x13, 0x34, 0x57, 0x79, 0x9B, 0xBC, 0xDF, 0xF1};
    processor.set_key(key);
    
    test_random_data(processor, "DES", 64);
    
    auto paths = read_test_paths(PATH);
    
    if (paths.empty())
    {
        std::cout << "\nNo test files to process.\n";
        return;
    }
    
    std::cout << "\nProcessing test files with DES:\n";
    for (const auto& path : paths)
    {
        std::cout << "\n";
        test_file_operation(processor, "DES", path);
    }
}

void demonstrate_triple_des(triple_des_mode mode, const std::string& mode_name)
{
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "DEMONSTRATION: 3DES - " << mode_name << "\n";
    std::cout << std::string(60, '=') << "\n";
    
    triple_des_algorithm* tdes = new triple_des_algorithm(mode);
    crypto_processor processor(tdes, 8);
    
    std::vector<uint8_t> key;
    if (mode == triple_des_mode::DES_EDE3 || mode == triple_des_mode::DES_EEE3)
    {
        key = {
            0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
            0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
            0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88
        };
    }
    else
    {
        key = {
            0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
            0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10
        };
    }
    
    processor.set_key(key);
    
    test_random_data(processor, "3DES-" + mode_name, 64);
    
    auto paths = read_test_paths(PATH);
    
    if (paths.empty())
    {
        std::cout << "\nNo test files to process.\n";
        return;
    }
    
    std::cout << "\nProcessing test files with 3DES-" << mode_name << ":\n";
    for (const auto& path : paths)
    {
        std::cout << "\n";
        test_file_operation(processor, "3DES-" + mode_name, path);
    }
}

void clean_results_directory(const std::string& base_path)
{
    std::string results_dir = base_path + "/results";
    
    if (file_exists(results_dir))
    {
        std::cout << "Cleaning results directory: " << results_dir << "\n";
        std::string cmd = "rm -rf \"" + results_dir + "\"";
        system(cmd.c_str());
    }
}

// ============================================================================
int main()
{
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║     DEMONSTRATION OF DES AND 3DES ALGORITHMS               ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    
    try
    {
        std::string base_dir = get_directory(std::string(PATH));
        clean_results_directory(base_dir);
        std::cout << "Results directory cleaned.\n\n";
        
        // DES
        demonstrate_des();
        
        // 3DES
        demonstrate_triple_des(triple_des_mode::DES_EDE3, "EDE3");
        demonstrate_triple_des(triple_des_mode::DES_EEE3, "EEE3");
        demonstrate_triple_des(triple_des_mode::DES_EDE2, "EDE2");
        demonstrate_triple_des(triple_des_mode::DES_EEE2, "EEE2");
        
        std::cout << "\n" << std::string(60, '=') << "\n";
        std::cout << "DEMONSTRATION COMPLETED SUCCESSFULLY\n";
        std::cout << std::string(60, '=') << "\n\n";
        
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "\nERROR: " << e.what() << "\n";
        return 1;
    }
}