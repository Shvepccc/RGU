#include <iostream>

#include "../../libs/fibonacci_queue.h"

int program_8_main(int argc, char* argv[])
{
    fibonacci_queue q1;
    for (int i = 1; i < 11; ++i)
    {
        char str[5];
        str[0] = '*';
        snprintf(str + 1, 3, "%d", i);
        q1.insert(str, i);
    }

    fibonacci_queue q2;
    for (int i = 11; i < 21; ++i)
    {
        char str[5];
        str[0] = '*';
        snprintf(str + 1, 3, "%d", i);
        q2.insert(str, i);
    }

    std::cout << "Queue 1:\n";
    q1.printHeap();
    std::cout << "\nQueue 2:\n";
    q2.printHeap();

    q1.merge(&q2);

    std::cout << "\nQueue after merge:\n";
    q1.printHeap();

    std::cout << "\nMax priority element: " << q1.find_max() << "\n";

    int size = q1.get_size();
    for (int i = 0; i < size; ++i)
    {
        char* str = q1.remove_max();
        std::cout << "\nQueue after deleting max priority element ( " << str << " ):\n";
        if (i > 15) q1.printHeap();
    }

    fibonacci_queue q3;
    q3.insert("*10", 10);
    q3.insert("*20", 20);
    q3.insert("*5 ", 5);
    q3.insert("*15", 15);
    q3.insert("*25", 25);
    q1 = q3;

    std::cout << "\n******\n";
    q3.printHeap();
    

    return 0;
}