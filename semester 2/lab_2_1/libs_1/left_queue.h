#ifndef left_queue_H_
#define left_queue_H_

#include "skew_queue.h"

class left_queue: public skew_queue
{
protected:
	class node final : public skew_queue::node
	{
	public:
		int rank;

		node(int k, const char* d, int ra, node* l, node* r);

	};

	int get_rank(node* n);
	node* merge_nodes(node* b1, node* b2);

public:
	priority_queue* merge(priority_queue* q) override;
};

#endif