#ifndef PROGRAMMINGSEMINARS_BINARY_SEARCH_TREE_H
#define PROGRAMMINGSEMINARS_BINARY_SEARCH_TREE_H

#include "../../libs/cust_string.h"

typedef string TKey;
typedef int TValue;
typedef int (*comparer)(TKey const*, TKey const*);
typedef void (*value_handle_callback)(TValue const*);

typedef struct bst_item {
    TKey key;
    TValue value;
    struct bst_item* left_subtree;
    struct bst_item* right_subtree;
} bst_item, * p_bst_item, ** pp_bst_item;

typedef enum binary_search_tree_disposal_strategy {
    swap_with_max_of_left_subtree,
    swap_with_min_of_right_subtree
} binary_search_tree_disposal_strategy;

typedef struct binary_search_tree {
    p_bst_item root;
    comparer keys_comparer;
    binary_search_tree_disposal_strategy disposal_strategy;
    void(*destructor)(TKey*, TValue*);
} binary_search_tree, bst, * p_bst;

int initialize_bst(
    p_bst bst_ptr,
    comparer keys_comparer,
    binary_search_tree_disposal_strategy disposal_strategy,
    void(*destructor)(TKey*, TValue*));

int set_disposal_strategy(
    p_bst bst_ptr,
    binary_search_tree_disposal_strategy disposal_strategy);

int deinitialize_bst(
    p_bst bst_ptr);

int insert_into_bst(
    p_bst bst_ptr,
    TKey bst_key,
    TValue const* value_to_insert_ptr);

int obtain_from_bst(
    binary_search_tree const* bst_ptr,
    TKey bst_key,
    TValue** bst_value_storage_ptr);

int dispose_from_bst(
    p_bst bst_ptr,
    TKey bst_key);

int bst_prefix_traversion(
    bst const* bst_ptr,
    value_handle_callback callback);

int bst_infix_traversion(
    bst const* bst_ptr,
    value_handle_callback callback);

int bst_postfix_traversion(
    bst const* bst_ptr,
    value_handle_callback callback);

void bst_deinitialize_inner(p_bst_item, void(*destructor)(TKey*, TValue*));

int bst_get_depth(bst const* bst_ptr);

int bst_save_to_txt(bst const* bst_ptr, char* name_to_save);

int bst_load_from_txt(char* filename, bst* bst_ptr);

int bst_print_bautiful(binary_search_tree const* bst_ptr);

void bst_print_tree(p_bst bst_ptr);

#endif //PROGRAMMINGSEMINARS_BINARY_SEARCH_TREE_H