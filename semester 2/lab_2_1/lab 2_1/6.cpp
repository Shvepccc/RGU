#include <iostream>
#include "../../libs/binary_queue.h"

int program_6_main(int argc, char* argv[])
{
	binary_queue bq;
	bq.insert("*10", 10);
	bq.insert("*2 ", 2);
	bq.insert("*3 ", 3);
	bq.insert("*15", 15);
	bq.insert("*16", 16);
	bq.insert("*17", 17);
	
	auto t = bq.get_size();
	for (int i = 0; i < t; ++i)
	{
		std::cout << bq.find_max() << "\n";
		bq.remove_max();
	}

	return 0;
}