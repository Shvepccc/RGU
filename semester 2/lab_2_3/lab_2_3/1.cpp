#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "simulate_elevators.h"

int program_1_main(int argc, char* argv[])
{
    int n, k;
    std::vector<int> limits;
    read_settings_from_file(n , k, limits, "task_1_data_2.txt");
    std::vector<passenger> passengers = read_passengers_from_file("task_1_data_1.txt");

    print_parse_data(passengers, limits, 18, 3);

	return 0;
}