#include "../bigfloat.h"

bigfloat::bigfloat() {}
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

bigfloat& bigfloat::operator+= (bigfloat const& arg)
{
	bigint new_nominator = arg._nominator * this->_denominator;
	this->_denominator *= arg._denominator;
	this->_nominator *= arg._denominator;
	this->_nominator += new_nominator;
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
	return *this + -arg;
}

bigfloat& bigfloat::operator*= (bigfloat const& arg)
{
	this->_nominator *= arg._nominator;
	this->_denominator *= arg._denominator;
	//simplify();
	return *this;
}

bigfloat& bigfloat::operator/= (bigfloat const& arg) {}


bigfloat bigfloat::operator* (bigfloat const& arg) const {}
bigfloat bigfloat::operator/ (bigfloat const& arg) const {}

bool bigfloat::operator== (bigfloat const& arg) const {}
bool bigfloat::operator!= (bigfloat const& arg) const {}

bool bigfloat::operator<  (bigfloat const& other) const {}
bool bigfloat::operator<= (bigfloat const& other) const {}
bool bigfloat::operator>  (bigfloat const& other) const {}
bool bigfloat::operator>= (bigfloat const& other) const {}

std::ostream& operator<<(
	std::ostream& stream,
	bigfloat const& value)
{ }

std::istream& operator>>(
	std::istream& stream,
	bigfloat& value)
{ }

bigfloat sin(bigfloat const& number, bigfloat const& EPS) {}
bigfloat cos(bigfloat const& number, bigfloat const& EPS) {}
bigfloat tg(bigfloat const& number, bigfloat const& EPS) {}
bigfloat ctg(bigfloat const& number, bigfloat const& EPS) {}
bigfloat sec(bigfloat const& number, bigfloat const& EPS) {}
bigfloat cosec(bigfloat const& number, bigfloat const& EPS) {}
bigfloat arcsin(bigfloat const& number, bigfloat const& EPS) {}
bigfloat arccos(bigfloat const& number, bigfloat const& EPS) {}
bigfloat arctg(bigfloat const& number, bigfloat const& EPS) {}
bigfloat arcctg(bigfloat const& number, bigfloat const& EPS) {}
bigfloat arcsec(bigfloat const& number, bigfloat const& EPS) {}
bigfloat arccosec(bigfloat const& number, bigfloat const& EPS) {}

bigfloat pow(bigfloat const& base, bigint const& exp) {}
bigfloat radical(bigfloat const& radicand, bigint const& index, bigfloat const& EPS) {}
bigfloat log2(bigfloat const& number, bigfloat const& EPS) {}
bigfloat ln(bigfloat const& number, bigfloat const& EPS) {}
bigfloat log10(bigfloat const& number, bigfloat const& EPS) {}
