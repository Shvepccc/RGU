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

	node* remove_max_inner(
		node* current_node, 
		char*& result_str, 
		int& result_priority, 
		bool delete_or_no);

	bool insert_inner(
		const char* key,
		unsigned long value,
		int randomized_priority,
		node*& current_node);


public:
	bool is_live() { return _head != nullptr; };
	decart_queue(
		int randomizer_seed = 123456,
		int randomizer_lower_bound = 0,
		int randomizer_upper_bound = 4056);
	~decart_queue();
	decart_queue& operator= (const decart_queue& arg);
	decart_queue(const decart_queue& arg);

	int get_size() { return _size; };

	virtual void insert(const char* data, int key) override;
	char* find_max() override;
	char* remove_max() override;
	char* remove_max(int& result_priority);
	priority_queue* merge(priority_queue* q) override;

	void print() {
		printBT("&", _head, false);
	}

	void printBT(const std::string& prefix, const decart_queue::node* node, bool isLeft) {
		if (node != nullptr) {
			std::cout << prefix;

			std::cout << (isLeft ? "}--" : "L__");
			std::cout << "[" << node->priority << ", " << node->r_priority << "] " << node->data << std::endl;

			printBT(prefix + (isLeft ? "|   " : "    "), node->left_subtree, true);
			printBT(prefix + (isLeft ? "|   " : "    "), node->right_subtree, false);
		}
	}

	void print_child()
	{
		std::cout << "-----\n";
		std::cout << "Data: " << this->_head->data << "\n";
		std::cout << "Left child data: " << this->_head->left_subtree->data << "\n";
		std::cout << "Right child data: " << this->_head->right_subtree->data << "\n\n";

		std::cout << "r_priotity: " << this->_head->r_priority << "\n";
		std::cout << "Left child r_priotity: " << this->_head->left_subtree->r_priority << "\n";
		std::cout << "Right child r_priotity: " << this->_head->right_subtree->r_priority << "\n";

		std::cout << "-----\n";
	}

};

#endif