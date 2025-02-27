#include <iostream>
#include "programs.h" 

#define START_PROGRAM 4;

int main(int argc, char* argv[]) {
    int prog_count = 4;
    int choice = START_PROGRAM;
    int (*programs[])(int argc, char* argv[]) = 
        { 
        nullptr, 
        program_1_main, 
        program_2_main,
        program_3_main,
        program_4_main
        };

    while (true) {
        if (!(choice >= 1 && choice <= prog_count)) {
            std::cout << "Enter program number (exit - 0): ";
            std::cin >> choice;
        }

        if (choice >= 1 && choice <= prog_count) {
            std::cout << "Running program " << choice << "...\n" << std::endl;
            programs[choice](argc, argv);
            return 0;
        }
        else if (choice == 0) {
            return 0;
        }
        else {
            std::cout << "Invalid number. Try again =) \n" << std::endl;
            choice = -1;
        }
    }

    return 0;
}