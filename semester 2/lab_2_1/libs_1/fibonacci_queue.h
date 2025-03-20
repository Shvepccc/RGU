#ifndef FIBONACCI_QUEUE_H_
#define FIBONACCI_QUEUE_H_

#include "priority_queue.h"
#include <iostream>

struct fibonacci_node
{
	int key;
	int degree;
	char* data;
	fibonacci_node* parent;
	fibonacci_node* child;
	fibonacci_node* left;
	fibonacci_node* right;

	fibonacci_node(int k, int deg, char* d, fibonacci_node* p, fibonacci_node* c, fibonacci_node* l, fibonacci_node* r)
		: key(k), degree(deg), parent(p), child(c), left(l), right(r)
	{
		data = new char[std::strlen(d) + 1];
		std::strcpy(data, d);
	}

};

class fibonacci_queue final: public priority_queue
{
private:
	fibonacci_node* _head;
	int _size;

	void swap(fibonacci_node& a, fibonacci_node& b);
	fibonacci_node* deep_copy(const fibonacci_node* arg);
	void merge_lists(fibonacci_node* n1, fibonacci_node* n2);
	void consolidate();
	void delete_tree(fibonacci_node* node);
	fibonacci_node* deep_copy(fibonacci_node* original);

public:
	fibonacci_queue() : _head(nullptr), _size(0) {};
	
	~fibonacci_queue();
	fibonacci_queue& operator= (const fibonacci_queue& arg);
	fibonacci_queue(const fibonacci_queue& arg);

	void insert(const char* data, int key) override;
	char* find_max() override;
	char* remove_max() override;
	priority_queue* merge(priority_queue* q) override;

	int get_size() { return _size;  };
	void printHeap()
	{
		if (_head == nullptr) {
			std::cout << "Heap is empty." << std::endl;
			return;
		}
		fibonacci_node* current = _head;
		do
		{
			std::cout << "B" << current->degree << " ";
			printTree(current);
			std::cout << std::endl;
			current = current->right;
		} while (current != _head);
	}

private:
	void printTree(fibonacci_node* root)
	{
		if (root == nullptr) return;
		std::cout << root->data << " ";
		fibonacci_node* child = root->child;
		if (root->child != nullptr) {
			do
			{
				printTree(child);
				child = child->right;
			} while (child != root->child);
		}
	}
};

#endif