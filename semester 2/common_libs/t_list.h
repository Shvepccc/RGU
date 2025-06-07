#ifndef T_LIST_H
#define T_LIST_H

#include <stdexcept>

template <typename T>
struct t_list_node {
    T data;
    t_list_node* next;
};

template <typename T>
class t_list {
private:
    t_list_node<T>* head;

public:
    t_list() : head(nullptr) {}

    void push(T val) {
        t_list_node<T>* n = new t_list_node<T>();
        n->data = val;
        n->next = head;
        head = n;
    }

    T pop() {
        if (!head)
            throw std::out_of_range("Pop from empty list");

        T p = head->data;
        t_list_node<T>* temp = head;
        head = head->next;
        delete temp;
        return p;
    }

    bool search(T val) const {
        t_list_node<T>* temp = head;
        while (temp) {
            if (temp->data == val)
                return true;
            temp = temp->next;
        }
        return false;
    }

    t_list_node<T>* get_head() const
    {
        return this->head;
    }

    void sort(int (*cmp)(const T&, const T&))
    {
        head = merge_sort(head, cmp);
    }

private:
    t_list_node<T>* merge_sort(t_list_node<T>* node, int (*cmp)(const T&, const T&)) {
        if (!node || !node->next) return node;

        t_list_node<T>* middle = split(node);
        t_list_node<T>* left = merge_sort(node, cmp);
        t_list_node<T>* right = merge_sort(middle, cmp);

        return merge(left, right, cmp);
    }

    t_list_node<T>* split(t_list_node<T>* node) {
        t_list_node<T>* slow = node;
        t_list_node<T>* fast = node->next;

        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }

        t_list_node<T>* middle = slow->next;
        slow->next = nullptr;
        return middle;
    }

    t_list_node<T>* merge(t_list_node<T>* a, t_list_node<T>* b, int (*cmp)(const T&, const T&)) {
        if (!a) return b;
        if (!b) return a;

        t_list_node<T>* result = nullptr;

        if (cmp(a->data, b->data) > 0) {
            result = a;
            result->next = merge(a->next, b, cmp);
        }
        else {
            result = b;
            result->next = merge(a, b->next, cmp);
        }

        return result;
    }

};

#endif // T_LIST_H
