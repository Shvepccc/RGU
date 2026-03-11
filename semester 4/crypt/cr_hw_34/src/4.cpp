#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>

#include "../include/crypto_context.hpp"
#include "../include/des_algorithm.hpp"

// -----------------------------------------------------------
const std::string INPUT_FILE = "test_data/gf.mp4";
const std::string ENC_FILE = "test_enc.bin";
const std::string DEC_FILE = "test_dec.mp4";

const cipher_mode_type MODE = cipher_mode_type::CTR;
const padding_mode_type PADDING = padding_mode_type::PKCS7;
const size_t BLOCK_SIZE = 8;
const size_t THREADS_COUNT = 12;
// -----------------------------------------------------------

std::vector<uint8_t> random_bytes(size_t n)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 255);
    std::vector<uint8_t> v(n);
    for (auto& b : v) b = static_cast<uint8_t>(dist(gen));
    return v;
}

bool files_equal(const std::string& f1, const std::string& f2)
{
    std::ifstream in1(f1, std::ios::binary);
    std::ifstream in2(f2, std::ios::binary);
    if (!in1 || !in2) return false;

    std::istreambuf_iterator<char> it1(in1), end1;
    std::istreambuf_iterator<char> it2(in2), end2;
    return std::equal(it1, end1, it2, end2);
}

int main() {
    try {
        auto key = random_bytes(8);
        auto iv = random_bytes(BLOCK_SIZE);

        std::cout << "Key: ";
        for (auto b : key) std::cout << std::hex << (int)b << " ";
        std::cout << std::dec << std::endl;

        std::cout << "IV : ";
        for (auto b : iv) std::cout << std::hex << (int)b << " ";
        std::cout << std::dec << std::endl;

        des_algorithm des;
        des.setup_round_keys(key);

        crypto_context ctx(&des, MODE, PADDING, iv, BLOCK_SIZE);

        std::cout << "Encrypting " << INPUT_FILE << " -> " << ENC_FILE << " ..." << std::endl;
        auto start_enc = std::chrono::high_resolution_clock::now();
        auto fut_enc = ctx.encrypt_file(INPUT_FILE, ENC_FILE, THREADS_COUNT);
        fut_enc.get();
        auto end_enc = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> enc_time = end_enc - start_enc;
        std::cout << "Encryption done in " << enc_time.count() << " seconds." << std::endl;

        std::cout << "Decrypting " << ENC_FILE << " -> " << DEC_FILE << " ..." << std::endl;
        auto start_dec = std::chrono::high_resolution_clock::now();
        auto fut_dec = ctx.decrypt_file(ENC_FILE, DEC_FILE, THREADS_COUNT);
        fut_dec.get();
        auto end_dec = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> dec_time = end_dec - start_dec;
        std::cout << "Decryption done in " << dec_time.count() << " seconds." << std::endl;

        if (files_equal(INPUT_FILE, DEC_FILE))
        {
            std::cout << "SUCCESS: Original and decrypted files match." << std::endl;
        }
        else
        {
            std::cout << "FAILURE: Files differ!" << std::endl;
        }

    }
    catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}