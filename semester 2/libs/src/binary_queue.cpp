#include "../binary_queue.h"
#include <cstring>

binary_queue::binary_queue() {
	_data_arr = new pq_node[16];
	_size = 0;
	_capacity = 16;
}
binary_queue::~binary_queue() {
	delete[] _data_arr;
}

void binary_queue::insert(const char* data, int key) {
    if (_size >= _capacity) {
        resize();
    }
    _data_arr[_size].data = const_cast<char*>(data);
    _data_arr[_size].key = key;
    shift_up(_size);
    _size++;
}

char* binary_queue::find_max() {
    if (_size == 0) return nullptr;
    //TODO: throw exception
    return const_cast<char*>(_data_arr[0].data);
}

char* binary_queue::remove_max() {
    if (_size == 0) return nullptr;
    //TODO: throw exception
    char* max_data = const_cast<char*>(_data_arr[0].data);
    _data_arr[0] = _data_arr[--_size];
    shift_down(0);
    return max_data;
}

void binary_queue::swap(pq_node& a, pq_node& b) {
    pq_node temp = a;
    a = b;
    b = temp;
}

void binary_queue::shift_up(int index) {
    while (index > 0 && _data_arr[(index - 1) / 2].key < _data_arr[index].key) {
        swap(_data_arr[(index - 1) / 2], _data_arr[index]);
        index = (index - 1) / 2;
    }
}

void binary_queue::shift_down(int index) {
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
    pq_node* new_arr = new pq_node[_capacity * 2];
    
    std::memcpy(new_arr, _data_arr, _size * sizeof(pq_node*));
    delete[] _data_arr;
    _data_arr = new_arr;
    _capacity *= 2;
}

priority_queue& binary_queue::merge(priority_queue& q) {
    binary_queue a;
    return a;
}