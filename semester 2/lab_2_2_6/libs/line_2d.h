#ifndef SIMPLE_LINE_H_
#define SIMPLE_LINE_H_

#include <iostream>
#include "hyper_line.h"
#include "vector.h"

#pragma region
constexpr double PI = 3.14159265358979323846;
constexpr double PI_2 = 1.57079632679489661923;
#pragma endregion


#include <cmath>
#include <algorithm>
#include <stdexcept>


// ---------------------------------------------------------------------


struct line_general_form
{
    double A;
    double B;
    double C;

    // Ax + By + C = 0
    line_general_form(double a, double b, double c) :
        A(a), B(b), C(c)
    {}
};

struct line_intercept_form
{
    double a;
    double b;

    // x/a + y/b = 1
    line_intercept_form(double a, double b) :
        a(a), b(b)
    {}
};

struct line_normal_form
{
    double A;
    double B;
    double p;

    // x*cos(alpha) + y*cos(beta) = p
    line_normal_form(double A_, double B_, double p_)
        : A(A_), B(B_), p(p_)
    {}
};

struct line_two_points_form
{
    double x1;
    double x2;
    double y1;
    double y2;

    // (x-x1)/(x2-x1)=(y-y1)/(y2-y1)
    line_two_points_form(double x1_, double y1_, double x2_, double y2_) 
        : x1(x1_), x2(x2_), y1(y1_), y2(y2_)
    {}
};

struct line_slope_intersect_form
{
    double k;
    double b;  

    // y = kx + b
    line_slope_intersect_form(double k_, double b_)
        : k(k_), b(b_)
    {}
};

struct line_parametric_form
{
    double l;  
    double x0; 
    double m;  
    double y0; 

    // x = l*t + x0
    // y = m*t + y0
    line_parametric_form(double l_, double x0_, double m_, double y0_)
        : l(l_), x0(x0_), m(m_), y0(y0_)
    {}
};



class line_2d
{
public:

    line_general_form data;

    line_2d(const line_general_form& lgf) : data(lgf) {}
    line_2d(const line_intercept_form& lif) : data(to_general_form(lif)) {}
    line_2d(const line_normal_form& lnf) : data(to_general_form(lnf)) {}
    line_2d(const line_two_points_form& ltp) : data(to_general_form(ltp)) {}
    line_2d(const line_slope_intersect_form& lsi) : data(to_general_form(lsi)) {}
    line_2d(const line_parametric_form& lpf) : data(to_general_form(lpf)) {}

#pragma region TO general_form
    line_general_form to_general_form(const line_intercept_form& arg);
    line_general_form to_general_form(const line_normal_form& arg);
    line_general_form to_general_form(const line_two_points_form& arg);
    line_general_form to_general_form(const line_slope_intersect_form& arg);
    line_general_form to_general_form(const line_parametric_form& arg);
#pragma endregion

#pragma region TO intercept_form
    line_intercept_form to_intercept_form(const line_general_form& arg);
    line_intercept_form to_intercept_form(const line_normal_form& arg);
    line_intercept_form to_intercept_form(const line_two_points_form& arg);
    line_intercept_form to_intercept_form(const line_slope_intersect_form& arg);
    line_intercept_form to_intercept_form(const line_parametric_form& arg);
#pragma endregion

#pragma region TO normal_form
    line_normal_form to_normal_form(const line_general_form& arg);
    line_normal_form to_normal_form(const line_intercept_form& arg);
    line_normal_form to_normal_form(const line_two_points_form& arg);
    line_normal_form to_normal_form(const line_slope_intersect_form& arg);
    line_normal_form to_normal_form(const line_parametric_form& arg);
#pragma endregion

#pragma region TO two_points_form
    line_two_points_form to_two_points_form(const line_general_form& arg);
    line_two_points_form to_two_points_form(const line_intercept_form& arg);
    line_two_points_form to_two_points_form(const line_normal_form& arg);
    line_two_points_form to_two_points_form(const line_slope_intersect_form& arg);
    line_two_points_form to_two_points_form(const line_parametric_form& arg);
#pragma endregion

#pragma region TO slope_intersect_form
    line_slope_intersect_form to_slope_intersect_form(const line_general_form& arg);
    line_slope_intersect_form to_slope_intersect_form(const line_intercept_form& arg);
    line_slope_intersect_form to_slope_intersect_form(const line_normal_form& arg);
    line_slope_intersect_form to_slope_intersect_form(const line_two_points_form& arg);
    line_slope_intersect_form to_slope_intersect_form(const line_parametric_form& arg);
#pragma endregion

#pragma region TO parametric_form
    line_parametric_form to_parametric_form(const line_general_form& arg);
    line_parametric_form to_parametric_form(const line_intercept_form& arg);
    line_parametric_form to_parametric_form(const line_normal_form& arg);
    line_parametric_form to_parametric_form(const line_two_points_form& arg);
    line_parametric_form to_parametric_form(const line_slope_intersect_form& arg);
#pragma endregion

    friend double distance_between_parallel_lines(const line_2d& line1, const line_2d& line2)
    {
        const line_general_form& l1 = line1.data;
        const line_general_form& l2 = line2.data;

        const double epsilon = 1e-10;
        if (abs(l1.A * l2.B - l2.A * l1.B) > epsilon)
        {
            throw std::runtime_error("Lines are not parallel");
        }

        double factor;
        if (abs(l2.A) > epsilon)
        {
            factor = l1.A / l2.A;
        }
        else if (abs(l2.B) > epsilon)
        {
            factor = l1.B / l2.B;
        }
        else
        {
            throw std::runtime_error("Invalid line equation");
        }

        return abs(l2.C * factor - l1.C) / sqrt(l1.A * l1.A + l1.B * l1.B);
    }

    double area_of_axes_triangle() const
    {
        const double& A = data.A;
        const double& B = data.B;
        const double& C = data.C;

        if (abs(C) < 1e-10) {
            return 0.0;
        }

        double x_intercept, y_intercept;

        if (abs(A) > 1e-10) {
            x_intercept = -C / A;
        }
        else {
            if (abs(B) > 1e-10) {
                x_intercept = std::numeric_limits<double>::infinity();
            }
            else {
                throw std::runtime_error("Invalid line equation");
            }
        }

        if (abs(B) > 1e-10) {
            y_intercept = -C / B;
        }
        else {
            if (abs(A) > 1e-10) {
                y_intercept = std::numeric_limits<double>::infinity();
            }
            else {
                throw std::runtime_error("Invalid line equation");
            }
        }

        if (std::isinf(x_intercept) || std::isinf(y_intercept)) {
            return 0.0;
        }

        return 0.5 * abs(x_intercept * y_intercept);
    }

    static double angle_between_lines(const line_2d& line1, const line_2d& line2)
    {
        const double A1 = line1.data.A;
        const double B1 = line1.data.B;
        const double A2 = line2.data.A;
        const double B2 = line2.data.B;

        double dot_product = A1 * A2 + B1 * B2;

        double norm1 = sqrt(A1 * A1 + B1 * B1);
        double norm2 = sqrt(A2 * A2 + B2 * B2);

        double cos_angle = dot_product / (norm1 * norm2);

        cos_angle = (cos_angle < -1.0) ? -1.0 : (cos_angle > 1.0) ? 1.0 : cos_angle;

        double angle = acos(cos_angle);
        return (angle > PI_2) ? PI - angle : angle;
    }

    // Версия, возвращающая угол в градусах
    static double angle_between_lines_deg(const line_2d& line1, const line_2d& line2)
    {
        return angle_between_lines(line1, line2) * 180.0 / PI;
    }

};

#pragma region TO general_form (realisation)
line_general_form line_2d::to_general_form(const line_intercept_form& arg)
{
    // x/a + y/b = 1  =>  (1/a)x + (1/b)y - 1 = 0
    return line_general_form(1.0/arg.a, 1.0/arg.b, -1.0);
}

line_general_form line_2d::to_general_form(const line_normal_form& arg)
{
    // x*cos(α) + y*cos(β) = p  =>  cos(α)x + cos(β)y - p = 0
    return line_general_form(arg.A, arg.B, -arg.p);
}

line_general_form line_2d::to_general_form(const line_two_points_form& arg)
{
    // (x-x1)/(x2-x1) = (y-y1)/(y2-y1)
    // Преобразуем к виду (y2-y1)(x-x1) - (x2-x1)(y-y1) = 0
    // Раскрываем скобки:
    // (y2-y1)x - (y2-y1)x1 - (x2-x1)y + (x2-x1)y1 = 0
    double A = arg.y2 - arg.y1;
    double B = -(arg.x2 - arg.x1);
    double C = (arg.x2 - arg.x1)*arg.y1 - (arg.y2 - arg.y1)*arg.x1;
    
    return line_general_form(A, B, C);
}

line_general_form line_2d::to_general_form(const line_slope_intersect_form& arg)
{
    // y = kx + b  =>  kx - y + b = 0
    return line_general_form(arg.k, -1.0, arg.b);
}

line_general_form line_2d::to_general_form(const line_parametric_form& arg)
{
    // x = l*t + x0
    // y = m*t + y0
    // Выражаем t из первого уравнения: t = (x - x0)/l
    // Подставляем во второе: y = m*(x - x0)/l + y0
    // Преобразуем: l*y = m*x - m*x0 + l*y0 => m*x - l*y - m*x0 + l*y0 = 0
    return line_general_form(arg.m, -arg.l, arg.l*arg.y0 - arg.m*arg.x0);
}
#pragma endregion

#pragma region TO intercept_form (realisation)
line_intercept_form line_2d::to_intercept_form(const line_general_form& arg)
{
    // Ax + By + C = 0 => x/(-C/A) + y/(-C/B) = 1
    if (arg.C == 0)
    {
        throw std::invalid_argument("Line passes through origin, cannot convert to intercept form");
    }
    double a = -arg.C / arg.A;
    double b = -arg.C / arg.B;
    return line_intercept_form(a, b);
}

line_intercept_form line_2d::to_intercept_form(const line_normal_form& arg)
{
    line_general_form gf = to_general_form(arg);
    return to_intercept_form(gf);
}

line_intercept_form line_2d::to_intercept_form(const line_two_points_form& arg)
{
    line_general_form gf = to_general_form(arg);
    return to_intercept_form(gf);
}

line_intercept_form line_2d::to_intercept_form(const line_slope_intersect_form& arg)
{
    // y = kx + b => y-intercept = b
    // Для x-intercept: y=0 => x = -b/k
    if (arg.k == 0)
    {
        throw std::invalid_argument("Horizontal line cannot be represented in intercept form");
    }
    double a = -arg.b / arg.k;  // x-intercept
    double b = arg.b;           // y-intercept
    return line_intercept_form(a, b);
}

line_intercept_form line_2d::to_intercept_form(const line_parametric_form& arg)
{
    // OX (y=0): t = -y0/m => x = l*(-y0/m) + x0
    // OY (x=0): t = -x0/l => y = m*(-x0/l) + y0
    if (arg.m == 0 || arg.l == 0)
    {
        throw std::invalid_argument("Line is parallel to one of axes, cannot convert to intercept form");
    }
    
    double a = arg.x0 - arg.l * arg.y0 / arg.m; 
    double b = arg.y0 - arg.m * arg.x0 / arg.l; 
    
    return line_intercept_form(a, b);
}
#pragma endregion

#pragma region TO normal_form (realisation)
line_normal_form line_2d::to_normal_form(const line_general_form& arg)
{
    // Ax + By + C = 0 => x*cos(α) + y*cos(β) = p
    // Нормализуем: A/sqrt(A²+B²), B/sqrt(A²+B²), C/sqrt(A²+B²)
    double norm = sqrt(arg.A * arg.A + arg.B * arg.B);
    if (norm == 0)
    {
        throw std::invalid_argument("Invalid line equation");
    }
    
    // Учитываем знак, чтобы p было положительным
    double sign = (arg.C > 0) ? -1.0 : 1.0;
    if (arg.C == 0) {
        sign = (arg.A + arg.B > 0) ? 1.0 : -1.0;
    }
    
    return line_normal_form(
        sign * arg.A / norm,
        sign * arg.B / norm,
        -sign * arg.C / norm
    );
}

line_normal_form line_2d::to_normal_form(const line_intercept_form& arg)
{
    line_general_form gf = to_general_form(arg);
    return to_normal_form(gf);
}

line_normal_form line_2d::to_normal_form(const line_two_points_form& arg)
{
    line_general_form gf = to_general_form(arg);
    return to_normal_form(gf);
}

line_normal_form line_2d::to_normal_form(const line_slope_intersect_form& arg)
{
    line_general_form gf = to_general_form(arg);
    return to_normal_form(gf);
}

line_normal_form line_2d::to_normal_form(const line_parametric_form& arg)
{
    line_general_form gf = to_general_form(arg);
    return to_normal_form(gf);
}
#pragma endregion

#pragma region TO two_points_form (realisation)
line_two_points_form line_2d::to_two_points_form(const line_general_form& arg)
{
    // Ax + By + C = 0
    // Находим две точки пересечения с осями
    double x1, y1, x2, y2;
    
    if(arg.B != 0)
    {
        // Первая точка: x=0 => y=-C/B
        x1 = 0;
        y1 = -arg.C/arg.B;
        
        // Вторая точка: y=0 => x=-C/A
        if(arg.A != 0)
        {
            x2 = -arg.C/arg.A;
            y2 = 0;
        }
        else
        {
            // Вертикальная линия
            x2 = x1;
            y2 = y1 + 1;
        }
    }
    else
    {
        // Горизонтальная линия
        x1 = -arg.C/arg.A;
        y1 = 0;
        x2 = x1 + 1;
        y2 = 0;
    }
    
    return line_two_points_form(x1, y1, x2, y2);
}

line_two_points_form line_2d::to_two_points_form(const line_intercept_form& arg)
{
    line_general_form gf = to_general_form(arg);
    return to_two_points_form(gf);
}

line_two_points_form line_2d::to_two_points_form(const line_normal_form& arg)
{
    line_general_form gf = to_general_form(arg);
    return to_two_points_form(gf);
}

line_two_points_form line_2d::to_two_points_form(const line_slope_intersect_form& arg)
{
    // y = kx + b
    // Первая точка: x=0 => y=b
    double x1 = 0;
    double y1 = arg.b;
    
    // Вторая точка: x=1 => y=k+b
    double x2 = 1;
    double y2 = arg.k + arg.b;
    
    return line_two_points_form(x1, y1, x2, y2);
}

line_two_points_form line_2d::to_two_points_form(const line_parametric_form& arg)
{
    // x = l*t + x0
    // y = m*t + y0
    // Первая точка при t=0
    double x1 = arg.x0;
    double y1 = arg.y0;
    
    // Вторая точка при t=1
    double x2 = arg.l + arg.x0;
    double y2 = arg.m + arg.y0;
    
    return line_two_points_form(x1, y1, x2, y2);
}
#pragma endregion

#pragma region TO slope_intersect_form
line_slope_intersect_form line_2d::to_slope_intersect_form(const line_general_form& arg)
{
    // Ax + By + C = 0 -> y = (-A/B)x + (-C/B)
    if(arg.B == 0)
    {
        throw std::invalid_argument("Vertical line cannot be represented in slope-intercept form");
    }
    
    double k = -arg.A / arg.B;
    double b = -arg.C / arg.B;
    
    return line_slope_intersect_form(k, b);
}

line_slope_intersect_form line_2d::to_slope_intersect_form(const line_intercept_form& arg)
{
    // x/a + y/b = 1 -> y = (-b/a)x + b
    double k = -arg.b / arg.a;
    double b = arg.b;
    
    return line_slope_intersect_form(k, b);
}

line_slope_intersect_form line_2d::to_slope_intersect_form(const line_normal_form& arg)
{
    // x*cos(α) + y*cos(β) = p -> y = (-cos(α)/cos(β))x + p/cos(β)
    if(arg.B == 0)
    {
        throw std::invalid_argument("Vertical line in normal form cannot be converted");
    }
    
    double k = -arg.A / arg.B;
    double b = arg.p / arg.B;
    
    return line_slope_intersect_form(k, b);
}

line_slope_intersect_form line_2d::to_slope_intersect_form(const line_two_points_form& arg)
{
    // k = (y2-y1)/(x2-x1)
    // b = y1 - k*x1
    if(arg.x1 == arg.x2)
    {
        throw std::invalid_argument("Vertical line cannot be represented in slope-intercept form");
    }
    
    double k = (arg.y2 - arg.y1) / (arg.x2 - arg.x1);
    double b = arg.y1 - k * arg.x1;
    
    return line_slope_intersect_form(k, b);
}

line_slope_intersect_form line_2d::to_slope_intersect_form(const line_parametric_form& arg)
{
    // x = l*t + x0
    // y = m*t + y0
    // Выражаем t из первого уравнения: t = (x - x0)/l
    // Подставляем во второе: y = (m/l)(x - x0) + y0
    if(arg.l == 0)
    {
        throw std::invalid_argument("Vertical line in parametric form cannot be converted");
    }
    
    double k = arg.m / arg.l;
    double b = arg.y0 - k * arg.x0;
    
    return line_slope_intersect_form(k, b);
}
#pragma endregion

#pragma region TO parametric_form
line_parametric_form line_2d::to_parametric_form(const line_general_form& arg)
{
    // Ax + By + C = 0
    // Выбираем точку (x0,y0) на прямой и направляющий вектор (l,m)
    double x0, y0, l, m;
    
    if(arg.B != 0)
    {
        // Если B ≠ 0, берем точку при x=0: (0, -C/B)
        x0 = 0;
        y0 = -arg.C / arg.B;
        // Направляющий вектор (-B, A)
        l = -arg.B;
        m = arg.A;
    }
    else
    {
        // Вертикальная линия (A ≠ 0, так как иначе уравнение не задает прямую)
        // Точка (-C/A, 0)
        x0 = -arg.C / arg.A;
        y0 = 0;
        // Направляющий вектор (0, 1)
        l = 0;
        m = 1;
    }
    
    return line_parametric_form(l, x0, m, y0);
}

line_parametric_form line_2d::to_parametric_form(const line_intercept_form& arg)
{
    // x/a + y/b = 1
    // Две точки: (a,0) и (0,b)
    // Направляющий вектор (-a, b)
    double l = -arg.a;
    double x0 = arg.a;
    double m = arg.b;
    double y0 = 0;
    
    return line_parametric_form(l, x0, m, y0);
}

line_parametric_form line_2d::to_parametric_form(const line_normal_form& arg)
{
    // Сначала преобразуем в general form
    line_general_form gf = to_general_form(arg);
    // Затем в parametric form
    return to_parametric_form(gf);
}

line_parametric_form line_2d::to_parametric_form(const line_two_points_form& arg)
{
    // Уже заданы две точки (x1,y1) и (x2,y2)
    // Направляющий вектор (x2-x1, y2-y1)
    double l = arg.x2 - arg.x1;
    double x0 = arg.x1;
    double m = arg.y2 - arg.y1;
    double y0 = arg.y1;
    
    return line_parametric_form(l, x0, m, y0);
}

line_parametric_form line_2d::to_parametric_form(const line_slope_intersect_form& arg)
{
    // y = kx + b
    // Две точки: (0,b) и (1,k+b)
    // Направляющий вектор (1, k)
    double l = 1;
    double x0 = 0;
    double m = arg.k;
    double y0 = arg.b;
    
    return line_parametric_form(l, x0, m, y0);
}
#pragma endregion

class segment
{
public:
    double x1, y1;
    double x2, y2;

    segment(double x1_, double y1_, double x2_, double y2_)
        : x1(x1_), y1(y1_), x2(x2_), y2(y2_)
    {
        if (x1 == x2 && y1 == y2) {
            throw std::invalid_argument("Segment cannot be a point");
        }
    }

    double length() const
    {
        return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    }

    friend double distance_from_line_to_segment(const line_2d& line, const segment& segm)
    {
        double A = line.data.A;
        double B = line.data.B;
        double C = line.data.C;

        double dist1 = A * segm.x1 + B * segm.y1 + C;
        double dist2 = A * segm.x2 + B * segm.y2 + C;

        if (dist1 * dist2 < 0.0)
        {
            return 0.0;
        }

        double abs_dist2 = abs(dist2) / sqrt(A * A + B * B);
        double abs_dist1 = abs(dist1) / sqrt(A * A + B * B);

        return (abs_dist1 < abs_dist2) ? abs_dist1 : abs_dist2;
    }

    friend cvector find_segments_intersection(const segment& seg1, const segment& seg2)
    {
        const double x1 = seg1.x1, y1 = seg1.y1;
        const double x2 = seg1.x2, y2 = seg1.y2;

        const double x3 = seg2.x1, y3 = seg2.y1;
        const double x4 = seg2.x2, y4 = seg2.y2;

        const double denom = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);

        if (fabs(denom) < 1e-10)
        {
            return cvector();
        }

        const double t = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denom;
        const double u = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denom;

        if (t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0)
        {
            return cvector{ x1 + t * (x2 - x1), y1 + t * (y2 - y1) };
        }

        return cvector();
    }
};


#endif