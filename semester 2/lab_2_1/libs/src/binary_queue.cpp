#include "../binary_queue.h"
#include <cstring>
#include <iostream>

binary_queue::binary_queue(const binary_queue& data) {
    copy_data(*this, const_cast<binary_queue&>(data));
}

void copy_data(binary_queue& dst, binary_queue& src) {
    binary_queue new_queue(src._size);
    std::memcpy(new_queue._data_arr, src._data_arr, src._size * sizeof(binary_queue::pq_node*));
}

binary_queue::binary_queue(int n) {
    _data_arr = new pq_node[n];
    _size = 0;
    _capacity = n;
}
binary_queue::binary_queue() {
	_data_arr = new pq_node[16];
	_size = 0;
	_capacity = 16;
}
binary_queue::~binary_queue() {
    for (int i = 0; i < _size; ++i) {
        delete[] _data_arr[i].data;
    }
	delete[] _data_arr;
}

binary_queue& binary_queue::operator= (const binary_queue& arg) {

    if (&arg != this) {
        copy_data(*this, const_cast<binary_queue&>(arg));
    }
    return *this;
}

void new_data() {

}

void binary_queue::insert(const char* data, int key) {
    if (_size >= _capacity) {
        resize();
    }
    _data_arr[_size].data = new char[std::strlen(data) + 1];
    std::strcpy(_data_arr[_size].data, data);
    _data_arr[_size].key = key;
    sift_up(_size);
    _size++;
}

char* binary_queue::find_max() {
    if (_size == 0)
        throw std::runtime_error("Queue is null");
    return const_cast<char*>(_data_arr[0].data);
}

char* binary_queue::remove_max() {
    if (_size == 0)
        throw std::runtime_error("Queue is null");

    char* max_data = _data_arr[0].data;

    char* data_copy = new char[std::strlen(max_data) + 1];
    std::strcpy(data_copy, max_data);

    delete[] max_data;

    _data_arr[0] = _data_arr[--_size];

    sift_down(0);

    return data_copy;
}

void binary_queue::swap(pq_node& a, pq_node& b) {
    pq_node temp = a;
    a = b;
    b = temp;
}

void binary_queue::sift_up(int index) {
    while (index > 0 && _data_arr[(index - 1) / 2].key < _data_arr[index].key) {
        swap(_data_arr[(index - 1) / 2], _data_arr[index]);
        index = (index - 1) / 2;
    }
}

void binary_queue::sift_down(int index) {
    while (2 * index + 1 < _size) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int largest = index;

        if (left < _size && _data_arr[left].key > _data_arr[largest].key) {
            largest = left;
        }
        if (right < _size && _data_arr[right].key > _data_arr[largest].key) {
            largest = right;
        }
        if (largest == index) {
            break;
        }
        swap(_data_arr[index], _data_arr[largest]);
        index = largest;
    }
}

void binary_queue::resize() {
    //pq_node* new_arr = new pq_node[_capacity * 2];
    //
    //std::memcpy(new_arr, _data_arr, _size * sizeof(pq_node*));
    //delete[] _data_arr;
    //_data_arr = new_arr;
    //_capacity *= 2;

    pq_node* new_arr = new pq_node[_capacity * 2];

    for (int i = 0; i < _size; ++i) {
        new_arr[i] = _data_arr[i];
    }
    delete[] _data_arr;
    _data_arr = new_arr;
    _capacity *= 2;
}

priority_queue* binary_queue::merge(priority_queue* q) {
    
    binary_queue* cast_q = dynamic_cast<binary_queue*>(q);
    int size = cast_q->get_size();

    for (int i = 0; i < size; ++i) {
        int key = cast_q->_data_arr[0].key;
        char* str = cast_q->remove_max();
        this->insert(str, key);
        delete[] str;
    }
    return this;
}

int binary_queue::get_max_priority() {
    if (_size == 0)
        throw std::runtime_error("Queue is null");
    return _data_arr[0].key;
}