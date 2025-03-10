#include <iostream>
#include "../../libs/binominal_queue.h"
#include <string>

int program_7_main(int argc, char* argv[])
{
    binominal_queue heap;

    // Insert elements
    heap.insert("10", 10);
    heap.insert("20", 20);
    heap.insert("5 ", 5);
    heap.insert("15", 15);
    heap.insert("25", 25);

    std::cout << "Binomial Heap after insertions:" << std::endl;
    heap.printHeap();

    // Extract max priority element
    char* max = heap.remove_max();
    std::cout << "\nExtracted max priority element 1: " << max << std::endl;
    max = heap.remove_max();
    std::cout << "\nExtracted max priority element 2: " << max << std::endl;
    max = heap.remove_max();
    std::cout << "\nExtracted max priority element 3: " << max << std::endl;
    std::cout << "\nBinomial Heap after extracting max priority element:" << std::endl;
    heap.printHeap();

    // Merge with another heap
    binominal_queue otherHeap;
    otherHeap.insert("7", 7);
    otherHeap.insert("12", 12);

    heap.merge_2(otherHeap);
    std::cout << "\nBinomial Heap after merging:" << std::endl;
    heap.printHeap();

	return 0;
}