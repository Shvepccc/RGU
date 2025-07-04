#define _CRT_SECURE_NO_WARNINGS

#include "elevator.h"
#include "passenger.h"
#include "simulate_elevators.h"

#include <ctime>
#include <iostream>

int program_1_main(int argc, char* argv[])
{
    try
    {
        controller("task_1_data_1.txt", "task_1_data_2.txt",
            "task_1_res_1.txt", "task_1_res_2.txt",
            60, 0.9);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
	return 0;
}