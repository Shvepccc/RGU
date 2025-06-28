#ifndef SIMPLE_PLANE_2D_H_
#define SIMPLE_PLANE_2D_H_

#include <iostream>
#include "vector.h"

// Ax + By + Cz + D = 0
struct plane_2d_general_form {
    double A, B, C, D;
    plane_2d_general_form(double A_, double B_, double C_, double D_) : A(A_), B(B_), C(C_), D(D_) {}
};

// A(x-x1) + B(y-y1) + C(z-z1) = 0
struct plane_2d_point_normal_form {
    double x1, y1, z1;
    double A, B, C;
    plane_2d_point_normal_form(double x1_, double y1_, double z1_, double A_, double B_, double C_) 
        : x1(x1_), y1(y1_), z1(z1_), A(A_), B(B_), C(C_) {}
};

// x/a + y/b + z/c = 1
struct plane_2d_intercept_form {
    double a, b, c;
    plane_2d_intercept_form(double a_, double b_, double c_) : a(a_), b(b_), c(c_) {}
};

// xcosα + ycosβ + zcosγ = p
struct plane_2d_normal_form {
    double cos_alpha, cos_beta, cos_gamma, p;
    plane_2d_normal_form(double ca, double cb, double cg, double p_) : cos_alpha(ca), cos_beta(cb), cos_gamma(cg), p(p_) {}
};

// r = r0 + t*u + s*v
struct plane_2d_parametric_form
{
    cvector r0;
    cvector t, s;

    plane_2d_parametric_form(const cvector& r0_, const cvector& t_, const cvector& s_)
        : r0(r0_), t(t_), s(s_) {}
};



class plane_2d
{
public:

    #pragma region TO general_form
    plane_2d_general_form to_general_form(const plane_2d_point_normal_form& arg);
    plane_2d_general_form to_general_form(const plane_2d_intercept_form& arg);
    plane_2d_general_form to_general_form(const plane_2d_normal_form& arg);
    plane_2d_general_form to_general_form(const plane_2d_parametric_form& arg);
    #pragma endregion

    #pragma region TO point_normal_form
    plane_2d_point_normal_form to_point_normal_form(const plane_2d_general_form& arg);
    plane_2d_point_normal_form to_point_normal_form(const plane_2d_intercept_form& arg);
    plane_2d_point_normal_form to_point_normal_form(const plane_2d_normal_form& arg);
    plane_2d_point_normal_form to_point_normal_form(const plane_2d_parametric_form& arg);
    #pragma endregion

    #pragma region TO intercept_form
    plane_2d_intercept_form to_intercept_form(const plane_2d_general_form& arg);
    plane_2d_intercept_form to_intercept_form(const plane_2d_point_normal_form& arg);
    plane_2d_intercept_form to_intercept_form(const plane_2d_normal_form& arg);
    plane_2d_intercept_form to_intercept_form(const plane_2d_parametric_form& arg);
    #pragma endregion

    #pragma region TO normal_form
    plane_2d_normal_form to_normal_form(const plane_2d_general_form& arg);
    plane_2d_normal_form to_normal_form(const plane_2d_point_normal_form& arg);
    plane_2d_normal_form to_normal_form(const plane_2d_intercept_form& arg);
    plane_2d_normal_form to_normal_form(const plane_2d_parametric_form& arg);
    #pragma endregion

    #pragma region TO parametric_form
    plane_2d_parametric_form to_parametric_form(const plane_2d_general_form& arg);
    plane_2d_parametric_form to_parametric_form(const plane_2d_point_normal_form& arg);
    plane_2d_parametric_form to_parametric_form(const plane_2d_intercept_form& arg);
    plane_2d_parametric_form to_parametric_form(const plane_2d_normal_form& arg);
    #pragma endregion

};

#pragma region TO general_form (realisation)
plane_2d_general_form plane_2d::to_general_form(const plane_2d_point_normal_form& arg)
{
    if(arg.A == 0 && arg.B == 0 && arg.C == 0)
    {
        throw std::invalid_argument("Normal vector cannot be zero");
    }

    double D = -arg.A * arg.x1 - arg.B * arg.y1 - arg.C * arg.z1;
    return plane_2d_general_form(arg.A, arg.B, arg.C, D);
}

plane_2d_general_form plane_2d::to_general_form(const plane_2d_intercept_form& arg)
{
    if(arg.a == 0 || arg.b == 0 || arg.c == 0)
    {
        throw std::invalid_argument("Intercept parameters cannot be zero");
    }
    
    double A = 1.0 / arg.a;
    double B = 1.0 / arg.b;
    double C = 1.0 / arg.c;
    double D = -1.0;
    
    return plane_2d_general_form(A, B, C, D);
}

plane_2d_general_form plane_2d::to_general_form(const plane_2d_normal_form& arg)
{
    if(arg.p < 0)
    {
        throw std::invalid_argument("Distance p cannot be negative");
    }

    double norm = sqrt(arg.cos_alpha*arg.cos_alpha + 
                      arg.cos_beta*arg.cos_beta + 
                      arg.cos_gamma*arg.cos_gamma);
    
    if(norm == 0)
    {
        throw std::invalid_argument("Normal vector cannot be zero");
    }

    double A = arg.cos_alpha / norm;
    double B = arg.cos_beta / norm;
    double C = arg.cos_gamma / norm;
    double D = -arg.p;
    
    return plane_2d_general_form(A, B, C, D);
}

plane_2d_general_form plane_2d::to_general_form(const plane_2d_parametric_form& arg)
{
    if(arg.t.size() != 3 || arg.s.size() != 3 || arg.r0.size() != 3)
    {
        throw std::invalid_argument("Vectors must be 3-dimensional");
    }

    cvector normal = cross_product_3d(arg.t, arg.s);
    
    if(module(normal) == 0)
    {
        throw std::invalid_argument("Direction vectors must be linearly independent");
    }
    
    double A = normal[0];
    double B = normal[1];
    double C = normal[2];
    double D = -scalar_product(normal, arg.r0);
    
    return plane_2d_general_form(A, B, C, D);
}
#pragma endregion

#pragma region TO point_normal_form (realisation)
plane_2d_point_normal_form plane_2d::to_point_normal_form(const plane_2d_general_form& arg)
{
    if(arg.A == 0 && arg.B == 0 && arg.C == 0)
    {
        throw std::invalid_argument("Invalid plane_2d equation - zero normal vector");
    }

    // Находим точку на плоскости (подставляем x=0,y=0 и находим z)
    double x0 = 0.0, y0 = 0.0, z0 = 0.0;
    
    if(arg.C != 0)
    {
        z0 = -arg.D / arg.C;
    }
    else if(arg.B != 0)
    {
        y0 = -arg.D / arg.B;
    }
    else
    {
        x0 = -arg.D / arg.A;
    }

    return plane_2d_point_normal_form(x0, y0, z0, arg.A, arg.B, arg.C);
}

plane_2d_point_normal_form plane_2d::to_point_normal_form(const plane_2d_intercept_form& arg)
{
    if(arg.a == 0 || arg.b == 0 || arg.c == 0)
    {
        throw std::invalid_argument("Intercept parameters cannot be zero");
    }

    // Точка на плоскости (a,0,0)
    double x0 = arg.a;
    double y0 = 0.0;
    double z0 = 0.0;

    // Нормальный вектор (1/a, 1/b, 1/c)
    double A = 1.0/arg.a;
    double B = 1.0/arg.b;
    double C = 1.0/arg.c;

    return plane_2d_point_normal_form(x0, y0, z0, A, B, C);
}

plane_2d_point_normal_form plane_2d::to_point_normal_form(const plane_2d_normal_form& arg)
{
    double norm = sqrt(arg.cos_alpha*arg.cos_alpha + 
                      arg.cos_beta*arg.cos_beta + 
                      arg.cos_gamma*arg.cos_gamma);
    
    if(norm == 0)
    {
        throw std::invalid_argument("Normal vector cannot be zero");
    }

    // Нормализуем вектор
    double A = arg.cos_alpha / norm;
    double B = arg.cos_beta / norm;
    double C = arg.cos_gamma / norm;

    // Точка на плоскости - проекция начала координат
    double x0 = arg.p * arg.cos_alpha;
    double y0 = arg.p * arg.cos_beta;
    double z0 = arg.p * arg.cos_gamma;

    return plane_2d_point_normal_form(x0, y0, z0, A, B, C);
}

plane_2d_point_normal_form plane_2d::to_point_normal_form(const plane_2d_parametric_form& arg)
{
    if(arg.t.size() != 3 || arg.s.size() != 3 || arg.r0.size() != 3)
    {
        throw std::invalid_argument("Vectors must be 3-dimensional");
    }

    cvector normal = cross_product_3d(arg.t, arg.s);
    
    if(module(normal) == 0)
    {
        throw std::invalid_argument("Direction vectors must be linearly independent");
    }

    // Нормальный вектор
    double A = normal[0];
    double B = normal[1];
    double C = normal[2];

    // Точка r0 уже лежит на плоскости
    double x0 = arg.r0[0];
    double y0 = arg.r0[1];
    double z0 = arg.r0[2];

    return plane_2d_point_normal_form(x0, y0, z0, A, B, C);
}
#pragma endregion

#pragma region TO intercept_form (realisation)
plane_2d_intercept_form plane_2d::to_intercept_form(const plane_2d_general_form& arg)
{
    if(arg.D == 0)
    {
        throw std::invalid_argument("plane_2d passes through origin, cannot convert to intercept form");
    }

    if(arg.A == 0 || arg.B == 0 || arg.C == 0)
    {
        throw std::invalid_argument("plane_2d is parallel to an axis, cannot convert to intercept form");
    }

    double a = -arg.D / arg.A;
    double b = -arg.D / arg.B;
    double c = -arg.D / arg.C;

    return plane_2d_intercept_form(a, b, c);
}

plane_2d_intercept_form plane_2d::to_intercept_form(const plane_2d_point_normal_form& arg)
{
    if(arg.A == 0 || arg.B == 0 || arg.C == 0)
    {
        throw std::invalid_argument("plane_2d is parallel to an axis, cannot convert to intercept form");
    }

    double D = -arg.A * arg.x1 - arg.B * arg.y1 - arg.C * arg.z1;
    if(D == 0)
    {
        throw std::invalid_argument("plane_2d passes through origin, cannot convert to intercept form");
    }

    double a = -D / arg.A;
    double b = -D / arg.B;
    double c = -D / arg.C;

    return plane_2d_intercept_form(a, b, c);
}

plane_2d_intercept_form plane_2d::to_intercept_form(const plane_2d_normal_form& arg)
{
    if(arg.p == 0)
    {
        throw std::invalid_argument("plane_2d passes through origin, cannot convert to intercept form");
    }

    if(arg.cos_alpha == 0 || arg.cos_beta == 0 || arg.cos_gamma == 0)
    {
        throw std::invalid_argument("plane_2d is parallel to an axis, cannot convert to intercept form");
    }

    double a = arg.p / arg.cos_alpha;
    double b = arg.p / arg.cos_beta;
    double c = arg.p / arg.cos_gamma;

    return plane_2d_intercept_form(a, b, c);
}

plane_2d_intercept_form plane_2d::to_intercept_form(const plane_2d_parametric_form& arg)
{
    if(arg.t.size() != 3 || arg.s.size() != 3 || arg.r0.size() != 3)
    {
        throw std::invalid_argument("Vectors must be 3-dimensional");
    }

    cvector normal = cross_product_3d(arg.t, arg.s);
    
    if(module(normal) == 0)
    {
        throw std::invalid_argument("Direction vectors must be linearly independent");
    }

    double A = normal[0];
    double B = normal[1];
    double C = normal[2];
    double D = -scalar_product(normal, arg.r0);

    if(D == 0)
    {
        throw std::invalid_argument("plane_2d passes through origin, cannot convert to intercept form");
    }

    if(A == 0 || B == 0 || C == 0)
    {
        throw std::invalid_argument("plane_2d is parallel to an axis, cannot convert to intercept form");
    }

    double a = -D / A;
    double b = -D / B;
    double c = -D / C;

    return plane_2d_intercept_form(a, b, c);
}
#pragma endregion

#pragma region TO normal_form (realisation)
plane_2d_normal_form plane_2d::to_normal_form(const plane_2d_general_form& arg)
{
    if(arg.A == 0 && arg.B == 0 && arg.C == 0)
    {
        throw std::invalid_argument("Invalid plane_2d equation - zero normal vector");
    }

    double norm = sqrt(arg.A*arg.A + arg.B*arg.B + arg.C*arg.C);
    double sign = (arg.D > 0) ? -1.0 : 1.0;

    double cos_alpha = sign * arg.A / norm;
    double cos_beta = sign * arg.B / norm;
    double cos_gamma = sign * arg.C / norm;
    double p = sign * fabs(arg.D) / norm;

    return plane_2d_normal_form(cos_alpha, cos_beta, cos_gamma, p);
}

plane_2d_normal_form plane_2d::to_normal_form(const plane_2d_point_normal_form& arg)
{
    if(arg.A == 0 && arg.B == 0 && arg.C == 0)
    {
        throw std::invalid_argument("Invalid plane_2d equation - zero normal vector");
    }

    double D = -arg.A*arg.x1 - arg.B*arg.y1 - arg.C*arg.z1;
    double norm = sqrt(arg.A*arg.A + arg.B*arg.B + arg.C*arg.C);
    double sign = (D > 0) ? -1.0 : 1.0;

    double cos_alpha = sign * arg.A / norm;
    double cos_beta = sign * arg.B / norm;
    double cos_gamma = sign * arg.C / norm;
    double p = sign * fabs(D) / norm;

    return plane_2d_normal_form(cos_alpha, cos_beta, cos_gamma, p);
}

plane_2d_normal_form plane_2d::to_normal_form(const plane_2d_intercept_form& arg)
{
    if(arg.a == 0 || arg.b == 0 || arg.c == 0)
    {
        throw std::invalid_argument("Intercept parameters cannot be zero");
    }

    double A = 1.0/arg.a;
    double B = 1.0/arg.b;
    double C = 1.0/arg.c;
    double D = -1.0;

    double norm = sqrt(A*A + B*B + C*C);
    double sign = (D > 0) ? -1.0 : 1.0;

    double cos_alpha = sign * A / norm;
    double cos_beta = sign * B / norm;
    double cos_gamma = sign * C / norm;
    double p = sign * fabs(D) / norm;

    return plane_2d_normal_form(cos_alpha, cos_beta, cos_gamma, p);
}

plane_2d_normal_form plane_2d::to_normal_form(const plane_2d_parametric_form& arg)
{
    if(arg.t.size() != 3 || arg.s.size() != 3 || arg.r0.size() != 3)
    {
        throw std::invalid_argument("Vectors must be 3-dimensional");
    }

    cvector normal = cross_product_3d(arg.t, arg.s);
    
    if(module(normal) == 0)
    {
        throw std::invalid_argument("Direction vectors must be linearly independent");
    }

    double A = normal[0];
    double B = normal[1];
    double C = normal[2];
    double D = -scalar_product(normal, arg.r0);

    double norm = sqrt(A*A + B*B + C*C);
    double sign = (D > 0) ? -1.0 : 1.0;

    double cos_alpha = sign * A / norm;
    double cos_beta = sign * B / norm;
    double cos_gamma = sign * C / norm;
    double p = sign * fabs(D) / norm;

    return plane_2d_normal_form(cos_alpha, cos_beta, cos_gamma, p);
}
#pragma endregion

#pragma region TO parametric_form (realisation)
plane_2d_parametric_form plane_2d::to_parametric_form(const plane_2d_general_form& arg)
{
    if(arg.A == 0 && arg.B == 0 && arg.C == 0)
    {
        throw std::invalid_argument("Invalid plane_2d equation - zero normal vector");
    }

    // Находим точку на плоскости
    double x0, y0, z0;
    if(arg.C != 0)
    {
        x0 = 0; y0 = 0; z0 = -arg.D/arg.C;
    }
    else if(arg.B != 0)
    {
        x0 = 0; z0 = 0; y0 = -arg.D/arg.B;
    }
    else
    {
        y0 = 0; z0 = 0; x0 = -arg.D/arg.A;
    }

    // Находим два направляющих вектора, перпендикулярных нормали
    cvector normal = { arg.A, arg.B, arg.C };
    cvector u, v;

    if(arg.A != 0 || arg.B != 0)
    {
        // Первый вектор в плоскости XY
        u = { -arg.B, arg.A, 0 };
        // Второй вектор через векторное произведение
        v = cross_product_3d(normal, u);
    }
    else
    {
        // Если плоскость параллельна XY
        u = { 1, 0, 0 };
        v = { 0, 1, 0 };
    }
    cvector res = { x0, y0, z0 };
    return plane_2d_parametric_form(res, u, v);
}

plane_2d_parametric_form plane_2d::to_parametric_form(const plane_2d_point_normal_form& arg)
{
    if (arg.A == 0 && arg.B == 0 && arg.C == 0)
    {
        throw std::invalid_argument("Invalid plane_2d equation - zero normal vector");
    }

    // Точка уже дана
    cvector r0 = { arg.x1, arg.y1, arg.z1 };
    cvector normal = {arg.A, arg.B, arg.C};
    cvector u, v;

    if (arg.A != 0 || arg.B != 0)
    {
        u = { - arg.B, arg.A, 0};
        v = cross_product_3d(normal, u);
    }
    else
    {
        u = { 1, 0, 0 };
        v = { 0, 1, 0 };
    }

    return plane_2d_parametric_form(r0, u, v);
}

plane_2d_parametric_form plane_2d::to_parametric_form(const plane_2d_intercept_form& arg)
{
    if(arg.a == 0 || arg.b == 0 || arg.c == 0)
    {
        throw std::invalid_argument("Intercept parameters cannot be zero");
    }

    // Точки пересечения с осями
    cvector p1{arg.a, 0, 0};
    cvector p2{0, arg.b, 0};
    cvector p3{ 0, 0, arg.c };

    // Направляющие векторы
    cvector u = p2 - p1;
    cvector v = p3 - p1;

    return plane_2d_parametric_form(p1, u, v);
}

plane_2d_parametric_form plane_2d::to_parametric_form(const plane_2d_normal_form& arg)
{
    double norm = sqrt(arg.cos_alpha*arg.cos_alpha + 
                      arg.cos_beta*arg.cos_beta + 
                      arg.cos_gamma*arg.cos_gamma);
    
    if(norm == 0)
    {
        throw std::invalid_argument("Normal vector cannot be zero");
    }

    // Точка на плоскости - проекция начала координат
    cvector r0 = { arg.p * arg.cos_alpha, arg.p * arg.cos_beta, arg.p * arg.cos_gamma };
    cvector normal = { arg.cos_alpha, arg.cos_beta, arg.cos_gamma };
    cvector u, v;

    if(arg.cos_alpha != 0 || arg.cos_beta != 0)
    {
        u = cvector{ -arg.cos_beta, arg.cos_alpha, 0 };
        v = cross_product_3d(normal, u);
    }
    else
    {
        u = cvector{1, 0, 0};
        v = cvector{ 0, 1, 0 };
    }

    return plane_2d_parametric_form(r0, u, v);
}
#pragma endregion

#endif