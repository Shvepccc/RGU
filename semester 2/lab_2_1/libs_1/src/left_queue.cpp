#include "../left_queue.h"
#include "../skew_queue.h"

int left_queue::get_rank(node* n) { return (n != nullptr) ? n->rank : 0; }

left_queue::node* left_queue::merge_nodes(node* n1, node* n2)
{
	if (n1 == nullptr)
		return n2;
	if (n2 == nullptr)
		return n1;

	if (n1->key < n2->key)
	{
		node* temp_node = n1;
		n1 = n2;
		n2 = temp_node;
	}
	n1->right = merge_nodes(static_cast<node*>(n1->right), n2);

	if (get_rank(static_cast<node*>(n1->left)) <
		get_rank(static_cast<node*>(n1->right))) {
		node* temp_node = static_cast<node*>(n1->left);
		n1->left = n1->right;
		n1->right = temp_node;
	}

	n1->rank = get_rank(static_cast<node*>(n1->right)) + 1;
}

priority_queue* left_queue::merge(priority_queue* q)
{
	left_queue* cast_q = dynamic_cast<left_queue*>(q);
	if (cast_q == nullptr)
	{
		throw std::invalid_argument("Bad queue type");
	}
	//skew_queue::merge(q);

	_head = merge_nodes(static_cast<node*>(_head),
		static_cast<node*>(cast_q->_head));

	_size += cast_q->_size;
	cast_q->_head = nullptr;
	cast_q->_size = 0;
	return this;
}

left_queue::node::node(int k, const char* d, int ra, left_queue::node* l, left_queue::node* r)
	: skew_queue::node(1, d, l, r), rank(ra)
{}