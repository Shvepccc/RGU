#ifndef STRING_H_
#define STRING_H_

#include <stdlib.h>

#define STRING_INITIAL_CAPACITY 16
#define STRING_GROWTH_FACTOR 2

typedef struct string_metadata_t {
	size_t size;
	size_t capacity;
} string_metadata_t;

typedef char* string;

#define __string_str_to_base(str) ((((string_metadata_t *)((str))) -1))
#define __string_base_to_str(str) ((char*)(((string_metadata_t *)((str))) + 1))

#define string_size(str) ((str) ? __string_str_to_base((str))->size : 0)
#define string_cap(str) ((str) ? __string_str_to_base((str))->capacity : 0)

string string_init();
string string_from(const char*, int len);
void string_free(string* str);

int string_cpy(string* src_str, string* dest_str);
string string_cpy_new(string const src_str);
int string_cat(string* str1, string* str2);

int string_push_back(string* str, char symbol);
int string_pop_back(string* str);

int string_flexographic_compare(string const str1, string const str2);
int string_flexographic_compare_char(string const str1, char* const str2);
int string_flexographic_compare_char_char(char* const str1, char* const str2);
int string_compare(string const str1, string const str2);

#endif // !STRING_H_
