#ifndef HYPER_PLANE_H_
#define HYPER_PLANE_H_

#include <iostream>
#include "hyper_line.h"

class hyper_plane
{
public:
	cvector _C;
	double _b;

	hyper_plane(cvector C, double b) :
		_C(C), _b(b)
	{
	}
	~hyper_plane();

	friend cvector crossing_hiper_plane_and_hiper_line(
	hyper_line& line, hyper_plane& plane)
	{
		double t;

		try
		{
			t = -1 * ((plane._b + scalar_product(plane._C, line._A)) / scalar_product(plane._C, line._Q));
		}
		catch (const std::exception& e)
		{
			throw std::exception("Line and plane are not crossing");
		}

		cvector X = line._Q * t + line._A;
	}
};

cvector crossing_hiper_plane_and_hiper_line(
	hyper_line& line, hyper_plane& plane);

#endif