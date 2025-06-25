#ifndef TESTING_H_
#define TESTING_H_

#include "../libs/vector.h"
#include <iostream>
#include <cmath>

void test_cvector()
{
    // Basic operations
    cvector v;
    for (int i = 1; i <= 3; i++) v.push_back(i);
    std::cout << "Initial: " << v << " (size: " << v.size() << ")\n";
    v[1] = 5;
    std::cout << "Modified: " << v << "\n";
    v.pop_back();
    std::cout << "After pop: " << v << "\n";

    // Vector math
    cvector a, b;
    for (int i = 1; i <= 3; i++) { a.push_back(i); b.push_back(i * 2); }
    std::cout << "\nVector ops:\na: " << a << "\nb: " << b
        << "\na+b: " << a + b << "\na*b: " << scalar_product(a, b)
        << "\na*2: " << a * 2 << "\n";

    // Cross products
    cvector x, y;
    x.push_back(1); x.push_back(0); x.push_back(0);
    y.push_back(0); y.push_back(1); y.push_back(0);
    std::cout << "\n3D cross: " << cross_product_3d(x, y) << "\n";

    // Gram-Schmidt
    cvector* basis[2] = { new cvector, new cvector };
    (*basis[0]).push_back(1); (*basis[0]).push_back(1);
    (*basis[1]).push_back(1); (*basis[1]).push_back(0);
    cvector* ortho = gram_schmidt(basis, 2);
    std::cout << "\nOrthogonalized:\n" << ortho[0] << "\n" << ortho[1] << "\n";

    // Cleanup
    delete basis[0]; delete basis[1]; delete[] ortho;
}

#endif