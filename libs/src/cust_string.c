#include "../../libs/cust_string.h"
#include <stdlib.h>
#include <string.h>
#include "../../libs/error.h"

string string_init() {
	string_metadata_t* str = NULL;
	str = (string_metadata_t*)malloc((sizeof(string_metadata_t)) + (sizeof(char) * STRING_INITIAL_CAPACITY));
	if (str == NULL) {
		return NULL;
	}
	str->size = 0;
	str->capacity = STRING_INITIAL_CAPACITY;
	__string_base_to_str(str)[0] = '\0';
	return __string_base_to_str(str);
}

string string_from(const char* str_src, int len) {
	string_metadata_t* str = NULL;
	str = (string_metadata_t*)malloc((sizeof(string_metadata_t)) + (sizeof(char) * (len+1)));
	if (str == NULL) {
		return NULL;
	}
	memcpy(__string_base_to_str(str), str_src, len * sizeof(char));
	str->size = len;
	str->capacity = len+1;
	__string_base_to_str(str)[len] = '\0';
	return __string_base_to_str(str);
}
void string_free(string* str) {
	free(__string_str_to_base(str));
	return;
}

int string_grow(string* str_ptr, size_t new_size) {
	if (str_ptr == NULL || *str_ptr == NULL) {
		return NULL_POINTER;
	}
	string_metadata_t* for_realloc = NULL;
	size_t current_size = string_size(*str_ptr);
	if (current_size == new_size) {
		return OK;
	}

	for_realloc = (string_metadata_t*)realloc(__string_str_to_base(*str_ptr), (sizeof(int) * new_size) + (sizeof(string_metadata_t)));
	if (for_realloc == NULL) {
		return MEMORY_ALLOCATE_ERROR;
	}

	for_realloc->capacity = new_size;
	if (new_size < current_size) {
		for_realloc->size = new_size;
	}

	*str_ptr = __string_base_to_str(for_realloc);

	return OK;
}

int string_cpy(string* src_str, string* dest_str) {
	int err;
	if (src_str == NULL || dest_str == NULL) {
		return NULL_POINTER;
	}
	if (string_cap(*dest_str) < string_size(*src_str)+1) {
		err = string_grow(dest_str, string_size(*src_str)+1);
		if (err) { return err; }
	}

	memcpy(*dest_str, *src_str, sizeof(char) * (string_size(*src_str)+1));
	__string_str_to_base(*dest_str)->size = string_size(*src_str);

	return OK;
}

string string_cpy_new(string const src_str) {
	int err;
	string_metadata_t* str = NULL;
	
	str = (string_metadata_t*)malloc((sizeof(string_metadata_t)) + (sizeof(char) * (string_size(src_str)+1)));
	if (str == NULL) {
		return NULL;
	}
	memcpy(__string_base_to_str(str), src_str, (string_size(src_str)+1) * sizeof(char));
	str->size = string_size(src_str);
	str->capacity = string_size(src_str)+1;

	return __string_base_to_str(str);
}

int string_cat(string* dest_str, string* src_str) {
	int err;
	if (dest_str == NULL || src_str == NULL) {
		return NULL_POINTER;
	}
	if (string_cap(*dest_str) < (string_size(*dest_str) + string_size(*src_str) + 1)) {
		err = string_grow(dest_str, string_size(*dest_str) + string_size(*src_str) + 1);
		if (err) { return err; }
	}
	memcpy(*dest_str + string_size(*dest_str), *src_str, string_size(*src_str) + 1);
	__string_str_to_base(*dest_str)->size = string_size(*dest_str) + string_size(*src_str);
	
	return OK;
}

int string_push_back(string* str, char symbol) {
	int err;
	if (str == NULL) {
		return NULL_POINTER;
	}
	if (string_cap(*str) < (string_size(*str) + 2)) {
		err = string_grow(str, string_size(*str) + 2);
		if (err) { return err; }
	}
	(*str)[string_size(*str)] = symbol;
	(*str)[string_size(*str)+1] = '\0';
	__string_str_to_base(*str)->size = string_size(*str)+1;
	return OK;
}

int string_pop_back(string* str) {
	int err;
	if (str == NULL) {
		return NULL_POINTER;
	}

	(*str)[string_size(*str)] = '\0';
	__string_str_to_base(*str)->size -= 1;
	return OK;
}

int string_flexographic_compare(string const str1, string const str2) {
	int i;
	for (i = 0; i < (string_size(str1) < string_size(str2) ? string_size(str1) : string_size(str2)); i++) {
		if (str1[i] != str2[i]) {
			return str1[i] - str2[i];
		}
	}
	return 0;
}

int string_flexographic_compare_char(string const str1, char* const str2) {
	int i;
	for (i = 0; i < (string_size(str1) < strlen(str2) ? string_size(str1) : strlen(str2)); i++) {
		if (str1[i] != str2[i]) {
			return str1[i] - str2[i];
		}
	}
	return 0;
}

int string_compare(string const str1, string const str2) {
	int i;
	if (string_size(str1) != string_size(str2)) {
		for (i = 0; i < (string_size(str1) > string_size(str2) ? string_size(str1) : string_size(str2)); i++) {
			if (str1[i] != str2[i]) {
				return str1[i] - str2[i];
			}
		}
		return 0;
	}
	else {
		return string_flexographic_compare(str1, str2);
	}
}