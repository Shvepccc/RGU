#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>

class complex_number
{
private:
	double _real_part;
	double _imaginary_part;

public:
	complex_number(double integer_part = 0.0, double imaginary_part = 0.0) 
	{ 
		_real_part = integer_part;
		_imaginary_part = imaginary_part;
	};

	complex_number& operator +=(complex_number const& arg)
	{
		this->_real_part += arg._real_part;
		this->_imaginary_part += arg._imaginary_part;
		return *this;
	}

	complex_number& operator -=(complex_number const& arg)
	{
		this->_real_part -= arg._real_part;
		this->_imaginary_part -= arg._imaginary_part;
		return *this;
	}

	complex_number& operator *=(complex_number const& arg)
	{
		double temp_real_part = (this->_real_part * arg._real_part) + ((this->_imaginary_part * arg._imaginary_part) * -1);
		double temp_imaginary_part = (this->_real_part * arg._imaginary_part) + (this->_imaginary_part * arg._real_part);
		this->_real_part = temp_real_part;
		this->_imaginary_part = temp_imaginary_part;
		return *this;
	}

	complex_number& operator /=(complex_number const& arg)
	{
		if ((arg._real_part * arg._real_part + arg._imaginary_part * arg._imaginary_part) == 0 ||
			(arg._real_part * arg._real_part + arg._imaginary_part * arg._imaginary_part) == 0)
		{
			throw std::overflow_error("Division by zero exception");
		}

		double temp_real_part = (this->_real_part * arg._real_part + this->_imaginary_part * arg._imaginary_part) /
			(arg._real_part * arg._real_part + arg._imaginary_part * arg._imaginary_part);

		double temp_imaginary_part = (arg._real_part * this->_imaginary_part - this->_real_part * arg._imaginary_part) /
			(arg._real_part * arg._real_part + arg._imaginary_part * arg._imaginary_part);

		this->_real_part = temp_real_part;
		this->_imaginary_part = temp_imaginary_part;

		return *this;
	}

	complex_number operator +(complex_number const& arg) const
	{
		complex_number copy(this->_real_part, this->_imaginary_part);
		return copy += arg;
	} 

	complex_number operator -(complex_number const& arg) const
	{
		complex_number copy(this->_real_part, this->_imaginary_part);
		return copy -= arg;
	}

	complex_number operator *(complex_number const& arg) const
	{
		complex_number copy(this->_real_part, this->_imaginary_part);
		return copy *= arg;
	}

	complex_number operator /(complex_number const& arg) const
	{
		complex_number copy(this->_real_part, this->_imaginary_part);
		return copy /= arg;
	}

	double get_mod() const
	{
		return sqrt(pow(this->_real_part, 2.0) + pow(this->_imaginary_part, 2.0));
	}

	double get_arg(double eps = 1e-10) const
	{
		if (eps < 0) 
		{
			throw std::invalid_argument("Epsilon must be bigger than 0");
		}

		if (this->_real_part < eps) 
		{
			return 0.0;
		}
		return atan2(this->_imaginary_part, this->_real_part);		
	}

	friend std::ostream& operator <<(std::ostream& os, complex_number const& arg) 
	{
		return os << arg._real_part << (arg._imaginary_part < 0 ? "" : "+") << arg._imaginary_part << 'i';
	}

	friend std::istream& operator >>(std::istream& os, complex_number & arg)
	{
		return os >> arg._real_part >> arg._imaginary_part;
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