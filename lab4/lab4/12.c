//#include <stdio.h>
//#include <stdarg.h>
//#include <stdlib.h>
//#include <ctype.h>
//#include <string.h>
//#include "../../libs/memory.h"
//
//#define MEMORY_ALLOCATE_ERROR 5
//
//int substr(char* to_find, int case_sensitive, char*** results, int*** positions, size_t* results_count, ...);
//char* processing_error(int err);
//
//int main() {
//	int err, i, j;
//	char** results;
//	int** positions;
//	size_t results_count = 0;
//	char* find_str = "hello";
//	char* str_1 = "Hello everyone! This is my vlog) I will talk about my life, for example travelling and work.";
//	char* str_2 = " - What did you just say ?! - Emmmm.... 'Hello, Vanessa!' - I'M NOT VANESSA! I'M VAN! - Oh, okey... Hello, Van)";
//	char* str_3 = "hello mother, hello father, hello sister, hello brother, hello little brother, hello cat, hello dog, hello WORLD!";
//
//	printf(" ------- Processing strings with case_sensitive = 0 ------- \n\n");
//	err = substr(find_str, 0, &results, &positions, &results_count, str_1, str_2, str_3, NULL);
//	if (err) {
//		printf("%s", processing_error(err));
//	}
//	else {
//		for (i = 0; i < results_count; i++) {
//			printf("String: \"%s\"\n", results[i]);
//			printf("Occurrences of the substring:\n");
//			int count = positions[i][0] + 1;
//			for (j = 1; j < count; j++) {
//				if (count - j == 1) {
//					printf("%d", positions[i][j]);
//				}
//				else {
//					printf("%d, ", positions[i][j]);
//				}
//			}
//			free(results[i]);
//			free(positions[i]);
//			printf("\n\n");
//		}
//	}
//	results_count = 0;
//
//	printf("\n\n ------- Processing strings with case_sensitive = 1 ------- \n\n");
//	err = substr(find_str, 1, &results, &positions, &results_count, str_1, str_2, str_3, NULL);
//	if (err) {
//		printf("%s", processing_error(err));
//	}
//	else {
//		for (i = 0; i < results_count; i++) {
//			printf("String: \"%s\"\n", results[i]);
//			printf("Occurrences of the substring:\n");
//			int count = positions[i][0] + 1;
//			for (j = 1; j < count; j++) {
//				if (count - j == 1) {
//					printf("%d", positions[i][j]);
//				}
//				else {
//					printf("%d, ", positions[i][j]);
//				}
//			}
//			free(results[i]);
//			free(positions[i]);
//			printf("\n\n");
//		}
//	}
//	free(results);
//	free(positions);
//	return 0;
//}
//
//int substr(char* to_find, int case_sensitive, char*** results, int*** positions, size_t* results_count, ...) {
//	int i, err, j;
//	char* str_ptr;
//	char* find_ptr;
//	int k = 0, pos = -1;
//	int size = 10;
//	int** temp_int_ptr_ptr = NULL;
//	char** temp_char_ptr_ptr = NULL;
//	int* temp_int_ptr = NULL;
//	char* temp_str_ptr = NULL;
//	int current_ans_index = 0, current_ans_size = 0;
//	int current_position_str_index = 1, current_position_str_size = 0;
//
//	if (to_find == NULL) { return 1; }
//	if (results == NULL) { return 2; }
//	if (positions == NULL) { return 3; }
//	if (results_count == NULL) { return 4; }
//
//	(*results) = (char**)malloc(size * sizeof(char*));
//	if ((*results) == NULL) {
//		return MEMORY_ALLOCATE_ERROR;
//	}
//
//	(*positions) = (int**)malloc(size * sizeof(int*));
//	if ((*positions) == NULL) {
//		free(*results);
//		return MEMORY_ALLOCATE_ERROR;
//	}
//	current_ans_size = size;
//
//	for (i = 0; i < current_ans_size; i++) {
//		(*positions)[i] = (int*)malloc(size * sizeof(int));
//		if ((*positions)[i] == NULL) {
//			for (j = 0; j < i; j++) {
//				free((*positions)[j]);
//			}
//			free(*results);
//			free(*positions);
//			return MEMORY_ALLOCATE_ERROR;
//		}
//		(*positions)[i][0] = 0;
//	}
//
//	find_ptr = to_find;
//	va_list args;
//	va_start(args, results_count);
//
//	while ((str_ptr = va_arg(args, char*)) != NULL) {
//		temp_str_ptr = str_ptr;
//		k = 0;
//		current_position_str_index = 1;
//		current_position_str_size = size;
//		if (current_ans_index >= current_ans_size) {
//			temp_int_ptr_ptr = (int**)cust_realloc((void**)positions, current_ans_size * 2 * sizeof(int*));
//			if (temp_int_ptr_ptr == NULL) {
//				for (j = 0; j < current_ans_size; j++) {
//					free((*positions)[j]);
//				}
//				free(*results);
//				free(*positions);
//				return MEMORY_ALLOCATE_ERROR;
//			}
//			(*positions) = temp_int_ptr_ptr;
//
//			temp_char_ptr_ptr = (char**)cust_realloc((void**)results, current_ans_size * 2 * sizeof(char*));
//			if (temp_char_ptr_ptr == NULL) {
//				for (j = 0; j < current_ans_size; j++) {
//					free((*positions)[j]);
//				}
//				free(*results);
//				free(*positions);
//				free(temp_int_ptr_ptr);
//				return MEMORY_ALLOCATE_ERROR;
//			}
//			(*results) = temp_char_ptr_ptr;
//
//			for (i = current_ans_size; i < current_ans_size*2; i++) {
//				(*positions)[i] = (int*)malloc(size * sizeof(int));
//				if ((*positions)[i] == NULL) {
//					for (j = 0; j < i; j++) {
//						free((*positions)[j]);
//					}
//					free(*results);
//					free(*positions);
//					free(temp_int_ptr_ptr);
//					free(temp_char_ptr_ptr);
//					return MEMORY_ALLOCATE_ERROR;
//				}
//				(*positions)[i][0] = 0;
//			}
//			current_ans_size *= 2;
//		}
//
//		while (*str_ptr != '\0') {
//			//printf("%c", *str_ptr);
//			if (case_sensitive == 0 && toupper(*str_ptr) == toupper(*find_ptr)) {
//				if (pos == -1) {
//					pos = k;
//				}
//				find_ptr++;
//			}
//			else if (*str_ptr == *find_ptr) {
//				if (pos == -1) {
//					pos = k;
//				}
//				find_ptr++;
//			}
//			else if (pos != -1 && strlen(find_ptr) == 0) {
//				if (current_position_str_index >= current_position_str_size) {
//					temp_int_ptr = (int*)cust_realloc((void**)(*positions)[current_ans_index], current_position_str_size * 2 * sizeof(int));
//					if (temp_int_ptr == NULL) {
//						for (j = 0; j < i; j++) {
//							free((*positions)[j]);
//						}
//						free(*results);
//						free(temp_int_ptr_ptr);
//						free(temp_char_ptr_ptr);
//						return MEMORY_ALLOCATE_ERROR;
//					}
//					current_position_str_size *= 2;
//				}
//
//				(*positions)[current_ans_index][current_position_str_index] = pos;
//				(*positions)[current_ans_index][0]++;
//				int cc = (*positions)[current_ans_index][0];
//				current_position_str_index++;
//				pos = -1;
//				find_ptr = to_find;
//			}
//			else {
//				pos = -1;
//				find_ptr = to_find;
//			}
//			str_ptr++;
//			k++;
//		}
//
//		if ((*positions)[current_ans_index][0] != 0) {
//			(*results)[current_ans_index] = temp_str_ptr;
//			current_ans_index++;
//			(*results_count)++;
//		}
//	}
//	free(temp_char_ptr_ptr);
//	free(temp_int_ptr_ptr);
//	free(temp_int_ptr);
//	va_end(args);
//	return 0;
//}
//
//char* processing_error(int err) {
//	switch (err)
//	{
//		case 1:
//			return "Parametr 'initial' is NULL";
//		case 2:
//			return "Parametr 'detector' is NULL";
//		case 3:
//			return "Parametr 'lexems' is NULL";
//		case 4:
//			return "Parametr 'lexems_count' is NULL";
//		case MEMORY_ALLOCATE_ERROR:
//			return "MEMORY_ALLOCATE_ERROR";
//		default:
//			return "Unknown error";
//	}
//}