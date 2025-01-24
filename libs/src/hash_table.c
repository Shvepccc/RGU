#include "../../libs/hash_table.h"
#include "../../libs/error.h"
#include "../../libs/cust_string.h"
#include <stdlib.h>
#include <string.h>

u_list** create_overflow_buckets(hash_table* hash_table) {
	if (hash_table == NULL) {
		return NULL_POINTER;
	}
	u_list** buckets = (u_list**)calloc(hash_table->size, sizeof(u_list*));
	for (int i = 0; i < hash_table->size; i++) {
		buckets[i] = NULL;
	}
	return buckets;
}

hash_table* ht_init(int start_size,
	size_t size_of_key,
	size_t size_of_value,
	int (*hash_function)(void*),
	void (*destructor)(void*),
	int (*keys_comparer)(void*, void*),
	int (*cust_cpy)(hash_table_item*, void*, void*))
{
	if (hash_function == NULL || keys_comparer == NULL) {
		return NULL_POINTER;
	}

	int i;
	hash_table* table = (hash_table*)malloc(sizeof(hash_table));
	if (table == NULL) {
		return MEMORY_ALLOCATE_ERROR;
	}

	table->size = start_size;
	table->size_of_key = size_of_key;
	table->size_of_value = size_of_value;
	table->destructor = destructor;
	table->hash_function = hash_function;
	table->keys_comparer = keys_comparer;
	table->count = 0;
	table->overflow_buckets = create_overflow_buckets(table);
	table->cust_cpy = cust_cpy;
	
	return table;
}

int ht_free(hash_table* hash_table) {
	if (hash_table == NULL) {
		return NULL_POINTER;
	}

	u_list** buckets = hash_table->overflow_buckets;
	for (int i = 0; i < hash_table->size; i++) {
		if (buckets[i]) {
			u_list_free(buckets[i]);
		}
	}
	free(buckets);
	free(hash_table);
	hash_table = NULL;

	return OK;
}

int ht_create_item_2(hash_table* hash_table, void* key, void* value, hash_table_item* item) {

	if (hash_table == NULL || key == NULL || value == NULL) {
		return NULL_POINTER;
	}

	(*item).key = (void*)malloc(hash_table->size_of_key);
	if ((*item).key == NULL) {
		return MEMORY_ALLOCATE_ERROR;
	}

	(*item).value = (void*)malloc(hash_table->size_of_value);
	if ((*item).value == NULL) {
		free((*item).key);
		return MEMORY_ALLOCATE_ERROR;
	}

	if (hash_table->cust_cpy != NULL) {
		hash_table->cust_cpy(item, key, value);
	}
	else {
		memcpy((*item).key, key, hash_table->size_of_key);
		memcpy((*item).value, value, hash_table->size_of_value);
	}

	return OK;
}

int ht_insert(hash_table* hash_table, void* key, void* value) {
	int index, err = 0;
	hash_table_item item;
	hash_table_item* item_ptr;
	u_list* list, new_list;
	//hash_table_item* item_ptr;
	u_list_node* temp_ptr = NULL;

	if (hash_table == NULL || key == NULL || value == NULL) {
		return NULL_POINTER;
	}

	err = ht_create_item_2(hash_table, key, value, &item);
	if (err) {
		return err;
	}

	index = hash_table->hash_function(key) % hash_table->size;
	list = hash_table->overflow_buckets[index];

	// TODO: realloc hash table

	if (list == NULL) {

		list = (u_list*)malloc(sizeof(u_list));
		if (list == NULL) {
			hash_table->destructor(&item);
			return MEMORY_ALLOCATE_ERROR;
		}
		
		*list = u_list_init(hash_table->destructor, sizeof(hash_table_item));
		u_list_push_back(list, &item);
		hash_table->overflow_buckets[index] = list;
		hash_table->count++;

	}
	else {
		temp_ptr = list->first_node;
		while (temp_ptr != NULL) {
			item_ptr = temp_ptr->data;
			if (hash_table->keys_comparer(item_ptr->key, key) == 0) {
				memcpy(item_ptr->value, item.value, hash_table->size_of_value);
				return OK;
			}
			temp_ptr = temp_ptr->next_node;
		}

		u_list_push_back(list, &item);
	}
	
	return OK;
}

int ht_search(hash_table* hash_table, void* key, void* result) {
	int index;
	hash_table_item* item;
	u_list_node* temp_ptr = NULL;

	if (hash_table == NULL || key == NULL) {
		return NULL_POINTER;
	}

	index = hash_table->hash_function(key) % hash_table->size;

	u_list* list = hash_table->overflow_buckets[index];
	if (list == NULL) return KEY_NOT_FOUND;
	
	temp_ptr = list->first_node;
	while (temp_ptr != NULL) {
		item = temp_ptr->data;
		if (hash_table->keys_comparer(item->key, key) == 0) {
			if (result != NULL) memcpy(result, item->value, hash_table->size_of_value);
			return OK;
		}
		temp_ptr = temp_ptr->next_node;
	}
	return KEY_NOT_FOUND;
}

int ht_search_cust_cpy(hash_table* hash_table, void* key, void* result,
	int(*cust_cpy_function)(void*, void*)) {
	int index;
	hash_table_item* item;
	u_list_node* temp_ptr = NULL;

	if (hash_table == NULL || key == NULL) {
		return NULL_POINTER;
	}

	index = hash_table->hash_function(key) % hash_table->size;

	u_list* list = hash_table->overflow_buckets[index];
	if (list == NULL) return KEY_NOT_FOUND;

	temp_ptr = list->first_node;
	while (temp_ptr != NULL) {
		item = temp_ptr->data;
		if (hash_table->keys_comparer(item->key, key) == 0) {
			if (result != NULL && cust_cpy_function != NULL) {
				cust_cpy_function(result, item->value);
			}
			return OK;
		}
		temp_ptr = temp_ptr->next_node;
	}
	return KEY_NOT_FOUND;
}

int ht_delete(hash_table* hash_table, void* key) {
	int index, deleting_index = 0;
	hash_table_item* current_item, * item;
	u_list_node* temp_ptr;
	u_list* list;

	if (hash_table == NULL || key == NULL) {
		return NULL_POINTER;
	}

	index = hash_table->hash_function(key) % hash_table->size;
	list = hash_table->overflow_buckets[index];

	if (list == NULL) {
		return KEY_NOT_FOUND;
	}
	else {
		// We have elements
		if (list->size == 1) {
			deleting_index = 0;
		}
		else {
			temp_ptr = list->first_node;
			while (temp_ptr != NULL) {
				item = temp_ptr->data;
				if (hash_table->keys_comparer(item->key, key) == 0) {
					break;
				}

				deleting_index++;
				temp_ptr = temp_ptr->next_node;
			}
		}
		u_list_delete_by_index(list, NULL, deleting_index);
		if (list->size == 0) {
			hash_table->overflow_buckets[index] = NULL;
		}
	}
	return OK;
}

int ht_print(hash_table* hash_table, void(*print_function)(int index, void* key, void* value)) {
	if (hash_table == NULL) {
		return NULL_POINTER;
	}
	hash_table_item* item;
	u_list* list;
	u_list_node* temp_ptr;

	printf("\n\nHash Table\n-------------------\n");
	for (int i = 0; i < hash_table->size; i++) {
		list = hash_table->overflow_buckets[i];
		if (list != NULL) {
			temp_ptr = list->first_node;
			while (temp_ptr != NULL) {
				item = temp_ptr->data;
				print_function(i, item->key, item->value);
				temp_ptr = temp_ptr->next_node;
			}
		}
	}
	printf("-------------------\n");
	return OK;
}