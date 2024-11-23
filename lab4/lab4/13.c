//#include<stdio.h>
//#include <stdlib.h>
//#include "../../libs/int_vector.h"
//#define MEMORY_ALLOCATE_ERROR 4
//
//int sums_decomposition(int value, int*** result_decompositions, size_t
//	* result_decompositions_count, int allowed_equal_sum_components);
//int __decompositions_inner(int_vector** arr, int* decompositions_capacity, int* decompositions_count,
//	int n, int_vector* temp_vec, int allowed_equal_sum_components);
//
//char* error_processing(int err);
//
//int main() {
//	int err, i, j;
//	int count = 0;
//	int_vector* arr;
//
//	err = sums_decomposition(7, &arr, &count, 1);
//	if (err) {
//		printf(error_processing(err));
//	}
//	else {
//		printf("Sums decomposition result with allowed_equal_sum_components = 1\n");
//		for (i = 0; i < count; i++) {
//			for (j = 0; j < int_vector_size(arr[i]); j++) {
//				printf("%d ", arr[i][j]);
//			}
//			int_vector_free(arr[i]);
//			printf("\n");
//		}
//	}
//	count = 0;
//	err = sums_decomposition(7, &arr, &count, 0);
//	if (err) {
//		printf(error_processing(err));
//	}
//	else {
//		printf("\nSums decomposition result with allowed_equal_sum_components = 0\n");
//		for (i = 0; i < count; i++) {
//			for (j = 0; j < int_vector_size(arr[i]); j++) {
//				printf("%d ", arr[i][j]);
//			}
//			int_vector_free(arr[i]);
//			printf("\n");
//		}
//	}
//	
//}
//
//int __decompositions_inner(int_vector** arr, int* decompositions_capacity, int* decompositions_count, int n, int_vector* temp_vec, int allowed_equal_sum_components) {
//	int i = 0, j = 0, temp_int = 0, err;
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
//			err = __decompositions_inner(arr, decompositions_capacity, decompositions_count, n - k, temp_vec, allowed_equal_sum_components);
//			if (err) { return err; }
//			int_vector_pop_back(temp_vec, &i);
//		}
//	}
//	return 0;
//}
//
//int sums_decomposition(int value, int_vector** result_decompositions, size_t
//	* result_decompositions_count, int allowed_equal_sum_components) {
//	int i, j, err;
//	int main_capacity = INT_VECTOR_INITIAL_CAPACITY;
//
//	if (result_decompositions == NULL) {
//		return 1;
//	}
//	if (result_decompositions_count == NULL) {
//		return 2;
//	}
//	if (value < 0) {
//		return 3;
//	}
//
//	*result_decompositions = (int_vector*)malloc(sizeof(int_vector) * main_capacity);
//	if ((*result_decompositions) == NULL) {
//		return MEMORY_ALLOCATE_ERROR;
//	}
//	for (i = 0; i < main_capacity; i++) {
//		(*result_decompositions)[i] = int_vector_init();
//		if ((*result_decompositions)[i] == NULL) {
//			return MEMORY_ALLOCATE_ERROR;
//		}
//	}
//	int_vector temp_vec = int_vector_init();
//	err = __decompositions_inner(result_decompositions, &main_capacity, result_decompositions_count, value, &temp_vec, allowed_equal_sum_components);
//	int_vector_free(temp_vec);
//	if (err) { return err; }
//	return 0;
//}
//
//char* error_processing(int err) {
//	switch (err)
//	{
//		case 1:
//			return "Result_decompositions pointer is null";
//		case 2:
//			return "Result_decompositions_count pointer is null";
//		case 3:
//			return "Value not natural";
//		case 4:
//			return "Memory allocate error";
//		default:
//			return "Unknown error";
//	}
//}