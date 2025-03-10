#ifndef BINARY_QUEUE_H_
#define BINARY_QUEUE_H_

#include "priority_queue.h"

class binary_queue : public priority_queue
{
	typedef struct priority_queue_node
	{
		char* data;
		int key;
	}pq_node;

	pq_node* _data_arr;
	int _size;
	int _capacity;
public:
	binary_queue();
	~binary_queue();

	void insert(const char* data, int key) override;
	char* find_max() override;
	char* remove_max() override;
	priority_queue& merge(priority_queue& q);

	int get_size() { return _size;  };

private:
	void swap(pq_node& a, pq_node& b);
	void shift_up  (int index);
	void shift_down(int index);
	void resize();
};
#endif