#ifndef RC4_H
#define RC4_H

#include <vector>
#include <cstdint>
#include <stdexcept>

class RC4
{
private:
    uint8_t S[256];
    int i;
    int j;

public:
    RC4(const std::vector<uint8_t>& key)
    {
        if (key.empty())
        {
            throw std::runtime_error("Key cannot be empty");
        }

        for (int k = 0; k < 256; k++)
        {
            S[k] = k;
        }

        j = 0;
        for (int k = 0; k < 256; k++)
        {
            j = (j + S[k] + key[k % key.size()]) % 256;
            std::swap(S[k], S[j]);
        }

        i = 0;
        j = 0;
    }

    void process(std::vector<uint8_t>& data)
    {
        for (size_t k = 0; k < data.size(); k++)
        {
            i = (i + 1) % 256;
            j = (j + S[i]) % 256;
            std::swap(S[i], S[j]);

            uint8_t rnd = S[(S[i] + S[j]) % 256];
            data[k] ^= rnd;
        }
    }
};

#endif