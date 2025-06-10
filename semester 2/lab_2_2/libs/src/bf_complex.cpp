#include <ostream>
#include "../bf_complex.h"

bf_complex::bf_complex(bigfloat integer_part, bigfloat imaginary_part) :
	_real_part(integer_part), 
	_imaginary_part(imaginary_part)
{};

bf_complex::bf_complex(
	double integer_numerator, double integer_denominator,
	double imaginary_numerator, double imaginary_denominator) :
	_real_part(bigfloat(integer_numerator, integer_denominator)),
	_imaginary_part(bigfloat(imaginary_numerator, imaginary_denominator))
{};

bf_complex& bf_complex::operator +=(bf_complex const& arg)
{
	this->_real_part += arg._real_part;
	this->_imaginary_part += arg._imaginary_part;
	return *this;
}

bf_complex& bf_complex::operator -=(bf_complex const& arg)
{
	this->_real_part -= arg._real_part;
	this->_imaginary_part -= arg._imaginary_part;
	return *this;
}

bf_complex& bf_complex::operator *=(bf_complex const& arg)
{
	//TODO: clear code
	bigfloat temp_real_part = (this->_real_part * arg._real_part) + ((this->_imaginary_part * arg._imaginary_part) * -1);
	bigfloat temp_imaginary_part = (this->_real_part * arg._imaginary_part) + (this->_imaginary_part * arg._real_part);
	this->_real_part = temp_real_part;
	this->_imaginary_part = temp_imaginary_part;
	return *this;
}

bf_complex& bf_complex::operator /=(bf_complex const& arg)
{
	if ((arg._real_part * arg._real_part + arg._imaginary_part * arg._imaginary_part) == 0 ||
		(arg._real_part * arg._real_part + arg._imaginary_part * arg._imaginary_part) == 0)
	{
		throw std::overflow_error("Division by zero exception");
	}

	bigfloat temp_real_part = (this->_real_part * arg._real_part + this->_imaginary_part * arg._imaginary_part) /
		(arg._real_part * arg._real_part + arg._imaginary_part * arg._imaginary_part);

	bigfloat temp_imaginary_part = (arg._real_part * this->_imaginary_part - this->_real_part * arg._imaginary_part) /
		(arg._real_part * arg._real_part + arg._imaginary_part * arg._imaginary_part);

	this->_real_part = temp_real_part;
	this->_imaginary_part = temp_imaginary_part;

	return *this;
}

bf_complex bf_complex::operator +(bf_complex const& arg) const
{
	bf_complex copy(this->_real_part, this->_imaginary_part);
	return copy += arg;
}

bf_complex bf_complex::operator -(bf_complex const& arg) const
{
	bf_complex copy(this->_real_part, this->_imaginary_part);
	return copy -= arg;
}

bf_complex bf_complex::operator *(bf_complex const& arg) const
{
	bf_complex copy(this->_real_part, this->_imaginary_part);
	return copy *= arg;
}

bf_complex bf_complex::operator /(bf_complex const& arg) const
{
	bf_complex copy(this->_real_part, this->_imaginary_part);
	return copy /= arg;
}

bigfloat bf_complex::get_mod() const
{
	return radical(pow(this->_real_part, 2.0) + pow(this->_imaginary_part, 2.0), 2, bigfloat(1, 1e5));
}

bigfloat bf_complex::get_arg(bigfloat eps) const
{
	if (eps < 0)
		throw std::invalid_argument("Epsilon must be bigger than 0");
	
	return atan2(this->_imaginary_part, this->_real_part, eps);
}

std::ostream& operator <<(std::ostream& os, bf_complex const& arg)
{
	if (arg._real_part != 0)
	{
		os << arg._real_part;
	}
		
	if (arg._imaginary_part != 0)
	{
		if (arg._real_part != 0) os << ((arg._imaginary_part < 0) ? "" : "+");
		os << arg._imaginary_part << 'i';
	}

	return os;
}

bigfloat collect_value(std::istream& stream)
{
	int flag = 1;
	bool first_ch_flag = true;
	bigint temp_bigint_1 = 0, temp_bigint_2 = 0;
	while (true)
	{
		char cur_ch = stream.peek();
		std::cout << cur_ch;
		if (std::isdigit(cur_ch))
		{
			if (flag == 1)
			{
				if (!first_ch_flag) temp_bigint_1 *= 10;
				temp_bigint_1 += (cur_ch - '0');
				first_ch_flag = false;
			}
			else
			{
				if (!first_ch_flag) temp_bigint_2 *= 10;
				temp_bigint_2 += (cur_ch - '0');
				first_ch_flag = false;
			}
			stream.get();
		}
		else if (cur_ch == '/' && flag == 1)
		{
			++flag;
			first_ch_flag = true;
			stream.get();
		}
		else
		{
			break;
		}
	}
	bigfloat result(temp_bigint_1, temp_bigint_2);
	return result;
}

std::istream& operator>>(
	std::istream& stream, 
	bf_complex& value)
{
	bigint temp_bigint_1, temp_bigint_2;
	bigfloat real_part, imaginary_part;
	int sign = 1;
	bool end_line_flag = false;

	// 7/6 + 3/4i without spaces(1) spaces(1)
	// 7 + 3/4i   (1)(1)
	// 7/6 + 3i   (1)(1)
	// 7 + 3i     (1)(1)
	//
	// 7    	  (1) 
	// 3i    	  (1) 
	// 7/6		  (1) 
	// 3/4i		  (1) 

	// skip spaces
	while (isspace(stream.peek())) stream.get();

	// processing first sign
	if (stream.peek() == '-')
	{
		sign = -1;
		stream.get();
	}

	// collect first ratio
	real_part = bigfloat(sign, 1) * collect_value(stream);
	//std::cout << real_part << " ";

	// skip spaces and check for single ratio
	if (stream.peek() == 'i') end_line_flag = true;
	while (!end_line_flag && isspace(stream.peek()))
	{
		if (stream.peek() == 10)
		{
			end_line_flag = true;
			break;
		}
		stream.get();
	}

	// if we have both ratio
	if (!end_line_flag)
	{
		// processing second sign
		if (stream.peek() == '-')
		{
			sign = -1;
		}
		else
		{
			sign = 1;
		}
		stream.get();

		// skip spaces
		while (isspace(stream.peek())) stream.get();

		// collect second ratio
		imaginary_part = sign * collect_value(stream);
		//std::cout << real_part << " ";
	}
	else
	{
		if (stream.peek() == 'i')
		{
			imaginary_part = real_part;
			real_part = bigfloat(0);
		}
		else
		{
			imaginary_part = bigfloat(0);
		}
	}	

	value = bf_complex(real_part, imaginary_part);
	return stream;
}