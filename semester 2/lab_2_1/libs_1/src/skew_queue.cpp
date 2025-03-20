#include "../skew_queue.h"

void skew_queue::swap(node& a, node& b)
{
	node temp_node = a;
	a = b;
	b = temp_node;
}

void skew_queue::insert(const char* data, int key)
{
	skew_queue new_skew_queue;
	node* node = new skew_queue::node(key, data, nullptr, nullptr);
	new_skew_queue._head = node;
	merge(&new_skew_queue);
	++_size;
}

char* skew_queue::find_max()
{
	if (_head == nullptr)
		throw std::runtime_error("Queue is empty");
	return _head->data;
}

skew_queue::node* skew_queue::merge_nodes(node* n1, node* n2)
{
	if (n1 == nullptr)
		return n2;
	if (n2 == nullptr)
		return n1;

	if (n1->key < n2->key)
		swap(*n1, *n2);

	n1->right = merge_nodes(n1->right, n2);
	return n1;
}

char* skew_queue::remove_max()
{
	if (_head == nullptr)
		throw std::runtime_error("Queue is empty");
	
	node* node_to_delete = _head;
	_head = merge_nodes(_head->left, _head->right);
	--_size;
	return node_to_delete->data;
}

priority_queue* skew_queue::merge(priority_queue* q)
{
	skew_queue* cast_q = dynamic_cast<skew_queue*>(q);

	if (this->_head == nullptr)
	{
		_head = cast_q->_head;
		_size = cast_q->_size;
		cast_q->_head = nullptr;
		cast_q = 0;
		return this;
	}
	if (cast_q->_head == nullptr)
		return this;

	_size += cast_q->_size;
	_head = merge_nodes(_head, cast_q->_head);
	cast_q->_head = nullptr;
	cast_q->_size = 0;
	return this;
}

//----------------------------------------------------------------------

void skew_queue::delete_queue(node* n)
{
	if (n == nullptr)
		return;

	delete_queue(n->left);
	delete_queue(n->right);
	delete n->data;
	delete n;
}

skew_queue::node* skew_queue::deep_copy(const node* arg)
{
	if (arg == nullptr)
		return nullptr;

	node* node = new skew_queue::node(arg->key, arg->data, nullptr, nullptr);

	node->left = deep_copy(arg->left);
	node->right = deep_copy(arg->right);
	return node;
}

//----------------------------------------------------------------------

skew_queue::~skew_queue() { delete_queue(_head); }

skew_queue& skew_queue::operator= (const skew_queue& arg)
{
	if (&arg == this) return *this;

	delete_queue(_head);
	_head = deep_copy(arg._head);
	_size = arg._size;
	return *this;
}

skew_queue::skew_queue(const skew_queue& arg)
{ 
	_head = deep_copy(arg._head);
	_size = arg._size;
}