#include "../t_list.h"
#include <stdexcept>

template <typename T>
t_list<T>::t_list() {
    head = nullptr;
}

template <typename T>
void t_list<T>::push(T val) {
    Node<T>* n = new Node<T>();
    n->data = val;
    n->next = head;
    head = n;
}

template <typename T>
T t_list<T>::pop() {
    if (head) {
        T p = head->data;
        Node<T>* temp = head;
        head = head->next;
        delete temp;
        return p;
    }
    throw std::out_of_range("Pop from empty list");
}

template <typename T>
bool t_list<T>::search(T val) {
    Node<T>* temp = head;
    while (temp) {
        if (temp->data == val) return true;
        temp = temp->next;
    }
    return false;
}
