#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "../../libs/hash_table.h"
#include "../../libs/error.h"

#include "../../libs/cust_string.h"

int hash_function(const string* str) {
	long int res = 0, value = 0, i;
	char c = 0;

	if (str == NULL) { return NULL_POINTER; }

	for (i = 0; i < string_size(*str); i++) {
		c = (*str)[i];
		value = 0;
		if (isdigit(c)) { value = c - '0'; }
		else if (isupper(c)) { value = c - 'A' + 10; }
		else if (islower(c)) { value = c - 'a' + 36; }
		else { 
			continue; //TODO: change it
		}
		res = res * 62 + value;
	}
	return res;
}

void print_function(int i, void const * key, void const * value) {
	if (key == NULL || value == NULL) return;
	printf("Index: %d \tKey: %s\t\tValue: %s\n", i, *(string const *)key, *(string const *)value);
}

void ht_destructor(hash_table_item* item) {
	if (item == NULL) return;
	string_free((string*)item->key);
	string_free((string*)item->value);
	free(item);
}

int cust_cpy(hash_table_item* item, void* key, void* value) {
	if (item == NULL || key == NULL || value == NULL) {
		return NULL_POINTER;
	}
	*(string*)(*item).key = string_cpy_new(*(string*)key);
	*(string*)(*item).value = string_cpy_new(*(string*)value);
}

int replace_all_occurrences(char* temp_str, const char* key, const char* value);

int main() {
	int HASHSIZE = 128, line = 0, i, define_flag = 0, read_flag = 0, err, read_level, str_index;
	FILE* data_file;
	FILE* result_file;
	u_list_node* temp_ptr;
	hash_table_item* temp_ht_item;
	char temp_str[BUFSIZ];
	char temp_read_str_1[BUFSIZ];
	char temp_read_str_2[BUFSIZ];
	char temp_read_str_0[BUFSIZ];
	string temp_string_key = string_init();
	string temp_string_value = string_init();
	size_t last_word_len;
	size_t new_word_len;

	if ((data_file = fopen("task_9.txt", "r")) == NULL) {
		string_free(&temp_string_key);
		string_free(&temp_string_value);
		return OPEN_FILE_ERROR;
	}

	if ((result_file = fopen("task_9_res.txt", "w")) == NULL) {
		fclose(data_file);
		string_free(&temp_string_key);
		string_free(&temp_string_value);
		return OPEN_FILE_ERROR;
	}

	hash_table* ht = ht_init(HASHSIZE, sizeof(string), sizeof(string), hash_function, ht_destructor, strcmp, cust_cpy);

	while (fgets(temp_str, BUFSIZ, data_file) != NULL) {
		read_flag = 1;

		if (temp_str[0] == '\n') {
			line++;
			continue;
		}

		define_flag = 0; read_level = 0; str_index = 0;
		for (i = 0; i < BUFSIZ; i++) {
			if (temp_str[i] == '\n') {
				if (read_level == 1) { temp_read_str_1[str_index] = 0; }
				else if (read_level == 2) { temp_read_str_2[str_index] = 0; }
				read_level++; str_index = 0;
				break;
			}
			else if (read_level <2 && temp_str[i] == ' ') {
				if (read_level == 1) { temp_read_str_1[str_index] = 0; }
				else if (read_level == 2) { temp_read_str_2[str_index] = 0; }

				read_level++; str_index = 0;
			}
			else if (temp_str[i] == '#' && define_flag == 0) {
				define_flag = 1;
			}
			else {
				if (define_flag == 1) {
					if (read_level == 1) {
						temp_read_str_1[str_index] = temp_str[i];
						str_index++;
					}
					else if (read_level == 2) {
						temp_read_str_2[str_index] = temp_str[i];
						str_index++;
					}
				}
				else {
					continue;
				}
			}
		}
		if (read_level == 0) {
			printf("ERROR: incorrect define in line %d\n", line);
		}
		else if (read_level == 1) {
			temp_read_str_2[str_index] = 0;
		}

		if (define_flag) {
			printf("*** #define %s %s\n", temp_read_str_1, temp_read_str_2);

			temp_string_key = string_from(temp_read_str_1, strlen(temp_read_str_1));
			temp_string_value = string_from(temp_read_str_2, strlen(temp_read_str_2));
			ht_insert(ht, &temp_string_key, &temp_string_value);
			string_free(&temp_string_key);
			string_free(&temp_string_value);
			temp_read_str_1[0] = 0;
			temp_read_str_2[0] = 0;
		}
		else {
			for (i = 0; i < ht->size; i++) {
				if (ht->overflow_buckets[i] != NULL) {
					temp_ptr = ht->overflow_buckets[i]->first_node;
					while (temp_ptr != NULL) {
						temp_ht_item = temp_ptr->data;
						if (strstr(&temp_str, *(string*)temp_ht_item->key) != NULL) {
							replace_all_occurrences(temp_str, *(string*)temp_ht_item->key, *(string*)temp_ht_item->value);
						}
						temp_ptr = temp_ptr->next_node;
					}
				}
			}
			printf("%s", temp_str);
			fprintf(result_file, temp_str);
		}
		line++;
	}
	if (!read_flag) {
		printf("File is empty =(. Programm is stopped.");
		string_free(&temp_string_key);
		string_free(&temp_string_value);
	}
	//ht_print(ht, print_function);

	fclose(result_file);
	fclose(data_file);
	ht_free(ht);

	_CrtDumpMemoryLeaks();

	return 0;
}

int replace_all_occurrences(char* temp_str, const char* key, const char* value) {

	if (key == NULL || value == NULL) return NULL_POINTER;

	int temp_len = strlen(temp_str);
	int key_len = strlen(key);
	int value_len = strlen(value);

	int diff = value_len - key_len;
	if (diff < 0) diff *= -1;
	int new_len = temp_len + diff * 100;
	char* new_str = (char*)malloc(new_len + 1);
	if (new_str == NULL) return MEMORY_ALLOCATE_ERROR; 

	char* src_ptr = temp_str;
	char* dst_ptr = new_str; 

	while (*src_ptr) {
		if (strncmp(src_ptr, key, key_len) == 0) {
			strcpy(dst_ptr, value);
			dst_ptr += value_len;
			src_ptr += key_len;
		}
		else {
			*dst_ptr++ = *src_ptr++;
		}
	}
	*dst_ptr = '\0';

	strcpy(temp_str, new_str);
	free(new_str);
}