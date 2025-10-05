#include <stdlib.h>
#include <string.h>
#include "../include/u_list.h"
#include "../include/error.h"

#include <stdio.h>

u_list u_list_init(void (*destructor)(void*), size_t size_of_element) {
	u_list list;
	list.first_node = NULL;
	list.destructor = destructor;
	list.size_of_element = size_of_element;
	list.size = 0;
	return list;
}

void u_list_free(u_list* list_ptr, int free_list) {
	u_list_node *item, *next = NULL;
	int mode = 0;
	if (list_ptr == NULL) {
		return;
	}
	if (list_ptr->destructor != NULL) { mode = 1; }

	item = list_ptr->first_node;
	while (item != NULL) {
		if (mode && item->data != NULL) {
			list_ptr->destructor(item->data);
		}
		next = item->next_node;
		free(item);
		item = next;
	}
	list_ptr->first_node = NULL;
	if (free_list)
	{
		free(list_ptr);
		list_ptr = NULL;
	}
}

int u_list_insert(u_list* list_ptr, void* element, size_t index_to_insert) {
	u_list_node* new_node, *temp_ptr;
	int i;
	if (list_ptr == NULL || element == NULL) {
		return NULL_POINTER;
	}

	new_node = (u_list_node*)malloc(sizeof(u_list_node));
	if (new_node == NULL) {
		return MEMORY_ALLOCATE_ERROR;
	}

	new_node->data = malloc(list_ptr->size_of_element);
	if (new_node->data == NULL) {
		free(new_node);
		return MEMORY_ALLOCATE_ERROR;
	}
	memcpy(new_node->data, element, list_ptr->size_of_element);

	if (index_to_insert == 0) {
		temp_ptr = list_ptr->first_node;
		list_ptr->first_node = new_node;
		new_node->next_node = temp_ptr;
	}
	else {
		temp_ptr = list_ptr->first_node;
		for (i = 0; i < index_to_insert-1; i++) {
			temp_ptr = temp_ptr->next_node;
			if (temp_ptr == NULL) {
				element = NULL;
				if (list_ptr->destructor != NULL) {
					list_ptr->destructor(new_node);
				}
				else {
					free(new_node->data);
					free(new_node);
				}
				return INDEX_OUT_OF_BORDERS;
			}
		}

		new_node->next_node = temp_ptr->next_node;
		temp_ptr->next_node = new_node;
	}
	list_ptr->size++;
	return OK;
}

int u_list_push_back(u_list* list_ptr, void* element) {
	u_list_node* new_node, * temp_ptr;
	int i;
	if (list_ptr == NULL || element == NULL) {
		return NULL_POINTER;
	}

	new_node = (u_list_node*)malloc(sizeof(u_list_node));
	if (new_node == NULL) {
		return MEMORY_ALLOCATE_ERROR;
	}
	new_node->data = (void *)malloc(list_ptr->size_of_element);
	if (new_node->data == NULL) {
		free(new_node);
		return MEMORY_ALLOCATE_ERROR;
	}
	memcpy(new_node->data, element, list_ptr->size_of_element);

	if (list_ptr->first_node == NULL) {
		list_ptr->first_node = new_node;
		new_node->next_node = NULL;
	}
	else {
		temp_ptr = list_ptr->first_node;
		while (temp_ptr != NULL) {
			if (memcmp(temp_ptr->data, new_node->data, list_ptr->size_of_element) == 0) {
				return VALUE_ALREADY_EXISTS;
			}
			if (temp_ptr->next_node == NULL) {
				break;
			}
			temp_ptr = temp_ptr->next_node;
    	}

		new_node->next_node = temp_ptr->next_node;
		temp_ptr->next_node = new_node;
	}
	list_ptr->size++;
	return OK;
}

int u_list_delete_by_index(u_list* list_ptr, void* element, size_t index_to_delete) {
	u_list_node* new_node, * temp_ptr, *item;
	int i, mode = 0;
	if (list_ptr == NULL) {
		return NULL_POINTER;
	}

	if (index_to_delete == 0) {
		temp_ptr = list_ptr->first_node->next_node;

		if (list_ptr->destructor != NULL) { mode = 1; }
		if (element != NULL) {
			memcpy(element, list_ptr->first_node->data, list_ptr->size_of_element);
		}
		if (mode) {
			list_ptr->destructor(list_ptr->first_node->data);
			free(list_ptr->first_node);
		}
		else {
			free(list_ptr->first_node);
		}
		list_ptr->first_node = temp_ptr;
	}
	else {

		temp_ptr = list_ptr->first_node;
		for (i = 0; i < (int)index_to_delete -1; i++) {
			temp_ptr = temp_ptr->next_node;
			if (temp_ptr == NULL) {
				element = NULL;
				return INDEX_OUT_OF_BORDERS;
			}
		}

		item = temp_ptr->next_node;
		temp_ptr->next_node = temp_ptr->next_node->next_node;
		if (list_ptr->destructor != NULL) { mode = 1; }
		if (element != NULL) {
			memcpy(element, item->data, list_ptr->size_of_element);
		}
		if (mode) {
			list_ptr->destructor(item->data);
			free(item);
		}
		else {
			free(item);
		}
	}
	list_ptr->size--;
	return OK;
}

int u_list_get_by_index(u_list* list_ptr, void* element, size_t index_to_get) {
	u_list_node* temp_ptr;
	int i = 0;
	if (list_ptr == NULL || element == NULL) {
		return NULL_POINTER;
	}

	temp_ptr = list_ptr->first_node;
	for (i = 0; i < index_to_get - 1; i++) {
		temp_ptr = temp_ptr->next_node;
		if (temp_ptr == NULL) {
			element = NULL;
			return INDEX_OUT_OF_BORDERS;
		}
	}
	memcpy(element, temp_ptr->data, list_ptr->size_of_element);
	return 0;
}

int u_list_traverse(u_list* list_ptr, int (*traverce_function)(u_list_node const*)) {
	u_list_node* temp_ptr;

	temp_ptr = list_ptr->first_node;
	while (temp_ptr != NULL) {
		traverce_function(temp_ptr);
		temp_ptr = temp_ptr->next_node;
	}
	return 0;
}

u_list_node* __sortedMerge(u_list_node* a, u_list_node* b, int (*comparator)(u_list_node*, u_list_node*)) {
	if (!a) return b;
	if (!b) return a;

	u_list_node* result = NULL;

	if (comparator(a, b) <= 0) {
		result = a;
		result->next_node = __sortedMerge(a->next_node, b, comparator);
	}
	else {
		result = b;
		result->next_node = __sortedMerge(a, b->next_node, comparator);
	}

	return result;
}

void __splitList(u_list_node* source, u_list_node** frontRef, u_list_node** backRef) {
	u_list_node* slow = source;
	u_list_node* fast = source->next_node;

	while (fast) {
		fast = fast->next_node;
		if (fast) {
			slow = slow->next_node;
			fast = fast->next_node;
		}
	}

	*frontRef = source;
	*backRef = slow->next_node;
	slow->next_node = NULL;
}

void __mergeSort(u_list_node** headRef, int (*comparator)(u_list_node*, u_list_node*)) {
	u_list_node* head = *headRef;
	if (!head || !head->next_node) {
		return;
	}

	u_list_node* a;
	u_list_node* b;

	__splitList(head, &a, &b);

	__mergeSort(&a, comparator);
	__mergeSort(&b, comparator);

	*headRef = __sortedMerge(a, b, comparator);
}

int u_list_mergeSort(u_list* list_ptr, int (*comparator)(u_list_node*, u_list_node*), int skip_first) {
	if (skip_first) {
		__mergeSort(&(list_ptr->first_node->next_node), comparator);
	}
	else {
		__mergeSort(&(list_ptr->first_node), comparator);
	}
	return 0;
}

int u_list_delete_by_value(u_list* list_ptr, void* value, int (*compare)(const void*, const void*)) {
    u_list_node *current, *prev;
    int mode = 0;
    
    if (list_ptr == NULL || value == NULL || compare == NULL) {
        return NULL_POINTER;
    }

    if (list_ptr->first_node == NULL) {
        return INDEX_OUT_OF_BORDERS;
    }

    current = list_ptr->first_node;
    if (compare(current->data, value) == 0) {
        list_ptr->first_node = current->next_node;
        
        if (list_ptr->destructor != NULL) {
            mode = 1;
        }
        
        if (mode) {
            list_ptr->destructor(current->data);
            free(current);
        } else {
            free(current->data);
            free(current);
        }
        
        list_ptr->size--;
        return OK;
    }

    prev = list_ptr->first_node;
    current = list_ptr->first_node->next_node;
    
    while (current != NULL) {
        if (compare(current->data, value) == 0) {
            prev->next_node = current->next_node;
            
            if (list_ptr->destructor != NULL) {
                mode = 1;
            }
            
            if (mode) {
                list_ptr->destructor(current->data);
                free(current);
            } else {
                free(current->data);
                free(current);
            }
            
            list_ptr->size--;
            return OK;
        }
        
        prev = current;
        current = current->next_node;
    }

    return INDEX_OUT_OF_BORDERS;
}

int u_list_contains(u_list* list_ptr, void* value, int (*compare)(const void*, const void*), int* result) {
    u_list_node* temp_ptr;
    
    if (list_ptr == NULL || value == NULL || compare == NULL || result == NULL) {
        return NULL_POINTER;
    }

    temp_ptr = list_ptr->first_node;
    while (temp_ptr != NULL) {
        if (compare(temp_ptr->data, value) == 0) {
            *result = 1;
            return OK;
        }
        temp_ptr = temp_ptr->next_node;
    }

    *result = 0;
    return OK;
}

size_t u_list_size(u_list* list_ptr) {
    if (list_ptr == NULL) {
        return 0;
    }
    return list_ptr->size;
}