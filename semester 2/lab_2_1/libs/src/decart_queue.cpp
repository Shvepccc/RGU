#include "../decart_queue.h"

void decart_queue::swap(node& a, node& b)
{
	node temp_node = a;
	a = b;
	b = temp_node;
}

decart_queue::decart_queue(
    int randomizer_seed,
    int randomizer_lower_bound,
    int randomizer_upper_bound) :
    _head(nullptr), _size(0),
    _randomizer_lower_bound(randomizer_lower_bound),
    _randomizer_upper_bound(randomizer_upper_bound)
{
    srand(randomizer_seed);
}

decart_queue::~decart_queue() 
{
    destroy_node(_head, true);
}

decart_queue& decart_queue::operator= (const decart_queue& obj) 
{
    if (this == &obj)
    {
        return *this;
    }

    destroy_node(_head, true);
    _head = clone_node(obj._head, true);
    _randomizer_lower_bound = obj._randomizer_lower_bound;
    _randomizer_upper_bound = obj._randomizer_upper_bound;

    return *this;
}

decart_queue::decart_queue(const decart_queue& obj) 
{
    _head = clone_node(obj._head, true);
    _randomizer_lower_bound = obj._randomizer_lower_bound;
    _randomizer_upper_bound = obj._randomizer_upper_bound;
}

void decart_queue::destroy_node(
    node* node_to_destruct,
    bool recursive)
{
    if (node_to_destruct == nullptr)
    {
        return;
    }

    delete[] node_to_destruct->data;
    if (recursive)
    {
        destroy_node(node_to_destruct->left_subtree, true);
        destroy_node(node_to_destruct->right_subtree, true);
    }
    delete[] node_to_destruct;
}

decart_queue::node* decart_queue::clone_node(
    node* node_to_clone,
    bool recursive)
{
    if (node_to_clone == nullptr)
    {
        return nullptr;
    }

    char* cloned_data = new char[std::strlen(node_to_clone->data) + 1];
    std::strcpy(cloned_data, node_to_clone->data);

    node* cloned_node = new node;
    cloned_node->data = cloned_data;
    cloned_node->priority = node_to_clone->priority;
    cloned_node->r_priority = node_to_clone->r_priority;

    cloned_node->left_subtree = recursive
        ? clone_node(node_to_clone->left_subtree, true)
        : nullptr;
    cloned_node->right_subtree = recursive
        ? clone_node(node_to_clone->right_subtree, true)
        : nullptr;

    return cloned_node;
}


void decart_queue::left_rotation(
    node*& subtree_root,
    bool validate = false)
{
    if (validate && (subtree_root == nullptr || subtree_root->right_subtree == nullptr))
    {
        return;
    }

    auto* temp = subtree_root;
    subtree_root = subtree_root->right_subtree;
    temp->right_subtree = subtree_root->left_subtree;
    subtree_root->left_subtree = temp;
}

void decart_queue::right_rotation(
    node*& subtree_root,
    bool validate = false)
{
    if (validate && (subtree_root == nullptr || subtree_root->left_subtree == nullptr))
    {
        return;
    }

    auto* temp = subtree_root;
    subtree_root = subtree_root->left_subtree;
    temp->left_subtree = subtree_root->right_subtree;
    subtree_root->right_subtree = temp;
}

int rand_int(int min, int max)
{
    return rand() % (max - min + 1);
}

// ------------------------------------------------------------

void decart_queue::insert(const char* data, int key)
{
    insert_inner(data, key, rand_int(_randomizer_lower_bound, _randomizer_upper_bound), _head);
    ++_size;
}

bool decart_queue::insert_inner(
    const char* data,
    unsigned long key,
    int randomized_priority,
    node*& current_node)
{
    if (current_node == nullptr)
    {
        current_node = new node();
        current_node->data = new char[std::strlen(data) + 1];
        std::strcpy(current_node->data, data);
        current_node->priority = key;
        current_node->r_priority = randomized_priority;
        current_node->left_subtree = nullptr;
        current_node->right_subtree = nullptr;
        return true;
    }

    if (key < current_node->priority)
    {
        bool inserted = insert_inner(data, key, randomized_priority, current_node->left_subtree);

        if (inserted && current_node->left_subtree->r_priority > current_node->r_priority)
        {
            right_rotation(current_node);
        }

        return inserted;
    }
    else
    {
        bool inserted = insert_inner(data, key, randomized_priority, current_node->right_subtree);

        if (inserted && current_node->right_subtree->r_priority > current_node->r_priority)
        {
            left_rotation(current_node);
        }

        return inserted;
    }
}

char* decart_queue::find_max()
{
    if (_head == nullptr)
        throw std::runtime_error("Queue is empty");

    char* result_str = nullptr;
    int priority = 0;
    remove_max_inner(_head, result_str, priority, false);
    return result_str;

}

char* decart_queue::remove_max() 
{
    if (_head == nullptr)
        throw std::runtime_error("Queue is empty");

    char* result_str = nullptr;
    int priority = 0;
    _head = remove_max_inner(_head, result_str, priority, true);
    --_size;
    return result_str;
}

char* decart_queue::remove_max(int& result_priority)
{
    if (_head == nullptr)
        throw std::runtime_error("Queue is empty");

    char* result_str = nullptr;
    _head = remove_max_inner(_head, result_str, result_priority, true);
    --_size;
    return result_str;
}

decart_queue::node* decart_queue::remove_max_inner(node* current_node, char*& result_str, int& result_priority, bool delete_or_no) 
{
    if (current_node->right_subtree == nullptr) 
    {
        result_str = new char[std::strlen(current_node->data) + 1];
        std::strcpy(result_str, current_node->data);
        result_priority = current_node->priority;

        if (delete_or_no)
        {
            node* temp = current_node;
            current_node = current_node->left_subtree;
            delete[] temp->data;
            delete temp;
        }

        return current_node;
    }

    current_node->right_subtree = remove_max_inner(current_node->right_subtree, result_str, result_priority, delete_or_no);

    if (current_node->left_subtree && (!current_node->right_subtree || current_node->left_subtree->r_priority > current_node->right_subtree->r_priority)) 
    {
        right_rotation(current_node);
    }
    else if (current_node->right_subtree && current_node->right_subtree->r_priority > current_node->r_priority) 
    {
        left_rotation(current_node);
    }

    return current_node;
}


priority_queue* decart_queue::merge(priority_queue* q)
{
    if (this == nullptr)
        return q;
    if (q == nullptr)
        return this;

    decart_queue* cast_q = dynamic_cast<decart_queue*>(q);
    if (cast_q == nullptr)
    {
        throw std::invalid_argument("Invalid queue type in merge");
    }

    int priority = 0;
    while (cast_q->get_size() != 0)
    {
        char* str = cast_q->remove_max(priority);
        insert(str, priority);
    }
    return this;
}