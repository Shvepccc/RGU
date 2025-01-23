#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#include "../../libs/error.h"
#include "../../libs/hash_table.h"
#include "../../libs/cust_string.h"
#include "../../libs/parse_config.h"

int hash_function_ordo(const string* str) {
	//TODO: change hash function
	long int res = 0, value = 0, i;
	char c = 0;

	if (str == NULL) { return NULL_POINTER; }

	for (i = 0; i < string_size(*str); i++) {
		c = (*str)[i];
		value = 0;
		if (isdigit(c)) { value = c - '0'; }
		else if (isupper(c)) { value = c - 'A' + 10; }
		else if (islower(c)) { value = c - 'a' + 36; }
		res = res * 62 + value;
	}
	return res;
}

void ht_print_function_ordo(int i, void const* key, void const* value) {
	if (key == NULL || value == NULL) return;
	printf("Index: %4.d Key: %-8s Value: %-8s\n", i, *(string const*)key, *(string const*)value);
}

void ht_destructor_ordo(hash_table_item* item) {
	if (item == NULL) return;
	string_free((string*)item->key);
	string_free((string*)item->value);
	free(item);
}

int cust_cpy_ordo(hash_table_item* item, void* key, void* value) {
	if (item == NULL || key == NULL || value == NULL) {
		return NULL_POINTER;
	}
	*(string*)(*item).key = string_cpy_new(*(string*)key);
	*(string*)(*item).value = string_cpy_new(*(string*)value);
	return OK;
}

int cust_string_cpm_ordo(const string* a, const string* b) {
	return strcmp(*a, *b);
}



int set_instructions(char* file_name, hash_table** ht_ptr, int_settings* settings_ptr) {
	FILE* settings_file;
	char symbol;
	int one_line_comment = 0, multi_line_comment = 0, first_part_is_done = 0, last_exit = 0, parenthesis_balacne = 0;
	int i, err = 0, line = 0;
	string first_part, second_part, operator;
	char* key_operators_array[] = { "add", "mult", "sub", "pow", "div", "rem", "xor", "and", "or", "=", "not", "input", "output" };

	if (file_name == NULL) {
		return NULL_POINTER;
	}

	if ((settings_file = fopen(file_name, "r")) == NULL) {
		return OPEN_FILE_ERROR;
	}

	*ht_ptr = ht_init(HASHSIZE_ORDO, sizeof(string), sizeof(string), 
		hash_function_ordo, ht_destructor_ordo, cust_string_cpm_ordo, cust_cpy_ordo);
	first_part = string_init();
	second_part = string_init();
	for (i = 0; i < (sizeof(key_operators_array) / sizeof(key_operators_array[0])); i++) {
		operator = string_from_l(key_operators_array[i]);
		ht_insert(*ht_ptr, &operator, &operator);
		string_free(&operator);
	}

	while (1) {
		if ((symbol = fgetc(settings_file)) == EOF) {
			if (string_size(first_part) != 0 || string_size(second_part) != 0) {
				last_exit = 1;
			}
			else {
				break;
			}
		}

		if (symbol == '\n') line++;

		if (symbol == '[') {

			first_part_is_done = 0;
			string_free(&first_part);
			string_free(&second_part);
			first_part = string_init();
			second_part = string_init();


			parenthesis_balacne++;
			multi_line_comment = 1;
			continue;
		}
		else if (multi_line_comment == 1 && symbol != ']') {
			continue;
		}
		else if (symbol == ']') {
			parenthesis_balacne--;
			if (parenthesis_balacne < 0) {
				break;
			}
			else if (parenthesis_balacne == 0) {
				multi_line_comment = 0;
			}
			continue;
		}

		if (symbol == '#') {
			one_line_comment = 1;
			continue;
		}
		else if (one_line_comment == 1 && symbol != '\n') {
			continue;
		}
		else if (one_line_comment == 1 && symbol == '\n') {
			one_line_comment = 0;
			continue;
		}

		if (symbol == ' ' || symbol == '\n' || last_exit) {
			if (first_part_is_done == 0) {
				if (string_flexographic_compare_char(first_part, "left=") == 0) {
					settings_ptr->assignment_type = 'l';
				}
				else if (string_flexographic_compare_char(first_part, "right=") == 0) {
					settings_ptr->assignment_type = 'r';
				}
				else if (string_flexographic_compare_char(first_part, "()op") == 0) {
					settings_ptr->operator_type = 1;
				}
				else if (string_flexographic_compare_char(first_part, "(op)") == 0) {
					settings_ptr->operator_type = 2;
				}
				else if (string_flexographic_compare_char(first_part, "op()") == 0) {
					settings_ptr->operator_type = 3;
				}
				else if (ht_search(*ht_ptr, &first_part, NULL) != OK) {
					printf("ERROR: unknown instruction - %s in line %d\n", first_part, line);
					err = UNKNOWN_FLAG;
				}

				if (symbol == '\n') {
					string_free(&first_part);
					first_part = string_init();
				}
			}
			else {
				ht_delete(*ht_ptr, &first_part);
				ht_insert(*ht_ptr, &first_part, &second_part);

				string_free(&first_part);
				string_free(&second_part);
				first_part = string_init();
				second_part = string_init();
			}
			first_part_is_done++;
			if (symbol == '\n') first_part_is_done = 0;
		}
		else {
			first_part_is_done == 0 ?
				string_push_back(&first_part, symbol) :
				string_push_back(&second_part, symbol);
		}

		if (last_exit == 1 || err != 0) {
			break;
		}
	}

	fclose(settings_file);

	if (!err) {
		err = ht_swap_key_with_value_ordo(ht_ptr);
	}

	if (parenthesis_balacne != 0 && err == 0) err = BALANCE_OF_PARENTHESIS_IS_BROKEN;
	if (err != 0) {
		ht_free(*ht_ptr);
		string_free(&first_part);
		string_free(&second_part);
		string_free(&operator);
		return err;
	}
	return OK;
}

int process_argv(int argc, char* argv[], int_settings* main_settings) {
	int err, i;
	int temp_int;

	if (main_settings == NULL) {
		return NULL_POINTER;
	}

	main_settings->base_assign = 10;
	main_settings->base_input = 10;
	main_settings->base_output = 10;
	main_settings->assignment_type = 'l';
	main_settings->debug_mode = 0;
	main_settings->operator_type = 1;
	main_settings->instructions_file_name = NULL;
	main_settings->settings_file_name = NULL;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "base_input") == 0) {
			err = convert_TO_decimal(argv[i + 1], 10, &temp_int);
			if (err) return err;
			if (temp_int < 2 || temp_int > 36) return INCORRECT_ARG;
			main_settings->base_input = temp_int;
			i++;
		}
		else if (strcmp(argv[i], "base_assign") == 0) {
			err = convert_TO_decimal(argv[i + 1], 10, &temp_int);
			if (err) return err;
			if (temp_int < 2 || temp_int > 36) return INCORRECT_ARG;
			main_settings->base_assign = temp_int;
			i++;
		}
		else if (strcmp(argv[i], "base_output") == 0) {
			err = convert_TO_decimal(argv[i + 1], 10, &temp_int);
			if (err) return err;
			if (temp_int < 2 || temp_int > 36) return INCORRECT_ARG;
			main_settings->base_output = temp_int;
			i++;
		}
		else if (strcmp(argv[i], "-d") == 0 ||
			strcmp(argv[i], "--debug") == 0 ||
			strcmp(argv[i], "/debug") == 0) {
			main_settings->debug_mode = 1;
		}
		else if (strcmp(argv[i], "-i") == 0) {
			main_settings->instructions_file_name = argv[i + 1];
			i++;
		}
		else if (strcmp(argv[i], "-s") == 0) {
			main_settings->settings_file_name = argv[i + 1];
			i++;
		}
		else {
			return UNKNOWN_FLAG;
		}
	}
	return OK;
}

void print_int_settings(int_settings* main_settings)
{
	printf("\n\nInterpreter Settings\n-------------------\n");
	printf("debug_mode: %d\n", main_settings->debug_mode);

	printf("base_input: %d\n", main_settings->base_input);
	printf("base_output: %d\n", main_settings->base_output);
	printf("base_assign: %d\n", main_settings->base_assign);

	printf("operator_type: %d\n", main_settings->operator_type);
	printf("assignment_type: %c\n", main_settings->assignment_type);

	printf("settings_file_name: %s\n", main_settings->settings_file_name);
	printf("instructions_file_name: %s\n", main_settings->instructions_file_name);
	printf("-------------------\n");
}


int ht_swap_key_with_value_ordo(hash_table** ht_ptr) {

	if (*ht_ptr == NULL) {
		return NULL_POINTER;
	}
	int err;
	hash_table_item* item;
	u_list* list;
	u_list_node* temp_ptr;
	hash_table* new_ht = ht_init(HASHSIZE_ORDO, sizeof(string), sizeof(string), 
		hash_function_ordo, ht_destructor_ordo, cust_string_cpm_ordo, cust_cpy_ordo);

	for (int i = 0; i < (*ht_ptr)->size; i++) {
		list = (*ht_ptr)->overflow_buckets[i];
		if (list != NULL) {
			temp_ptr = list->first_node;
			while (temp_ptr != NULL) {
				item = temp_ptr->data;
				//print_function(i, item->key, item->value);
				err = ht_insert(new_ht, item->value, item->key);
				if (err) {
					ht_free(*ht_ptr);
					ht_free(new_ht);
					return err;
				}
				temp_ptr = temp_ptr->next_node;
			}
		}
	}
	ht_free(*ht_ptr);
	*ht_ptr = new_ht;
	return OK;
}