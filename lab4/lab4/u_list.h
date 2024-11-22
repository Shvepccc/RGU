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
} u_list;

u_list u_list_init(void (*destructor)(void*));
void u_list_free(u_list* list_ptr);

int u_list_insert(u_list* list_ptr, void* element, size_t index_to_insert);

#endif