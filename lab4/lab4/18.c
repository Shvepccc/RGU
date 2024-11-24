//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#define OK 0
//#define MEMORY_ALLOCATE_ERROR 5
//#define NULL_POINTER 7
//
//int combinations(int* items, size_t items_count, int** result_combinations, size_t
//	* result_combinations_count, int (*equality_comparer)(int const*, int const*), size_t k);
//int __combinations_inner(size_t items_count, int* current_combination, size_t current_combination_index_to_insert,
//	int* not_inserted_items, size_t not_inserted_items_count, int* const* result, size_t* result_index_to_insert_combination, size_t k, int index);
//
//int compare_int_value(int const* a, int const* b);
//unsigned long long int factorial(unsigned int N);
//	
//int main() {
//	int i, j, err;
//	int** result_subsets = NULL;
//	size_t result_subsets_count = 0;
//	int const items_count = 20;
//	size_t k = 5;
//
//	int* arr = (int*)malloc(items_count * sizeof(int));
//	if (!arr)
//	{
//		printf("Memory allocate error");
//		return 1;
//	}
//
//	for (i = 0; i < items_count; ++i)
//	{
//		arr[i] = i + 1;
//	}
//
//	err = combinations(arr, items_count, &result_subsets, &result_subsets_count, compare_int_value, k);
//	switch (err)
//	{
//	case OK:
//		printf("Subsets:\n");
//		for (i = 0; i < result_subsets_count; ++i)
//		{
//			printf("%d:%s [ ", i+1, i+1 <= 9 ? " " : "");
//
//			for (j = 0; j < k; ++j)
//			{
//				printf("%d%s ", result_subsets[i][j], j == k - 1 ? "" : ",");
//			}
//
//			printf("]\n");
//
//			free(result_subsets[i]);
//		}
//		free(result_subsets);
//		break;
//	case 1:
//		printf("items pointer is NULL");
//		break;
//	case 2:
//		printf("result_combinations pointer is NULL");
//		break;
//	case 3:
//		printf("result_combinations_count pointer is NULL");
//		break;
//	case 4:
//		printf("equality_comparer pointer is NULL");
//		break;
//	case MEMORY_ALLOCATE_ERROR:
//		printf("Memory allocate error");
//		break;
//	case 6:
//		printf("Two identical values were found. Change values and restart the program.");
//		break;
//	case 7:
//		printf("A NULL pointer has been detected in the internal function. Contact the creator of the program...");
//		break;
//	default:
//		printf("Unknown error 0_o");
//		break;
//	}
//
//	free(arr);
//
//	return OK;
//}
//
//int compare_int_value(int const* a, int const* b) {
//	return *a - *b;
//}
//
//int combinations(int* items, size_t items_count, int** result_combinations, size_t
//	* result_combinations_count, int (*equality_comparer)(int const*, int const*), size_t k) {
//
//	int i, j, err;
//	int* current_combination = NULL;
//	int** target_combinations = NULL;
//	size_t combinations_count = 1;
//	size_t result_index_to_insert_combination = 0;
//
//	if (items == NULL) {
//		return 1;
//	}
//	if (result_combinations == NULL) {
//		return 2;
//	}
//	if (result_combinations_count == NULL) {
//		return 3;
//	}
//	if (equality_comparer == NULL) {
//		return 4;
//	}
//
//	for (i = 0; i < items_count - 1; i++) {
//		for (j = i + 1; j < items_count; j++) {
//			if (equality_comparer(items + i, items + j) == 0) {
//				return 6;
//			}
//		}
//	}
//
//	if ((current_combination = (int*)malloc(sizeof(int) * k)) == NULL) {
//		return MEMORY_ALLOCATE_ERROR;
//	}
//
//	combinations_count = factorial(items_count) / (factorial(items_count - k) * factorial(k));
//
//	if ((target_combinations = (int**)malloc(sizeof(int*) * combinations_count)) == NULL) {
//		free(current_combination);
//		return MEMORY_ALLOCATE_ERROR;
//	}
//
//	for (i = 0; i < combinations_count; i++) {
//		if ((target_combinations[i] = (int*)malloc(sizeof(int) * k)) == NULL) {
//
//			for (j = 0; j < i; j++) {
//				free(target_combinations[j]);
//			}
//
//			free(target_combinations);
//			free(current_combination);
//			return MEMORY_ALLOCATE_ERROR;
//		}
//	}
//
//	err = __combinations_inner(items_count, current_combination, 0,
//		items, items_count, target_combinations, &result_index_to_insert_combination, k, 0);
//
//	if (err) {
//		return err;
//	}
//
//	free(current_combination);
//
//	*result_combinations = target_combinations;
//	*result_combinations_count = combinations_count;
//
//	return OK;
//}
//
//int __combinations_inner(size_t items_count, int* current_combination, size_t current_combination_index_to_insert,
//	int* not_inserted_items, size_t not_inserted_items_count, int* const* result, size_t* result_index_to_insert_combination, size_t k, int index) {
//
//	if (current_combination == NULL ||
//		not_inserted_items == NULL ||
//		result == NULL ||
//		result_index_to_insert_combination == NULL) {
//
//		return NULL_POINTER;
//	}
//
//	int not_inserted_items_index = 0, err;
//
//	if (items_count - not_inserted_items_count == k) {
//		memcpy(result[*result_index_to_insert_combination], current_combination, sizeof(int) * k);
//		//for (int i = 0; i < k; i++) {
//		//	printf("%d ", result[*result_index_to_insert_combination][i]);
//		//}
//		//printf("\n");
//		++(*result_index_to_insert_combination);
//		return OK;
//	}
//
//	for (not_inserted_items_index = index; not_inserted_items_index < items_count; not_inserted_items_index++) {
//		current_combination[current_combination_index_to_insert] = not_inserted_items[not_inserted_items_index];
//
//		err = __combinations_inner(items_count, current_combination, current_combination_index_to_insert + 1,
//			not_inserted_items, not_inserted_items_count-1, result, result_index_to_insert_combination, k, not_inserted_items_index + 1);
//		if (err) { return err; }
//	}
//
//	return OK;
//}
//
//unsigned long long int factorial(unsigned int N) {
//	unsigned long long int fact = 1, i;
//
//	for (i = 1; i <= N; i++) {
//		fact *= i;
//	}
//
//	return fact;
//}