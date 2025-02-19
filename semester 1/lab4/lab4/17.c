//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#define OK 0
//#define MEMORY_ALLOCATE_ERROR 5
//#define NULL_POINTER 7
//
//int subsets(int* items, size_t items_count, int** result_subsets, size_t* result_subsets_count, int
//(*equality_comparer)(int const*, int const*));
//int __subsets_inner(int* items, size_t items_count, size_t index_to_get_next_item,
//	int* current_subset, size_t current_subset_size, int** result, size_t* result_index_to_insert_subset);
//int compare_int_value(int const* a, int const* b);
//
//int main() {
//	int i, j, err;
//	int** result_subsets = NULL;
//	size_t result_subsets_count = 0;
//	int const items_count = 5;
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
//	err = subsets(arr, items_count, &result_subsets, &result_subsets_count, compare_int_value);
//	switch (err)
//	{
//	case OK:
//		printf("Subsets:\n");
//		for (i = 1; i < result_subsets_count; ++i)
//		{
//			printf("%d:%s [ ", i, i <= 9 ? " " : "");
//
//			for (j = 1; j < result_subsets[i][0]; ++j)
//			{
//				printf("%d%s ", result_subsets[i][j], j == result_subsets[i][0] - 1 ? "" : ",");
//			}
//
//			printf("]\n");
//
//			free(result_subsets[i]);
//		}
//		free(result_subsets);
//		result_subsets = NULL;
//		break;
//	case 1:
//		printf("items pointer is NULL");
//		break;
//	case 2:
//		printf("result_subsets pointer is NULL");
//		break;
//	case 3:
//		printf("result_subsets_count pointer is NULL");
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
//	arr = NULL;
//	return OK;
//}
//
//int compare_int_value(int const* a, int const* b) {
//	return *a - *b;
//}
//
//int subsets(int* items, size_t items_count, int** result_subsets, size_t* result_subsets_count, int
//(*equality_comparer)(int const*, int const*)) {
//	
//	int i, j, err;
//	int* current_subset = NULL;
//	int** target_subsets = NULL;
//	size_t subsets_count = 1;
//	size_t result_index_to_insert_subset = 0;
//
//	if (items == NULL) {
//		return 1;
//	}
//	if (result_subsets == NULL) {
//		return 2;
//	}
//	if (result_subsets_count == NULL) {
//		return 3;
//	}
//	if (equality_comparer == NULL) {
//		return 4;
//	}
//
//	for (i = 0; i < items_count - 1; i++) {
//		for (j = i + 1; j < items_count; j++) {
//			if (equality_comparer(items + i, items + j) == 0) {
//				printf("%d %d %d ", items[i], items[j], equality_comparer(items + i, items + j));
//				return 6;
//			}
//		}
//	}
//
//	if ((current_subset = (int*)malloc(sizeof(int) * (items_count + 1))) == NULL) {
//		return MEMORY_ALLOCATE_ERROR;
//	}
//
//	for (i = 0; i < items_count; i++) {
//		subsets_count *= 2;
//	}
//
//	if ((target_subsets = (int**)malloc(sizeof(int*) * subsets_count)) == NULL) {
//		free(current_subset);
//		current_subset = NULL;
//		return MEMORY_ALLOCATE_ERROR;
//	}
//
//	for (i = 0; i < subsets_count; i++) {
//		if ((target_subsets[i] = (int*)malloc(sizeof(int) * items_count)) == NULL) {
//
//			for (j = 0; j < i; j++) {
//				free(target_subsets[j]);
//			}
//
//			free(target_subsets);
//			free(current_subset);
//			current_subset = NULL;
//			target_subsets = NULL;
//			return MEMORY_ALLOCATE_ERROR;
//		}
//	}
//
//	err = __subsets_inner(items, items_count, 0, current_subset, 1, target_subsets, result_subsets_count);
//	if (err) {
//		return err;
//	}
//
//	free(current_subset);
//	current_subset = NULL;
//
//	*result_subsets = target_subsets;
//	*result_subsets_count = subsets_count;
//
//	return OK;
//}
//
//int __subsets_inner(int* items, size_t items_count, size_t index_to_get_next_item, 
//	int* current_subset, size_t current_subset_size, int** result, size_t* result_index_to_insert_subset) {
//
//	int err;
//	if (items == NULL ||
//		current_subset == NULL ||
//		result == NULL ||
//		result_index_to_insert_subset == NULL) {
//		return NULL_POINTER;
//	}
//
//	result[*result_index_to_insert_subset] = (int*)malloc(sizeof(int) * current_subset_size);
//	memcpy(result[*result_index_to_insert_subset], current_subset, sizeof(int) * current_subset_size);
//	result[*result_index_to_insert_subset][0] = current_subset_size;
//	(*result_index_to_insert_subset)++;
//
//	for (int i = index_to_get_next_item; i < items_count; i++) {
//		current_subset[current_subset_size] = items[i];
//		err = __subsets_inner(items, items_count, i + 1, current_subset, current_subset_size + 1, result, result_index_to_insert_subset);
//		if (err) { return err; }
//	}
//	return OK;
//}