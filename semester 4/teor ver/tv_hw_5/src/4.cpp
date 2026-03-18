#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <limits>
#include <numeric>

std::string generate_text(size_t length, const std::vector<double>& freqs)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> dist(freqs.begin(), freqs.end());
    std::string result;
    for (size_t i = 0; i < length; ++i)
    {
        result += static_cast<char>(dist(gen) + 'A');
    }
    return result;
}

std::vector<int> generate_key_vigenere(size_t length)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 25);
    std::vector<int> key;
    for (size_t i = 0; i < length; ++i)
    {
        key.push_back(dist(gen));
    }
    return key;
}

std::vector<int> generate_key_vernam(size_t length)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 255);
    std::vector<int> key;
    for (size_t i = 0; i < length; ++i)
    {
        key.push_back(dist(gen));
    }
    return key;
}

std::string vigenere_encrypt(const std::string& text, const std::vector<int>& key)
{
    std::string result;
    size_t key_len = key.size();
    for (size_t i = 0; i < text.size(); ++i)
    {
        int p = text[i] - 'A';
        int k = key[i % key_len];
        int c = (p + k) % 26;
        result += static_cast<char>(c + 'A');
    }
    return result;
}

std::string vigenere_decrypt(const std::string& cipher, const std::vector<int>& key)
{
    std::string result;
    size_t key_len = key.size();
    for (size_t i = 0; i < cipher.size(); ++i)
    {
        int c = cipher[i] - 'A';
        int k = key[i % key_len];
        int p = (c - k + 26) % 26;
        result += static_cast<char>(p + 'A');
    }
    return result;
}

std::vector<int> text_to_bytes(const std::string& text)
{
    std::vector<int> bytes;
    for (char ch : text)
    {
        bytes.push_back(static_cast<int>(ch));
    }
    return bytes;
}

std::string bytes_to_text(const std::vector<int>& bytes)
{
    std::string text;
    for (int b : bytes)
    {
        text += static_cast<char>(b);
    }
    return text;
}

std::vector<int> vernam_encrypt(const std::vector<int>& text_bytes, const std::vector<int>& key)
{
    std::vector<int> result;
    size_t key_len = key.size();
    for (size_t i = 0; i < text_bytes.size(); ++i)
    {
        result.push_back(text_bytes[i] ^ key[i % key_len]);
    }
    return result;
}

std::vector<int> vernam_decrypt(const std::vector<int>& cipher_bytes, const std::vector<int>& key)
{
    std::vector<int> result;
    size_t key_len = key.size();
    for (size_t i = 0; i < cipher_bytes.size(); ++i)
    {
        result.push_back(cipher_bytes[i] ^ key[i % key_len]);
    }
    return result;
}

double index_of_coincidence(const std::vector<int>& data, int alphabet_size)
{
    std::vector<int> counts(alphabet_size, 0);
    for (int x : data)
    {
        counts[x]++;
    }
    double n = static_cast<double>(data.size());
    if (n < 2.0) return 0.0;
    double ic = 0.0;
    for (int c : counts)
    {
        ic += c * (c - 1);
    }
    ic /= (n * (n - 1));
    return ic;
}

int find_key_length(const std::vector<int>& cipher, int max_len, int alphabet_size)
{
    double best_ic = -1.0;
    int best_len = 1;
    for (int len = 1; len <= max_len; ++len)
    {
        std::vector<std::vector<int>> groups(len);
        for (size_t i = 0; i < cipher.size(); ++i)
        {
            groups[i % len].push_back(cipher[i]);
        }
        double avg_ic = 0.0;
        int valid_groups = 0;
        for (const auto& g : groups)
        {
            if (g.size() < 2) continue;
            avg_ic += index_of_coincidence(g, alphabet_size);
            ++valid_groups;
        }
        if (valid_groups > 0)
        {
            avg_ic /= valid_groups;
        }
        if (avg_ic > best_ic)
        {
            best_ic = avg_ic;
            best_len = len;
        }
    }
    return best_len;
}

double chi_square(const std::vector<int>& decrypted_group, const std::vector<double>& expected_freqs, int alphabet_size)
{
    std::vector<int> counts(alphabet_size, 0);
    for (int x : decrypted_group)
    {
        counts[x]++;
    }
    double chi2 = 0.0;
    double n = static_cast<double>(decrypted_group.size());
    for (int i = 0; i < alphabet_size; ++i)
    {
        double expected = n * expected_freqs[i];
        double observed = static_cast<double>(counts[i]);
        if (expected > 0.0)
        {
            chi2 += (observed - expected) * (observed - expected) / expected;
        }
    }
    return chi2;
}

int find_key_byte_vigenere(const std::vector<int>& group, const std::vector<double>& freqs)
{
    int best_k = 0;
    double best_score = std::numeric_limits<double>::max();
    for (int k = 0; k < 26; ++k)
    {
        std::vector<int> decrypted;
        for (int c : group)
        {
            int p = (c - k + 26) % 26;
            decrypted.push_back(p);
        }
        double score = chi_square(decrypted, freqs, 26);
        if (score < best_score)
        {
            best_score = score;
            best_k = k;
        }
    }
    return best_k;
}

std::vector<int> find_key_vigenere(const std::vector<int>& cipher, int key_len, const std::vector<double>& freqs)
{
    std::vector<std::vector<int>> groups(key_len);
    for (size_t i = 0; i < cipher.size(); ++i)
    {
        groups[i % key_len].push_back(cipher[i]);
    }
    std::vector<int> key;
    for (int pos = 0; pos < key_len; ++pos)
    {
        int k = find_key_byte_vigenere(groups[pos], freqs);
        key.push_back(k);
    }
    return key;
}

int find_key_byte_vernam(const std::vector<int>& group, const std::vector<double>& freqs)
{
    int best_k = -1;
    double best_score = std::numeric_limits<double>::max();
    for (int k = 0; k < 256; ++k)
    {
        std::vector<int> decrypted_indices;
        bool valid = true;
        for (int c : group)
        {
            int p = c ^ k;
            if (p < 'A' || p > 'Z')
            {
                valid = false;
                break;
            }
            decrypted_indices.push_back(p - 'A');
        }
        if (!valid) continue;
        double score = chi_square(decrypted_indices, freqs, 26);
        if (score < best_score)
        {
            best_score = score;
            best_k = k;
        }
    }
    return best_k;
}

std::vector<int> find_key_vernam(const std::vector<int>& cipher, int key_len, const std::vector<double>& freqs)
{
    std::vector<std::vector<int>> groups(key_len);
    for (size_t i = 0; i < cipher.size(); ++i)
    {
        groups[i % key_len].push_back(cipher[i]);
    }
    std::vector<int> key;
    for (int pos = 0; pos < key_len; ++pos)
    {
        int k = find_key_byte_vernam(groups[pos], freqs);
        if (k == -1)
        {
            k = 0;
        }
        key.push_back(k);
    }
    return key;
}

void print_hex(const std::vector<int>& bytes)
{
    for (size_t i = 0; i < bytes.size(); ++i)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << bytes[i];
        if ((i + 1) % 32 == 0) std::cout << "\n";
        else std::cout << " ";
    }
    std::cout << std::dec << "\n";
}

int main()
{
    std::vector<double> english_freqs = {
        0.08167, 0.01492, 0.02782, 0.04253, 0.12702,
        0.02228, 0.02015, 0.06094, 0.06966, 0.00153,
        0.00772, 0.04025, 0.02406, 0.06749, 0.07507,
        0.01929, 0.00095, 0.05987, 0.06327, 0.09056,
        0.02758, 0.00978, 0.02360, 0.00150, 0.01974,
        0.00074
    };

    const size_t text_length = 500;
    const int max_key_len_search = 20;
    const size_t vigenere_key_len = 5;
    const size_t vernam_key_len = 5;

    std::string plaintext = generate_text(text_length, english_freqs);
    std::cout << "Plaintext (first 100 chars): " << plaintext.substr(0, 100) << "...\n\n";

    std::vector<int> vigenere_key = generate_key_vigenere(vigenere_key_len);
    std::cout << "Vigenere key (numbers): ";
    for (int k : vigenere_key) std::cout << k << " ";
    std::cout << "\n";

    std::string vigenere_cipher = vigenere_encrypt(plaintext, vigenere_key);
    std::cout << "Vigenere ciphertext (first 100 chars): " << vigenere_cipher.substr(0, 100) << "...\n";

    std::vector<int> vigenere_cipher_ints;
    for (char c : vigenere_cipher) vigenere_cipher_ints.push_back(c - 'A');

    int found_vigenere_key_len = find_key_length(vigenere_cipher_ints, max_key_len_search, 26);
    std::cout << "Found Vigenere key length: " << found_vigenere_key_len << "\n";

    std::vector<int> found_vigenere_key = find_key_vigenere(vigenere_cipher_ints, found_vigenere_key_len, english_freqs);
    std::cout << "Recovered Vigenere key: ";
    for (int k : found_vigenere_key) std::cout << k << " ";
    std::cout << "\n";

    std::string decrypted_vigenere = vigenere_decrypt(vigenere_cipher, found_vigenere_key);
    std::cout << "Decrypted text (first 100 chars): " << decrypted_vigenere.substr(0, 100) << "...\n";
    std::cout << "Decryption successful? " << (plaintext == decrypted_vigenere ? "Yes" : "No") << "\n\n";

    std::vector<int> vernam_key = generate_key_vernam(vernam_key_len);
    std::cout << "Vernam key (hex): ";
    print_hex(vernam_key);

    std::vector<int> plain_bytes = text_to_bytes(plaintext);
    std::vector<int> vernam_cipher = vernam_encrypt(plain_bytes, vernam_key);
    std::cout << "Vernam ciphertext (hex, first 100 bytes):\n";
    std::vector<int> cipher_preview(vernam_cipher.begin(), vernam_cipher.begin() + 100);
    print_hex(cipher_preview);

    int found_vernam_key_len = find_key_length(vernam_cipher, max_key_len_search, 256);
    std::cout << "Found Vernam key length: " << found_vernam_key_len << "\n";

    std::vector<int> found_vernam_key = find_key_vernam(vernam_cipher, found_vernam_key_len, english_freqs);
    std::cout << "Recovered Vernam key (hex): ";
    print_hex(found_vernam_key);

    std::vector<int> decrypted_vernam_bytes = vernam_decrypt(vernam_cipher, found_vernam_key);
    std::string decrypted_vernam = bytes_to_text(decrypted_vernam_bytes);
    std::cout << "Decrypted text (first 100 chars): " << decrypted_vernam.substr(0, 100) << "...\n";
    std::cout << "Decryption successful? " << (plaintext == decrypted_vernam ? "Yes" : "No") << "\n";

    return 0;
}