//#include "../libs/bigint.h"
#ifndef BIGFLOAT_H_
#define BIGFLOAT_H_

#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>

using bigint = boost::multiprecision::cpp_int;

class bigfloat
{
private:
	bigint _numerator;
	bigint _denominator;

	
	void simplify();

public:
	bigint gcd(bigint a, bigint b);

	bigfloat();
	~bigfloat();
	bigfloat(double value);
	bigfloat(bigint value);

	bigfloat(bigint numerator, bigint denominator);
	bigfloat(double numerator, double denominator);

	bigfloat operator-() const;
	bigfloat& negate();

	bigfloat& operator+= (bigfloat const &arg);
	bigfloat& operator-= (bigfloat const &arg);
	bigfloat& operator/= (bigfloat const &arg);
	bigfloat& operator*= (bigfloat const &arg);

	bigfloat operator+ (bigfloat const &arg) const;
	bigfloat operator- (bigfloat const &arg) const;
	bigfloat operator* (bigfloat const &arg) const;
	bigfloat operator/ (bigfloat const &arg) const;

	friend bigfloat operator* (bigfloat const& lhs, double rhs);
	friend bigfloat operator* (double lhs, bigfloat const& rhs);

	bigint compare(bigfloat const& arg) const;

	bool operator== (bigfloat const& arg) const;
	bool operator!= (bigfloat const& arg) const;

	bool operator<  (bigfloat const& other) const;
	bool operator<= (bigfloat const& other) const;
	bool operator>  (bigfloat const& other) const;
	bool operator>= (bigfloat const& other) const;

	friend std::ostream& operator<<(
		std::ostream& stream,
		bigfloat const& value);

	friend std::istream& operator>>(
		std::istream& stream,
		bigfloat& value);

	bool is_zero();

private:
	friend bigfloat calculate_row(
		bigfloat const& value,
		bigfloat(*calc_member_function)(bigfloat const&, unsigned int),
		bigfloat const& eps);

	friend bigfloat sin_member(bigfloat const& x, unsigned int n);
	friend bigfloat cos_member(bigfloat const& x, unsigned int n);
	friend bigfloat tg_member(bigfloat const& x, unsigned int n);
	friend bigfloat cosec_member(bigfloat const& x, unsigned int n);
	friend bigfloat arcsin_member(bigfloat const& x, unsigned int n);
	friend bigfloat arctg_member(bigfloat const& x, unsigned int n);

	friend bigfloat pi_member(bigfloat const& x, unsigned int n);

public:
	friend bigfloat sin(bigfloat const& number, bigfloat const& EPS);
	friend bigfloat cos(bigfloat const& number, bigfloat const& EPS);
	friend bigfloat tg(bigfloat const& number, bigfloat const& EPS);
	friend bigfloat ctg(bigfloat const& number, bigfloat const& EPS);
	friend bigfloat sec(bigfloat const& number, bigfloat const& EPS);
	friend bigfloat cosec(bigfloat const& number, bigfloat const& EPS);

	friend bigfloat arcsin(bigfloat const& number, bigfloat const& EPS);
	friend bigfloat arccos(bigfloat const& number, bigfloat const& EPS);
	friend bigfloat arctg(bigfloat const& number, bigfloat const& EPS);
	friend bigfloat arcctg(bigfloat const& number, bigfloat const& EPS);

	friend bigfloat pow(bigfloat const& base, int exp);
	friend bigfloat radical(bigfloat const& radicand, unsigned int const& index, bigfloat const& EPS);

	//TODO: log, log2 and log10 did't work
	friend bigfloat log2(bigfloat const& number, bigfloat const& EPS);
	friend bigfloat ln(bigfloat const& number, bigfloat const& EPS);
	friend bigfloat log10(bigfloat const& number, bigfloat const& EPS);

	friend bigfloat atan2(bigfloat const& y, bigfloat const& x, bigfloat const& EPS);

	friend bigfloat abs(bigfloat const& arg) 
	{
		return ((arg < 0) ? -arg : arg);
	}

	static bigfloat PI();

	friend bigint floor_div(bigfloat const& numerator, bigfloat const& denominator);
	friend double to_double(const bigfloat& value);
};

bigfloat bernoulli(int n);

bigfloat sin(const bigfloat& number, const bigfloat& EPS);
bigfloat cos(const bigfloat& number, const bigfloat& EPS);
bigfloat tg(const bigfloat& number, const bigfloat& EPS);
bigfloat ctg(const bigfloat& number, const bigfloat& EPS);
bigfloat sec(const bigfloat& number, const bigfloat& EPS);
bigfloat cosec(const bigfloat& number, const bigfloat& EPS);

#endif // BIGFLOAT_H_