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

    auto comparison_result = strcmp(data, current_node->data);
    if (comparison_result == 0)
    {
        return false;
    }

    auto recursive_insertion_status = insert_inner(data, key, randomized_priority, comparison_result < 0
        ? current_node->left_subtree
        : current_node->right_subtree);

    if (recursive_insertion_status)
    {
        if (comparison_result < 0 && current_node->left_subtree->priority < current_node->priority)
        {
            right_rotation(current_node);
        }
        else if (comparison_result > 0 && current_node->right_subtree->priority < current_node->priority)
        {
            left_rotation(current_node);
        }
    }

    return recursive_insertion_status;
}

char* decart_queue::find_max()
{
    if (_head == nullptr)
        throw std::runtime_error("Queue is empty");
    return _head->data;
}

char* decart_queue::remove_max()
{
    if (_head == nullptr)
        throw std::runtime_error("Queue is empty");

    char* result_str = nullptr;
    auto is_removed = remove_inner(&result_str, _head, removal_status::heap_sift_down);
    --_size;
    return result_str;
}

//TODO: write merge
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
    
    this->_head = merge_inner(this->_head, cast_q->_head);
    this->_size += cast_q->_size;
    q = nullptr;
    return this;
}

decart_queue::node* decart_queue::merge_inner(node* a, node* b)
{
    if (a == nullptr)
        return b;
    if (b == nullptr)
        return a;


    if (a->priority  > b->priority)
    {
        a->right_subtree = merge_inner(a->right_subtree, b);
    }
    else
    {
        b->left_subtree = merge_inner(a, b->left_subtree);
    }
    return b;
}

bool decart_queue::remove_inner(
    char** removed_value,
    node*& current_node,
    removal_status status)
{
    if (current_node == nullptr)
    {
        return false;
    }
   
    if (status == removal_status::heap_sift_down)
    {
        if (current_node->left_subtree == nullptr && current_node->right_subtree == nullptr)
        {
            *removed_value = new char[std::strlen(current_node->data) + 1];
            std::strcpy(*removed_value, current_node->data);

            destroy_node(current_node, false);
            current_node = nullptr;
            return true;
        }
        else if (current_node->left_subtree != nullptr && current_node->right_subtree != nullptr)
        {
            if (current_node->left_subtree->priority < current_node->right_subtree->priority)
            {
                //QUESTION: why left, not right?
                left_rotation(current_node);
                return remove_inner(removed_value, current_node->left_subtree, removal_status::heap_sift_down);
            }
            else
            {
                right_rotation(current_node);
                return remove_inner(removed_value, current_node->right_subtree, removal_status::heap_sift_down);
            }
        }
        else if (current_node->left_subtree != nullptr)
        {
            //QUESTION: why right, not left?
            right_rotation(current_node);
            return remove_inner(removed_value, current_node->right_subtree, removal_status::heap_sift_down);
        }
        else
        {
            left_rotation(current_node);
            return remove_inner(removed_value, current_node->left_subtree, removal_status::heap_sift_down);
        }
    }

    return false;
}