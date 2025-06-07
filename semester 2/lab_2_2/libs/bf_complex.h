#ifndef BF_COMPLEX_H
#define BF_COMPLEX_H

#include <iostream>
#include "../libs/bigfloat.h"

class bf_complex
{
private:
	bigfloat _real_part;
	bigfloat _imaginary_part;

public:
	bf_complex(
		bigfloat integer_part = bigfloat(0.0), 
		bigfloat imaginary_part = bigfloat(0.0));

	bf_complex(
		double integer_numerator, double integer_denominator,
		double imaginary_numerator, double imaginary_denominator);

	bf_complex& operator +=(bf_complex const& arg);
	bf_complex& operator -=(bf_complex const& arg);
	bf_complex& operator *=(bf_complex const& arg);
	bf_complex& operator /=(bf_complex const& arg);

	bf_complex operator +(bf_complex const& arg) const;
	bf_complex operator -(bf_complex const& arg) const;
	bf_complex operator *(bf_complex const& arg) const;
	bf_complex operator /(bf_complex const& arg) const;

	bigfloat get_mod() const;
	bigfloat get_arg(bigfloat eps = bigfloat(1, 1e5)) const;

	friend std::ostream& operator <<(
		std::ostream& os,
		bf_complex const& arg);

	friend std::istream& operator >>(
		std::istream& os,
		bf_complex& arg);
};

#endif // !BF_COMPLEX_H