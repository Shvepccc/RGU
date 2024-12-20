#include <stdlib.h>
#include <string.h>
#include "../../libs/cust_stack.h"
#include "../../libs/error.h"

stack stack_init(void (*destructor)(void*), size_t size_of_element) {
	stack list;
	list.first_node = NULL;
	list.destructor = destructor;
	list.size_of_element = size_of_element;
	return list;
}

void stack_free(stack* stack_ptr) {
	stack_node *item, *temp_item;
	int mode = 0;
	if (stack_ptr == NULL) {
		return;
	}
	if (stack_ptr->destructor != NULL) { mode = 1; }
	item = stack_ptr->first_node;
	while (item) {
		if (mode) {
			stack_ptr->destructor(item->data);
		}
		temp_item = item;
		item = item->next_node;
		free(temp_item);
	}
	stack_ptr->first_node = NULL;
}

int stack_push_back(stack* stack_ptr, void* element) {
	stack_node* new_node, * temp_ptr;
	int i;
	if (stack_ptr == NULL || element == NULL) {
		return NULL_POINTER;
	}

	new_node = (stack_node*)malloc(sizeof(stack_node));
	if (new_node == NULL) {
		return MEMORY_ALLOCATE_ERROR;
	}
	new_node->data = malloc(stack_ptr->size_of_element);
	if (new_node->data == NULL) {
		free(new_node);
		return MEMORY_ALLOCATE_ERROR;
	}
	memcpy(new_node->data, element, stack_ptr->size_of_element);

	if (stack_ptr->first_node == NULL) {
		stack_ptr->first_node = new_node;
		new_node->next_node = NULL;
	}
	else {
		temp_ptr = stack_ptr->first_node;
		while (temp_ptr->next_node != NULL) {
			temp_ptr = temp_ptr->next_node;
		}

		new_node->next_node = temp_ptr->next_node;
		temp_ptr->next_node = new_node;
	}
	return OK;
}

int stack_pop(stack* stack_ptr, void* element) {
	stack_node* new_node, * temp_ptr, *last_temp_ptr = NULL, *item;
	int i, mode = 0;
	if (stack_ptr == NULL) {
		return NULL_POINTER;
	}

	temp_ptr = stack_ptr->first_node;
	if (stack_ptr->first_node == NULL) {
		element = NULL;
		return 0;
	}
	else if (temp_ptr->next_node == NULL) {
		item = temp_ptr;
		stack_ptr->first_node->next_node = temp_ptr->next_node;

		if (stack_ptr->destructor != NULL) { mode = 1; }
		if (element != NULL) {
			memcpy(element, item->data, stack_ptr->size_of_element);
		}
		if (mode) {
			stack_ptr->destructor(item);
		}
		else {
			free(item);
		}
	}
	else {
		while (temp_ptr->next_node != NULL) {
			last_temp_ptr = temp_ptr;
			temp_ptr = temp_ptr->next_node;
		}

		item = temp_ptr;
		last_temp_ptr->next_node = temp_ptr->next_node;

		if (stack_ptr->destructor != NULL) { mode = 1; }
		if (element != NULL) {
			memcpy(element, item->data, stack_ptr->size_of_element);
		}
		if (mode) {
			stack_ptr->destructor(item);
		}
		else {
			free(item);
		}
	}
	return OK;
}

int stack_get(stack* stack_ptr, void* element) {
	stack_node* temp_ptr;
	int i = 0;
	if (stack_ptr == NULL || element == NULL) {
		return NULL_POINTER;
	}

	temp_ptr = stack_ptr->first_node;
	while (temp_ptr->next_node != NULL) {
		temp_ptr = temp_ptr->next_node;
		if (temp_ptr == NULL) {
			element = NULL;
			return INDEX_OUT_OF_BORDERS;
		}
	}
	memcpy(element, temp_ptr->data, stack_ptr->size_of_element);
	return 0;
}