#include "../bigfloat.h"
#include <iostream>
#include <math.h>

bigfloat small_eps(5, 1e3);

unsigned int ctz(const bigint& n) 
{
	if (n == 0) return 0;
	bigint x = n;
	unsigned int count = 0;

	while ((x & 1) == 0) {
		x >>= 1;
		++count;
	}
	return count;
}

bigfloat::bigfloat() :
	_numerator(0),
	_denominator(0)
{}

bigfloat::bigfloat(double value) {
	_numerator = static_cast<bigint>(value * 1e6);
	_denominator = static_cast<bigint>(1e6);
	simplify();
}

bigfloat::bigfloat(bigint value) {
	_numerator = value;
	_denominator = 1;
	simplify();
}

bigfloat::~bigfloat() {}

bigfloat::bigfloat(bigint numerator, bigint denominator) :
	_numerator(numerator),
	_denominator(denominator)
{
	simplify();
}

bigfloat::bigfloat(double numerator, double denominator) :
	_numerator(static_cast<bigint>(numerator)),
	_denominator(static_cast<bigint>(denominator))
{
	simplify();
}

bigfloat bigfloat::operator-() const
{
	bigfloat negate = *this;
	return negate.negate();
}

bigfloat& bigfloat::negate()
{
	//-this->_denominator;
	this->_numerator *= -1; //TODO: CHANGE IT
	return *this;
}

void bigfloat::simplify()
{
	bigint divider = boost::multiprecision::gcd(abs(_numerator), abs(_denominator));
	if (divider == 1) {
		return;
	}
	_numerator /= divider;
	_denominator /= divider;

	if (_denominator < 0) {
		_numerator = -_numerator;
		_denominator = -_denominator;
	}
}

bool bigfloat::is_zero()
{
	return (_numerator == 0 && _denominator == 0);
}

bigfloat& bigfloat::operator+= (bigfloat const& arg)
{
	if (this->is_zero()) { *this = arg; return *this; }
	if ((const_cast<bigfloat&>(arg)).is_zero()) { return *this; }

	bigint new_numerator = arg._numerator * this->_denominator;
	this->_numerator *= arg._denominator;
	this->_numerator += new_numerator;
	this->_denominator *= arg._denominator;
	simplify();
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
	return copy + (-arg);
}

bigfloat& bigfloat::operator*= (bigfloat const& arg)
{
	this->_numerator *= arg._numerator;
	this->_denominator *= arg._denominator;
	simplify();
	return *this;
}

bigfloat operator* (bigfloat const& lhs, double rhs) {
	return lhs * bigfloat(rhs);
}

bigfloat operator* (double lhs, bigfloat const& rhs) {
	return bigfloat(lhs) * rhs;
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
	simplify();
	return *this;
}

bigfloat bigfloat::operator/ (bigfloat const& arg) const
{
	bigfloat copy = *this;
	return copy /= arg;
}


bigint bigfloat::compare(bigfloat const &arg) const
{
	bigint lhs = this->_numerator * arg._denominator;
	bigint rhs = arg._numerator * this->_denominator;

	if (lhs < rhs) return -1;
	if (lhs > rhs) return 1;
	return 0;
}

bool bigfloat::operator== (bigfloat const& arg) const 
{
	return this->compare(arg) == 0;
}

bool bigfloat::operator!= (bigfloat const& arg) const 
{
	return this->compare(arg) != 0;
}

bool bigfloat::operator<  (bigfloat const& arg) const 
{
	return this->compare(arg) < 0;
}

bool bigfloat::operator<= (bigfloat const& arg) const 
{
	return this->compare(arg) <= 0;
}

bool bigfloat::operator>  (bigfloat const& arg) const 
{
	return this->compare(arg) > 0;
}

bool bigfloat::operator>= (bigfloat const& arg) const 
{
	return this->compare(arg) >= 0;
}

std::ostream& operator<<(
	std::ostream& stream,
	bigfloat const& value)
{
	return stream << value._numerator << "/" << value._denominator;
}

std::istream& operator>>(
	std::istream& stream,
	bigfloat& value)
{
	char ch;
	bool negative = false;
	bigint numerator = 0, denominator = 1;

	while (stream.peek() == ' ') stream.get();

	if (stream.peek() == '-') {
		negative = true;
		stream.get();
	}
	stream >> numerator;

	if (stream.peek() == '/') {
		stream.get();
	}
	stream >> denominator;

	if (negative) numerator = -numerator;

	value = bigfloat(numerator, denominator);
	return stream;
}

bigfloat calculate_row(
	bigfloat const& value, 
	bigfloat(*calc_member_function)(bigfloat const&, unsigned int), 
	bigfloat const& eps)
{
	//std::cout << to_double(value) << std::endl;
	bigfloat res;
	bigfloat temp;
	unsigned int n = 1;
	while (true)
	{
		temp = calc_member_function(value, n);
		res += temp;
		//std::cout << temp << " ";
		if (abs(temp) < eps) { break; }
		++n;
	}
	//std::cout << "(" << n << ") ";
	return res;
}

bigfloat sin_member(bigfloat const& x, unsigned int n)
{
	bigfloat res = x;
	for (unsigned int i = 1; i < n; ++i)
	{
		bigfloat multiplier = (x*x) / ((2 * i) * (2 * i + 1));
		res *= -multiplier;
	}

	return res;
}

bigfloat cos_member(bigfloat const& x, unsigned int n)
{
	bigfloat res = 1;
	for (unsigned int i = 1; i < n; ++i)
	{
		bigfloat multiplier = (x*x) / ((2 * i) * (2 * i - 1));
		res *= -multiplier;
	}

	return res;
}

bigfloat bernoulli(int n) {

	if (n == 0) return bigfloat(1.0);
	if (n == 1) return bigfloat(-0.5);
	if (n % 2 != 0) return bigfloat(0.0);

	bigfloat* B = new bigfloat[n + 1];
	B[0] = bigfloat(1);
	B[1] = -bigfloat(0.5);

	for (int m = 2; m <= n; ++m) {
		if (m % 2 != 0) {
			B[m] = bigfloat(0.0);
			continue;
		}

		bigfloat sum = 0;
		bigint binom = 1;
		for (int k = 0; k < m; ++k) {
			sum += bigfloat(binom) * B[k];
			binom = binom * (m + 1 - k) / (k + 1);
		}
		B[m] = -sum / bigfloat(m + 1);
	}

	return B[n];
}

bigint factorial_2n(unsigned int n)
{
	bigint fac = 1;
	for (int i = 1; i <= n; ++i)
	{
		fac = fac * 2 * i * (2 * i - 1);
	}
	return fac;
}

bigint factorial_n(unsigned int n)
{
	bigint fac = 1;
	for (int i = 1; i <= n; ++i)
	{
		fac *= i;
	}
	return fac;
}

bigfloat tg_member(bigfloat const& x, unsigned int n)
{
	bigfloat res = 1;
	
	auto deg = (1 << (2 * n));
	bigfloat B = bernoulli(2 * n);
	bigfloat coeff = (pow(-1, n + 1) * deg * (deg - 1) * B) / factorial_2n(n);
	res = coeff * pow(x, 2 * n - 1);

	return res;
}


bigfloat cosec_member(bigfloat const& x, unsigned int n)
{
	--n;
	if (n == 0)
		return bigfloat(1.0) / x;

	bigfloat res = 1;
	
	auto deg = (1 << (2 * n - 1));
	bigfloat B = bernoulli(2 * n);
	bigfloat coeff = (pow(-1, n + 1) * 2 * (deg - 1) * B) / factorial_2n(n);
	res = coeff * pow(x, 2 * n - 1);

	return res;
}

bigfloat arcsin_member(bigfloat const& x, unsigned int n) 
{
	--n;
	if (n == 0)
		return x;

	bigfloat res = 1;
	bigfloat a = factorial_2n(n);
	bigfloat b = (1 << (2 * n)) * factorial_n(n) * factorial_n(n) * (2 * n + 1);
	bigfloat coeff = a / b;
	res = coeff * pow(x, 2 * n + 1);

	return res;
}

bigfloat arctg_member(bigfloat const& x, unsigned int n) 
{
	--n;
	bigfloat num = pow(x, 2 * n + 1);
	bigfloat denom = 2 * n + 1;
	//return pow(-1, n) * num / denom;
	return ((n % 2 == 0) ? 1 : -1) * pow(x, 2 * n + 1) / (2 * n + 1);
}

//bigfloat pi_member(bigfloat const& x, unsigned int n)
//{
//	--n;
//	if (n == 0)
//		return bigfloat(1.0);
//
//	return bigfloat(((n % 2 == 0) ? 1 : -1), 2 * n + 1);
//}

bigfloat pi_member(bigfloat const& x, unsigned int n)
{
	--n;
	if (n == 0) return 3;
	static bigfloat four(4.0);

	bigfloat sign = (n % 2 == 0) ? 1 : -1;
	bigfloat denom = (2 * n) * (2 * n + 1) * (2 * n + 2);
	return sign * (four / denom);
}

bigfloat sin(bigfloat const& number, bigfloat const& EPS)
{
	bigfloat two_pi = bigfloat::PI() * 2;
	bigfloat reduced = number - two_pi * floor_div(number, two_pi);
	return calculate_row(reduced, sin_member, EPS);
}

bigfloat cos(bigfloat const& number, bigfloat const& EPS)
{
	bigfloat two_pi = bigfloat::PI() * 2;
	bigfloat reduced = number - two_pi * floor_div(number, two_pi);
	return calculate_row(reduced, cos_member, EPS);
}

bigfloat tg(bigfloat const& number, bigfloat const& EPS)
{
	//bigfloat pi = bigfloat::PI();
	//bigfloat reduced = number - pi * floor_div(number, pi) - (pi / 2);

	bigfloat pi = bigfloat::PI();
	bigfloat two_pi = pi * 2;
	bool negate = false;

	bigfloat reduced = number - two_pi * floor_div(number + pi, two_pi);

	if (reduced > pi / 2)
	{
		reduced = pi - reduced;
		negate = !negate;
		reduced.simplify();
	}		
	else if (reduced <= -pi / 2)
	{
		reduced = -pi - reduced;
		negate = !negate;
		reduced.simplify();
	}
	else if (reduced < bigfloat(0))
	{
		reduced = -reduced;
		negate = !negate;
		reduced.simplify();
	}

	bigfloat err = abs(reduced / (pi / 2));
	
	if (err == bigfloat(1.0))
		throw std::invalid_argument("The tg argument must not be equal to pi/2 or 3*pi/2.");

	bigfloat res = calculate_row(reduced, tg_member, EPS);

	res.simplify();
	return negate ? -res : res;
}

bigfloat ctg(bigfloat const& number, bigfloat const& EPS)
{
	bigfloat pi = bigfloat::PI();
	bigfloat two_pi = pi * 2;

	bigfloat reduced = number - two_pi * floor_div(number, two_pi);

	if (reduced > pi)
		reduced = two_pi - reduced;

	//std::cout << "reduced: " << to_double(reduced) << "\n";
	if (reduced == bigfloat(0) || reduced == pi)
		throw std::invalid_argument("The ctg argument must not be equal to 0 or pi");

	return bigfloat(1.0) / calculate_row(reduced, tg_member, EPS);
}

bigfloat sec(bigfloat const& number, bigfloat const& EPS)
{
	bigfloat pi = bigfloat::PI();
	bigfloat two_pi = pi * 2;

	bigfloat reduced = number - two_pi * floor_div(number + pi, two_pi);
	if (reduced > pi / 2)
		reduced = pi - reduced;
	else if (reduced < -pi / 2)
		reduced = -pi - reduced;

	bigfloat err = abs(reduced / (pi / 2));

	if (err == bigfloat(1.0))
		throw std::invalid_argument("The sec argument must not be equal to pi/2 or 3*pi/2.");

	return bigfloat(1.0) / cos(reduced, EPS);
}

bigfloat cosec(bigfloat const& number, bigfloat const& EPS)
{
	bigfloat pi = bigfloat::PI();
	bigfloat two_pi = pi * 2;

	bigfloat reduced = number - two_pi * floor_div(number, two_pi);

	if (reduced > pi)
		reduced = two_pi - reduced;

	if (reduced == bigfloat(0) || reduced == pi)
		throw std::invalid_argument("The cosec argument must not be equal to 0 or pi");

	return calculate_row(reduced, cosec_member, EPS);
}

bigfloat arcsin(bigfloat const& number, bigfloat const& EPS)
{
	if (abs(number) > bigfloat(1.0))
		throw std::invalid_argument("Abs of argument of arcsin should be less or equel of 1.");

	bigfloat inner_eps = (abs(number) > bigfloat(7, 10)) ? small_eps : EPS;

	return calculate_row(number, arcsin_member, inner_eps);
}

bigfloat arccos(bigfloat const& number, bigfloat const& EPS)
{
	if (abs(number) > bigfloat(1.0))
		throw std::invalid_argument("Abs of argument of arccos should be less or equel of 1.");

	bigfloat inner_eps = (abs(number) > bigfloat(7, 10)) ? small_eps : EPS;

	return  (bigfloat::PI() / 2) - arcsin(number, EPS);
}

bigfloat arctg(bigfloat const& number, bigfloat const& EPS)
{	
	bigfloat new_arg = number;
	bigfloat one(1);
	bigfloat pi = bigfloat::PI();
	bigfloat two_pi = pi * 2;
	bigfloat res;

	//new_arg = number - two_pi * floor_div(number + pi, two_pi);

	if (new_arg > one)
	{
		res = (pi / 2) - calculate_row(one / new_arg, arctg_member, EPS);
	}
	else if (new_arg < -one)
	{
		res = (-pi / 2) + calculate_row(one / abs(new_arg), arctg_member, EPS);
	}
	else if (new_arg > bigfloat(1, 2))
	{
		new_arg = (new_arg - one) / (new_arg + one);
		res = (pi / bigfloat(4)) + calculate_row(new_arg, arctg_member, EPS);
	}
	else if (new_arg < -bigfloat(1, 2))
	{
		new_arg = (new_arg + one) / (one - new_arg);
		res = (-pi / 4) + calculate_row(new_arg, arctg_member, EPS);
	}
	else
	{
		bigfloat inner_eps = (abs(new_arg) > bigfloat(7, 10)) ? small_eps : EPS;
		res = calculate_row(new_arg, arctg_member, inner_eps);
	}
	res.simplify();
	return res;
}

bigfloat arcctg(bigfloat const& number, bigfloat const& EPS)
{
	bigfloat pi = bigfloat::PI();
	bigfloat inner_eps = (abs(number) > bigfloat(7, 10)) ? small_eps : EPS;
	return (bigfloat::PI() / 2) - calculate_row(number, arctg_member, inner_eps);

	if (abs(number) > bigfloat(1))
	{
		return calculate_row(bigfloat(1) / number, arctg_member, inner_eps);
	}
	else
	{
		return (bigfloat::PI() / 2) - calculate_row(number, arctg_member, inner_eps);
	}
}

bigfloat atan2(bigfloat const& y, bigfloat const& x, bigfloat const& EPS)
{
	bigfloat zero(0);
	bigfloat pi = bigfloat::PI();

	if (x > zero)
		return arctg(y / x, EPS);

	if (x < zero && y >= zero)
		return arctg(y / x, EPS) + pi;

	if (x < zero && y < zero)
		return arctg(y / x, EPS) - pi;

	if (x == zero && y > zero)
		return (bigfloat::PI() / 2);

	if (x == zero && y < zero)
		return -(pi / 2);

	return bigfloat(zero);
}

bigfloat pow(bigfloat const& base, int exp)
{
	if (exp < 0)
		return bigfloat(1.0) / pow(base, -exp);

	bigfloat res = 1;
	for (int i = 0; i < exp; ++i)
	{
		res._numerator *= base._numerator;
		res._denominator *= base._denominator;
	}
	return res;
}

//bigfloat radical(bigfloat const& radicand, unsigned int const& index, bigfloat const& EPS) 
//{
//	if (radicand < 0 && index % 2 == 0)
//		throw std::invalid_argument("Cannot extract even root from negative number.");
//	if (index == 0)
//		throw std::invalid_argument("Index can not be zero.");
//
//	bigfloat x = radicand;
//	bigfloat delta;
//
//	do {
//		bigfloat x_prev = x;
//		x = ((index - 1) * x + radicand / pow(x, index - 1)) / index;
//		delta = abs(x - x_prev);
//	} while (delta > EPS);
//
//	x.simplify();
//	return x;
//}

bigfloat radical(bigfloat const& radicand, unsigned int const& index, bigfloat const& EPS)
{
	if (radicand < 0 && index % 2 == 0)
	{
		throw std::invalid_argument("Cannot extract even root from negative number.");
	}
	if (index == 0)
	{
		throw std::invalid_argument("Index cannot be zero.");
	}

	bigfloat x = (radicand >= 0) ? 1.0 : -1.0;
	bigfloat delta;
	int max_iter = 1000;
	int iter = 0;

	do {
		bigfloat x_prev = x;
		bigfloat power = pow(x, index - 1);

		if (abs(power) < EPS)
		{
			x = (radicand >= 0) ? EPS : -EPS;
			power = pow(x, index - 1);
		}

		x = ((index - 1) * x + radicand / power) / index;
		delta = abs(x - x_prev);

		if (++iter > max_iter)
		{
			throw std::runtime_error("Max iterations exceeded. No convergence.");
		}

	} while (delta > EPS);

	x.simplify();
	return x;
}

bigfloat sqrt(bigfloat const& number, bigfloat const& eps)
{
	if (number < bigfloat(0))
	{
		throw std::invalid_argument("Cannot calculate square root of a negative number");
	}
	if (number == bigfloat(0))
	{
		return bigfloat(0);
	}

	double approx_double = (double)(number._numerator).convert_to<double>() / (number._denominator).convert_to<double>();
	if (approx_double <= 0.0)
	{
		throw std::invalid_argument("Cannot calculate square root of non-positive number");
	}
	approx_double = std::sqrt(approx_double);

	bigfloat guess(approx_double);
	if (guess == bigfloat(0)) {
		guess = bigfloat(1);
	}

	bigfloat two(2);
	bigfloat prev_guess;
	int max_iter = 1000000;
	int iter = 0;

	do {
		prev_guess = guess;
		if (guess == bigfloat(0))
		{
			throw std::runtime_error("sqrt: guess became zero, division by zero avoided");
		}
		guess = (guess + (number) / guess) / two;

		bigfloat diff = abs(guess - prev_guess);
		bigfloat rel_diff = diff / abs(guess);
		if (rel_diff < eps && iter > 0) {
			break;
		}

		iter++;
		if (iter > max_iter)
		{
			throw std::runtime_error("sqrt: did not converge within max iterations");
		}
	} while (true);

	return guess;
}

//bigfloat ln(bigfloat const& number, bigfloat const& EPS)
//{
//	if (number <= 0) {
//		throw std::invalid_argument("Value must be positive to clculate logarithm");
//	}
//
//	bigfloat result = 0;
//	bigfloat term = (number - 1) / (number + 1);
//	bigfloat term_squared = term * term;
//	bigfloat current_term = term;
//	bigint n = 1;
//
//	while (abs(current_term) > EPS) {
//		result += current_term / n;
//		current_term *= term_squared;
//		n += 2;
//	}
//	return 2 * result;
//}

bigfloat ln(bigfloat const& number, bigfloat const& EPS)
{
	if (number <= 0)
		throw std::invalid_argument("Value must be positive to calculate logarithm");

	if (number == bigfloat(1))
		return bigfloat(0);

	bigfloat x = number;
	bigfloat log2val = bigfloat(1143, 1649);//ln(2)

	int k = 0;

	while (x > bigfloat(2)) {
		x /= 2;
		k++;
	}
	while (x < bigfloat(0.5)) {
		x *= 2;
		k--;
	}

	bigfloat term = (x - 1) / (x + 1);
	bigfloat term_squared = term * term;
	bigfloat current_term = term;
	bigfloat result = 0;
	bigint n = 1;

	while (abs(current_term) > EPS) {
		result += current_term / n;
		current_term *= term_squared;
		n += 2;
	}

	result *= 2;
	result += k * log2val;
	result.simplify();
	return result;
}


bigfloat log2(bigfloat const& number, bigfloat const& EPS)
{
	bigfloat res = ln(number, EPS) / ln(2, EPS);
	res.simplify();
	return res;
}

bigfloat log10(bigfloat const& number, bigfloat const& EPS)
{
	bigfloat res = ln(number, EPS) / ln(10, EPS);
	res.simplify();
	return res;
}

bigfloat bigfloat::PI()
{
	//std::cout << "calculating pi" << std::endl;
	//static bigfloat pi = calculate_row(bigfloat(1.0), pi_member, bigfloat(1, 1e10));
	static bigfloat pi(3141592653589793238462643383279502884197.0, 1e39);
	return pi;
}

bigint floor_div(bigfloat const& numerator, bigfloat const& denominator)
{
	bigint a_num = numerator._numerator;
	bigint a_den = numerator._denominator;
	bigint b_num = denominator._numerator;
	bigint b_den = denominator._denominator;

	bigint num = a_num * b_den;
	bigint den = a_den * b_num;

	if ((num >= 0 && den > 0) || (num <= 0 && den < 0)) {
		return num / den;
	}
	else {
		return (num - den + 1) / den;
	}
}

double to_double(const bigfloat& value)
{
	if (value._numerator == 0) return 0.0;

	using boost::multiprecision::cpp_int;

	bool negative = value._numerator < 0;

	cpp_int abs_num = abs(value._numerator);
	cpp_int abs_den = abs(value._denominator);

	cpp_int integer_part = abs_num / abs_den;
	cpp_int remainder = abs_num % abs_den;

	double result = integer_part.convert_to<double>();

	if (remainder != 0)
	{
		const int max_fraction_bits = 53;
		cpp_int scaled = (remainder << max_fraction_bits);
		cpp_int frac = scaled / abs_den;

		double fraction = frac.convert_to<double>() / (1ULL << max_fraction_bits);

		result += fraction;
	}

	return negative ? -result : result;
}

char* bigint_to_cstring(const bigint& value)
{
	bigint temp = value;
	size_t digits = 0;
	if (temp == 0) digits = 1;
	else {
		while (temp != 0) {
			temp /= 10;
			++digits;
		}
	}

	char* result = new char[digits + 2];
	result[digits] = '\0';

	temp = value;
	for (size_t i = digits; i > 0; --i)
	{
		bigint digit = temp % 10;
		result[i - 1] = static_cast<char>('0' + static_cast<int>(digit));
		temp /= 10;
	}

	return result;
}

char* to_cstring(const bigfloat& value, size_t precision)
{
	const bigint& num = value._numerator;
	const bigint& den = value._denominator;

	bigint int_part = num / den;
	bigint rem = num % den;

	char* int_str = bigint_to_cstring(int_part);

	size_t int_len = 0;
	while (int_str[int_len] != '\0') ++int_len;

	size_t total_len = int_len + 1 + precision + 1;
	char* result = new char[total_len];

	for (size_t i = 0; i < int_len; ++i)
		result[i] = int_str[i];

	delete[] int_str;

	result[int_len] = '.';

	bigint r = rem;
	for (size_t i = 0; i < precision; ++i)
	{
		r *= 10;
		bigint digit = r / den;
		r %= den;
		result[int_len + 1 + i] = static_cast<char>('0' + static_cast<int>(digit));
	}

	result[total_len - 1] = '\0';
	return result;
}
