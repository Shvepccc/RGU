#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>

#include "../libs/decart_queue.h"


int program_11_main(int argc, char* argv[])
{
    {
        decart_queue q1;
        for (int i = 1; i < 11; ++i)
        {
            char str[5];
            str[0] = '*';
            snprintf(str + 1, 3, "%d", i);
            q1.insert(str, i);
        }

        q1.insert("*62", 62);
        q1.insert("*54", 54);
        q1.insert("*48", 48);

        std::cout << "\n****** HEAP 1 ******\n\n";
        q1.print();

        decart_queue q2;
        for (int i = 11; i < 21; ++i)
        {
            char str[5];
            str[0] = '*';
            snprintf(str + 1, 3, "%d", i);
            q2.insert(str, i);
        }
        std::cout << "\n****** HEAP 2 ******\n\n";
        q2.print();

        q1.merge(&q2);

        std::cout << "\n****** MERGE HEAP ******\n\n";
        q1.print();

        q1.print_child();

        std::cout << "\nMax priority element: " << q1.find_max() << "\n";

        int size = q1.get_size();
        for (int i = 0; i < size; ++i)
        {
            char* str = q1.remove_max();
            std::cout << "\nDeleting max priority element ( " << str << " )\n";
            delete[] str;
        }

        //***********
        if (q2.is_live())
        {
            std::cout << "Q2\n";
            size = q2.get_size();
            for (int i = 0; i < size; ++i)
            {
                char* str = q2.remove_max();
                std::cout << "\nDeleting max priority element ( " << str << " )\n";
                delete[] str;
            }
        }
        //***********

        decart_queue q3;
        q3.insert("*10", 10);
        q3.insert("*20", 20);
        q3.insert("*5 ", 5);
        q3.insert("*15", 15);
        q3.insert("*25", 25);
        q1 = q3;

        std::cout << "\n****** Queue 3 ******\n";

        size = q3.get_size();
        for (int i = 0; i < size; ++i)
        {
            char* str = q3.remove_max();
            std::cout << "\nDeleting max priority element ( " << str << " )\n";
            delete[] str;
        }
    }
    _CrtDumpMemoryLeaks();
    return 0;
}