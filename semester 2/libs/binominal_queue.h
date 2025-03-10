#ifndef BINOMINAL_QUEUE_H_
#define BINOMINAL_QUEUE_H_

#include "priority_queue.h"
#include <iostream>
#define MAX_DEGREE_OF_QUEUE 64

struct binominal_node
{
	int key;
	int degree;
	char* data;
	binominal_node* parent;
	binominal_node* child;
	binominal_node* next;

	binominal_node(int k, int deg, char* d, binominal_node* p, binominal_node* c, binominal_node* n)
		: key(k), degree(deg), data(d), parent(p), child(c), next(n) {
	}

};

class binominal_queue : public priority_queue
{
private:
	binominal_node* _head;

	binominal_queue& create_node(const char* data, int key);

	void swap(binominal_node& a, binominal_node& b);
	binominal_node* merge_trees(binominal_node* b1, binominal_node* b2);
	void merge_trees_heap();


public:
	binominal_queue() : _head(nullptr) {};
	//TODO: write destructor
	~binominal_queue() = default;

	void insert(const char* data, int key) override;
	char* find_max() override;
	char* remove_max() override;
	void merge_2(binominal_queue& q);

	//priority_queue& merge(priority_queue& q) { return new binominal_queue; };

	void printHeap()
	{
		binominal_node* current = _head;
		while (current != nullptr) {
			std::cout << "B" << current->degree << " ";
			printTree(current);
			std::cout << std::endl;
			current = current->next;
		}
	}

private:
	void printTree(binominal_node* root)
	{
		std::cout << root->data << " ";
		binominal_node* child = root->child;
		while (child != nullptr) {
			printTree(child);
			child = child->next;
		}
	}
};

#endif