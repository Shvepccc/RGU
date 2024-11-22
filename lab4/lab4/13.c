//#include<stdio.h>
//#include <stdlib.h>
//#include "../../libs/int_vector.h"
//#include "../../libs/error.h"
//
//int main() {
//	int err, i, j;
//	int count = 0;
//	int_vector* arr;
//
//	err = decompositions(5, &arr, &count);
//	if (err) {
//		printf("error");
//	}
//	else {
//		for (i = 0; i < count; i++) {
//			for (j = 0; j < int_vector_size(arr[i]); j++) {
//				printf("%d ", arr[i][j]);
//			}
//			printf("\n");
//		}
//	}
//}
//
//int __dec(int_vector** arr, int* decompositions_capacity, int* decompositions_count, int n) {
//	int i = 0, j = 0;
//	int_vector temp_vec = int_vector_init();
//	int decompositions_count_local;
//	if (n == 0) {
//		if (*decompositions_capacity < ++(*decompositions_count))
//		{
//			*arr = realloc(arr, sizeof(int_vector) * (*decompositions_capacity) * INT_VECTOR_GROWTH_FACTOR);
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
//		for (i = 0; int_vector_size(temp_vec)) {
//		
//		}
//		(*arr)[*decompositions_count] = temp_vec;
//		return 0;
//	}
//
//	for (int k = n; k > 0; k--) {
//		int_vector_push_back(&temp_vec, k);
//		__dec(arr, decompositions_capacity, decompositions_count, n - k);
//		int_vector_pop_back(&temp_vec, &i);
//	}
//	return 0;
//}
//
//int decompositions(int n, int_vector** arr, int* count) {
//	int i, j;
//	int main_capacity = INT_VECTOR_INITIAL_CAPACITY;
//	*arr = (int_vector*)malloc(sizeof(int_vector) * main_capacity);
//	if ((*arr) == NULL) {
//		return MEMORY_ALLOCATE_ERROR;
//	}
//	for (i = 0; i < main_capacity; i++) {
//		(*arr)[i] = int_vector_init();
//		if ((*arr)[i] == NULL) {
//			return MEMORY_ALLOCATE_ERROR;
//		}
//	}
//
//	__dec(arr, &main_capacity, count, n);
//	return 0;
//}