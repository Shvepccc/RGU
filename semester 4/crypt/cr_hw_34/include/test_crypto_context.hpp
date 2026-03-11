#ifndef CRYPTO_TESTS_H
#define CRYPTO_TESTS_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <future>
#include <cstring>
#include "../include/crypto_context.hpp"
#include "../include/des_algorithm.hpp"

class crypto_tests
{
private:
    static int total_tests;
    static int passed_tests;

    static void check(bool condition, const std::string& test_name)
    {
        total_tests++;
        if (condition)
        {
            passed_tests++;
            std::cout << "[PASS] " << test_name << std::endl;
        }
        else
        {
            std::cout << "[FAIL] " << test_name << std::endl;
        }
    }

    static bool vectors_equal(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b)
    {
        if (a.size() != b.size()) return false;
        for (size_t i = 0; i < a.size(); i++)
            if (a[i] != b[i]) return false;
        return true;
    }

    static void print_vector(const std::vector<uint8_t>& v, const std::string& name)
    {
        std::cout << name << " (" << v.size() << "): ";
        for (auto b : v) std::cout << std::hex << (int)b << " ";
        std::cout << std::dec << std::endl;
    }

public:
    static void test_context_creation()
    {
        des_algorithm des;
        std::vector<uint8_t> key = {1,2,3,4,5,6,7,8};
        des.setup_round_keys(key);
        std::vector<uint8_t> iv(8, 0);

        try
        {
            crypto_context ctx1(&des, cipher_mode_type::ECB, padding_mode_type::PKCS7, iv, 8);
            crypto_context ctx2(&des, cipher_mode_type::CBC, padding_mode_type::Zeros, iv, 8);
            crypto_context ctx3(&des, cipher_mode_type::PCBC, padding_mode_type::ANSI_X923, iv, 8);
            check(true, "Context creation with valid modes");
        }
        catch(...)
        {
            check(false, "Context creation with valid modes");
        }

        try
        {
            crypto_context ctx_bad(nullptr, cipher_mode_type::ECB, padding_mode_type::PKCS7, iv, 8);
            check(false, "Context creation with null cipher should throw");
        }
        catch(const std::invalid_argument&)
        {
            check(true, "Context creation with null cipher throws");
        }
        catch(...)
        {
            check(false, "Context creation with null cipher throws wrong exception");
        }
    }

    static void test_ecb_mode()
    {
        des_algorithm des;
        std::vector<uint8_t> key = {1,2,3,4,5,6,7,8};
        des.setup_round_keys(key);
        std::vector<uint8_t> iv(8, 0);

        // PKCS7
        crypto_context ctx_ecb_pkcs(&des, cipher_mode_type::ECB, padding_mode_type::PKCS7, iv, 8);
        std::vector<uint8_t> plain = {'H','e','l','l','o'};
        std::vector<uint8_t> enc, dec;
        ctx_ecb_pkcs.encrypt(plain, enc, 1);
        ctx_ecb_pkcs.decrypt(enc, dec, 1);
        check(vectors_equal(plain, dec), "ECB + PKCS7");

        // Zeros
        crypto_context ctx_ecb_zero(&des, cipher_mode_type::ECB, padding_mode_type::Zeros, iv, 8);
        ctx_ecb_zero.encrypt(plain, enc, 1);
        ctx_ecb_zero.decrypt(enc, dec, 1);
        check(vectors_equal(plain, dec), "ECB + Zeros");

        // ANSI X923
        crypto_context ctx_ecb_ansi(&des, cipher_mode_type::ECB, padding_mode_type::ANSI_X923, iv, 8);
        ctx_ecb_ansi.encrypt(plain, enc, 1);
        ctx_ecb_ansi.decrypt(enc, dec, 1);
        check(vectors_equal(plain, dec), "ECB + ANSI X923");

        // ISO10126
        crypto_context ctx_ecb_iso(&des, cipher_mode_type::ECB, padding_mode_type::ISO10126, iv, 8);
        ctx_ecb_iso.encrypt(plain, enc, 1);
        ctx_ecb_iso.decrypt(enc, dec, 1);
        check(vectors_equal(plain, dec), "ECB + ISO10126");

        // Пустые данные
        plain.clear();
        ctx_ecb_pkcs.encrypt(plain, enc, 1);
        ctx_ecb_pkcs.decrypt(enc, dec, 1);
        check(dec.empty(), "ECB + empty data");

        // Один блок
        plain = {'1','2','3','4','5','6','7','8'};
        ctx_ecb_pkcs.encrypt(plain, enc, 1);
        ctx_ecb_pkcs.decrypt(enc, dec, 1);
        check(vectors_equal(plain, dec), "ECB + exact block size");
    }

    static void test_cbc_mode()
    {
        des_algorithm des;
        std::vector<uint8_t> key = {1,2,3,4,5,6,7,8};
        des.setup_round_keys(key);
        std::vector<uint8_t> iv = {0,1,2,3,4,5,6,7};

        crypto_context ctx_cbc(&des, cipher_mode_type::CBC, padding_mode_type::PKCS7, iv, 8);
        std::vector<uint8_t> plain = {'H','e','l','l','o',' ','W','o','r','l','d'};
        std::vector<uint8_t> enc, dec;
        ctx_cbc.encrypt(plain, enc, 1);
        ctx_cbc.decrypt(enc, dec, 1);
        check(vectors_equal(plain, dec), "CBC + PKCS7");

        std::vector<uint8_t> bad_iv = {1,1,1,1,1,1,1,1};
        crypto_context ctx_bad_iv(&des, cipher_mode_type::CBC, padding_mode_type::PKCS7, bad_iv, 8);
        std::vector<uint8_t> dec_bad;
        ctx_bad_iv.decrypt(enc, dec_bad, 1);
        check(!vectors_equal(plain, dec_bad), "CBC + wrong IV gives different result");
    }

    static void test_pcbc_mode()
    {
        des_algorithm des;
        std::vector<uint8_t> key = {1,2,3,4,5,6,7,8};
        des.setup_round_keys(key);
        std::vector<uint8_t> iv = {0,1,2,3,4,5,6,7};

        crypto_context ctx_pcbc(&des, cipher_mode_type::PCBC, padding_mode_type::ANSI_X923, iv, 8);
        std::vector<uint8_t> plain = {'P','C','B','C'};
        std::vector<uint8_t> enc, dec;
        ctx_pcbc.encrypt(plain, enc, 1);
        ctx_pcbc.decrypt(enc, dec, 1);
        check(vectors_equal(plain, dec), "PCBC + ANSI X923");
    }

    static void test_cfb_mode()
    {
        des_algorithm des;
        std::vector<uint8_t> key = {1,2,3,4,5,6,7,8};
        des.setup_round_keys(key);
        std::vector<uint8_t> iv = {0,1,2,3,4,5,6,7};

        crypto_context ctx_cfb(&des, cipher_mode_type::CFB, padding_mode_type::PKCS7, iv, 8);
        std::vector<uint8_t> plain = {'C','F','B',' ','m','o','d','e'};
        std::vector<uint8_t> enc, dec;
        ctx_cfb.encrypt(plain, enc, 1);
        ctx_cfb.decrypt(enc, dec, 1);
        check(vectors_equal(plain, dec), "CFB + PKCS7");
    }

    static void test_ofb_mode()
    {
        des_algorithm des;
        std::vector<uint8_t> key = {1,2,3,4,5,6,7,8};
        des.setup_round_keys(key);
        std::vector<uint8_t> iv = {0,1,2,3,4,5,6,7};

        crypto_context ctx_ofb(&des, cipher_mode_type::OFB, padding_mode_type::PKCS7, iv, 8);
        std::vector<uint8_t> plain = {'O','F','B'};
        std::vector<uint8_t> enc, dec;
        ctx_ofb.encrypt(plain, enc, 1);
        ctx_ofb.decrypt(enc, dec, 1);
        check(vectors_equal(plain, dec), "OFB + PKCS7");
    }

    static void test_ctr_mode()
    {
        des_algorithm des;
        std::vector<uint8_t> key = {1,2,3,4,5,6,7,8};
        des.setup_round_keys(key);
        std::vector<uint8_t> iv = {0,0,0,0,0,0,0,0};

        crypto_context ctx_ctr(&des, cipher_mode_type::CTR, padding_mode_type::PKCS7, iv, 8);
        std::vector<uint8_t> plain = {'C','T','R',' ','c','o','u','n','t','e','r'};
        std::vector<uint8_t> enc, dec;
        ctx_ctr.encrypt(plain, enc, 1);
        ctx_ctr.decrypt(enc, dec, 1);
        check(vectors_equal(plain, dec), "CTR + PKCS7");
    }

    static void test_random_delta_mode()
    {
        des_algorithm des;
        std::vector<uint8_t> key = {1,2,3,4,5,6,7,8};
        des.setup_round_keys(key);
        std::vector<uint8_t> iv = {0,1,2,3,4,5,6,7};

        crypto_context ctx_rd(&des, cipher_mode_type::RANDOM_DELTA, padding_mode_type::PKCS7, iv, 8);
        std::vector<uint8_t> plain = {'R','a','n','d','o','m'};
        std::vector<uint8_t> enc, dec;
        ctx_rd.encrypt(plain, enc, 1);
        ctx_rd.decrypt(enc, dec, 1);
        check(vectors_equal(plain, dec), "RANDOM_DELTA + PKCS7");
    }

    static void test_padding_edge_cases()
    {
        des_algorithm des;
        std::vector<uint8_t> key = {1,2,3,4,5,6,7,8};
        des.setup_round_keys(key);
        std::vector<uint8_t> iv(8, 0);

        zeros_padding zero_pad;
        std::vector<uint8_t> data_with_zeros = {'a','b','c', 0,0,0};
        std::vector<uint8_t> data_without_zeros = {'a','b','c', 1,2,3};
        
        auto unpadded1 = zero_pad.unpad(data_with_zeros, 8);
        check(unpadded1.size() == 3 && unpadded1[0] == 'a' && unpadded1[1] == 'b' && unpadded1[2] == 'c', 
            "Zeros unpad removes trailing zeros");
        
        auto unpadded2 = zero_pad.unpad(data_without_zeros, 8);
        check(vectors_equal(unpadded2, data_without_zeros), 
            "Zeros unpad leaves non-zero data unchanged");

        pkcs7_padding pkcs_pad;
        std::vector<uint8_t> bad_pkcs1 = {'a','b','c', 5,5,5,5,6};
        try {
            pkcs_pad.unpad(bad_pkcs1, 8);
            check(false, "PKCS7 unpad with inconsistent pad bytes should throw");
        } catch(const std::runtime_error&) {
            check(true, "PKCS7 unpad throws on inconsistent pad bytes");
        } catch(...) {
            check(false, "PKCS7 unpad throws wrong exception");
        }
        
        std::vector<uint8_t> bad_pkcs2 = {'a','b','c', 9,9,9,9,9};
        try {
            pkcs_pad.unpad(bad_pkcs2, 8);
            check(false, "PKCS7 unpad with pad > block size should throw");
        } catch(const std::runtime_error&) {
            check(true, "PKCS7 unpad throws on pad > block size");
        } catch(...) {
            check(false, "PKCS7 unpad throws wrong exception");
        }
        
        // Пустой блок
        try {
            pkcs_pad.unpad({}, 8);
            check(false, "PKCS7 unpad on empty data should throw");
        } catch(...) {
            check(true, "PKCS7 unpad throws on empty data");
        }

        ansi_x923_padding ansi_pad;
        std::vector<uint8_t> ansi_bad = {'a','b','c', 0,0,0,0, 5};
        try {
            auto res = ansi_pad.unpad(ansi_bad, 8);
            check(res.size() == 3, "ANSI X923 unpad works correctly");
        } catch(...) {
            check(false, "ANSI X923 unpad should not throw");
        }

        iso10126_padding iso_pad;
        std::vector<uint8_t> iso_bad = {'a','b','c', 0,0,0,0, 9};
        try {
            iso_pad.unpad(iso_bad, 8);
            check(false, "ISO10126 unpad with pad > size should throw");
        } catch(const std::runtime_error&) {
            check(true, "ISO10126 unpad throws on pad > size");
        }
    }

    static void test_multithreading()
    {
        des_algorithm des;
        std::vector<uint8_t> key = {1,2,3,4,5,6,7,8};
        des.setup_round_keys(key);
        std::vector<uint8_t> iv(8, 0);

        crypto_context ctx(&des, cipher_mode_type::ECB, padding_mode_type::PKCS7, iv, 8);

        std::vector<uint8_t> plain;
        for (int i = 0; i < 100; i++) plain.push_back(i % 256);

        std::vector<uint8_t> enc1, dec1;
        std::vector<uint8_t> enc4, dec4;

        ctx.encrypt(plain, enc1, 1);
        ctx.decrypt(enc1, dec1, 1);

        ctx.encrypt(plain, enc4, 4);
        ctx.decrypt(enc4, dec4, 4);

        check(vectors_equal(dec1, dec4) && vectors_equal(dec1, plain), "Multithreading ECB with 4 threads");
    }

    static void test_file_operations()
    {
        des_algorithm des;
        std::vector<uint8_t> key = {1,2,3,4,5,6,7,8};
        des.setup_round_keys(key);
        std::vector<uint8_t> iv(8, 0);

        crypto_context ctx(&des, cipher_mode_type::CBC, padding_mode_type::PKCS7, iv, 8);

        std::vector<uint8_t> original = {'F','i','l','e',' ','t','e','s','t','.'};
        {
            std::ofstream f("test_in.bin", std::ios::binary);
            f.write((char*)original.data(), original.size());
        }

        auto fut_enc = ctx.encrypt_file("test_in.bin", "test_enc.bin", 2);
        fut_enc.get();

        auto fut_dec = ctx.decrypt_file("test_enc.bin", "test_out.bin", 2);
        fut_dec.get();

        std::ifstream f_out("test_out.bin", std::ios::binary);
        std::vector<uint8_t> result((std::istreambuf_iterator<char>(f_out)), std::istreambuf_iterator<char>());

        bool ok = vectors_equal(original, result);
        check(ok, "File encryption/decryption");

        std::remove("test_in.bin");
        std::remove("test_enc.bin");
        std::remove("test_out.bin");
    }

    static void test_error_handling()
    {
        des_algorithm des;
        std::vector<uint8_t> key = {1,2,3,4,5,6,7,8};
        des.setup_round_keys(key);
        std::vector<uint8_t> iv_good(8, 0);
        std::vector<uint8_t> iv_bad(7, 0);

        try {
            crypto_context ctx_bad_iv(&des, cipher_mode_type::CBC, padding_mode_type::PKCS7, iv_bad, 8);
            std::vector<uint8_t> data(8, 0);
            std::vector<uint8_t> out;
            ctx_bad_iv.encrypt(data, out, 1);
            check(false, "CBC with wrong IV size should throw on encrypt");
        } catch(const std::invalid_argument&) {
            check(true, "CBC throws on wrong IV size");
        } catch(...) {
            check(false, "CBC throws wrong exception on wrong IV size");
        }

        crypto_context ctx_ecb(&des, cipher_mode_type::ECB, padding_mode_type::PKCS7, iv_good, 8);
        std::vector<uint8_t> data_not_aligned(7, 0);
        std::vector<uint8_t> out;
        try {
            ctx_ecb.encrypt(data_not_aligned, out, 1);
            check(true, "ECB with padding accepts non-aligned data");
        } catch(...) {
            check(false, "ECB with padding should not throw on non-aligned data");
        }

    }

    static void run_all()
    {
        total_tests = 0;
        passed_tests = 0;
        std::cout << "========================================\n";
        std::cout << "      CRYPTO LIBRARY UNIT TESTS        \n";
        std::cout << "========================================\n";

        test_context_creation();
        test_ecb_mode();
        test_cbc_mode();
        test_pcbc_mode();
        test_cfb_mode();
        test_ofb_mode();
        test_ctr_mode();
        test_random_delta_mode();
        test_padding_edge_cases();
        test_multithreading();
        test_file_operations();
        test_error_handling();

        std::cout << "========================================\n";
        std::cout << "Tests passed: " << passed_tests << "/" << total_tests << "\n";
        std::cout << "========================================\n";
    }
};

int crypto_tests::total_tests = 0;
int crypto_tests::passed_tests = 0;

#endif