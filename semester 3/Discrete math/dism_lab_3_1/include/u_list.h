#ifndef UNIDIRECTIONAL_LIST_H_
#define UNIDIRECTIONAL_LIST_H_

#include <stdlib.h>

typedef struct u_list_node {
	void* data;
	struct u_list_node* next_node;
} u_list_node;

typedef struct u_list {
	u_list_node* first_node;
	void (*destructor)(void*);
	size_t size_of_element;
	size_t size;
} u_list;

u_list u_list_init(void (*destructor)(void*), size_t size_of_element);
void u_list_free(u_list* list_ptr, int free_list);

int u_list_insert(u_list* list_ptr, void* element, size_t index_to_insert);
int u_list_push_back(u_list* list_ptr, void* element);
int u_list_delete_by_index(u_list* list_ptr, void* element, size_t index_to_delete);
int u_list_get_by_index(u_list* list_ptr, void* element, size_t index_to_insert);
int u_list_delete_by_value(u_list* list_ptr, void* value, int (*compare)(const void*, const void*));

int u_list_contains(u_list* list_ptr, void* value, int (*compare)(const void*, const void*), int* result);
size_t u_list_size(u_list* list_ptr);

int u_list_traverse(u_list* list_ptr, int (*traverce_function)(u_list_node const*));
int u_list_mergeSort(u_list* list_ptr, int (*comparator)(u_list_node*, u_list_node*), int skip_first);

#endif