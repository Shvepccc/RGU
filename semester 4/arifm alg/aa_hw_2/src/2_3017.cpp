#include <vector>
#include <complex>
#include <cmath>
#include <iostream>
#include <algorithm>
#include "../include/fft.hpp"

std::vector<int> cartesian_sum(const std::vector<int>& A, const std::vector<int>& B)
{
    int max_val = 10 * std::max(A.size(), B.size());
    int size = 1;
    while (size <= 2 * max_val) size *= 2;
    
    std::vector<std::complex<double>> fa(size, 0.0);
    std::vector<std::complex<double>> fb(size, 0.0);
    
    for (int x : A) fa[x] = fa[x] + 1.0;
    for (int y : B) fb[y] = fb[y] + 1.0;
    
    double angle = 2.0 * M_PI / size;
    std::complex<double> root(cos(angle), sin(angle));
    
    fft(fa, root, false);
    fft(fb, root, false);
    
    for (int i = 0; i < size; i++) fa[i] = fa[i] * fb[i];
    
    fft(fa, std::complex<double>(cos(angle), -sin(angle)), true);
    
    std::vector<int> C(2 * max_val + 1, 0);
    for (int i = 0; i <= 2 * max_val; i++)
    {
        C[i] = static_cast<int>(round(fa[i].real()));
    }
    
    return C;
}

int main()
{
     std::vector<int> A = 
        {2, 4, 6, 8, 10, 12, 14, 16, 18, 20,
         22, 24, 26, 28, 30, 32, 34, 36, 38, 40,
          42, 44, 46, 48, 50, 52, 54, 56, 58, 60,
           62, 64, 66, 68, 70, 72, 74, 76, 78, 80,
            82, 84, 86, 88, 90, 92, 94, 96, 98, 100};

    std::vector<int>B = 
        {1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21,
         23, 25, 27, 29, 31, 33, 35, 37, 39,
          41, 43, 45, 47, 49, 51, 53, 55, 57, 59,
           61, 63, 65, 67, 69, 71, 73, 75, 77, 79,
            81, 83, 85, 87, 89, 91, 93, 95, 97, 99};
    
    std::vector<int> C = cartesian_sum(A, B);
    
    for (int i = 0; i < C.size(); i++)
    {
        if (C[i] > 0)
        {
            std::cout << "Сумма " << i << ": " << C[i] << " раз" << std::endl;
        }
    }
    
    return 0;
}