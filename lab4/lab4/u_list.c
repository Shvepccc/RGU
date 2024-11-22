#include <stdlib.h>
#include "u_list.h"
#include "../../libs/error.h"

u_list u_list_init(void (*destructor)(void*)) {
	u_list list;
	list.first_node = NULL;
	list.destructor = destructor;
	return list;
}

void u_list_free(u_list* list_ptr) {
	u_list_node *item, *temp_item;
	int mode = 0;
	if (list_ptr == NULL) {
		return;
	}
	if (list_ptr->destructor != NULL) { mode = 1; }
	item = list_ptr->first_node;
	while (item) {
		if (mode) {
			list_ptr->destructor(item->data);
		}
		temp_item = item;
		item = item->next_node;
		free(temp_item);
	}
	list_ptr->first_node = NULL;
}

int u_list_insert(u_list* list_ptr, void* element, size_t index_to_insert) {
	u_list_node* new_node, *temp_ptr;
	if (list_ptr == NULL || element == NULL) {
		return NULL_POINTER;
	}

	if (index_to_insert == 0) {
		new_node = (u_list_node*)malloc(sizeof(u_list_node));
		if (new_node == NULL) {
			return MEMORY_ALLOCATE_ERROR;
		}
		new_node->data = element;
		temp_ptr = list_ptr->first_node;
		list_ptr->first_node = new_node;
		new_node->next_node = temp_ptr;
	}
	else {
	
	}

	return OK;
}