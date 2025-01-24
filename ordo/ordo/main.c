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
#include "../../libs/process_notation.h"
#include "../../libs/execute_instruction.h"


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

int print_errors(int err) {
	if (err > 2000) {
		printf("Error: BALANCE_OF_PARENTHESIS_IS_BROKEN (2001)\n");
	}
	else {
		printf("Error: %s (code - %d)\n", errors_names_array[err - 101], err);
	}
}

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

	err = run_instruction(&ht_operators_ptr, &main_settings); // divine function hehe...
	if (err) {
		print_errors(err);
		return err;
	}

	//TODO: free memory
	string_free(&main_settings.equal_mark_str);

	_CrtDumpMemoryLeaks();

	return 0;
}