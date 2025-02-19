#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include <stdio.h>
#include "../../libs/u_list.h"

typedef	struct hash_table_item {
	void* key;
	void* value;
} hash_table_item;

typedef struct hash_table {
	//hash_table_item** items;
	u_list** overflow_buckets;
	int size;
	int count;
	size_t size_of_key;
	size_t size_of_value;
	void (*destructor)(void*);
	int (*hash_function)(void*);
	int (*keys_comparer)(void*, void*);
	int (*cust_cpy)(hash_table_item*, void*, void*);
} hash_table;

hash_table* ht_init(int start_size,
	size_t size_of_key,
	size_t size_of_value,
	int (*hash_function)(void*),
	void (*destructor)(void*),
	int (*keys_comparer)(void*, void*),
	int (*cust_cpy)(hash_table_item*, void*, void*));

int ht_free(hash_table* hash_table);

int ht_insert(hash_table* hash_table, void* key, void* value);
int ht_search(hash_table* hash_table, void* key, void* result);
int ht_search_cust_cpy(hash_table* hash_table, void* key, void* result,
	int(*cust_cpy_function)(void*, void*));
int ht_delete(hash_table* hash_table, void* key);

//void ht_print(hash_table* hash_table);
int ht_print(hash_table* hash_table, void(*print_function)(int index, void* key, void* value));

#endif
