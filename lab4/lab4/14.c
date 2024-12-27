//#include <stdio.h>
//#define ARRAY_SIZE 20
//typedef int tvalue;
//
//int find_the_longest_sawtooth_subsequence(tvalue const* sequence, size_t
//* subsequence_start_index_storage, size_t* subsequence_length_storage, int (*comparer)(tvalue const*,
//		tvalue const*), int is_comparison_is_strict);
//
//int comparer_function(tvalue const*, tvalue const*);
//char* error_processing(int err);
//
//int main() {
//	int err;
//	size_t start_index, length;
//	tvalue arr[ARRAY_SIZE] = { 1, 3, 2, 4, 3, 5, 4, 6, 5, 7, 7, 12, 11, 13, 12, 14, 13, 15, 14, 16 };
//	
//	err = find_the_longest_sawtooth_subsequence(arr, &start_index, &length, comparer_function, 0);
//	if (err) {
//		printf(error_processing(err));
//	}
//	else {
//		printf("Result of processing array with parametr is_comparison_is_strict = 0\n");
//		printf("Start index: %d, length: %d \n\n", start_index, length);
//	}
//
//	start_index = 0; length = 0;
//	err = find_the_longest_sawtooth_subsequence(arr, &start_index, &length, comparer_function, 1);
//	if (err) {
//		printf(error_processing(err));
//	}
//	else {
//		printf("Result of processing array with parametr is_comparison_is_strict = 1\n");
//		printf("Start index: %d, length: %d \n\n", start_index, length);
//	}
//	return 0;
//}
//
//int comparer_function(tvalue const* a, tvalue const* b) {
//	if (*a == *b) {
//		return 0;
//	}
//	return (*a) - (*b);
//}
//
//int find_the_longest_sawtooth_subsequence(tvalue const* sequence, size_t
//	* subsequence_start_index_storage, size_t* subsequence_length_storage, int (*comparer)(tvalue const*,
//		tvalue const*), int is_comparison_is_strict) {
//
//	int i = 0;
//	int start_index, current_len = 0;
//	int result_of_compare, is_subsequence = 0, last_is_subsequence = 0;
//
//	if (sequence == NULL) {
//		return 1;
//	}
//	if (subsequence_start_index_storage == NULL) {
//		return 2;
//	}
//	if (subsequence_length_storage == NULL) {
//		return 3;
//	}
//	if (comparer == NULL) {
//		return 4;
//	}
//
//	*subsequence_start_index_storage = -1;
//	*subsequence_length_storage = 0;
//
//	for (i = 0; i < ARRAY_SIZE; i++) {
//
//		if (is_subsequence == 0) {
//			start_index = i;
//		}
//
//		result_of_compare = comparer_function(&sequence[i], &sequence[i+1]);
//		if (is_comparison_is_strict != 0 ? (result_of_compare > 0) : (result_of_compare >= 0)) {
//			is_subsequence = -1;
//			current_len++;
//		}
//		else if (is_comparison_is_strict != 0 ? (result_of_compare <= 0) : (result_of_compare < 0)) {
//			is_subsequence = 1;
//			current_len++;
//		}
//
//		if (last_is_subsequence == is_subsequence) {
//			if (current_len > (*subsequence_length_storage)) {
//			 	*subsequence_start_index_storage = start_index;
//			 	*subsequence_length_storage = current_len;
//			}
//			is_subsequence = 0;
//			current_len = 0;
//		}
//		last_is_subsequence = is_subsequence;
//	}
//	if (start_index != -1 && current_len != 0 && *subsequence_length_storage == 0) {
//		*subsequence_start_index_storage = start_index;
//		*subsequence_length_storage = current_len;
//	}
//
//	return 0;
//}
//
//char* error_processing(int err) {
//	switch (err)
//	{
//		case 1:
//			return "sequence pointer is NULL";
//		case 2:
//			return "subsequence_start_index_storage pointer is NULL";
//		case 3:
//			return "subsequence_length_storage pointer is NULL";
//		case 4:
//			return "Memory allocate error";
//		default:
//			return "Unknown error";
//	}
//}