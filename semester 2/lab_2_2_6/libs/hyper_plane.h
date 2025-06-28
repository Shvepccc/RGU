#ifndef HYPER_PLANE_H_
#define HYPER_PLANE_H_

#include <iostream>
#include "hyper_line.h"
#include "matrix.h"

class hyper_plane
{
public:
	cvector _C;
	double _b;

	hyper_plane(cvector C, double b) :
		_C(C), _b(b)
	{
	}

	friend cvector crossing_hiper_plane_and_hiper_line(
		hyper_line& line, hyper_plane& plane)
	{
		double t;

		try
		{
			t = -1 * ((plane._b + scalar_product(plane._C, line._A)) /
				scalar_product(plane._C, line._Q));
		}
		catch (const std::exception& e)
		{
			return cvector{};
		}

		cvector X = line._Q * t + line._A;
		return X;
	}

	friend cvector crossing_hiper_plane_and_two_points(
		hyper_plane& plane, cvector& line_point_a, cvector& line_point_b)
	{
		double cross_factor = (scalar_product(plane._C, line_point_a) + plane._b) *
			(scalar_product(plane._C, line_point_b) + plane._b);

		if (cross_factor < 0)
		{
			cvector Q = line_point_b - line_point_a;
			hyper_line new_line(Q, line_point_a);

			return crossing_hiper_plane_and_hiper_line(new_line, plane);
		}
		else
		{
			return cvector{};
		}
	}

	friend double distance_from_point_to_plane(
		hyper_plane& plane, cvector point)
	{
		double numerator;
		double denominator = 0;

		numerator = abs(scalar_product(plane._C, point) + plane._b);
		
		for (int j = 0; j < point.size(); ++j)
		{
			denominator += plane._C[j] * plane._C[j];
		}
		denominator = sqrt(denominator);

		return numerator / denominator;
	}

	friend matrix crossing_hyper_planes(
		hyper_plane* planes_arr, int planes_count)
	{
		int size = planes_arr[0]._C.size();
		matrix a(planes_count, size);
		matrix b(planes_count, 1);

		for (int i = 0; i < planes_count; ++i)
		{
			if (planes_arr[i]._C.size() != size)
			{
				throw std::invalid_argument("Planes must be the same dimension");
			}

			for (int j = 0; j < size; ++j)
			{
				a[i][j] = planes_arr[i]._C[j];
			}
			b[i][0] = -planes_arr[i]._b;
		}

		return solve_gauss(a, b);
	}
};

#endif