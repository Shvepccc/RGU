//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#define MEMORY_ALLOCATE_ERROR 5
//#define NULL_POINTER 7
//#define GOOD_JOB 0
//
//int permutations(int* items, size_t items_count, int*** result, size_t* result_count,
//	int (*equality_comparer)(int const*, int const*));
//int __permutations_inner(size_t items_count, int* current_permutation, size_t current_permutation_index_to_insert,
//	int* not_inserted_items, size_t not_inserted_items_count, int* const* result, size_t* result_index_to_insert_permutation);
//int compare_int_value(int const* a, int const* b);
//
//int main() {
//	int i, j, err;
//	int** result_permutations = NULL;
//	size_t result_permutations_count = 0;
//	int const items_count = 5;
//
//	srand((unsigned int)time(NULL));
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
//		arr[i] = rand() % 201 - 100;
//	}
//
//	err = permutations(arr, items_count, &result_permutations, &result_permutations_count, compare_int_value);
//	switch (err)
//	{
//		case GOOD_JOB:
//			printf("Permutation:\n");
//			for (i = 0; i < result_permutations_count; ++i)
//			{
//				printf("%d:%s [ ", i, i <= 9 ? " " : "");
//
//				for (j = 0; j < items_count; ++j)
//				{
//					printf("%d%s ", result_permutations[i][j], j == items_count - 1 ? "" : ",");
//				}
//
//				printf("]\n");
//
//				free(result_permutations[i]);
//			}
//			free(result_permutations);
//			result_permutations = NULL;
//			break;
//		case 1:
//			printf("items pointer is NULL");
//			break;
//		case 2:
//			printf("result_permutations pointer is NULL");
//			break;
//		case 3:
//			printf("result_permutations_count pointer is NULL");
//			break;
//		case 4:
//			printf("equality_comparer pointer is NULL");
//			break;
//		case MEMORY_ALLOCATE_ERROR:
//			printf("Memory allocate error");
//			break;
//		case 6:
//			printf("Two identical values were found. Change values and restart the program.");
//			break;
//		case 7:
//			printf("A NULL pointer has been detected in the internal function. Contact the creator of the program...");
//			break;
//		default:
//			printf("Unknown error 0_o");
//			break;
//	}
//
//	free(arr);
//	arr = NULL;
//	return 0;
//}
//
//int compare_int_value( int const* a,int const* b) {
//	return *a - *b;
//}
//
//int permutations(int* items, size_t items_count, int*** result, size_t* result_count, 
//	int (*equality_comparer)(int const*, int const *)) {
//
//	int i, j, err = 0;
//	int* current_permutation = NULL;
//	int* not_inserted_items = NULL;
//	int** target_permutations = NULL;
//	size_t permutations_count = 1;
//	size_t result_index_to_insert_permutation = 0;
//
//	if (items == NULL) {
//		return 1;
//	}
//	if (result == NULL) {
//		return 2;
//	}
//	if (result_count == NULL) {
//		return 3;
//	}
//	if (equality_comparer == NULL) {
//		return 4;
//	}
//
//	for (i = 0; i < items_count - 1; i++) {
//		for (j = i + 1; j < items_count; j++) {
//			if (equality_comparer(items + i, items + j) == 0) {
//				//printf("%d %d %d ", items[i], items[j], equality_comparer(items + i, items + j));
//				return 6;
//			}
//		}
//	}
//	
//	if ((current_permutation = (int*)malloc(sizeof(int) * items_count)) == NULL) {
//		return MEMORY_ALLOCATE_ERROR;
//	}
//
//	if ((not_inserted_items = (int*)malloc(sizeof(int) * items_count)) == NULL) {
//		free(current_permutation);
//		current_permutation = NULL;
//		return MEMORY_ALLOCATE_ERROR;
//	}
//
//	for (i = 2; i <= items_count; i++) {
//		permutations_count *= i;
//	}
//
//	if ((target_permutations = (int**)malloc(sizeof(int*) * permutations_count)) == NULL) {
//		free(current_permutation);
//		free(not_inserted_items);
//		current_permutation = NULL;
//		not_inserted_items = NULL;
//		return MEMORY_ALLOCATE_ERROR;
//	}
//
//	for (i = 0; i < permutations_count; i++) {
//		if ((target_permutations[i] = (int*)malloc(sizeof(int) * items_count)) == NULL) {
//			
//			for (j = 0; j < i; j++) {
//				free(target_permutations[j]);
//			}
//
//			free(target_permutations);
//			free(not_inserted_items);
//			free(current_permutation);
//			current_permutation = NULL;
//			not_inserted_items = NULL;
//			target_permutations = NULL;
//			return MEMORY_ALLOCATE_ERROR;
//		}
//	}
//	
//	memcpy(not_inserted_items, items, items_count * sizeof(int));
//
//	err = __permutations_inner(items_count, current_permutation, 0, 
//		not_inserted_items, items_count, target_permutations, &result_index_to_insert_permutation);
//
//	if (err) {
//		return err;
//	}
//
//	free(current_permutation);
//	free(not_inserted_items);
//	current_permutation = NULL;
//	not_inserted_items = NULL;
//
//	*result = target_permutations;
//	*result_count = permutations_count;
//
//	return GOOD_JOB;
//}
//
//int __permutations_inner(size_t items_count, int* current_permutation, size_t current_permutation_index_to_insert, 
//	int* not_inserted_items, size_t not_inserted_items_count, int* const* result, size_t* result_index_to_insert_permutation) {
//
//	if (current_permutation == NULL ||
//		not_inserted_items == NULL ||
//		result == NULL ||
//		result_index_to_insert_permutation == NULL) {
//		
//		return NULL_POINTER;
//	}
//
//	int not_inserted_items_index, err;
//
//	if (not_inserted_items_count == 0) {
//		memcpy(result[*result_index_to_insert_permutation], current_permutation,
//			sizeof(int) * items_count);
//
//		++*result_index_to_insert_permutation;
//		return GOOD_JOB;
//	}
//
//	for (not_inserted_items_index = 0; not_inserted_items_index < not_inserted_items_count; not_inserted_items_index++) {
//		current_permutation[current_permutation_index_to_insert] = not_inserted_items[not_inserted_items_index];
//
//		memcpy(not_inserted_items + not_inserted_items_index, not_inserted_items + not_inserted_items_index + 1,
//			(not_inserted_items_count - not_inserted_items_index - 1) * sizeof(int));
//
//		err = __permutations_inner(items_count, current_permutation, current_permutation_index_to_insert +1, 
//			not_inserted_items, not_inserted_items_count - 1, result, result_index_to_insert_permutation);
//		if (err) { return err; }
//
//		memcpy(not_inserted_items + not_inserted_items_index + 1, not_inserted_items + not_inserted_items_index,
//			(not_inserted_items_count - not_inserted_items_index - 1) * sizeof(int));
//
//		not_inserted_items[not_inserted_items_index] = current_permutation[current_permutation_index_to_insert];
//	}
//
//	return GOOD_JOB;
//}