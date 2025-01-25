#ifndef PARSE_CONFIG_H_
#define PARSE_CONFIG_H_

#define HASHSIZE_ORDO 128

typedef struct int_settings {
	int base_input;
	int base_output;
	int base_assign;
	int assignment_type;
	int operator_type;
	int debug_mode;
	char* settings_file_name;
	char* instructions_file_name;
	string equal_mark_str;
} int_settings;

void ht_print_function_ordo(int i, void const* key, void const* value);
int set_configuration(char* file_name, hash_table** ht_ptr, int_settings* settings_ptr);
int process_argv(int argc, char* argv[], int_settings* main_settings);
void print_int_settings(int_settings* main_settings);
int hash_function_ordo(string* str);
int cust_string_cpm_ordo(const string* a, const string* b);

#endif