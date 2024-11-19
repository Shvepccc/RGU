#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../libs/error.h"
#include "../../libs/int_vector.h"

int_vector int_vector_init() {
	int_vec_metadata_t* vec = NULL;
	vec = (int_vec_metadata_t*)malloc((sizeof(int_vec_metadata_t)) + (sizeof(int) * INT_VECTOR_INITIAL_CAPACITY));
	if (vec == NULL) {
		return NULL;
	}
	vec->size = INT_VECTOR_INITIAL_CAPACITY;
	vec->capacity = INT_VECTOR_INITIAL_CAPACITY;
	return __int_vector_base_to_vec(vec);
}

int_vector int_vector_from(int* arr, size_t len) {
	int_vec_metadata_t* vec = NULL;
	vec = (int_vec_metadata_t*)malloc((sizeof(int_vec_metadata_t)) + (sizeof(int) * INT_VECTOR_INITIAL_CAPACITY));
	if (vec == NULL) {
		return NULL;
	}
	memcpy(__int_vector_base_to_vec(vec), arr, len * sizeof(int));
	vec->size = len;
	vec->capacity = len;
	return __int_vector_base_to_vec(vec);
}

void int_vector_free(int_vector vec) {
	free(__int_vector_vec_to_base(vec));
	return 0;
}

int int_vector_grow(int_vector* vec_ptr, size_t new_size) {
	if (vec_ptr == NULL || *vec_ptr == NULL) {
		return NULL_POINTER;
	}
	int_vec_metadata_t *for_realloc = NULL;
	size_t current_size = int_vector_size(*vec_ptr);
	if (current_size == new_size) {
		return OK;
	}

	for_realloc = (int_vec_metadata_t*)realloc(__int_vector_vec_to_base(*vec_ptr), (sizeof(int)*new_size) + (sizeof(int_vec_metadata_t)));
	if (for_realloc == NULL) {
		return MEMORY_ALLOCATE_ERROR;
	}

	for_realloc->capacity = new_size;
	if (new_size < current_size) {
		for_realloc->size = new_size;
	}

	*vec_ptr = __int_vector_base_to_vec(for_realloc);

	return OK;
}
int int_vector_insert(int_vector* vec_ptr, int element, size_t index_to_insert) {
	int err;
	if (vec_ptr == NULL || *vec_ptr == NULL) {
		return NULL_POINTER;
	}

	if (int_vector_size(*vec_ptr) == int_vector_cap(*vec_ptr)) {
		err = int_vector_grow(vec_ptr, int_vector_cap(*vec_ptr) * GROWTH_FACTOR);
		if (err) {
			return err;
		}
	}

	memcpy((*vec_ptr) + index_to_insert + 1, (*vec_ptr) + index_to_insert,
		sizeof(int) * (int_vector_size(*vec_ptr) - index_to_insert));

	(*vec_ptr)[index_to_insert] = element;

	__int_vector_vec_to_base(*vec_ptr)->size++;

	return OK;
}

int int_vector_push_back(int_vector* vec_ptr, int element) {
	int err;
	size_t index_to_insert;
	if (vec_ptr == NULL || *vec_ptr == NULL) {
		return NULL_POINTER;
	}

	if (int_vector_size(*vec_ptr) == int_vector_cap(*vec_ptr)) {
		err = int_vector_grow(vec_ptr, int_vector_cap(*vec_ptr) * GROWTH_FACTOR);
		if (err) {
			return err;
		}
	}
	index_to_insert = int_vector_size(*vec_ptr);
	memcpy((*vec_ptr) + index_to_insert + 1, (*vec_ptr) + index_to_insert,
		sizeof(int) * (int_vector_size(*vec_ptr) - index_to_insert));

	(*vec_ptr)[index_to_insert] = element;

	__int_vector_vec_to_base(*vec_ptr)->size++;

	return OK;
}

int int_vector_pop(int_vector* vec_ptr, int* element, size_t index_to_pop) {

	if (vec_ptr == NULL || *vec_ptr == NULL) {
		return NULL_POINTER;
	}

	*element = *((*vec_ptr) + index_to_pop);

	memcpy((*vec_ptr) + index_to_pop, (*vec_ptr) + index_to_pop + 1,
		sizeof(int) * (int_vector_size(*vec_ptr) - index_to_pop));
	
	__int_vector_vec_to_base(*vec_ptr)->size--;

	return OK;
}

int int_vector_pop_back(int_vector* vec_ptr, int* element) {
	if (vec_ptr == NULL || *vec_ptr == NULL) {
		return NULL_POINTER;
	}

	*element = *((*vec_ptr) + int_vector_size(*vec_ptr) - 1);
	__int_vector_vec_to_base(*vec_ptr)->size--;

	return OK;
}