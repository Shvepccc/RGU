//#include <stdio.h>
//#include <stdlib.h>
//#include "../../libs/error.h"
//#include "../../libs/memory.h"
//
//int tokenize(char* initial, int (*detector)(int), int accept_empty_lexems, char*** lexems, size_t* lexems_count);
//int detector(char symbol);
//
//int main() {
//	int err, i;
//	char* str_1 = "Hi everyone! Today I want to talk you about my hobbies.";
//	char* str_2 = "Hi  everyone!  Today  I  want  to talk you about my hobbies.";
//	char** lexems;
//	size_t lexems_count = 0;
//
//	printf("Result of processing string without empty lexems:\n");
//	err = tokenize(str_1, detector, 0, &lexems, &lexems_count);
//	if (err) { 
//		printf("MEMORY_ALLOCATE_ERROR!");
//	}
//	for (i = 0; i < lexems_count; i++) {
//		printf("*%s*, ", lexems[i]);
//		free(lexems[i]);
//	}
//	free(lexems);
//
//	printf("\n\nResult of processing string with empty lexems and accept_empty_lexems = 1:\n");
//	lexems_count = 0;
//	err = tokenize(str_2, detector, 1, &lexems, &lexems_count);
//	if (err) {
//		printf("MEMORY_ALLOCATE_ERROR!");
//	}
//	for (i = 0; i < lexems_count; i++) {
//		printf("*%s*, ", lexems[i]);
//		free(lexems[i]);
//	}
//	free(lexems);
//
//	printf("\n\nResult of processing string with empty lexems and accept_empty_lexems = 0:\n");
//	lexems_count = 0;
//	err = tokenize(str_2, detector, 0, &lexems, &lexems_count);
//	if (err) {
//		printf("MEMORY_ALLOCATE_ERROR!");
//	}
//	for (i = 0; i < lexems_count; i++) {
//		printf("*%s*, ", lexems[i]);
//		free(lexems[i]);
//	}
//	free(lexems);
//
//	printf("\n\n");
//	return OK;
//}
//
//int detector(char symbol) {
//	return symbol == ' ';
//}
//
//int tokenize(char* initial, int (*detector)(int), int accept_empty_lexems, char*** lexems, size_t* lexems_count) {
//	int err;
//	int size = 10;
//	int current_lexeme_size = 10, current_lexeme_index = 0;
//	int current_array_size = 0, current_array_index = 0;
//	char* ptr = initial;
//	char* temp_char, *temp_char_2;
//	char last_char =0;
//
//	*lexems = (char**)malloc(size * sizeof(char*));
//	if (*lexems == NULL) {
//		return MEMORY_ALLOCATE_ERROR;
//	}
//	current_array_size = size;
//
//	temp_char = (char*)malloc(size * sizeof(char));
//	if (temp_char == NULL) {
//		return MEMORY_ALLOCATE_ERROR;
//	}
//	current_lexeme_size = size;
//
//	while (*ptr != 0) {
//		if (current_array_index >= current_array_size) {
//			err = cust_realloc((void**)lexems, current_array_size * 2 * sizeof(char*));
//			if (err) {
//				return MEMORY_ALLOCATE_ERROR;
//			}
//			current_array_size *= 2;
//		}
//
//		if (detector(*ptr)) {
//			if (*ptr == last_char && accept_empty_lexems != 0) {
//				temp_char_2 = (char*)malloc(sizeof(char)); 
//				if (temp_char_2 == NULL) {
//					return MEMORY_ALLOCATE_ERROR;
//				}
//				temp_char_2 = '\0';
//				(*lexems)[current_array_index] = temp_char_2;
//				(*lexems_count)++;
//				current_array_index++;
//			}
//			else if (current_lexeme_index > 0) {
//				temp_char[current_lexeme_index] = 0;
//				(*lexems)[current_array_index] = temp_char;
//				(*lexems_count)++;
//				current_lexeme_index = 0;
//				current_lexeme_size = 0;
//				current_array_index++;
//			}
//			ptr++;
//
//			temp_char = (char*)malloc(size * sizeof(char));
//			if (temp_char == NULL) {
//				return MEMORY_ALLOCATE_ERROR;
//			}
//			current_lexeme_size = size;
//		}
//		else {
//			if (current_lexeme_index >= current_lexeme_size) {
//				err = cust_realloc((void**)temp_char, current_lexeme_size * 2 * sizeof(char));
//				if (err) {
//					return MEMORY_ALLOCATE_ERROR;
//				}
//				current_lexeme_size *= 2;
//			}
//
//			temp_char[current_lexeme_index] = *ptr;
//			current_lexeme_index++;
//			ptr++;
//		}
//		last_char = *(ptr-1);
//	}
//
//	if (current_lexeme_index > 0) {
//		temp_char[current_lexeme_index] = 0;
//		(*lexems)[current_array_index] = temp_char;
//		(*lexems_count)++;
//	}
//	else {
//		free(temp_char);
//	}
//
//	return OK;
//	
//}