#include <vector>
#include <complex>
#include <cmath>
#include <iostream>
#include <iomanip>
#include "../include/fft.hpp"

void three_dimensional_fft
(
    std::vector<std::vector<std::vector<std::complex<double>>>>& data,
    bool inverse
)
{
    int n1 = data.size();
    int n2 = data[0].size();
    int n3 = data[0][0].size();

    for (int i1 = 0; i1 < n1; ++i1)
    {
        for (int i2 = 0; i2 < n2; ++i2)
        {
            std::vector<std::complex<double>> row(n3);
            for (int i3 = 0; i3 < n3; ++i3)
            {
                row[i3] = data[i1][i2][i3];
            }

            double angle = 2.0 * M_PI / n3;
            std::complex<double> root(cos(angle), sin(angle));
            if (inverse) root = std::complex<double>(cos(angle), -sin(angle));

            fft(row, root, inverse);

            for (int i3 = 0; i3 < n3; ++i3)
            {
                data[i1][i2][i3] = row[i3];
            }
        }
    }

    for (int i1 = 0; i1 < n1; ++i1)
    {
        for (int i3 = 0; i3 < n3; ++i3)
        {
            std::vector<std::complex<double>> row(n2);
            for (int i2 = 0; i2 < n2; ++i2)
            {
                row[i2] = data[i1][i2][i3];
            }

            double angle = 2.0 * M_PI / n2;
            std::complex<double> root(cos(angle), sin(angle));
            if (inverse) root = std::complex<double>(cos(angle), -sin(angle));

            fft(row, root, inverse);

            for (int i2 = 0; i2 < n2; ++i2)
            {
                data[i1][i2][i3] = row[i2];
            }
        }
    }

    for (int i2 = 0; i2 < n2; ++i2)
    {
        for (int i3 = 0; i3 < n3; ++i3)
        {
            std::vector<std::complex<double>> row(n1);
            for (int i1 = 0; i1 < n1; ++i1)
            {
                row[i1] = data[i1][i2][i3];
            }

            double angle = 2.0 * M_PI / n1;
            std::complex<double> root(cos(angle), sin(angle));
            if (inverse) root = std::complex<double>(cos(angle), -sin(angle));

            fft(row, root, inverse);

            for (int i1 = 0; i1 < n1; ++i1)
            {
                data[i1][i2][i3] = row[i1];
            }
        }
    }
}

int main()
{
    const int n1 = 4, n2 = 4, n3 = 4;

    std::vector<std::vector<std::vector<std::complex<double>>>> data(
        n1, std::vector<std::vector<std::complex<double>>>(
            n2, std::vector<std::complex<double>>(n3)));

    for (int i1 = 0; i1 < n1; ++i1)
    {
        for (int i2 = 0; i2 < n2; ++i2)
        {
            for (int i3 = 0; i3 < n3; ++i3)
            {
                data[i1][i2][i3] = std::complex<double>(i1 * 100 + i2 * 10 + i3, 0);
            }
        }
    }

    std::cout << "Original 3D array (" << n1 << " x " << n2 << " x " << n3 << "):\n";
    for (int i1 = 0; i1 < n1; ++i1)
    {
        std::cout << "i1 = " << i1 << ":\n";
        for (int i2 = 0; i2 < n2; ++i2)
        {
            for (int i3 = 0; i3 < n3; ++i3)
            {
                std::cout << std::setw(6) << data[i1][i2][i3].real() << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    auto original = data;

    three_dimensional_fft(data, false);

    std::cout << "After forward FFT (real parts, rounded):\n";
    for (int i1 = 0; i1 < n1; ++i1)
    {
        std::cout << "i1 = " << i1 << ":\n";
        for (int i2 = 0; i2 < n2; ++i2)
        {
            for (int i3 = 0; i3 < n3; ++i3)
            {
                std::cout << std::setw(10) << std::round(data[i1][i2][i3].real() * 100) / 100 << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    three_dimensional_fft(data, true);

    std::cout << "After inverse FFT (should match original):\n";
    double max_error = 0.0;
    for (int i1 = 0; i1 < n1; ++i1)
    {
        std::cout << "i1 = " << i1 << ":\n";
        for (int i2 = 0; i2 < n2; ++i2)
        {
            for (int i3 = 0; i3 < n3; ++i3)
            {
                double val = std::round(data[i1][i2][i3].real() * 100) / 100;
                double orig = original[i1][i2][i3].real();
                double error = std::abs(val - orig);
                if (error > max_error) max_error = error;
                std::cout << std::setw(6) << val << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    std::cout << "Maximum absolute error: " << max_error << "\n";
    if (max_error < 0.01)
    {
        std::cout << "SUCCESS: Inverse transform correctly recovered original data.\n";
    }
    else
    {
        std::cout << "FAIL: Reconstruction error exceeds tolerance.\n";
    }

    return 0;
}