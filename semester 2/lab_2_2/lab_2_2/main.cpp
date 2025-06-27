#define START_PROGRAM 0;

#include <iostream>
#include "programms.h"

int main(int argc, char* argv[]) {
    int prog_count = 9;
    int choice = START_PROGRAM;
    int (*programs[])(int argc, char* argv[]) =
    {
    nullptr,
    program_2_main, // Good (kind of like)
    program_3_main, // Good
    program_5_main, // Good
    program_6_main,
    program_8_main, // Good
    program_9_main  // Good
    };

    while (true) {
        if (!(choice >= 1 && choice <= prog_count))
        {
            std::cout << "Enter program number:\n2 (bigfloat), \n3 (polynom), \n5 (complex value), \n8 (chain fractions), \n9 (calculate pi)\n(exit - 0): ";
            std::cin >> choice;
        }

        if (choice > 1 && choice <= prog_count &&
            (choice != 4 && choice != 7 && choice != 6))
        {
            if (choice == 5) choice -= 1;
            if (choice >= 8) choice -= 2;
            std::cout << "Running program " << choice << "...\n" << std::endl;
            programs[--choice](argc, argv);
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