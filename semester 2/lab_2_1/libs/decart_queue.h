#ifndef DECART_QUEUE_H_
#define DECART_QUEUE_H_

#include "priority_queue.h"
#include <iostream>


class decart_queue final : public priority_queue
{
private:

	struct node final
	{
		char* data;
		int priority;
		unsigned long r_priority;
		node* left_subtree;
		node* right_subtree;
	};

private:
	node* _head;
	int _size;
	int _randomizer_lower_bound;
	int _randomizer_upper_bound;


	void swap(node& a, node& b);
	static void destroy_node(node* node_to_destruct, bool recursive);
	static node* clone_node(node* node_to_clone, bool recursive);

	void left_rotation(node*& subtree_root, bool validate);
	void right_rotation(node*& subtree_root, bool validate);

	enum removal_status
	{
		bst_find,
		heap_sift_down
	};

	bool remove_inner(
		char** removed_value,
		node*& current_node,
		removal_status status);

	bool insert_inner(
		const char* key,
		unsigned long value,
		int randomized_priority,
		node*& current_node);

	node* merge_inner(node* a, node* b);

public:
	decart_queue(
		int randomizer_seed = 123456,
		int randomizer_lower_bound = 0,
		int randomizer_upper_bound = 1024);
	~decart_queue();
	decart_queue& operator= (const decart_queue& arg);
	decart_queue(const decart_queue& arg);

	int get_size() { return _size; };

	virtual void insert(const char* data, int key) override;
	char* find_max() override;
	char* remove_max() override;
	priority_queue* merge(priority_queue* q) override;

};

#endif