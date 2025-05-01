#include "../bigfloat.h"

int zero_arr[1] = { 0 };
bigint zero_bigint(zero_arr, 1);

bigfloat::bigfloat() :
	_numerator(zero_bigint),
	_denominator(zero_bigint)
{}

bigfloat::~bigfloat() {}

bigfloat bigfloat::operator-() const
{
	bigfloat negate = *this;
	return negate.negate();
}

bigfloat& bigfloat::negate()
{
	-this->_denominator;
	return *this;
}

void simplify()
{

}

// bigint gcd(bigint a, bigint b)
// {
// 	if (a == 0) return b;
// 	if (b == 0) return a;
// 
// 	int az = __builtin_ctz(a);
// 	int bz = __builtin_ctz(b);
// 	int shift = std::min(az, bz);
// 	a >>= az, b >>= bz;
// 
// 	while (a != 0) {
// 		int diff = a - b;
// 		b = std::min(a, b);
// 		a = std::abs(diff);
// 		a >>= __builtin_ctz(a);
// 	}
// 
// 	return b << shift;
// }

bigfloat& bigfloat::operator+= (bigfloat const& arg)
{
	bigint new_numerator = arg._numerator * this->_denominator;
	this->_denominator *= arg._denominator;
	this->_numerator *= arg._denominator;
	this->_numerator += new_numerator;
	//simplify();
	return *this;
}

bigfloat bigfloat::operator+ (bigfloat const& arg) const
{
	bigfloat copy = *this;
	return copy += arg;
}

bigfloat& bigfloat::operator-= (bigfloat const& arg)
{
	return *this += -arg;
}

bigfloat bigfloat::operator- (bigfloat const& arg) const
{
	bigfloat copy = *this;
	return copy + -arg;
}

bigfloat& bigfloat::operator*= (bigfloat const& arg)
{

	this->_numerator *= arg._numerator;
	this->_denominator *= arg._denominator;
	//simplify();
	return *this;
}

bigfloat bigfloat::operator* (bigfloat const& arg) const
{
	bigfloat copy = *this;
	return copy *= arg;
}

bigfloat& bigfloat::operator/= (bigfloat const& arg)
{
	this->_denominator *= arg._numerator;
	this->_numerator *= arg._denominator;
	//simplify();
	return *this;
}

bigfloat bigfloat::operator/ (bigfloat const& arg) const
{
	bigfloat copy = *this;
	return copy /= arg;
}

//bool bigfloat::operator== (bigfloat const& arg) const 
//{
//	return (this->_denominator == arg._denominator) &&
//		(this->_numerator == arg._numerator);
//}

//bool bigfloat::operator!= (bigfloat const& arg) const 
//{
//	return !(*this == arg);
//}

// bool bigfloat::operator<  (bigfloat const& arg) const 
// {
// }
// 
// bool bigfloat::operator<= (bigfloat const& arg) const 
// {
// }
// 
// bool bigfloat::operator>  (bigfloat const& arg) const 
// {
// }
// 
// bool bigfloat::operator>= (bigfloat const& arg) const 
// {
// }
// 
// 
// std::ostream& operator<<(
// 	std::ostream& stream,
// 	bigfloat const& value)
// { }
// 
// std::istream& operator>>(
// 	std::istream& stream,
// 	bigfloat& value)
// { }
// 
// bigfloat sin(bigfloat const& number, bigfloat const& EPS) {}
// bigfloat cos(bigfloat const& number, bigfloat const& EPS) {}
// bigfloat tg(bigfloat const& number, bigfloat const& EPS) {}
// bigfloat ctg(bigfloat const& number, bigfloat const& EPS) {}
// bigfloat sec(bigfloat const& number, bigfloat const& EPS) {}
// bigfloat cosec(bigfloat const& number, bigfloat const& EPS) {}
// bigfloat arcsin(bigfloat const& number, bigfloat const& EPS) {}
// bigfloat arccos(bigfloat const& number, bigfloat const& EPS) {}
// bigfloat arctg(bigfloat const& number, bigfloat const& EPS) {}
// bigfloat arcctg(bigfloat const& number, bigfloat const& EPS) {}
// bigfloat arcsec(bigfloat const& number, bigfloat const& EPS) {}
// bigfloat arccosec(bigfloat const& number, bigfloat const& EPS) {}
// 
// bigfloat pow(bigfloat const& base, bigint const& exp) {}
// bigfloat radical(bigfloat const& radicand, bigint const& index, bigfloat const& EPS) {}
// bigfloat log2(bigfloat const& number, bigfloat const& EPS) {}
// bigfloat ln(bigfloat const& number, bigfloat const& EPS) {}
// bigfloat log10(bigfloat const& number, bigfloat const& EPS) {}
