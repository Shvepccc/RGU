//#include<stdio.h>
//#include <stdlib.h>
//#include "../../libs/int_vector.h"
//#define MEMORY_ALLOCATE_ERROR 4
//
//int towers_construction(int blocks_count, int_vector** result_towers, size_t* result_towers_count, int
//	allowed_partial_blocks_usage, int allowed_adjacent_layers_blocks_equal_count);
//int __towers_construction_inner(int_vector** arr, int* decompositions_capacity, int* decompositions_count,
//	int n, int_vector* temp_vec, int allowed_equal_sum_components);
//
//char* error_processing(int err);
//
//int main() {
//	int err, i, j;
//	int count = 0;
//	int_vector* arr;
//	int blocks_count = 3;
//
//	err = towers_construction(blocks_count, &arr, &count, 0, 0);
//	if (err) {
//		printf(error_processing(err));
//	}
//	else {
//		printf("Tower construction result with allowed_partial_blocks_usage = 0, allowed_adjacent_layers_blocks_equal_count = 0\n");
//		for (i = 0; i < count; i++) {
//			for (j = 0; j < int_vector_size(arr[i]); j++) {
//				printf("%d ", arr[i][j]);
//			}
//			int_vector_free(arr[i]);
//			printf("\n"); 
//		}
//		int_vector_free(arr);
//		arr = NULL;
//		printf("\n");
//	}
//	count = 0;
//	err = towers_construction(blocks_count, &arr, &count, 0, 1);
//	if (err) {
//		printf(error_processing(err));
//	}
//	else {
//		printf("Tower construction result with allowed_partial_blocks_usage = 0, allowed_adjacent_layers_blocks_equal_count = 1\n");
//		for (i = 0; i < count; i++) {
//			for (j = 0; j < int_vector_size(arr[i]); j++) {
//				printf("%d ", arr[i][j]);
//			}
//			int_vector_free(arr[i]);
//			printf("\n");
//		}
//		int_vector_free(arr);
//		arr = NULL;
//		printf("\n");
//	}
//	count = 0;
//	err = towers_construction(blocks_count, &arr, &count, 1, 0);
//	if (err) {
//		printf(error_processing(err));
//	}
//	else {
//		printf("Tower construction result with allowed_partial_blocks_usage = 1, allowed_adjacent_layers_blocks_equal_count = 0\n");
//		for (i = 0; i < count; i++) {
//			for (j = 0; j < int_vector_size(arr[i]); j++) {
//				printf("%d ", arr[i][j]);
//			}
//			int_vector_free(arr[i]);
//			printf("\n");
//		}
//		int_vector_free(arr);
//		arr = NULL;
//		printf("\n");
//	}
//	count = 0;
//	err = towers_construction(blocks_count, &arr, &count, 1, 1);
//	if (err) {
//		printf(error_processing(err));
//	}
//	else {
//		printf("Tower construction result with allowed_partial_blocks_usage = 1, allowed_adjacent_layers_blocks_equal_count = 1\n");
//		for (i = 0; i < count; i++) {
//			for (j = 0; j < int_vector_size(arr[i]); j++) {
//				printf("%d ", arr[i][j]);
//			}
//			int_vector_free(arr[i]);
//			printf("\n");
//		}
//		int_vector_free(arr);
//		arr = NULL;
//		printf("\n");
//	}
//
//	return 0;
//}
//
//int __towers_construction_inner(int_vector** arr, int* decompositions_capacity, int* decompositions_count, int n, int_vector* temp_vec, int allowed_equal_sum_components) {
//	int i = 0, j = 0, temp_int = 0, err, t;
//	if (n == 0) {
//		if (*decompositions_capacity < *decompositions_count)
//		{
//			*arr = realloc(*arr, sizeof(int_vector) * (*decompositions_capacity) * INT_VECTOR_GROWTH_FACTOR);
//			if ((*arr) == NULL) {
//				return MEMORY_ALLOCATE_ERROR;
//			}
//			for (i = *decompositions_capacity; i < (*decompositions_capacity) * 2; i++) {
//				(*arr)[i] = int_vector_init();
//				if ((*arr)[i] == NULL) {
//					for (t = 0; t < i; t++) {
//						free((*arr)[t]);
//					}
//					free(*arr);
//					*arr = NULL;
//					return MEMORY_ALLOCATE_ERROR;
//				}
//			}
//			(*decompositions_capacity) *= INT_VECTOR_GROWTH_FACTOR;
//		}
//		int_vector new_vec = int_vector_init();
//		for (i = 0; i < int_vector_size(*temp_vec); i++) {
//			int_vector_push_back(&new_vec, (*temp_vec)[i]);
//		}
//		(*arr)[(*decompositions_count)++] = new_vec;
//		return 0;
//	}
//
//	for (int k = n; k > 0; k--) {
//		int_vector_get_back(temp_vec, &temp_int);
//		
//		if (temp_int == NULL || (allowed_equal_sum_components > 0 ? (k <= temp_int) : (k < temp_int)) ) {
//			int_vector_push_back(temp_vec, k);
//			err = __towers_construction_inner(arr, decompositions_capacity, decompositions_count, n - k, temp_vec, allowed_equal_sum_components);
//			if (err) { return err; }
//			int_vector_pop_back(temp_vec, &i);
//		}
//	}
//	return 0;
//}
//
//int towers_construction(int blocks_count, int_vector** result_towers, size_t* result_towers_count, int
//	allowed_partial_blocks_usage, int allowed_adjacent_layers_blocks_equal_count) {
//	int i, j, err, t;
//	int main_capacity = INT_VECTOR_INITIAL_CAPACITY;
//	int_vector temp_vec;
//
//	if (result_towers == NULL) {
//		return 1;
//	}
//	if (result_towers_count == NULL) {
//		return 2;
//	}
//	if (blocks_count < 0) {
//		return 3;
//	}
//
//	*result_towers = (int_vector*)malloc(sizeof(int_vector) * main_capacity);
//	if ((*result_towers) == NULL) {
//		return MEMORY_ALLOCATE_ERROR;
//	}
//	for (i = 0; i < main_capacity; i++) {
//		(*result_towers)[i] = int_vector_init();
//		if ((*result_towers)[i] == NULL) {
//			for (t = 0; t < i; t++) {
//				free((*result_towers)[t]);
//			}
//			free(*result_towers);
//			*result_towers = NULL;
//			return MEMORY_ALLOCATE_ERROR;
//		}
//	}
//	
//	for (i = blocks_count; i > (allowed_partial_blocks_usage == 0 ? (blocks_count - 1) : (0)); i--) {
//		temp_vec = int_vector_init();
//		err = __towers_construction_inner(result_towers, &main_capacity, result_towers_count, i, &temp_vec, allowed_adjacent_layers_blocks_equal_count);
//		int_vector_free(temp_vec);
//		if (err) { return err; }
//	}
//	return 0;
//}
//
//char* error_processing(int err) {
//	switch (err)
//	{
//		case 1:
//			return "result_towers pointer is null";
//		case 2:
//			return "result_towers_count pointer is null";
//		case 3:
//			return "The number of cubes must be a positive number";
//		case 4:
//			return "Memory allocate error";
//		default:
//			return "Unknown error";
//	}
//}