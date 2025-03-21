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
		: key(k), degree(deg), parent(p), child(c), next(n) 
	{
		data = new char[std::strlen(d) + 1];
		std::strcpy(data, d);
	}

};

class binominal_queue final: public priority_queue
{
private:
	binominal_node* _head;

	void swap(binominal_node& a, binominal_node& b);
	binominal_node* merge_trees(binominal_node* b1, binominal_node* b2);
	void merge_trees_heap();
	void delete_tree(binominal_node* node);
	binominal_node* deep_copy(const binominal_node* arg);


public:
	binominal_queue() : _head(nullptr) {};
	~binominal_queue();
	binominal_queue& operator= (const binominal_queue& arg);
	binominal_queue(const binominal_queue& arg);


	void insert(const char* data, int key) override;
	char* find_max() override;
	char* remove_max() override;
	priority_queue* merge(priority_queue* q) override;

	void printHeap()
	{
		binominal_node* current = _head;
		while (current != nullptr) 
		{
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
		while (child != nullptr)
		{
			printTree(child);
			child = child->next;
		}
	}
};

#endif