#ifndef HYPER_LINE_H_
#define HYPER_LINE_H_

#include <iostream>
#include "vector.h"

class hyper_line
{
public:
	cvector _Q;
	cvector _A;

	hyper_line(cvector Q, cvector A) :
		_Q(Q), _A(A)
	{}

	friend bool check_points_belong_to_line(
	cvector& line_point_a, cvector& line_point_b,
		cvector* points_colection, int points_count)
	{
		if (points_colection == nullptr)
		{
			throw std::invalid_argument("Null pointer");
		}
		if (line_point_a.size() != line_point_b.size())
		{
			throw std::invalid_argument("Size of vectors should be the same");
		}
		int size = line_point_a.size();

		for (int i = 0; i < points_count; ++i)
		{
			if (size != points_colection[i].size())
			{
				throw std::invalid_argument("Size of vectors should be the same");
			}

			cvector Q = line_point_a - line_point_b;
			double last_t = 0;
			for (int j = 0; j < size; ++j)
			{
				double t = (points_colection[i][j] - line_point_a[j]) / Q[j];

				if (last_t - t > 1e-10 && j != 0)
				{
					return false;
				}

				last_t = t;
			}
		}
		return true;
	}

	friend double distance_from_point_to_line(
		hyper_line& line, cvector& M)
	{
		double q_module = module(line._Q);
		if (q_module < 1e-10)
		{
			throw std::invalid_argument("Direction vector cannot be zero");
		}

		cvector AM = M - line._A;
		double a = pow(module(AM), 2);
		double b = pow(scalar_product(AM, line._Q), 2) / pow(q_module, 2);
		return sqrt(a - b);
	}

};

bool check_points_belong_to_line(
	cvector& line_point_a, cvector& line_point_b,
	cvector* points_colection, int points_count);

#endif