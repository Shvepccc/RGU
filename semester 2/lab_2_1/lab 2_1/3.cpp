#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>

class complex_number
{
private:
	double _integer_part;
	double _imaginary_part;

public:
	complex_number(double integer_part = 0.0, double imaginary_part = 0.0) 
	{ 
		_integer_part = integer_part;
		_imaginary_part = imaginary_part;
	};

	complex_number operator +(complex_number const& arg) const
	{
		return complex_number(this->_integer_part + arg._integer_part, 
			this->_imaginary_part + arg._imaginary_part);
	}

	complex_number operator -(complex_number const& arg) const
	{
		return complex_number(this->_integer_part - arg._integer_part,
			this->_imaginary_part - arg._imaginary_part);
	}

	
	complex_number operator *(complex_number const& arg) const
	{
		return complex_number((this->_integer_part * arg._integer_part) + ((this->_imaginary_part * arg._imaginary_part) * -1),
			(this->_integer_part * arg._imaginary_part) + (this->_imaginary_part * arg._integer_part));
	}

	complex_number operator /(complex_number const& arg) const
	{
		double temp_integer_part = (this->_integer_part * arg._integer_part + this->_imaginary_part * arg._imaginary_part) /
			(arg._integer_part * arg._integer_part + arg._imaginary_part * arg._imaginary_part);

		double temp_imaginary_part = (arg._integer_part * this->_imaginary_part - this->_integer_part * arg._imaginary_part) / 
			(arg._integer_part * arg._integer_part + arg._imaginary_part * arg._imaginary_part);

		return complex_number(temp_integer_part, temp_imaginary_part);
	}

	complex_number operator +=(complex_number const& arg)
	{
		this->_integer_part += arg._integer_part;
		this->_imaginary_part += arg._imaginary_part;
		return *this;
	}

	complex_number operator -=(complex_number const& arg)
	{
		this->_integer_part -= arg._integer_part;
		this->_imaginary_part -= arg._imaginary_part;
		return *this;
	}

	complex_number operator *=(complex_number const& arg)
	{
		this->_integer_part = (this->_integer_part * arg._integer_part) + ((this->_imaginary_part * arg._imaginary_part) * -1);
		this->_imaginary_part = (this->_integer_part * arg._imaginary_part) + (this->_imaginary_part * arg._integer_part);
		return *this;
	}

	complex_number operator /=(complex_number const& arg)
	{
		double temp_integer_part = (this->_integer_part * arg._integer_part + this->_imaginary_part * arg._imaginary_part) /
			(arg._integer_part * arg._integer_part + arg._imaginary_part * arg._imaginary_part);

		double temp_imaginary_part = (arg._integer_part * this->_imaginary_part - this->_integer_part * arg._imaginary_part) /
			(arg._integer_part * arg._integer_part + arg._imaginary_part * arg._imaginary_part);

		this->_integer_part = temp_integer_part;
		this->_imaginary_part = temp_imaginary_part;

		return *this;
	}

	double get_mod() const
	{
		return sqrt(pow(this->_integer_part, 2.0) + pow(this->_imaginary_part, 2.0));
	}

	double get_arg() const
	{
		if (this->_integer_part > 0 && this->_imaginary_part >= 0) 
		{
			return atan(this->_imaginary_part/this->_integer_part);
		}
		else if (this->_integer_part < 0 && this->_imaginary_part >= 0)
		{
			return M_PI - atan(abs(this->_imaginary_part / this->_integer_part));
		}
		else if (this->_integer_part < 0 && this->_imaginary_part < 0)
		{
			return M_PI + atan(abs(this->_imaginary_part / this->_integer_part));
		}
		else if (this->_integer_part > 0 && this->_imaginary_part < 0)
		{
			return 2*M_PI - atan(abs(this->_imaginary_part / this->_integer_part));
		}
		else if (this->_integer_part == 0 && this->_imaginary_part > 0)
		{
			return M_PI_2;
		}
		else if (this->_integer_part == 0 && this->_imaginary_part < 0)
		{
			return (3*M_PI)/2;
		}
	}

	friend std::ostream& operator <<(std::ostream& os, complex_number const& arg) 
	{
		return os << arg._integer_part << (arg._imaginary_part < 0 ? "" : "+") << arg._imaginary_part << 'i';
	}

	friend std::istream& operator >>(std::istream& os, complex_number & arg)
	{
		return os >> arg._integer_part >> arg._imaginary_part;
	}

};

int program_3_main(int argc, char* argv[])
{

	complex_number a(1.0, 2.0);
	complex_number b(3.0, 4.0);
	complex_number d(1.0, -1.0);
	complex_number e(3.0, 6.0);
	complex_number g = d * e;
	g /= e;
	std::cout << "g = " << g << "\n";

	complex_number c = a - b + b;
	c += e;

	std::cout << "c = " << c << "\n";
	std::cout << "Mod of 'c' = " << c.get_mod() << "\n";
	std::cout << "Arg of 'c' = " << c.get_arg() << "\n";

	complex_number read_test;
	std::cout << "Enter complex value: ";
	std::cin >> read_test;
	std::cout << "Reasult of read coplex value: " << read_test << "\n";
	std::cout << std::endl;
	
	return 0;
}