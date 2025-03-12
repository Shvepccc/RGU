#define START_PROGRAM 7;

#include <iostream>
#include "programs.h" 

int main(int argc, char* argv[]) {
    int prog_count = 7;
    int choice = START_PROGRAM;
    int (*programs[])(int argc, char* argv[]) = 
        { 
        nullptr, 
        program_1_main, 
        program_2_main,
        program_3_main,
        program_4_main,
        program_6_main,
        program_7_main
        };

    while (true) {
        if (!(choice >= 1 && choice <= prog_count)) {
            std::cout << "Enter program number [1 - 4, 6 - " << prog_count << "] (exit - 0): ";
            std::cin >> choice;
        }

        if (choice >= 1 && choice <= prog_count && choice != 5) {
            std::cout << "Running program " << choice << "...\n" << std::endl;
            if (choice > 5) choice--;
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