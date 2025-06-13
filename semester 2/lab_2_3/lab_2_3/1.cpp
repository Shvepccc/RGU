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
            "", "");
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
	return 0;
}