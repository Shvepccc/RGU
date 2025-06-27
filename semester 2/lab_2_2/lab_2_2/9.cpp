#include <iostream>
#include <fstream>
#include <boost/multiprecision/cpp_int.hpp>

#include "../libs/bigfloat.h"

bigfloat defaultEPS(bigint(1), bigint(1e100));

bool compare_file_with_cstring(const char* filename, const char* str)
{
	std::ifstream file(filename, std::ios::in);
	if (!file.is_open()) return false;

	char ch_file;
	size_t i = 0;

	std::cout << "\nComparing...\n";
	while (file.get(ch_file))
	{
		if (str[i] == '\0') return false;
		if (ch_file != str[i])
		{
			std::cout << i << " ";
			return false; 
		}
		++i;
		if (i % 500 == 0)
		{
			std::cout << i << " ";
		}
	}
	std::cout << "\n";
	return true;
}

bigfloat calculate_row(
	bigfloat(*calc_member_function)(unsigned int, bigfloat&),
	bigfloat eps)
{
	bigfloat res;
	bigfloat temp;
	bigfloat storage;
	unsigned int n = 0;
	std::cout << "Iteration: ";
	while (temp > eps || n == 0)
	{
		temp = calc_member_function(n, storage);
		res += temp;
		++n;
		if (n % 500 == 0)
		{
			std::cout << n << " ";
		}
	}
	std::cout << "\n";
	return res;
}

#pragma region way 1

bigfloat Beily_Boruen_Plaff_member(unsigned int n, bigfloat& storage)
{
	return bigfloat(1, bigint(1) << (4 * n)) *
		(bigfloat(4, 8 * n + 1) - bigfloat(2, 8 * n + 4)
		- bigfloat(1, 8 * n + 5) - bigfloat(1, 8 * n + 6));
}

bigfloat Beily_Boruen_Plaff(bigfloat eps = defaultEPS)
{
	return calculate_row(Beily_Boruen_Plaff_member, eps) * bigfloat(bigint(1e10), 1);
}

#pragma endregion

#pragma region way 2

bigfloat Bellar_member(unsigned int n, bigfloat& storage)
{
	return bigfloat(bigint((n % 2 == 0) ? 1 : -1), bigint(1) << (10 * n)) * 
		(-bigfloat(32, 4 * n + 1)
		- bigfloat(1, 4 * n + 3)
		+ bigfloat(256, 10 * n + 1)
		- bigfloat(64, 10 * n + 3)
		- bigfloat(4, 10 * n + 5)
		- bigfloat(4, 10 * n + 7)
		+ bigfloat(1, 10 * n + 9));
}

bigfloat Bellar(bigfloat eps = defaultEPS)
{
	return bigfloat(1, 64) * calculate_row(Bellar_member, defaultEPS) * bigfloat(bigint(1e20), 1);
}

#pragma endregion

#pragma region way 3

struct Chudnovsky_State {
	bigint fact_k = 1; 
	bigint fact_3k = 1;
	bigint fact_6k = 1;
	bigint Ck = 1;     
	int k = 0;
};

bigfloat Chudnovsky_member(Chudnovsky_State& state)
{
	static const bigint C = pow(bigint(640320), 3);
	static const bigint A = 13591409;
	static const bigint B = 545140134;

	// Update factorials
	if (state.k > 0)
	{
		int k = state.k;

		for (int i = 6 * (k - 1) + 1; i <= 6 * k; ++i)
			state.fact_6k *= i;

		for (int i = 3 * (k - 1) + 1; i <= 3 * k; ++i)
			state.fact_3k *= i;

		state.fact_k *= k;

		state.Ck *= C;
	}

	bigfloat term = bigfloat(
		((state.k % 2 == 0) ? 1 : -1) * state.fact_6k * (A + B * state.k),
		state.fact_3k * pow(state.fact_k, 3) * state.Ck);

	state.k += 1;
	return term;
}

bigfloat Chudnovsky(bigfloat eps = defaultEPS)
{
	Chudnovsky_State state;
	bigfloat sum = 0;

	std::cout << "Iteration: ";
	bigfloat temp;
	int i = 0;
	while(temp > eps || i == 0)
	{
		temp = Chudnovsky_member(state);
		sum += temp;
		if (i % 250 == 0)
		{
			std::cout << i << " ";
		}
		++i;
	}
	std::cout << "\n";

	bigfloat coeff = bigfloat(1, 426880 * sqrt(10005));

	return pow(coeff * sum, -1) * bigfloat(1, bigint(1e10));
}

#pragma endregion

#pragma region way 4

struct Ramanudjan_State {
	bigint fact_k = 1;
	bigint fact_4k = 1;
	bigint Ck = 1;
	int k = 0;
};

bigfloat Ramanudjan_member(Ramanudjan_State& state)
{
	static const bigint A = 1103;
	static const bigint B = 26390;
	static const bigint C = pow(bigint(396), 4);

	// Update factorials
	if (state.k > 0)
	{
		int k = state.k;

		for (int i = 4 * (k - 1) + 1; i <= 4 * k; ++i)
			state.fact_4k *= i;

		state.fact_k *= k;

		state.Ck *= C;
	}

	bigfloat term = bigfloat(
		state.fact_4k * (A + B * state.k),
		pow(state.fact_k, 4) * state.Ck);

	state.k += 1;
	return term;
}

bigfloat Ramanudjan(bigfloat eps = defaultEPS)
{
	Ramanudjan_State state;
	bigfloat sum = 0;

	std::cout << "Iteration: ";
	int i = 0;
	bigfloat temp;
	while (temp > eps || i == 0)
	{
		sum += Ramanudjan_member(state);
		if (i % 250 == 0)
		{
			std::cout << i << " ";
		}
		++i;
	}
	std::cout << "\n";

	bigfloat coeff = sqrt(bigfloat(2)) * 2 / 9801;

	return pow(coeff * sum, -1);
}

#pragma endregion


int program_9_main(int argc, char* argv[])
{
	char* my_res;
	int count_of_numbers = 200;
	
	//way 1
	my_res = to_cstring(Beily_Boruen_Plaff(), count_of_numbers);
	std::cout << "Beily_Boruen_Plaff: " << my_res << "\n\n";

	//way 2
	my_res = to_cstring(Bellar(), count_of_numbers);
	std::cout << "Bellar: " << my_res << "\n\n";
	
	//way 3
	my_res = to_cstring(Chudnovsky(), count_of_numbers);
	std::cout << "Chudnovsky: " << my_res << "\n\n";

	//way 4
	my_res = to_cstring(Ramanudjan(), count_of_numbers);
	std::cout << "Ramanudjan: " << my_res << "\n\n";

    return 0;
}

//std::cout << compare_file_with_cstring("pi_500000.txt", my_res) << std::endl;