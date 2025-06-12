#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

#include "passenger.h"
#include "elevator.h"
#include "simulate_elevators.h"

#include <ctime>

int program_1_main(int argc, char* argv[])
{
	//std::tm t{};
	//std::tm v{};
	//
	//for (int i = 0; i < 90; ++i)
	//{
	//	//t.tm_min += 1;
	//	add_minutes(t, 1);
	//	std::cout << t.tm_hour << ":" << t.tm_min << "\n";
	//}

	controller("task_1_data_1.txt", "task_1_data_2.txt",
		"", "");

	return 0;
}