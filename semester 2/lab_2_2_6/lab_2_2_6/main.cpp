#define START_PROGRAM 1;

#include <iostream>
#include "programms.h"

int main(int argc, char* argv[])
{
    int prog_count = 1;
    int choice = START_PROGRAM;
    int (*programs[])(int argc, char* argv[]) =
    {
    nullptr,
    program_6_main
    };

    while (true)
    {
        if (!(choice >= 1 && choice <= prog_count))
        {
            std::cout << "Enter program number [1 - " << prog_count << "] (exit - 0): ";
            std::cin >> choice;
        }

        if (choice >= 1 && choice <= prog_count)
        {
            std::cout << "Running program " << choice << "...\n" << std::endl;
            programs[choice](argc, argv);
            return 0;
        }
        else if (choice == 0)
        {
            return 0;
        }
        else
        {
            std::cout << "Invalid number. Try again =) \n" << std::endl;
            choice = -1;
        }
    }

    return 0;
}