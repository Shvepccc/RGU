#include <iostream>

class  logical_values_array
{
private:
	unsigned int _value;

public:
	logical_values_array(unsigned int val = 0) :_value(val) {}

	void print_value() const
	{
		std::cout << _value << std::endl;
	}

	static bool equals(logical_values_array const& arg_1, logical_values_array const & arg_2)
	{
		return arg_1._value == arg_2._value;
	}

	int get_bit(int n) const
	{
		if (n < 0 || n >(sizeof(unsigned int) * 8)) 
		{
			throw std::invalid_argument("The bit number in 'get_bit' function or in '[]' is too large");
		}
		return (_value >> n) & 1;
	}

	int operator [](int i) const { return get_bit(i); }

	void get_binary(char* str) const
	{
		if (str == nullptr)
		{
			throw std::invalid_argument("Null pointer");
		}

		int i;
		int size = (8 * sizeof(unsigned int));
		unsigned int temp_uint = _value;
		for (i = 0; i < size; ++i) {
			str[size - i-1] = (temp_uint & 1) + '0';
			temp_uint = temp_uint >> 1;
		}
		str[size] = 0;
	}
	
	//not
	logical_values_array lva_not () const
	{
		return logical_values_array{ ~_value };
	}

	//conjunction
	logical_values_array lva_and (logical_values_array const& arg) const
	{
		return logical_values_array{ _value & arg._value };
	}

	//disjunction
	logical_values_array lva_or (logical_values_array const& arg) const
	{
		return logical_values_array{ _value | arg._value };
	}

	//implication
	logical_values_array lva_imp(logical_values_array const& arg) const
	{
		return logical_values_array{ ~_value | arg._value };
	}

	//coimplication
	logical_values_array lva_coimp(logical_values_array const& arg) const
	{
		return logical_values_array{ _value | ~arg._value };
	}

	//xor
	logical_values_array lva_xor(logical_values_array const& arg) const
	{
		return logical_values_array{ _value ^ arg._value };
	}

	//equivalence
	logical_values_array lva_equl(logical_values_array const& arg) const
	{
		return ~(_value ^ arg._value);
	}

	//arrow of the Pier
	logical_values_array lva_Pier(logical_values_array const& arg) const
	{
		return logical_values_array{ ~(_value | arg._value) };
	}

	//Schaeffer's stroke
	logical_values_array lva_Shef(logical_values_array const& arg) const
	{
		return logical_values_array{ ~(_value & arg._value) };
	}
};



int program_2_main(int argc, char* argv[]) 
{
	logical_values_array a(1);
	logical_values_array b(0);
	logical_values_array c(1);
	logical_values_array d(1);
	logical_values_array e(1);
	logical_values_array A(5);
	logical_values_array res = (d.lva_or(c.lva_not())).lva_coimp(a.lva_and(b));

	std::cout << "Result of expression: ";
	res.print_value();

	try
	{
		std::cout << "Result of A.get_bit(0) [A = 5] = " << A.get_bit(0) << "\n";
		std::cout << "Result of A[1] [A = 5] = " << A[1] << "\n";

		char* str = new char[sizeof(unsigned int) * 8 + 1];
		A.get_binary(str);
		std::cout << "Result A.get_binary(): " << str << "\n";
		std::cout << std::endl;
		delete[] str;
	}
	catch (const std::exception&)
	{
		std::cout << "Oh! You have some error... x_x" << std::endl;
	}

    return 0;
}