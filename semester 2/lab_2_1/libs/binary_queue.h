#ifndef BINARY_QUEUE_H_
#define BINARY_QUEUE_H_

#include "priority_queue.h"

class binary_queue final: public priority_queue
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
	binary_queue(int n);
	binary_queue();
	~binary_queue();

	binary_queue(const binary_queue& data);
	binary_queue& operator= (const binary_queue&);

	void insert(const char* data, int key) override;
	char* find_max() override;
	char* remove_max() override;
	priority_queue* merge(priority_queue* q) override;

	int get_size() { return _size;  };

private:
	void swap(pq_node& a, pq_node& b);
	void sift_up  (int index);
	void sift_down(int index);
	void resize();
	int get_max_priority();
	friend void copy_data(binary_queue& a, binary_queue& b);
};
#endif