#include "../binominal_queue.h"

void binominal_queue::swap(binominal_node& a, binominal_node& b) {
	binominal_node temp = a;
	a = b;
	b = temp;
}

binominal_node* binominal_queue::merge_trees(binominal_node* b1, binominal_node* b2)
{
	// Ensure b1 has the smaller key
	if (b1->key > b2->key)
		swap(*b1, *b2);
	// Make b1 the parent of b2
	b2->parent = b1;
	// Link b2 as the first child of b1
	b2->next = b1->child;
	// Update b1's child to b2
	b1->child = b2;
	// Increase the degree of b1
	b1->degree++;
	return b1;
}

void binominal_queue::merge_trees_heap()
{
	binominal_node* current_node = _head;
	binominal_node* next_node;
	_head = nullptr;

	binominal_node* trees[MAX_DEGREE_OF_QUEUE] = { nullptr };

	while (current_node != nullptr)
	{
		next_node = current_node->next;
		current_node->next = nullptr;
		int degree = current_node->degree;

		while (trees[degree] != nullptr)
		{
			current_node = merge_trees(current_node, trees[degree]);
			trees[degree] = nullptr;
			degree++;
		}
		trees[degree] = current_node;
		current_node = next_node;
	}

	for (int i = 0; i < MAX_DEGREE_OF_QUEUE; ++i)
	{
		if (trees[i] != nullptr) {
			trees[i]->next = _head;
			_head = trees[i];
		}
	}
}

void binominal_queue::merge_2(binominal_queue& q)
{
	binominal_node* new_head = nullptr;
	binominal_node** current_node = &new_head;
	binominal_node* h1 = _head;
	binominal_node* h2 = q._head;

	while (h1 != nullptr && h2 != nullptr)
	{
		if (h1->degree <= h2->degree)
		{
			*current_node = h1;
			h1 = h1->next;
		}
		else
		{
			*current_node = h2;
			h2 = h2->next;
		}

		current_node = &((*current_node)->next);
	}

	if (h1 != nullptr)
		*current_node = h1;
	if (h2 != nullptr)
		*current_node = h2;

	_head = new_head;

	merge_trees_heap();

	q._head = nullptr;
}

void binominal_queue::insert(const char* data, int key)
{
	binominal_queue new_queue;
	binominal_node* new_node = new binominal_node(key, 0, const_cast<char*>(data), nullptr, nullptr, nullptr);
	new_queue._head = new_node;

	merge_2(new_queue);
}

char* binominal_queue::find_max()
{
	if (_head == nullptr)
		throw std::runtime_error("Queue is empty");

	binominal_node* max_priority_node = _head;
	binominal_node* prevMin = nullptr;
	binominal_node* current_node = _head->next;
	binominal_node* prev = _head;

	while (current_node != nullptr) {
		if (current_node->key < max_priority_node->key) {
			max_priority_node = current_node;
			prevMin = prev;
		}
		prev = current_node;
		current_node = current_node->next;
	}

	return max_priority_node->data;
}

char* binominal_queue::remove_max()
{
	if (_head == nullptr)
		throw std::runtime_error("Queue is empty");

	binominal_node* max_priority_node = _head;
	binominal_node* prevMin = nullptr;
	binominal_node* current_node = _head->next;
	binominal_node* prev = _head;

	//find max priority element
	while (current_node != nullptr) {
		if (current_node->key < max_priority_node->key) {
			max_priority_node = current_node;
			prevMin = prev;
		}
		prev = current_node;
		current_node = current_node->next;
	}

	if (prevMin == nullptr)
		_head = _head->next;
	else
		prevMin->next = max_priority_node->next;

	// delete node from queue
	binominal_queue newHeap;
	binominal_node* child = max_priority_node->child;
	while (child != nullptr) {
		binominal_node* next = child->next;
		child->parent = nullptr;
		child->next = newHeap._head;
		newHeap._head = child;
		child = next;
	}

	char* result = max_priority_node->data;
	delete max_priority_node;
	merge_2(newHeap);

	return result;
}

