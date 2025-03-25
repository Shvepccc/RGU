#include "../libs/bigint.h"

class bigfloat
{
private:
	bigint _nominator;
	bigint _denominator;

public:
	bigfloat();
	~bigfloat();

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
	friend bigfloat arcsec(bigfloat const& number, bigfloat const& EPS);
	friend bigfloat arccosec(bigfloat const& number, bigfloat const& EPS);

	friend bigfloat pow(bigfloat const& base, bigint const& exp);
	friend bigfloat radical(bigfloat const& radicand, bigint const& index, bigfloat const& EPS);
	friend bigfloat log2(bigfloat const& number, bigfloat const& EPS);
	friend bigfloat ln(bigfloat const& number, bigfloat const& EPS);
	friend bigfloat log10(bigfloat const& number, bigfloat const& EPS);

	//static bigfloat PI(bigfloat const& EPS);
};