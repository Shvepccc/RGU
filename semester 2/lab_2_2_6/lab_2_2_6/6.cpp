#include <vector>
#include <iostream>
#include "../libs/vector.h"
#include "../libs/matrix.h"
#include "../libs/hyper_line.h"
#include "../libs/hyper_plane.h"
#include "../libs/plane_2d.h"
#include "../libs/line_2d.h"

#include <cmath>
#include <cassert>
#include <iomanip>

//void test_gauss_solver() {
//    try {
//        // Тест 1: Простая система 2x2
//        std::cout << "\nTest 1: Simple 2x2 system\n";
//        matrix a1(2, 2);
//        matrix b1(2, 1);
//
//        a1[0][0] = 2; a1[0][1] = 1;
//        a1[1][0] = 1; a1[1][1] = -1;
//
//        b1[0][0] = 5;
//        b1[1][0] = 1;
//
//        matrix x1 = solve_gauss(a1, b1);
//        std::cout << "Solution:\n" << x1;
//
//        // Тест 2: Система 3x3
//        std::cout << "\nTest 2: 3x3 system\n";
//        matrix A2(3, 3);
//        matrix b2(3, 1);
//
//        A2[0][0] = 1; A2[0][1] = 2; A2[0][2] = 3;
//        A2[1][0] = 3; A2[1][1] = 5; A2[1][2] = 7;
//        A2[2][0] = 1; A2[2][1] = 3; A2[2][2] = 4;
//
//        b2[0][0] = 3;
//        b2[1][0] = 0;
//        b2[2][0] = 1;
//
//        matrix x2 = solve_gauss(A2, b2);
//        std::cout << "Solution:\n" << x2;
//
//        // Тест 3: Вырожденная система
//        std::cout << "\nTest 3: Singular matrix\n";
//        matrix A3(2, 2);
//        matrix b3(2, 1);
//
//        A3[0][0] = 1; A3[0][1] = 2;
//        A3[1][0] = 2; A3[1][1] = 4;
//
//        b3[0][0] = 3;
//        b3[1][0] = 6;
//
//        try {
//            matrix x3 = solve_gauss(A3, b3);
//        }
//        catch (const std::exception& e) {
//            std::cout << "Caught exception (expected): " << e.what() << "\n";
//        }
//
//        std::cout << "\nRank of matrix: ";
//        matrix m(3, 3);
//        m[0][0] = 1; m[0][1] = 2; m[0][2] = 3;
//        m[1][0] = 4; m[1][1] = 5; m[1][2] = 6;
//        m[2][0] = 7; m[2][1] = 8; m[2][2] = 9;
//
//        //m[0][0] = 1; m[0][1] = 0; m[0][2] = 0;
//        //m[1][0] = 0; m[1][1] = 1; m[1][2] = 0;
//        //m[2][0] = 0; m[2][1] = 0; m[2][2] = 1;
//
//        int r = m.rank();
//        std::cout << r << "\n";
//
//        cvector cvec1 = { 1, 0, 0 };
//        cvector cvec2 = { 0, 1, 0 };
//        cvector cvec3 = { 0, 0, 1 };
//
//        cvector* vectors[] = {&cvec1, &cvec2, &cvec3};
//
//        int dim = size_of_linear_shell(vectors, 3);
//        std::cout << "Size of linear shell: " << dim << "\n";
//
//
//        cvector v1, v2;
//        v1.push_back(1); v1.push_back(0);
//        v2.push_back(0); v2.push_back(1);
//
//        cvector* basis[] = { &v1, &v2 };
//
//        cvector test1, test2;
//        test1.push_back(2); test1.push_back(3);
//        test2.push_back(1); test2.push_back(1); test2.push_back(1);
//
//        bool result1 = is_vector_in_span(test1, basis, 2); // true
//        std::cout << "Size of linear shell result 1: " << result1 << "\n";
//        bool result2 = is_vector_in_span(test2, basis, 2); // false
//        std::cout << "Size of linear shell result 2: " << result2 << "\n";
//    }
//    catch (const std::exception& e) {
//        std::cerr << "Error: " << e.what() << std::endl;
//    }
//}

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

void test_matrix()
{
    std::cout << "\n\n=== Testing Matrix Functionality ===\n";

    // Test constructors
    std::cout << "1. Testing constructors:\n";
    matrix m1(3, 3, 1.0); // Create 3x3 matrix filled with 1.0
    std::cout << "Matrix 3x3 filled with 1.0:\n" << m1;

    matrix m2(2, 4, 2.5); // Create 2x4 matrix filled with 2.5
    std::cout << "Matrix 2x4 filled with 2.5:\n" << m2;

    matrix m3(m1); // Test copy constructor
    std::cout << "Copied matrix (should be same as m1):\n" << m3;

    // Test assignment operator
    std::cout << "2. Testing assignment operator:\n";
    matrix m4 = m2;
    std::cout << "Assigned matrix (should be same as m2):\n" << m4;

    // Test arithmetic operations
    std::cout << "3. Testing arithmetic operations:\n";
    matrix m5(2, 2, 1.0);
    matrix m6(2, 2, 2.0);

    std::cout << "Matrix m5:\n" << m5;
    std::cout << "Matrix m6:\n" << m6;

    matrix m7 = m5 + m6;
    std::cout << "m5 + m6 (should be all 3.0):\n" << m7;

    matrix m8 = m6 - m5;
    std::cout << "m6 - m5 (should be all 1.0):\n" << m8;

    matrix m9(2, 3);
    matrix m10(3, 2);
    // Fill with values for multiplication test
    double val = 1.0;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            m9[i][j] = val;
            m10[j][i] = val;
            val += 1.0;
        }
    }
    std::cout << "Matrix m9 (2x3):\n" << m9;
    std::cout << "Matrix m10 (3x2):\n" << m10;

    matrix m11 = m9 * m10;
    std::cout << "m9 * m10 (should be 2x2 matrix):\n" << m11;

    matrix m12 = m5 * 5.0;
    std::cout << "m5 * 5.0 (should be all 5.0):\n" << m12;

    matrix m13 = 3.0 * m6;
    std::cout << "3.0 * m6 (should be all 6.0):\n" << m13;

    // Test determinant
    std::cout << "4. Testing determinant:\n";
    matrix m14(3, 3);
    // Fill with values for determinant test
    m14[0][0] = 1; m14[0][1] = 2; m14[0][2] = 3;
    m14[1][0] = 4; m14[1][1] = 5; m14[1][2] = 6;
    m14[2][0] = 7; m14[2][1] = 8; m14[2][2] = 9;

    std::cout << "Matrix m14:\n" << m14;
    std::cout << "Determinant of m14 (should be 0): " << m14.det() << "\n";

    matrix m15(2, 2);
    m15[0][0] = 1; m15[0][1] = 2;
    m15[1][0] = 3; m15[1][1] = 4;
    std::cout << "Matrix m15:\n" << m15;
    std::cout << "Determinant of m15 (should be -2): " << m15.det() << "\n";

    // Test transpose
    std::cout << "5. Testing transpose:\n";
    matrix m16(2, 3);
    val = 1.0;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            m16[i][j] = val++;
        }
    }
    std::cout << "Original matrix (2x3):\n" << m16;
    matrix m17 = m16.transp();
    std::cout << "Transposed matrix (3x2):\n" << m17;

    // Test inverse
    std::cout << "6. Testing inverse:\n";
    matrix m18(2, 2);
    m18[0][0] = 4; m18[0][1] = 7;
    m18[1][0] = 2; m18[1][1] = 6;
    std::cout << "Matrix m18:\n" << m18;
    try {
        matrix m19 = m18.reverse();
        std::cout << "Inverse of m18:\n" << m19;
        std::cout << "Check inverse (should be identity matrix):\n" << m18 * m19;
    }
    catch (const std::exception& e) {
        std::cout << "Error calculating inverse: " << e.what() << "\n";
    }

    // Test solving system
    std::cout << "7. Testing Gauss method:\n";
    matrix A(3, 3);
    matrix B(3, 1);

    A[0][0] = 2; A[0][1] = 1; A[0][2] = -1;
    A[1][0] = -3; A[1][1] = -1; A[1][2] = 2;
    A[2][0] = -2; A[2][1] = 1; A[2][2] = 2;

    B[0][0] = 8;
    B[1][0] = -11;
    B[2][0] = -3;

    std::cout << "Matrix A:\n" << A;
    std::cout << "Matrix B:\n" << B;

    try {
        matrix X = solve_gauss(A, B);
        std::cout << "Solution X:\n" << X;
    }
    catch (const std::exception& e) {
        std::cout << "Error solving system: " << e.what() << "\n";
    }

    // Test rank
    std::cout << "8. Testing rank:\n";
    matrix m20(3, 4);
    m20[0][0] = 1; m20[0][1] = 2; m20[0][2] = 3; m20[0][3] = 4;
    m20[1][0] = 5; m20[1][1] = 6; m20[1][2] = 7; m20[1][3] = 8;
    m20[2][0] = 9; m20[2][1] = 10; m20[2][2] = 11; m20[2][3] = 12;

    std::cout << "Matrix m20:\n" << m20;
    std::cout << "Rank of m20 (should be 2): " << m20.rank() << "\n";

    std::cout << "\n=== Matrix Testing Complete ===\n";
}

bool vectors_equal(const cvector& v1, const cvector& v2, double epsilon = 1e-6) {
    if (v1.size() != v2.size()) return false;
    for (size_t i = 0; i < v1.size(); ++i) {
        if (fabs(v1[i] - v2[i]) > epsilon) return false;
    }
    return true;
}

void test_hyper_line() {
    std::cout << "=== Testing hyper_line ===" << std::endl;

    // Тест 1: Проверка принадлежности точек прямой (2D случай)
    {
        cvector a({ 1.0, 1.0 });
        cvector b({ 2.0, 2.0 });
        std::vector<cvector> points_on_line = {
            cvector({3.0, 3.0}),
            cvector({0.5, 0.5}),
            cvector({1.5, 1.5})
        };

        bool result = check_points_belong_to_line(a, b, points_on_line.data(), points_on_line.size());
        std::cout << "Test 1.1 (Points on 2D line): " << (result ? "PASSED" : "FAILED") << std::endl;

        std::vector<cvector> points_not_on_line = {
            cvector({1.0, 2.0}),
            cvector({2.0, 1.0})
        };

        result = check_points_belong_to_line(a, b, points_not_on_line.data(), points_not_on_line.size());
        std::cout << "Test 1.2 (Points not on 2D line): " << (!result ? "PASSED" : "FAILED") << std::endl;
    }

    // Тест 2: Проверка принадлежности точек прямой (3D случай)
    {
        cvector a({ 1.0, 1.0, 1.0 });
        cvector b({ 2.0, 2.0, 2.0 });
        std::vector<cvector> points = {
            cvector({3.0, 3.0, 3.0}),
            cvector({1.5, 1.5, 1.5}),
            cvector({0.0, 0.0, 0.0})
        };

        bool result = check_points_belong_to_line(a, b, points.data(), points.size());
        std::cout << "Test 2 (Points on 3D line): " << (result ? "PASSED" : "FAILED") << std::endl;
    }

    // Тест 3: Проверка обработки исключений
    {
        cvector a({ 1.0, 2.0 });
        cvector b({ 3.0, 4.0, 5.0 }); // Разные размеры

        try {
            check_points_belong_to_line(a, b, nullptr, 1);
            std::cout << "Test 3.1 (Exception handling): FAILED (no exception)" << std::endl;
        }
        catch (const std::invalid_argument& e) {
            std::cout << "Test 3.1 (Exception handling): PASSED" << std::endl;
        }

        std::vector<cvector> points = { cvector({1.0, 2.0}) };
        try {
            check_points_belong_to_line(a, b, points.data(), points.size());
            std::cout << "Test 3.2 (Exception handling): FAILED (no exception)" << std::endl;
        }
        catch (const std::invalid_argument& e) {
            std::cout << "Test 3.2 (Exception handling): PASSED" << std::endl;
        }
    }

    {
        // Тест 1: Простой 2D случай - расстояние от точки (0,1) до оси X (y=0)
        hyper_line line(cvector({ 1.0, 0.0 }), cvector({ 0.0, 0.0 })); // Прямая y = 0 (ось X)
        cvector point({ 0.0, 1.0 }); // Точка (0,1)

        try {
            double dist = distance_from_point_to_line(line, point);
            if (fabs(dist - 1.0) < 1e-6) {
                std::cout << "Test 1 (Basic 2D): PASSED" << std::endl;
            }
            else {
                std::cout << "Test 1 (Basic 2D): FAILED (got " << dist << ", expected 1.0)" << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cout << "Test 1 (Basic 2D): FAILED (exception: " << e.what() << ")" << std::endl;
        }

        // Тест 2: Проверка обработки нулевого вектора направления
        try {
            hyper_line zero_line(cvector({ 0.0, 0.0 }), cvector({ 1.0, 1.0 }));
            distance_from_point_to_line(zero_line, point);
            std::cout << "Test 2 (Zero vector): FAILED (no exception)" << std::endl;
        }
        catch (const std::invalid_argument& e) {
            std::cout << "Test 2 (Zero vector): PASSED" << std::endl;
        }
        catch (...) {
            std::cout << "Test 2 (Zero vector): FAILED (wrong exception)" << std::endl;
        }
    }
}

void test_hyper_plane() {
    std::cout << "\n=== Testing hyper_plane ===" << std::endl;

    // Тест 4: Пересечение гиперплоскости и гиперпрямой
    {
        hyper_plane plane(cvector({ 1.0, 1.0 }), -2.0); // Плоскость x + y = 2
        hyper_line line(cvector({ 1.0, 1.0 }), cvector({ 0.0, 0.0 })); // Прямая y = x

        cvector cross_point = crossing_hiper_plane_and_hiper_line(line, plane);
        cvector expected({ 1.0, 1.0 });

        std::cout << "Test 4 (Plane-line crossing): "
            << (vectors_equal(cross_point, expected) ? "PASSED" : "FAILED") << std::endl;
    }

    // Тест 5: Пересечение гиперплоскости и линии между двумя точками
    {
        hyper_plane plane(cvector({ 0.0, 0.0, 1.0 }), -5.0); // Плоскость z = 5
        cvector a({ 1.0, 2.0, 4.0 });
        cvector b({ 3.0, 4.0, 6.0 }); // Линия пересекает плоскость z=5

        cvector cross_point = crossing_hiper_plane_and_two_points(plane, a, b);
        cvector expected({ 2.0, 3.0, 5.0 });

        std::cout << "Test 5 (Two points-plane crossing): "
            << (vectors_equal(cross_point, expected) ? "PASSED" : "FAILED") << std::endl;

        // Случай без пересечения
        cvector c({ 1.0, 2.0, 6.0 });
        cvector d({ 3.0, 4.0, 7.0 }); // Обе точки выше плоскости z=5

        cross_point = crossing_hiper_plane_and_two_points(plane, c, d);
        std::cout << "Test 5.2 (No crossing): "
            << (cross_point.size() == 0 ? "PASSED" : "FAILED") << std::endl;
    }

    // Тест 6: Расстояние от точки до плоскости
    {
        hyper_plane plane(cvector({ 1.0, 0.0 }), -3.0); // Плоскость x = 3
        cvector point({ 5.0, 2.0 });

        double distance = distance_from_point_to_plane(plane, point);
        std::cout << "Test 6.1 (2D distance): "
            << (fabs(distance - 2.0) < 1e-6 ? "PASSED" : "FAILED") << std::endl;

        hyper_plane plane3d(cvector({ 0.0, 0.0, 1.0 }), -4.0); // Плоскость z = 4
        cvector point3d({ 1.0, 2.0, 6.0 });

        distance = distance_from_point_to_plane(plane3d, point3d);
        std::cout << "Test 6.2 (3D distance): "
            << (fabs(distance - 2.0) < 1e-6 ? "PASSED" : "FAILED") << std::endl;
    }
}

// Helper function to compare doubles with tolerance
bool approx_equal(double a, double b, double epsilon = 1e-6) {
    return fabs(a - b) < epsilon;
}

// Helper function to print test results
void print_test_result(const std::string& test_name, bool passed) {
    std::cout << std::left << std::setw(50) << test_name
        << (passed ? "PASSED" : "FAILED") << std::endl;
}

// Test general form conversions
void test_general_form_conversions() {
    plane_2d converter;

    // Test point-normal to general
    {
        plane_2d_point_normal_form pn(1, 2, 3, 4, 5, 6);
        auto gf = converter.to_general_form(pn);
        bool passed = approx_equal(gf.A, 4) && approx_equal(gf.B, 5) &&
            approx_equal(gf.C, 6) && approx_equal(gf.D, -32);
        print_test_result("Point-normal to general form", passed);
    }

    // Test intercept to general
    {
        plane_2d_intercept_form intercept(4, 5, 10); // x/4 + y/5 + z/10 = 1
        auto gf = converter.to_general_form(intercept);

        // Проверяем что уравнение сохраняет точки пересечения
        bool passed = approx_equal(-gf.D / gf.A, 4) &&  // x-intercept
            approx_equal(-gf.D / gf.B, 5) &&  // y-intercept
            approx_equal(-gf.D / gf.C, 10);   // z-intercept
        print_test_result("Intercept to general (math check)", passed);
    }

    // Test normal to general
    {
        double mag = sqrt(1.0 / 3.0);
        plane_2d_normal_form nf(mag, mag, mag, 5);
        auto gf = converter.to_general_form(nf);
        bool passed = approx_equal(gf.A, mag) && approx_equal(gf.B, mag) &&
            approx_equal(gf.C, mag) && approx_equal(gf.D, -5);
        print_test_result("Normal to general form", passed);
    }

    // Test parametric to general
    {
        cvector r0{ 1, 0, 0 };
        cvector u{ 1, 1, 0 };
        cvector v{ 0, 1, 1 };
        plane_2d_parametric_form pf(r0, u, v);
        auto gf = converter.to_general_form(pf);

        // The normal should be u × v = (1, -1, 1)
        // Plane equation: 1(x-1) -1(y-0) +1(z-0) = 0 => x - y + z = 1
        bool passed = approx_equal(gf.A, 1) && approx_equal(gf.B, -1) &&
            approx_equal(gf.C, 1) && approx_equal(gf.D, -1);
        print_test_result("Parametric to general form", passed);
    }
}

// Test point-normal form conversions
void test_point_normal_conversions() {
    plane_2d converter;

    // Test general to point-normal
    {
        plane_2d_general_form gf(2, 3, 4, 5);
        auto pn = converter.to_point_normal_form(gf);

        bool passed = approx_equal(pn.A, 2) && approx_equal(pn.B, 3) && approx_equal(pn.C, 4);
        passed = passed && approx_equal(2 * pn.x1 + 3 * pn.y1 + 4 * pn.z1 + 5, 0);
        print_test_result("General to point-normal form", passed);
    }

    // Test intercept to point-normal
    {
        plane_2d_intercept_form intercept(2, 3, 4);
        auto pn = converter.to_point_normal_form(intercept);

        bool passed = approx_equal(pn.A, 1.0 / 2) && approx_equal(pn.B, 1.0 / 3) && approx_equal(pn.C, 1.0 / 4);
        passed = passed && approx_equal(pn.x1, 2) && approx_equal(pn.y1, 0) && approx_equal(pn.z1, 0);
        print_test_result("Intercept to point-normal form", passed);
    }

    // Test normal to point-normal
    {
        double mag = sqrt(1.0 / 3.0);
        plane_2d_normal_form nf(mag, mag, mag, 5);
        auto pn = converter.to_point_normal_form(nf);

        bool passed = approx_equal(pn.A, mag) && approx_equal(pn.B, mag) && approx_equal(pn.C, mag);
        passed = passed && approx_equal(pn.x1, 5 * mag) && approx_equal(pn.y1, 5 * mag) && approx_equal(pn.z1, 5 * mag);
        print_test_result("Normal to point-normal form", passed);
    }

    // Test parametric to point-normal
    {
        cvector r0{ 1, 0, 0 };
        cvector u{ 1, 1, 0 };
        cvector v{ 0, 1, 1 };
        plane_2d_parametric_form pf(r0, u, v);
        auto pn = converter.to_point_normal_form(pf);

        bool passed = approx_equal(pn.A, 1) && approx_equal(pn.B, -1) && approx_equal(pn.C, 1);
        passed = passed && approx_equal(pn.x1, 1) && approx_equal(pn.y1, 0) && approx_equal(pn.z1, 0);
        print_test_result("Parametric to point-normal form", passed);
    }
}

// Test intercept form conversions
void test_intercept_conversions() {
    plane_2d converter;

    // Test general to intercept
    {
        plane_2d_general_form gf(1, 2, 3, -6);
        auto intercept = converter.to_intercept_form(gf);

        bool passed = approx_equal(intercept.a, 6) && approx_equal(intercept.b, 3) && approx_equal(intercept.c, 2);
        print_test_result("General to intercept form", passed);
    }

    // Test point-normal to intercept
    {
        plane_2d_point_normal_form pn(1, 1, 1, 1, 2, 3);
        auto intercept = converter.to_intercept_form(pn);

        bool passed = approx_equal(intercept.a, 6) && approx_equal(intercept.b, 3) && approx_equal(intercept.c, 2);
        print_test_result("Point-normal to intercept form", passed);
    }

    // Test normal to intercept (when possible)
    {
        double p = 5;
        double mag = sqrt(1.0 / 3.0);
        plane_2d_normal_form nf(mag, mag, mag, p);
        auto intercept = converter.to_intercept_form(nf);

        double expected = p / mag;
        bool passed = approx_equal(intercept.a, expected) &&
            approx_equal(intercept.b, expected) &&
            approx_equal(intercept.c, expected);
        print_test_result("Normal to intercept form", passed);
    }

    // Test parametric to intercept
    {
        cvector r0{ 1, 0, 0 };
        cvector u{ 1, 1, 0 };
        cvector v{ 0, 1, 1 };
        plane_2d_parametric_form pf(r0, u, v);
        auto intercept = converter.to_intercept_form(pf);

        bool passed = approx_equal(intercept.a, 1) && approx_equal(intercept.b, -1) && approx_equal(intercept.c, 1);
        print_test_result("Parametric to intercept form", passed);
    }
}

// Test normal form conversions
void test_normal_form_conversions() {
    plane_2d converter;

    // Test general to normal
    {
        plane_2d_general_form gf(1, 1, 1, -3);
        auto nf = converter.to_normal_form(gf);

        double mag = sqrt(1.0 / 3.0);
        bool passed = approx_equal(nf.cos_alpha, mag) &&
            approx_equal(nf.cos_beta, mag) &&
            approx_equal(nf.cos_gamma, mag) &&
            approx_equal(nf.p, sqrt(3));
        print_test_result("General to normal form", passed);
    }

    // Test point-normal to normal
    {
        plane_2d_point_normal_form pn(0, 0, 0, 0, 0, 1);
        auto nf = converter.to_normal_form(pn);

        bool passed = approx_equal(nf.cos_alpha, 0) &&
            approx_equal(nf.cos_beta, 0) &&
            approx_equal(nf.cos_gamma, 1) &&
            approx_equal(nf.p, 0);
        print_test_result("Point-normal to normal (simple XY plane)", passed);
    }

    // Test intercept to normal
    {
        plane_2d_intercept_form intercept(1, 1, 1);
        auto nf = converter.to_normal_form(intercept);

        double mag = sqrt(1.0 / 3.0);
        bool passed = approx_equal(nf.cos_alpha, mag) &&
            approx_equal(nf.cos_beta, mag) &&
            approx_equal(nf.cos_gamma, mag) &&
            approx_equal(nf.p, mag);
        print_test_result("Intercept to normal form", passed);
    }

    // Test parametric to normal
    {
        cvector r0{ 1, 0, 0 };
        cvector u{ 1, 1, 0 };
        cvector v{ 0, 1, 1 };
        plane_2d_parametric_form pf(r0, u, v);
        auto nf = converter.to_normal_form(pf);

        double mag = sqrt(1.0 / 3.0);
        bool passed = approx_equal(nf.cos_alpha, mag) &&
            approx_equal(nf.cos_beta, -mag) &&
            approx_equal(nf.cos_gamma, mag) &&
            approx_equal(nf.p, mag);
        print_test_result("Parametric to normal form", passed);
    }
}

// Test parametric form conversions
void test_parametric_conversions() {
    plane_2d converter;

    // Test general to parametric
    {
        plane_2d_general_form gf(0, 0, 1, -2); // z = 2
        auto pf = converter.to_parametric_form(gf);

        bool passed = approx_equal(pf.r0[2], 2.0) &&
            approx_equal(scalar_product(pf.t, { 0,0,1 }), 0) &&
            approx_equal(scalar_product(pf.s, { 0,0,1 }), 0) &&
            !vectors_equal(pf.t, pf.s);
        print_test_result("General to parametric (simple case)", passed);
    }

    // Test point-normal to parametric
    {
        plane_2d_point_normal_form pn(1, 0, 0, 1, -1, 1);
        auto pf = converter.to_parametric_form(pn);

        cvector normal{ 1, -1, 1 };
        cvector t = pf.t;
        cvector s = pf.s;

        bool passed = approx_equal(pf.r0[0], 1) && approx_equal(pf.r0[1], 0) && approx_equal(pf.r0[2], 0);
        passed = passed && approx_equal(scalar_product(t, normal), 0);
        passed = passed && approx_equal(scalar_product(s, normal), 0);
        passed = passed && !vectors_equal(t, s);
        print_test_result("Point-normal to parametric form", passed);
    }

    // Test intercept to parametric
    {
        plane_2d_intercept_form intercept(1, 1, 1); // x + y + z = 1
        auto pf = converter.to_parametric_form(intercept);

        cvector normal{ 1, 1, 1 };
        cvector t = pf.t;
        cvector s = pf.s;

        bool passed = approx_equal(pf.r0[0], 1) && approx_equal(pf.r0[1], 0) && approx_equal(pf.r0[2], 0);
        passed = passed && approx_equal(scalar_product(t, normal), 0);
        passed = passed && approx_equal(scalar_product(s, normal), 0);
        passed = passed && !vectors_equal(t, s);
        print_test_result("Intercept to parametric form", passed);
    }

    // Test normal to parametric
    {
        plane_2d_normal_form nf(0, 0, 1, 2);
        auto pf = converter.to_parametric_form(nf);

        bool passed = approx_equal(pf.r0[2], 2.0) &&
            approx_equal(pf.r0[0], 0.0) &&
            approx_equal(pf.r0[1], 0.0) &&
            approx_equal(pf.t[2], 0.0) &&
            approx_equal(pf.s[2], 0.0) &&
            !vectors_equal(pf.t, pf.s);

        print_test_result("Normal to parametric (simple case)", passed);
    }
}


int program_6_main(int argc, char* argv[])
{
    test_cvector();

    test_matrix();

    test_hyper_line();
    test_hyper_plane();

    std::cout << "\n\nTesting Plane Conversion Functions\n";
    std::cout << "=================================\n";

    test_general_form_conversions();
    test_point_normal_conversions();
    test_intercept_conversions();
    test_normal_form_conversions();
    test_parametric_conversions();

    {
        line_2d line1(line_general_form(2, -1, 3)); 
        line_2d line2(line_general_form(4, -2, 5)); 

        try {
            double dist = distance_between_parallel_lines(line1, line2);
            std::cout << "Distance between parallel lines: " << dist << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    {
        line_2d line(line_general_form(2, 3, -6));
        double area = line.area_of_axes_triangle();
        std::cout << "Area of triangle: " << area << std::endl;

        line_2d horizontal(line_general_form(0, 1, -5));
        std::cout << "Area for horizontal line: "
            << horizontal.area_of_axes_triangle() << std::endl;
    }

    {
        // x - y = 0 (y = x)
        line_2d line1(line_general_form(1, -1, 0));

        // x + y = 0 (y = -x)
        line_2d line2(line_general_form(1, 1, 0));

        double angle_rad = line_2d::angle_between_lines(line1, line2);
        double angle_deg = line_2d::angle_between_lines_deg(line1, line2);

        std::cout << "Angle between lines (radians): " << angle_rad << std::endl; // 1.5708
        std::cout << "Angle between lines (degrees): " << angle_deg << std::endl; // 90
    }

    {
        line_2d line(line_general_form(2, -1, 1));

        segment segment(0, 0, 1, 1);

        double dist = distance_from_line_to_segment(line, segment);
        std::cout << "Distance from line to segment: " << dist << std::endl; // (0.447214)
    }

    {
        segment seg1(0, 0, 3, 3);
        segment seg2(0, 3, 3, 0);

        auto intersection = find_segments_intersection(seg1, seg2);

        if (!intersection.empty())
        {
            std::cout << "Intersection point: (" << intersection[0] << ", " << intersection[1] << ")\n";
        }
        else {
            std::cout << "Segments do not intersect\n";
        }
    }

    return 0;
}