#ifndef PRIORITY_QUEUE_H_
#define PRIORITY_QUEUE_H_

class priority_queue
{
public:
	virtual void insert(const char* data, int key) = 0;
	virtual char* find_max() = 0;
	virtual char* remove_max() = 0;
	virtual priority_queue* merge(priority_queue* q) = 0;

	virtual ~priority_queue() = default;
};
#endif	