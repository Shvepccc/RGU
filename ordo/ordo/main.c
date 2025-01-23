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


int main(int argc, char* argv[]) {
	hash_table* ht_operators_ptr = NULL;
	int_settings main_settings;
	int err = 0, i;
	int temp_int;

	if (argc < 3) {
		return NOT_ENOUGH_ARGS;
	}

	err = process_argv(argc, argv, &main_settings);
	if (err) return err;

	err = set_instructions(main_settings.settings_file_name, &ht_operators_ptr, &main_settings);
	if (err == 0) ht_print(ht_operators_ptr, ht_print_function_ordo);
	if (err) return err;

	//TODO: Interpreter

	printf("Exit code: %d\n", err);
	print_int_settings(&main_settings);

	_CrtDumpMemoryLeaks();

	return 0;
}