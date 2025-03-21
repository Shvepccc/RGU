#include "../fibonacci_queue.h"

void fibonacci_queue::swap(fibonacci_node& a, fibonacci_node& b) {
	fibonacci_node temp = a;
	a = b;
	b = temp;
}

void fibonacci_queue::merge_lists(fibonacci_node* n1, fibonacci_node* n2)
{
	if (n1 == nullptr || n2 == nullptr) return;

	fibonacci_node* L = n1->left;
	fibonacci_node* R = n2->right;

	n2->right = n1;
	n1->left = n2;
	L->right = R;
	R->left = L;
}

priority_queue* fibonacci_queue::merge(priority_queue* q)
{
	fibonacci_queue* cast_q = dynamic_cast<fibonacci_queue*>(q);
	if (cast_q->_size == 0)
	{
		return this;
	}
	else if (this->_size == 0)
	{
		_head = cast_q->_head;
		_size = cast_q->_size;
		return this;
	}
	else
	{
		merge_lists(_head, cast_q->_head);
	}

	_size += cast_q->_size;
	_head = (_head->key > cast_q->_head->key) ? _head : cast_q->_head;
    delete_tree(cast_q->_head);
    cast_q->_head = nullptr;
	return this;
}

void fibonacci_queue::insert(const char* data, int key) 
{
	fibonacci_node* new_node = new fibonacci_node(key, 0, const_cast<char*>(data), nullptr, nullptr, nullptr, nullptr);
	if (_size == 0) {
		_head = new_node;
		_head->right = _head;
		_head->left = _head;
	}
	else {
		fibonacci_node* prev_right = _head->right;
		_head->right = new_node;
		new_node->left = _head;
		new_node->right = prev_right;
		prev_right->left = new_node;

		if (new_node->key > _head->key) {
			_head = new_node;
		}
	}

	new_node->parent = nullptr;
	++_size;
}

char* fibonacci_queue::find_max()
{
	if (_head == nullptr)
		throw std::runtime_error("Queue is empty");
	return _head->data;
}

char* fibonacci_queue::remove_max() {
    if (_head == nullptr)
        throw std::runtime_error("Queue is empty");

    fibonacci_node* prev_max = _head;

    fibonacci_node* child = _head->child;
    if (child != nullptr) {
        fibonacci_node* start = child;
        do {
            fibonacci_node* next_child = child->right;
            merge_lists(_head, child);
            child->parent = nullptr;
            child = next_child;
        } while (child != start);
    }

    char* result = prev_max->data;
    char* data_copy = nullptr;
    data_copy = new char[std::strlen(result) + 1];
    std::strcpy(data_copy, result);

    if (prev_max->right == prev_max) {
        _head = nullptr;
    }
    else {
        fibonacci_node* L = _head->left;
        fibonacci_node* R = _head->right;
        L->right = R;
        R->left = L;

        if (_head == prev_max) {
            _head = (_head->right == prev_max) ? nullptr : _head->right;
        }

        consolidate();
    }

    delete[] prev_max->data;
    delete prev_max;
    --_size;

    return data_copy;
}

void fibonacci_queue::consolidate() {
    if (!_head) return;
    size_t arr_size = static_cast<size_t>(std::ceil(std::log2(_size)) + 1);
    fibonacci_node** trees = new fibonacci_node * [arr_size] {};

    fibonacci_node* start = _head;
    fibonacci_node* current = _head;
    do {
        fibonacci_node* next = current->right;
        int d = current->degree;
        while (trees[d]) {
            fibonacci_node* other = trees[d];
            if (current->key < other->key) {
                fibonacci_node* temp = current;
                current = other;
                other = temp;
            }
            merge_lists(current->child, other);
            other->parent = current;
            trees[d] = nullptr;
            d++;
        }
        trees[d] = current;
        current = next;
    } while (current != start);

    _head = nullptr;
    for (size_t i = 0; i < arr_size; ++i) {
        if (trees[i] && (!_head || trees[i]->key > _head->key)) {
            _head = trees[i];
        }
    }
    delete[] trees;
}

void fibonacci_queue::delete_tree(fibonacci_node* node) {
    if (!node) return;

    fibonacci_node* start = node;
    while (node != start) {
        fibonacci_node* next = node->right; 
        delete_tree(node->child);           
        delete[] node->data;                
        delete node;                        
        node = next;                        
    }
}

fibonacci_queue::~fibonacci_queue() {
    delete_tree(_head);
    _head = nullptr;
}

fibonacci_queue& fibonacci_queue::operator= (const fibonacci_queue& arg)
{
	if (this != &arg) {
		delete_tree(_head);
		_head = deep_copy(arg._head);
	}
	return *this;
}

fibonacci_queue::fibonacci_queue(const fibonacci_queue& arg)
{
	_head = deep_copy(arg._head);
}

fibonacci_node* fibonacci_queue::deep_copy(fibonacci_node* node) {
    if (node == nullptr) {
        return nullptr;
    }

    fibonacci_node* new_head = new fibonacci_node(
        node->key,
        node->degree,
        node->data,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    );

    const fibonacci_node* current = node->right;
    fibonacci_node* new_current = new_head;

    while (current != node) {
        fibonacci_node* new_node = new fibonacci_node(
            current->key,
            current->degree,
            current->data,
            nullptr,
            nullptr,
            nullptr,
            nullptr
        );

        new_current->right = new_node;
        new_node->left = new_current;

        new_current = new_node;
        current = current->right;
    }

    new_current->right = new_head;
    new_head->left = new_current;

    current = node;
    new_current = new_head;
    do {
        if (current->child) {
            new_current->child = deep_copy(current->child);
            new_current->child->parent = new_current;
        }
        current = current->right;
        new_current = new_current->right;
    } while (current != node);

    return new_head;
}