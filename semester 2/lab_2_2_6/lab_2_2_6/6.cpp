#include <iostream>
#include "../libs/vector.h"
#include "../libs/matrix.h"

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
    for (int i = 1; i <= 3; i++)
    { 
        a.push_back(i); 
        b.push_back(i * 2);
    }
    std::cout << "\nVector ops:\na: " << a << "\nb: " << b;
    
    std::cout << "\na+b: " << (a + b);
        
    std::cout << "\na*b: " << scalar_product(a, b);

    std::cout << "\na*2: " << (a * 2) << "\n";

    // Cross products
    cvector x, y;
    x.push_back(1); x.push_back(0); x.push_back(0);
    y.push_back(0); y.push_back(1); y.push_back(0);
    std::cout << "\n3D cross: " << cross_product_3d(x, y) << "\n";

    // Gram-Schmidt
    cvector* basis[2] = { new cvector{}, new cvector{} };
    (*basis[0]).push_back(1);
    (*basis[0]).push_back(1);
    (*basis[1]).push_back(1);
    (*basis[1]).push_back(0);

    cvector* ortho = gram_schmidt(basis, 2);
    std::cout << "\nOrthogonalized:\n" << ortho[0] << "\n" << ortho[1] << "\n";
}

void test_gauss_solver() {
    try {
        // Тест 1: Простая система 2x2
        std::cout << "\nTest 1: Simple 2x2 system\n";
        matrix a1(2, 2);
        matrix b1(2, 1);

        a1[0][0] = 2; a1[0][1] = 1;
        a1[1][0] = 1; a1[1][1] = -1;

        b1[0][0] = 5;
        b1[1][0] = 1;

        matrix x1 = solve_gauss(a1, b1);
        std::cout << "Solution:\n" << x1;

        // Тест 2: Система 3x3
        std::cout << "\nTest 2: 3x3 system\n";
        matrix A2(3, 3);
        matrix b2(3, 1);

        A2[0][0] = 1; A2[0][1] = 2; A2[0][2] = 3;
        A2[1][0] = 3; A2[1][1] = 5; A2[1][2] = 7;
        A2[2][0] = 1; A2[2][1] = 3; A2[2][2] = 4;

        b2[0][0] = 3;
        b2[1][0] = 0;
        b2[2][0] = 1;

        matrix x2 = solve_gauss(A2, b2);
        std::cout << "Solution:\n" << x2;

        // Тест 3: Вырожденная система
        std::cout << "\nTest 3: Singular matrix\n";
        matrix A3(2, 2);
        matrix b3(2, 1);

        A3[0][0] = 1; A3[0][1] = 2;
        A3[1][0] = 2; A3[1][1] = 4;

        b3[0][0] = 3;
        b3[1][0] = 6;

        try {
            matrix x3 = solve_gauss(A3, b3);
        }
        catch (const std::exception& e) {
            std::cout << "Caught exception (expected): " << e.what() << "\n";
        }

        std::cout << "\nRank of matrix: ";
        matrix m(3, 3);
        m[0][0] = 1; m[0][1] = 2; m[0][2] = 3;
        m[1][0] = 4; m[1][1] = 5; m[1][2] = 6;
        m[2][0] = 7; m[2][1] = 8; m[2][2] = 9;

        //m[0][0] = 1; m[0][1] = 0; m[0][2] = 0;
        //m[1][0] = 0; m[1][1] = 1; m[1][2] = 0;
        //m[2][0] = 0; m[2][1] = 0; m[2][2] = 1;

        int r = m.rank();
        std::cout << r << "\n";

        cvector cvec1 = { 1, 0, 0 };
        cvector cvec2 = { 0, 1, 0 };
        cvector cvec3 = { 0, 0, 1 };

        cvector* vectors[] = {&cvec1, &cvec2, &cvec3};

        int dim = size_of_linear_shell(vectors, 3);
        std::cout << "Size of linear shell: " << dim << "\n";


        cvector v1, v2;
        v1.push_back(1); v1.push_back(0);
        v2.push_back(0); v2.push_back(1);

        cvector* basis[] = { &v1, &v2 };

        cvector test1, test2;
        test1.push_back(2); test1.push_back(3);
        test2.push_back(1); test2.push_back(1); test2.push_back(1);

        bool result1 = is_vector_in_span(test1, basis, 2); // true
        std::cout << "Size of linear shell result 1: " << result1 << "\n";
        bool result2 = is_vector_in_span(test2, basis, 2); // false
        std::cout << "Size of linear shell result 2: " << result2 << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int program_6_main(int argc, char* argv[])
{
    test_cvector();

    test_gauss_solver();

    return 0;
}