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
	bigint divider = gcd(abs(_numerator), abs(_denominator));
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

bigint bigfloat::gcd(bigint a, bigint b) //TODO: relocate it to bigint.cpp
{
	if (a == 0) return b;
	if (b == 0) return a;
	
	int az = ctz(a);
	int bz = ctz(b);
	int shift = (az < bz) ? az : bz;
	a >>= az, b >>= bz;
	
	while (a != b) {
		if (a > b) {
			a = (a - b) >> ctz(a - b);
		}
		else {
			b = (b - a) >> ctz(b - a);
		}
	}
	
	return b << shift;
	return 0;
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
	//std::cout << " (" << n << ")\n";
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
	return pow(-1, n) * num / denom;
}

bigfloat pi_member(bigfloat const& x, unsigned int n)
{
	--n;
	if (n == 0)
		return bigfloat(1.0);

	return bigfloat(pow(-1, n), 2 * n + 1);
}

bigfloat sin(bigfloat const& number, bigfloat const& EPS)
{
	bigfloat two_pi = bigfloat::PI() * 2;
	bigfloat reduced = number - two_pi * floor_div(number, two_pi);
	//std::cout << reduced << "\n";
	return calculate_row(reduced, sin_member, EPS);
}

bigfloat cos(bigfloat const& number, bigfloat const& EPS) {
	return calculate_row(number, cos_member, EPS);
}

bigfloat tg(bigfloat const& number, bigfloat const& EPS) {
	if (abs(number) > bigfloat(1.57075))
		throw std::invalid_argument("Argument of abs(tg) should be less than Pi/2.");
	return calculate_row(number, tg_member, EPS);
}

bigfloat ctg(bigfloat const& number, bigfloat const& EPS) {
	if (abs(number) > bigfloat(1.57075) || abs(number) < 0)
		throw std::invalid_argument("Argument of ctg should be less than Pi/2 and more than 0.");
	return bigfloat(1.0) / calculate_row(number, tg_member, EPS);
}

bigfloat sec(bigfloat const& number, bigfloat const& EPS) {
	if (abs(number) > bigfloat(1.57075))
		throw std::invalid_argument("Argument of abs(tg) should be less than Pi/2.");
	return bigfloat(1.0) / cos(number, EPS);
}

bigfloat cosec(bigfloat const& number, bigfloat const& EPS) {
	if (abs(number) > bigfloat(1.57075) || abs(number) < 0)
		throw std::invalid_argument("Argument of ctg should be less than Pi/2 and more than 0.");
	return calculate_row(number, cosec_member, EPS);
}

bigfloat arcsin(bigfloat const& number, bigfloat const& EPS) {
	if (abs(number) > bigfloat(1.0))
		throw std::invalid_argument("Abs of argument of arcsin should be less or equel of 1.");
	bigfloat inner_eps = (number > bigfloat(8, 10)) ? small_eps : EPS;
	return calculate_row(number, arcsin_member, inner_eps);
}

bigfloat arccos(bigfloat const& number, bigfloat const& EPS) {
	if (abs(number) > bigfloat(1.0))
		throw std::invalid_argument("Abs of argument of arccos should be less or equel of 1.");
	bigfloat inner_eps = (number > bigfloat(8, 10)) ? small_eps : EPS;
	return  (bigfloat::PI() / 2) - arcsin(number, EPS);
}

bigfloat arctg(bigfloat const& number, bigfloat const& EPS) {
	if (abs(number) > bigfloat(1.0))
		throw std::invalid_argument("Abs of argument of arctg should be less or equel of 1.");

	bigfloat inner_eps = (number > bigfloat(8, 10)) ? small_eps : EPS;
	return calculate_row(number, arctg_member, inner_eps);
}

bigfloat arcctg(bigfloat const& number, bigfloat const& EPS) {
	if (abs(number) > bigfloat(1.0))
		throw std::invalid_argument("Abs of argument of arcctg should be less or equel of 1.");
	bigfloat inner_eps = (number > bigfloat(8, 10)) ? small_eps : EPS;
	return (bigfloat::PI() / 2) - calculate_row(number, arctg_member, inner_eps);
}

bigfloat atan2(bigfloat const& y, bigfloat const& x, bigfloat const& EPS)
{
	if (x > 0)
		return arctg(y / x, EPS);

	if (x < 0 && y >= 0)
		return arctg(y / x, EPS) + bigfloat::PI();

	if (x < 0 && y < 0)
		return arctg(y / x, EPS) - bigfloat::PI();

	if (x == 0 && y > 0)
		return (bigfloat::PI() / 2);

	if (x == 0 && y < 0)
		return -(bigfloat::PI() / 2);

	return bigfloat(0);
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

bigfloat radical(bigfloat const& radicand, unsigned int const& index, bigfloat const& EPS) 
{
	if (radicand < 0 && index % 2 == 0)
		throw std::invalid_argument("Cannot extract even root from negative number.");
	if (index == 0)
		throw std::invalid_argument("Index can not be zero.");

	bigfloat x = radicand;
	bigfloat delta;

	do {
		bigfloat x_prev = x;
		x = ((index - 1) * x + radicand / pow(x, index - 1)) / index;
		delta = abs(x - x_prev);
	} while (delta > EPS);

	x.simplify();
	return x;
}

bigfloat ln(bigfloat const& number, bigfloat const& EPS)
{
	if (number <= 0) {
		throw std::invalid_argument("Value must be positive to clculate logarithm");
	}

	bigfloat result = 0;
	bigfloat term = (number - 1) / (number + 1);
	bigfloat term_squared = term * term;
	bigfloat current_term = term;
	bigint n = 1;

	while (abs(current_term) > EPS) {
		result += current_term / n;
		current_term *= term_squared;
		n += 2;
	}
	return 2 * result;
}

bigfloat log2(bigfloat const& number, bigfloat const& EPS) {
	return ln(number, EPS) / ln(2, EPS);
}

bigfloat log10(bigfloat const& number, bigfloat const& EPS) {
	return ln(number, EPS) / ln(10, EPS);
}

bigfloat bigfloat::PI() {
	static bigfloat pi = 4 * calculate_row(bigfloat(1.0), pi_member, bigfloat(1, 600));
	return pi;
}

bigint floor_div(bigfloat const& numerator, bigfloat const& denominator)
{
	// a/b = (a_n / a_d) / (b_n / b_d) = (a_n * b_d) / (a_d * b_n)
	bigint a_num = numerator._numerator;
	bigint a_den = numerator._denominator;
	bigint b_num = denominator._numerator;
	bigint b_den = denominator._denominator;

	// Реальное деление дробей: (a_num * b_den) / (a_den * b_num)
	bigint num = a_num * b_den;
	bigint den = a_den * b_num;

	// Выполняем целочисленное деление с округлением вниз
	if ((num >= 0 && den > 0) || (num <= 0 && den < 0)) {
		// положительный результат — обычное деление
		return num / den;
	}
	else {
		// отрицательный результат — округляем вниз
		return (num - den + 1) / den;
	}
}

//double to_double(const bigfloat& value)
//{
//	double num = value._numerator.convert_to<double>();
//	double den = value._denominator.convert_to<double>();
//
//	return num / den;
//}

double to_double(const bigfloat& value)
{
	// Быстрый выход: если числитель ноль, сразу возвращаем 0.0
	if (value._numerator == 0) return 0.0;

	using boost::multiprecision::cpp_int;

	// Получаем знак
	bool negative = value._numerator < 0;

	cpp_int abs_num = abs(value._numerator);
	cpp_int abs_den = abs(value._denominator);

	// Целая часть от деления
	cpp_int integer_part = abs_num / abs_den;
	cpp_int remainder = abs_num % abs_den;

	// Преобразуем целую часть
	double result = integer_part.convert_to<double>();

	// Если есть остаток — добавим дробную часть
	if (remainder != 0)
	{
		// Для вычисления дробной части нужно ограничить количество точных бит
		// Преобразуем остаток / знаменатель в double вручную

		// Используем 53 бита (максимальная точность double)
		const int max_fraction_bits = 53;
		cpp_int scaled = (remainder << max_fraction_bits); // * 2^53
		cpp_int frac = scaled / abs_den;

		double fraction = frac.convert_to<double>() / (1ULL << max_fraction_bits);

		result += fraction;
	}

	return negative ? -result : result;
}
