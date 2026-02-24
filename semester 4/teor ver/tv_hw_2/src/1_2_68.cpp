#include <iostream>
#include "../include/help.hpp"

int main()
{
    double elder_probability = 0.3;
    double student_probability = 0.5;
    double target_probability = 0.9;
    int max_additional_students = 20;

    for (int additional_students = 0; additional_students <= max_additional_students; ++additional_students)
    {
        int informed_count = 0;

        for (int exp = 0; exp < NUM_EXPERIMENTS; ++exp)
        {
            bool informed = false;

            int elder_random = getRandomNumber(1, 100);
            if (elder_random <= elder_probability * 100)
            {
                informed = true;
            }

            for (int i = 0; i < additional_students && !informed; ++i)
            {
                int student_random = getRandomNumber(1, 100);
                if (student_random <= student_probability * 100)
                {
                    informed = true;
                }
            }

            if (informed)
            {
                informed_count++;
            }
        }

        double probability = static_cast<double>(informed_count) / NUM_EXPERIMENTS;
        std::cout << "Additional students: " << additional_students
                  << ", Probability: " << probability << std::endl;

        if (probability > target_probability)
        {
            std::cout << "Answer: " << additional_students << " additional students needed" << std::endl;
            break;
        }
    }

    return 0;
}