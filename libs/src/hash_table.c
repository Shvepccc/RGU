#include "../../libs/hash_table.h"
#include "../../libs/error.h"
#include <stdlib.h>
#include <string.h>

void handle_collision(hash_table* hash_table, unsigned long index, hash_table_item* item) {
	if (hash_table == NULL) {
		return NULL_POINTER;
	}
	u_list* head, list;

	head = hash_table->overflow_buckets[index];

	if (head == NULL) {
		// Create the list
		list = u_list_init(hash_table->destructor, sizeof(hash_table_item));
		u_list_push_back(&list, item);
		hash_table->overflow_buckets[index] = &list;
		return;
	}
	else {
		// Insert to the list
		u_list_push_back(&head, item);
		return;
	}
}

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

void free_overflow_buckets(hash_table* hash_table) {
	if (hash_table == NULL) {
		return NULL_POINTER;
	}
	u_list** buckets = hash_table->overflow_buckets;
	for (int i = 0; i < hash_table->size; i++)
		u_list_free(buckets[i]);
	free(buckets);
}

hash_table* ht_init(int start_size,
	size_t size_of_key,
	size_t size_of_value,
	int (*hash_function)(void*),
	void (*destructor)(void*),
	int (*keys_comparer)(void*, void*))
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

	table->items = (hash_table_item*)calloc(start_size, sizeof(hash_table_item));
	for (i = 0; i < start_size; i++) {
		table->items[i] = NULL;
	}

	table->overflow_buckets = create_overflow_buckets(table);
	
	return table;
}

int ht_free(hash_table* hash_table) {

	if (hash_table == NULL) {
		return NULL_POINTER;
	}

	for (int i = 0; i < hash_table->size; i++) {
		hash_table_item* item = hash_table->items[i];
		if (item != NULL) {
			if (hash_table->destructor != NULL ) {
				hash_table->destructor(item);
			}
			else {
				free(item->key);
				free(item->value);
			}
			free(item);
		}
	}

	free_overflow_buckets(hash_table);
	free(hash_table->items);
	free(hash_table);

	return OK;
}

hash_table_item* ht_create_item(hash_table* hash_table, void* key, void* value) {

	if (hash_table == NULL || key == NULL || value == NULL) {
		return NULL_POINTER;
	}

	hash_table_item* item = (hash_table_item*)malloc(sizeof(hash_table_item));
	if (item == NULL) {
		return MEMORY_ALLOCATE_ERROR;
	}

	item->key = (void*)malloc(hash_table->size_of_key);
	if (item->key == NULL) {
		free(item);
		return MEMORY_ALLOCATE_ERROR;
	}

	item->value = (void*)malloc(hash_table->size_of_value);
	if (item->value == NULL) {
		free(item->key);
		free(item);
		return MEMORY_ALLOCATE_ERROR;
	}

	memcpy(item->key, key, hash_table->size_of_key);
	memcpy(item->value, value, hash_table->size_of_value);

	return item;
}

int ht_insert(hash_table* hash_table, void* key, void* value) {
	int index;
	hash_table_item* current_item, * item;

	if (hash_table == NULL || key == NULL || value == NULL) {
		return NULL_POINTER;
	}

	item = ht_create_item(hash_table, key, value);

	index = hash_table->hash_function(key);
	current_item = hash_table->items[index];

	if (current_item == NULL) {
		// Create new item

		// TODO: realloc hash table

		hash_table->items[index] = item;
		hash_table->count++;
	}
	else if (hash_table->keys_comparer(current_item->key, key) == 0) {
		// Update value in item
		memcpy(hash_table->items[index]->value, value, hash_table->size_of_value);
	}
	else {
		// Collision
		handle_collision(hash_table, index, item);
	}

	return OK;
}

int ht_search(hash_table* hash_table, void* key, void* result) {
	int index;
	hash_table_item* item;
	u_list_node* temp_ptr = NULL;

	if (hash_table == NULL || key == NULL || result == NULL) {
		return NULL_POINTER;
	}

	index = hash_table->hash_function(key);
	item = hash_table->items[index];
	
	u_list* list = hash_table->overflow_buckets[index];
	if (list != NULL) temp_ptr = list->first_node;
	
	while (item != NULL) {
		if (hash_table->keys_comparer(item->key, key) == 0) {
			memcpy(result, item->value, hash_table->size_of_value);
			return OK;
		}
		if (temp_ptr == NULL) return KEY_NOT_FOUND;
	
		item = temp_ptr->data;
		if (list != NULL) temp_ptr = temp_ptr->next_node;
	}
	return KEY_NOT_FOUND;
}

int ht_delete(hash_table* hash_table, void* key) {
	int index, deleting_index = 0;
	hash_table_item* current_item, * item;
	u_list_node* temp_ptr;

	if (hash_table == NULL || key == NULL) {
		return NULL_POINTER;
	}

	index = hash_table->hash_function(key);
	item = hash_table->items[index];

	u_list* list = hash_table->overflow_buckets[index];

	if (item == NULL) {
		return KEY_NOT_FOUND;
	}
	else if (list == NULL && hash_table->keys_comparer(item->key, key) == 0) {
		// No collision. Delete item and set table index to NULL
		hash_table->items[index] = NULL;
		if (hash_table->destructor != NULL ) hash_table->destructor(item);
		free(item);
		hash_table->count--;
	}
	else if (list != NULL) {
		// We have collision. Find item and delete it
		if (list->first_node->next_node == NULL) {
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
		hash_table->overflow_buckets[index] = NULL;
	}
	return OK;
}

void ht_print(hash_table* hash_table) {
	if (hash_table == NULL) {
		return NULL_POINTER;
	}
	hash_table_item* item;
	printf("\nHash Table\n-------------------\n");
	for (int i = 0; i < hash_table->count; i++) {
		if (hash_table->items[i]) {
			printf("Index:%d, Key:%s, Value:%s", i, hash_table->items[i]->key, hash_table->items[i]->value);
			if (hash_table->overflow_buckets[i]) {
				printf(" => Overflow Bucket => ");
				u_list* head = hash_table->overflow_buckets[i];
				u_list_node* temp_ptr = head->first_node;
				while (temp_ptr) {
					item = temp_ptr->data;
					printf("Key:%s, Value:%s ", item->key, item->value);
					temp_ptr = temp_ptr->next_node;
				}
			}
			printf("\n");
		}
	}
	printf("-------------------\n");
}