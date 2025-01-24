#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../../libs/error.h"
#include "../../libs/hash_table.h"
#include "../../libs/cust_string.h"
#include "../../libs/convertors.h"
#include "../../libs/parse_config.h"
#include "../../libs/process_notation.h"
#include "../../libs/execute_instruction.h"

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

void ht_print_function_variables_ordo(int i, void const* key, void const* value) {
	if (key == NULL || value == NULL) return;
	printf("Index: %4.d Key: %-8s Value: %-4.d\n", i, *(string const*)key, *(int const*)value);
}

int run_instruction(hash_table** operators_ht_ptr, int_settings* main_settings) {
	char symbol = 0, last_symbol;
	FILE* istructions_file;
	int last_exit = 0, line = 0, part_number = 0, multi_line_comment = 0, parenthesis_balacne = 0, one_line_comment = 0;
	int equal_mark_index = 0, equal_mark_len, is_unary = 0, read_file_flag = 0, debug_flag = 0;
	int err = 0;
	string first_part, second_part, temp_string;

	if ((istructions_file = fopen(main_settings->instructions_file_name, "r")) == NULL) {
		return OPEN_FILE_ERROR;
	}

	first_part = string_init();
	second_part = string_init();
	temp_string = string_init();
	equal_mark_len = strlen(main_settings->equal_mark_str);

	hash_table* variables_ht = ht_init(HASHSIZE_ORDO * 2, sizeof(string), sizeof(int),
		hash_function_ordo, ht_destructor_ordo_variables, cust_string_cpm_ordo, cust_cpy_ordo_variables);

	while (1) {

		if (debug_flag) {
			//TODO: debug functions
			printf("\nBREAKPOINT\n------------------------------------\n");
			string_free(&temp_string);
			temp_string = string_init();
			debug_flag = 0;
		}

		last_symbol = symbol;
		if ((symbol = fgetc(istructions_file)) == EOF) {
			if (string_size(first_part) != 0 || string_size(second_part) != 0) {
				last_exit = 1;
			}
			else {
				break;
			}
		}
		read_file_flag = 1;

		if (equal_mark_index != 0 && part_number == 1) {
			equal_mark_index--;
			if (equal_mark_index == 0) {
				part_number++;
			}
			continue;
		}

		if (symbol == '\n') {
			if (strstr(temp_string, "BREAKPOINT") != NULL) {
				debug_flag = 1;
			}
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
			string_push_back(&temp_string, symbol);
			continue;
		}

		//TODO: change handle equal mark for the best result
		if (symbol == main_settings->equal_mark_str[equal_mark_index]) {
			equal_mark_index++;
		}
		else {
			equal_mark_index = 0;
		}

		if ((equal_mark_index == equal_mark_len) || symbol == ';' || last_exit ||
			(symbol == '(' && part_number == 0)) {

			if (part_number == 2) {
				if (first_part[string_size(first_part)-1] == ' ') {
					string_pop_back(&first_part);
				}
				printf("\nINSTRUCTION: '%s' || %s || '%s'\n", first_part, main_settings->equal_mark_str, second_part);
				
				if (is_unary) {
					err = unary_operations(operators_ht_ptr, &variables_ht, &first_part, &second_part, main_settings);
				}
				else {
					if (main_settings->assignment_type = 'l') {
						err = binary_operations(operators_ht_ptr, &variables_ht, &first_part, &second_part, main_settings);
					}
					else {
						err = binary_operations(operators_ht_ptr, &variables_ht, &second_part, &first_part, main_settings);
					}
				}
				if (err) break;

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

	if (!read_file_flag) {
		return EMPTY_FILE;
	}

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
	else {
		return INVALID_INSTRUCTION;
	}
	return OK;
}

int lexem_character_selector_ordo(char c) {
	if (c == ')' || c == '(' || c == ' ' || c == '\0') {
		return 1;
	}
	return 0;
}

int ignore_lexems(char c) {
	if (c == ',' || c == ' ') {
		return 1;
	}
	return 0;
}

int priorities(char* operator) {
	// ~ - 6
	// ^ - 5
	// *, /, % - 4
	// +, - - 3
	// & - 2
	// ^ - 1
	// | - 0

	if (strcmp(operator, "|") == 0) {
		return 0;
	}
	else if (strcmp(operator, "^") == 0) {
		return 1;
	}
	else if (strcmp(operator, "&") == 0) {
		return 2;
	}
	else if (strcmp(operator, "add") == 0 ||
		strcmp(operator, "sub") == 0)
	{
		return 3;
	}
	else if (strcmp(operator, "mult") == 0 ||
		strcmp(operator, "div") == 0 ||
		strcmp(operator, "rem") == 0)
	{
		return 4;
	}
	else if (strcmp(operator, "pow") == 0) {
		return 5;
	}
	else if (strcmp(operator, "not") == 0) {
		return 6;
	}
	else if (strcmp(operator, "(") == 0)
	{
		return INT_MIN;
	}
}

int plus_func(int a, int b) {
	return a + b;
}
int sub_func(int a, int b) {
	return a - b;
}
int mult_func(int a, int b) {
	return a * b;
}
int div_func(int a, int b) {
	return a / b;
}
int mod_func(int a, int b) {
	return a % b;
}
int pow_func(int a, int b) {
	return (int)pow((double)a, b);
}
int xor_func(int a, int b) {
	return a ^ b;
}
int and_func(int a, int b) {
	return a & b;
}
int or_func(int a, int b) {
	return a | b;
}
int not_func(int a, int b) {
	return ~a;
}

int binary_operations(hash_table** operators_ht_ptr,
	hash_table** vars_ht_ptr,
	string* first_part, string* second_part,
	int_settings* main_settings) {
	int err = 0, temp_int = 0, oper_type;
	char* postfix_expression = NULL;

	oper plus = { "add", binary, plus_func};
	oper sub = { "sub", binary, sub_func};
	oper mult = { "mult", binary, mult_func};
	oper div = { "div", binary, div_func};
	oper mod = { "rem", binary, mod_func};
	oper pow_op = { "pow", binary, pow_func};
	oper xor_op = { "xor", binary, xor_func };
	oper and_op = { "and", binary, and_func };
	oper or_op = { "or", binary, or_func };
	oper not_op = { "not", unary, not_func };

	if (operators_ht_ptr == NULL || vars_ht_ptr == NULL ||
		first_part == NULL || second_part == NULL || main_settings == NULL) {
		return NULL_POINTER;
	}

	string_push_back(second_part, '\0');

	if (main_settings->operator_type == 1) {
		//TODO: prefix to postfix
	}
	else if (main_settings->operator_type == 2) {
		err = infix_to_postfix_notation_border(*second_part, lexem_character_selector_ordo, ignore_lexems, priorities,
			&postfix_expression, operators_ht_ptr, main_settings);
	}
	else if (main_settings->operator_type == 3) {
		//TODO: postfix to postfix
	}

	if (err) return err;
	printf("(postf) '%s'\n", postfix_expression);

	err = calculate_postfix_expression_border(postfix_expression, &temp_int, main_settings, operators_ht_ptr, vars_ht_ptr,
		10, plus, sub, mult, div, mod, pow_op, xor_op, and_op, or_op, not_op);
	if (err) return err;
	printf("(res) %d\n", temp_int);

	ht_insert(*vars_ht_ptr, first_part, &temp_int);

	return OK;
}