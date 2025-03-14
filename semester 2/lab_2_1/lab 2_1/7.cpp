#include <iostream>
#include "../../libs/binominal_queue.h"
#include <string>

int program_7_main(int argc, char* argv[])
{
    binominal_queue heap;
    
    // Insert elements
    heap.insert("*10", 10);
    heap.insert("*20", 20);
    heap.insert("*5 ", 5);
    heap.insert("*15", 15);
    heap.insert("*25", 25);

    for (int i = 20; i < 44; ++i) 
    {
        char str[5];
        str[0] = '*';
        snprintf(str + 1, 3, "%d", i);
        heap.insert(str, i);
    }
    
    std::cout << "Binomial Heap after insertions:" << std::endl;
    heap.printHeap();
    std::cout << "\n";

    int nn = 10;
    for (int i = 0; i < nn; ++i) 
    {
        char* max = heap.remove_max();
        std::cout << max << "\n";
    }
    std::cout << "\nBinomial Heap after extracting " << nn <<" max priority element:" << std::endl;
    heap.printHeap();
    
    // Merge with another heap
    binominal_queue otherHeap;
    otherHeap.insert("*7", 7);
    otherHeap.insert("*55", 55);
    otherHeap.insert("*12", 12);
    otherHeap.insert("*63", 63);
    
    heap.merge(&otherHeap);
    std::cout << "\nBinomial Heap after merging:" << std::endl;
    heap.printHeap();

    binominal_queue hp = heap;
    hp.printHeap();
    
    return 0;
}