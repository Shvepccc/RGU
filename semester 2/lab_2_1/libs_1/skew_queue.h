#ifndef SKEW_QUEUE_H_
#define SKEW_QUEUE_H_

#include "priority_queue.h"
#include <iostream>


class skew_queue: public priority_queue
{
protected:
	class node
	{
	public:
		int key;
		char* data;
		node* left;
		node* right;

		node() = default;

		node(int k, const char* d, node* l, node* r)
			: key(k), left(l), right(r)
		{
			data = new char[std::strlen(d) + 1];
			std::strcpy(data, d);
		}
	};

	node* _head;
	int _size;

	void swap(node& a, node& b);
	node* merge_nodes(node* n1, node* n2);
	void delete_queue(node* node);
	node* deep_copy(const node* arg);


public:
	skew_queue() : _head(nullptr), _size(0) {};
	~skew_queue();
	skew_queue& operator= (const skew_queue& arg);
	skew_queue(const skew_queue& arg);

	int get_size() { return _size; };


	void insert(const char* data, int key) override;
	char* find_max() override;
	char* remove_max() override;
	priority_queue* merge(priority_queue* q) override;

};

#endif