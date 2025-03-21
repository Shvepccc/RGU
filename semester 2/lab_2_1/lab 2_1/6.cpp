#include <iostream>
#include "../libs/binary_queue.h"

int program_6_main(int argc, char* argv[])
{
	binary_queue bq;
	bq.insert("*10", 10);
	bq.insert("*2 ", 2);
	bq.insert("*3 ", 3);
	bq.insert("*15", 15);
	bq.insert("*16", 16);
	bq.insert("*17", 17);

	binary_queue bq_2;
	bq_2.insert("*53", 53);
	bq_2.insert("*1", 1);
	bq_2.insert("*72", 72);

	for (int i = 20; i < 34; ++i) {
		char str[5];
		str[0] = '*';
		snprintf(str+1, 3, "%d", i);
		bq_2.insert(str, i);
	}

	bq.merge(&bq_2);
	
	auto t = bq.get_size();
	for (int i = 0; i < t; ++i)
	{
		std::cout << bq.find_max() << "\n";
		bq.remove_max();
	}

	return 0;
}