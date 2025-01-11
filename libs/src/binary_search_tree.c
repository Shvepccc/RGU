#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include "../../libs/binary_searsh_tree.h"
#include "../../libs/error.h"

#include <string.h>

int swap(
    void* first,
    void* second,
    size_t bytes_to_swap_count)
{
    int i;
    for (i = 0; i < bytes_to_swap_count; ++i) {
        *((unsigned char*)first + i) ^= *((unsigned char*)second + i);
        *((unsigned char*)second + i) ^= *((unsigned char*)first + i);
        *((unsigned char*)first + i) ^= *((unsigned char*)second + i);
    }

    return 0;
}

int initialize_bst(
    p_bst bst_ptr,
    comparer keys_comparer,
    binary_search_tree_disposal_strategy disposal_strategy,
    void(*destructor)(TKey*, TValue*))
{
    if (bst_ptr == NULL || keys_comparer == NULL)
    {
        return NULL_POINTER;
    }

    bst_ptr->root = NULL;
    bst_ptr->keys_comparer = keys_comparer;
    bst_ptr->disposal_strategy = disposal_strategy;
    bst_ptr->destructor = destructor;
    
    return 0;
}

int set_disposal_strategy(
    p_bst bst_ptr,
    binary_search_tree_disposal_strategy disposal_strategy)
{
    if (bst_ptr == NULL)
    {
        return NULL_POINTER;
    }

    bst_ptr->disposal_strategy = disposal_strategy;

    return OK;
}

int deinitialize_bst(
    p_bst bst_ptr)
{
    if (bst_ptr == NULL)
        return NULL_POINTER;

    bst_deinitialize_inner(bst_ptr->root, bst_ptr->destructor);
    bst_ptr->root = NULL;
    return OK;
}

int insert_into_bst(
    p_bst bst_ptr,
    TKey bst_key,
    TValue const* value_to_insert_ptr) {

    int comparer_result;

    if (bst_ptr == NULL || value_to_insert_ptr == NULL) {
        return NULL_POINTER;
    }

    pp_bst_item iterator = &(bst_ptr->root);

    while (*iterator != NULL) {
        //printf("%s %s = %d\n", bst_key, (*iterator)->key, strcmp(bst_key, (*iterator)->key));
        comparer_result = bst_ptr->keys_comparer(&(*iterator)->key, &bst_key);
        if (comparer_result == 0)
        {
            return VALUE_ALREADY_EXISTS;
        }

        iterator = comparer_result < 0
            ? &(*iterator)->left_subtree
            : &(*iterator)->right_subtree;
    }

    if ((*iterator = (p_bst_item)malloc(sizeof(bst_item))) == NULL) {
        return MEMORY_ALLOCATE_ERROR;
    }

    (*iterator)->left_subtree = NULL;
    (*iterator)->right_subtree = NULL;

    //memcpy(&((*iterator)->key), &bst_key, sizeof(TKey));
    (*iterator)->key = string_cpy_new(bst_key);
    memcpy(&((*iterator)->value), value_to_insert_ptr, sizeof(TValue));

    return  0;
}

int obtain_from_bst(
    binary_search_tree const* bst_ptr,
    TKey bst_key,
    TValue** bst_value_storage_ptr) {
    p_bst_item current_node_ptr;
    int keys_comparison_result = 0;

    if (bst_ptr == NULL || bst_value_storage_ptr == NULL) {
        return NULL_POINTER;
    }

    current_node_ptr = bst_ptr->root;

    while (current_node_ptr != NULL) {
        //printf("%s %s = %d\n", bst_key, current_node_ptr->key, strcmp(bst_key, current_node_ptr->key));
        keys_comparison_result = bst_ptr->keys_comparer(&current_node_ptr->key, &bst_key);

        if (keys_comparison_result == 0) {
            //printf(" ---------- %s %d\n", bst_key, current_node_ptr->value);
            *bst_value_storage_ptr = &current_node_ptr->value;
            return OK;
        }

        current_node_ptr = keys_comparison_result > 0
            ? current_node_ptr->right_subtree
            : current_node_ptr->left_subtree;
    }

    return KEY_NOT_FOUND;
}

int dispose_from_bst(
    p_bst bst_ptr,
    TKey bst_key)
{
    pp_bst_item current_node_ptr, for_swap_node_ptr;
    p_bst_item temporary_node_ptr;
    int keys_comparison_result = 0;

    if (bst_ptr == NULL) {
        return NULL_POINTER;
    }

    current_node_ptr = &bst_ptr->root;

    while (current_node_ptr != NULL) {
        keys_comparison_result = bst_ptr->keys_comparer(&bst_key, &(*current_node_ptr)->key);

        if (keys_comparison_result == 0)
        {
            if ((*current_node_ptr)->left_subtree != NULL &&
                (*current_node_ptr)->right_subtree != NULL)
            {
                for_swap_node_ptr = bst_ptr->disposal_strategy == swap_with_max_of_left_subtree
                    ? &(*current_node_ptr)->left_subtree
                    : &(*current_node_ptr)->right_subtree;
                while ((bst_ptr->disposal_strategy == swap_with_max_of_left_subtree
                    ? (*for_swap_node_ptr)->right_subtree
                    : (*for_swap_node_ptr)->left_subtree) != NULL)
                {
                    for_swap_node_ptr = bst_ptr->disposal_strategy == swap_with_max_of_left_subtree
                        ? &(*for_swap_node_ptr)->right_subtree
                        : &(*for_swap_node_ptr)->left_subtree;
                }

                swap((void*)&(*current_node_ptr)->key, (void*)&(*for_swap_node_ptr)->key, sizeof(TKey));
                swap((void*)&(*current_node_ptr)->value, (void*)&(*for_swap_node_ptr)->value, sizeof(TValue));

                current_node_ptr = for_swap_node_ptr;
            }

            if ((*current_node_ptr)->left_subtree == NULL &&
                (*current_node_ptr)->right_subtree == NULL) {
                free(*current_node_ptr);
                *current_node_ptr = NULL;
            }
            else {
                temporary_node_ptr = (*current_node_ptr)->left_subtree == NULL
                    ? (*current_node_ptr)->right_subtree
                    : (*current_node_ptr)->left_subtree;
                free(*current_node_ptr);
                *current_node_ptr = temporary_node_ptr;
            }

            return OK;
        }

        current_node_ptr = keys_comparison_result > 0
            ? &(*current_node_ptr)->right_subtree
            : &(*current_node_ptr)->left_subtree;
    }

    return KEY_NOT_FOUND;
}

void bst_prefix_traversion_inner(
    bst_item const* subtree,
    void (*callback)(TValue const*)) {
    if (subtree == NULL) {
        return;
    }
    callback(&subtree->value);
    bst_prefix_traversion_inner(subtree->left_subtree, callback);
    bst_prefix_traversion_inner(subtree->right_subtree, callback);
}

void bst_infix_traversion_inner(
    bst_item const* subtree,
    void (*callback)(TValue const*)) {
    if (subtree == NULL) {
        return;
    }
    bst_prefix_traversion_inner(subtree->left_subtree, callback);
    callback(&subtree->value);
    bst_prefix_traversion_inner(subtree->right_subtree, callback);
}

void bst_postfix_traversion_inner(
    bst_item const* subtree,
    void (*callback)(TValue const*)) {
    if (subtree == NULL) {
        return;
    }
    bst_prefix_traversion_inner(subtree->left_subtree, callback);
    bst_prefix_traversion_inner(subtree->right_subtree, callback);
    callback(&subtree->value);
}

int bst_prefix_traversion(
    bst const* bst_ptr,
    void (*callback)(TValue const*)) {

    bst_prefix_traversion_inner(bst_ptr->root, callback);
    return 0;
}

int bst_infix_traversion(
    bst const* bst_ptr,
    void (*callback)(TValue const*)) {
    bst_infix_traversion_inner(bst_ptr->root, callback);
    return 0;
}

int bst_postfix_traversion(
    bst const* bst_ptr,
    void (*callback)(TValue const*)) {
    bst_postfix_traversion_inner(bst_ptr->root, callback);
    return 0;
}

void bst_deinitialize_inner(p_bst_item subtree, void(*destructor)(TKey*, TValue*)) {
    if (subtree == NULL)
        return;

    bst_deinitialize_inner(subtree->left_subtree, destructor);
    bst_deinitialize_inner(subtree->right_subtree, destructor);

    if (destructor != NULL) {
        destructor(&subtree->key, &subtree->value);
    }
    free(subtree);
}

int bst_get_depth(bst const* bst_ptr) {
    return bst_get_depth_inner(bst_ptr->root);;
}

int bst_get_depth_inner(p_bst_item subtree)
{
    if (subtree == NULL) {
        return 0;
    }

    int left, right;
    if (subtree->left_subtree != NULL) {
        left = bst_get_depth_inner(subtree->left_subtree);
    }
    else
        left = -1;
    if (subtree->right_subtree != NULL) {
        right = bst_get_depth_inner(subtree->right_subtree);
    }
    else
        right = -1;

    int max = left > right ? left : right;
    return max + 1;

}

void bst_save_to_txt_inner(
    bst_item const* subtree, FILE* file) {
    if (subtree == NULL) {
        return;
    }
    fprintf(file, "%s %d\n", subtree->key, subtree->value);

    bst_save_to_txt_inner(subtree->left_subtree, file);
    
    bst_save_to_txt_inner(subtree->right_subtree, file);
}

int bst_save_to_txt(bst const* bst_ptr, char* filename) {
    FILE* file;
    if ((file = fopen(filename, "w")) == NULL) {
        return OPEN_FILE_ERROR;
    }
    bst_save_to_txt_inner(bst_ptr->root, file);
    fclose(file);
}

void print_tree_2(int* data) {
    printf("%d\n", *data);
}

int bst_load_from_txt(char* filename, bst* bst_ptr) {
    FILE* file;
    char temp_str[BUFSIZ];
    char temp_read_str[BUFSIZ];
    int temp_read_int = 0, read_flag = 0;
    string temp_string;

    if ((file = fopen(filename, "r")) == NULL) {
        return OPEN_FILE_ERROR;
    }

    while ((fgets(temp_str, BUFSIZ, file)) != NULL) {
        read_flag = 1;
        sscanf(temp_str, "%s %d", temp_read_str, &temp_read_int);

        temp_string = string_from(temp_read_str, strlen(temp_read_str));

        insert_into_bst(bst_ptr, temp_string, &temp_read_int);
        string_free(&temp_string);
    }
    fclose(file);
    if (!read_flag) {
        return EMPTY_FILE;
    }
    return 0;
}

void print_beautiful_bst_inner(p_bst_item node, int level, char* prefix) {
    if (node == NULL) {
        return;
    }

    char new_prefix[BUFSIZ];
    snprintf(new_prefix, sizeof(new_prefix), "%s    ", prefix);
    print_beautiful_bst_inner(node->right_subtree, level + 1, new_prefix);
    printf("%s|-- %s (%d)\n", prefix, node->key, node->value);
    print_beautiful_bst_inner(node->left_subtree, level + 1, new_prefix);
}

int bst_print_bautiful(binary_search_tree const* bst_ptr) {
    if (bst_ptr == NULL || bst_ptr->root == NULL) {
        return NULL_POINTER;
    }
    print_beautiful_bst_inner(bst_ptr->root, 0, "");
}

//#define MAX_LEVELS 20
//#define SPACING 2
//#define WIDTH_CONCOLE 100
//
//void print_level(p_bst_item node, int level, int depth, int offset, int width, char** buffer, int spacing) {
//    if (node == NULL || level >= depth) {
//        return;
//    }
//
//    int middle = offset + width / 2;
//
//    char temp[50];
//    snprintf(temp, sizeof(temp), "%s (%d)", node->key, node->value);
//    strncpy(buffer[level] + middle - (strlen(temp) / 2), temp, strlen(temp));
//
//    if (node->left_subtree) {
//        buffer[level + 1][middle - spacing] = '/';
//        print_level(node->left_subtree, level + 2, depth, offset, width / 2, buffer, spacing);
//    }
//    if (node->right_subtree) {
//        buffer[level + 1][middle + spacing] = '\\';
//        print_level(node->right_subtree, level + 2, depth, offset + width / 2, width / 2, buffer, spacing);
//    }
//}
//
//void bst_print_tree(p_bst bst_ptr) {
//    if (bst_ptr == NULL) {
//        printf("Tree is empty.\n");
//        return;
//    }
//    int spacing = 0;
//    int depth = bst_get_depth(bst_ptr) + 1;
//    if (depth <= 5) {
//        spacing = 1;
//    }
//    else if (depth > 5 && depth <= 10) {
//        spacing = 3;
//    }
//    else {
//        spacing = 4;
//    }
//
//    char** buffer = (char**)malloc(sizeof(char*) * MAX_LEVELS);
//    for (int i = 0; i < MAX_LEVELS; ++i) {
//        buffer[i] = (char*)malloc(sizeof(char) * WIDTH_CONCOLE);
//        memset(buffer[i], ' ', WIDTH_CONCOLE);
//    }
//
//    print_level(bst_ptr->root, 0, depth, 0, (1 << depth) * spacing, buffer, spacing);
//
//    for (int i = 0; i < depth; ++i) {
//        buffer[i][WIDTH_CONCOLE - 1] = '\0';
//        printf("%s\n", buffer[i]);
//    }
//
//    for (int i = 0; i < MAX_LEVELS; ++i) {
//        free(buffer[i]);
//    }
//    free(buffer);
//}