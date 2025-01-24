#define _CRT_SECURE_NO_WARNINGS

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <stdio.h>
#include <string.h>

#include "../../libs/error.h"
#include "../../libs/hash_table.h"
#include "../../libs/cust_string.h"
#include "../../libs/convertors.h"
#include "../../libs/parse_config.h"
#include "../../libs/convert_notations.h"

/* --- GET INSTRUCTION --- */

void ht_destructor_ordo_variables(hash_table_item* item) {
	if (item == NULL) return;
	string_free((string*)item->key);
	free(item->value);
	free(item);
}

int cust_cpy_ordo_variables(hash_table_item* item, void* key, void* value) {
	if (item == NULL || key == NULL || value == NULL) {
		return NULL_POINTER;
	}
	*(string*)(*item).key = string_cpy_new(*(string*)key);
	memcpy((*item).value, value, sizeof(int));
	return OK;
}

int run(hash_table** operators_ht_ptr, int_settings* main_settings) {
	char symbol = 0, last_symbol;
	FILE* istructions_file;
	int last_exit = 0, line = 0, part_number = 0, multi_line_comment = 0, parenthesis_balacne = 0, one_line_comment = 0;
	int equal_mark_index = 0, equal_mark_len, is_unary = 0;
	int err = 0;
	string first_part, second_part;

	if ((istructions_file = fopen(main_settings->instructions_file_name, "r")) == NULL) {
		return OPEN_FILE_ERROR;
	}

	first_part = string_init();
	second_part = string_init();
	equal_mark_len = strlen(main_settings->equal_mark_str);

	hash_table* variables_ht = ht_init(HASHSIZE_ORDO*2, sizeof(string), sizeof(int),
		hash_function_ordo, ht_destructor_ordo_variables, cust_string_cpm_ordo, cust_cpy_ordo_variables);

	while(1) {
		//TODO: check empty file
		last_symbol = symbol;
		if ((symbol = fgetc(istructions_file)) == EOF) {
			if (string_size(first_part) != 0 || string_size(second_part) != 0) {
				last_exit = 1;
			}
			else {
				break;
			}
		}

		if (equal_mark_index != 0 && part_number == 1) { 
			equal_mark_index--;
			if (equal_mark_index == 0) {
				part_number++;
			}
			continue;
		}

		if (symbol == '\n') { 
			line++; 
			one_line_comment = 0;
			continue;
		}

		if (symbol == ' ' && (last_symbol == ' ' || last_symbol == ';') || (symbol == ')' && is_unary)) {
			continue;
		}

		if (symbol == '[') {
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

		//TODO: change handle equal mark
		if (symbol == main_settings->equal_mark_str[equal_mark_index]) {
			equal_mark_index++;
		}
		else {
			equal_mark_index = 0;
		}

		if ((equal_mark_index == equal_mark_len) || symbol == ';' || last_exit || 
			(symbol == '(' && part_number == 0)) {

			if (part_number == 2) {
				printf("\nINSTRUCTION: '%s' || %s || '%s'\n", first_part, main_settings->equal_mark_str, second_part);

				//TODO: do instruction
				if (is_unary) {
					err = unary_operations(operators_ht_ptr, &variables_ht, &first_part, &second_part, main_settings);
					if (err) break;
				}
				else {
					if (main_settings->assignment_type = 'l') {
						binary_operations(operators_ht_ptr, &variables_ht, &first_part, &second_part, main_settings);
					}
					else {
						binary_operations(operators_ht_ptr, &variables_ht, &second_part, &first_part, main_settings);
					}
				}

				string_free(&first_part);
				string_free(&second_part);
				first_part = string_init();
				second_part = string_init();
				is_unary = 0;
			}
			if (symbol == '(' && part_number == 0) {
				part_number++;
				is_unary = 1;
			}
			part_number++;
			if (symbol == ';') part_number = 0;
		}
		else {
			if (part_number == 0) { string_push_back(&first_part, symbol); }
			else if (part_number == 2) { string_push_back(&second_part, symbol); }
		}

		if (last_exit == 1 || err != 0) {
			break;
		}
	}
	fclose(istructions_file);
	string_free(&first_part);
	string_free(&second_part);

	if (err) return err;

	return OK;
}

int cust_cpy_for_search_ordo(void* dst, void* src) {
	if (dst == NULL || src == NULL) {
		return NULL_POINTER;
	}
	*(string*)dst = string_cpy_new(*(string*)src);
	return OK;
}

int unary_operations(hash_table** operators_ht_ptr, 
	hash_table** vars_ht_ptr, 
	string* first_part, string* second_part,
	int_settings* main_settings) 
{
	string temp_string;
	int err, temp_int = 0;
	char temp_str[BUFSIZ];
	char* temp_str_ptr;

	if (operators_ht_ptr == NULL || vars_ht_ptr == NULL || 
		first_part == NULL || second_part == NULL || main_settings == NULL) {
		return NULL_POINTER;
	}
	
	err = ht_search_cust_cpy(*operators_ht_ptr, first_part, &temp_string, cust_cpy_for_search_ordo);
	if (err) return INVALID_INSTRUCTION;

	if (strcmp(temp_string, "input") == 0) {
		printf("Enter value for '%s': ", *second_part);
		err = scanf("%s", temp_str);
		if (err != 1) return INCORRECT_ARG;

		convert_TO_decimal(temp_str, main_settings->base_input, &temp_int);
		ht_insert(*vars_ht_ptr, second_part, &temp_int);
	}
	else if (strcmp(temp_string, "output") == 0) {
		err = ht_search(*vars_ht_ptr, second_part, &temp_int);
		if (err) return INVALID_INSTRUCTION;

		convert_FROM_decimal(temp_int, main_settings->base_output, &temp_str_ptr);
		printf("%s = %s\n", *second_part, temp_str_ptr);
	}
	else if (strcmp(temp_string, "not") == 0) {

	}
	return OK;
}

int lexem_character_selector_ordo(char c) {
	if (c == '(' || c == ')' || c == ' ' || 
		c == '\0' || c == ',') {
		return 1;
	}
	return 0;
}

int priorities(char* operator) {
	// ^ - 2
	// *, /, % - 1
	// +, - - 0
	if (strcmp(operator, "add") == 0 ||
		strcmp(operator, "sub") == 0)
	{
		return 0;
	}
	else if (strcmp(operator, "mult") == 0 ||
		strcmp(operator, "div") == 0 ||
		strcmp(operator, "rem") == 0)
	{
		return 1;
	}
	else if (strcmp(operator, "pow") == 0)
	{
		return 2;
	}
	else if (strcmp(operator, "(") == 0)
	{
		return INT_MIN;
	}
}

int binary_operations(hash_table** operators_ht_ptr, 
	hash_table** vars_ht_ptr, 
	string* first_part, string* second_part,
	int_settings* main_settings) {
	int err;
	char* postfix_expression;

	if (operators_ht_ptr == NULL || vars_ht_ptr == NULL ||
		first_part == NULL || second_part == NULL || main_settings == NULL) {
		return NULL_POINTER;
	}

	string_push_back(second_part, '\0');
	err = convert_to_postfix_notation_border(*second_part, lexem_character_selector_ordo, priorities, 
		&postfix_expression, operators_ht_ptr, main_settings);
	if (err) return err;
	printf("(postf) '%s'\n", postfix_expression);

	return OK;
}

/* --- END GET INSTRUCTION --- */


char* errors_names_array[] = { "INCORRECT_BASE",
	"MEMORY_ALLOCATE_ERROR",
	"UNKNOWN_FLAG",
	"TOO_MANY_ARGS",
	"NOT_ENOUGH_ARGS",
	"OPEN_FILE_ERROR",
	"INCORRECT_ARG",
	"NULL_POINTER",
	"INDEX_OUT_OF_BORDERS",
	"BAD_DIVISION",
	"EMPTY_FILE",
	"KEY_NOT_FOUND",
	"VALUE_ALREADY_EXISTS",
	"OBJECT_IS_EMPTY",
	"UNKNOWN_SYMBOL",
	"INVALID_INSTRUCTION" };

int main(int argc, char* argv[]) {
	hash_table* ht_operators_ptr = NULL;
	int_settings main_settings;
	int err = 0, i;
	int temp_int;

	if (argc < 3) {
		return NOT_ENOUGH_ARGS;
	}

	err = process_argv(argc, argv, &main_settings);
	if (err) {
		print_errors(err);
		return err;
	}

	err = set_configuration(main_settings.settings_file_name, &ht_operators_ptr, &main_settings);
	if (err == 0) ht_print(ht_operators_ptr, ht_print_function_ordo);
	if (err) {
		print_errors(err);
		return err;
	}

	print_int_settings(&main_settings);

	err = run(&ht_operators_ptr, &main_settings);
	if (err) {
		print_errors(err);
		return err;
	}

	//TODO: free memory
	string_free(&main_settings.equal_mark_str);

	_CrtDumpMemoryLeaks();

	return 0;
}

int print_errors(int err) {
	if (err > 2000) {
		printf("Error: BALANCE_OF_PARENTHESIS_IS_BROKEN (2001)\n");
	}
	else {
		printf("Error: %s (code - %d)\n", errors_names_array[err - 101], err);
	}
}