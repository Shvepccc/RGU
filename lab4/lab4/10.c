#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "../../libs/error.h"
#include <math.h>

#define STDIN_STREAM 1
#define FILE_STREAM 2
#define STR_STREAM 3

#define P_INT 1
#define P_DOUBLE 2
#define P_CHAR 3
#define P_FLOAT 4
#define P_STRING 5

#define INVALID_STREAM_PTR -1

int _preprocessing_scan_data(int stream, void* stream_ptr, char* format, va_list args);
int __scan_from_stream(int stream, void** stream_ptr, char dest[BUFSIZ]);

int overscanf(char* format, ...);
int overfscanf(FILE* stream, char* format, ...);
int oversscanf(char* str, char* format, ...);

int convert_TO_decimal(char* n, int base, int* ans);
char* error_processing(int err);
void generate_fib_count(int n, int* fib_arr, int* count);

int rome_TO_int(char* str, int* ans);
int str_TO_float(char* str, double* ans);

int main() {
	int err;
	char dest[BUFSIZ];
	FILE* document;
	char str[] = "45 -35 17.45 78.681468 hello h 173 aaa AAA CXXXII 0000101000011 a12 12 A12 12";

	int d = 0, o = 0, x = 0, X = 0, Ro = 0, Zr = 0, Cv = 0, CV = 0;
	double f = 0.0;
	double lf = 0.0;
	unsigned int u = 0;
	char c;
	char s[BUFSIZ];

	// TERMINAL
	err = overscanf("%d %u %f %lf %s %c %o %x %X %Ro %Zr %Cv %CV", &d, &u, &f, &lf, s, &c, &o, &x, &X, &Ro, &Zr, &Cv, &CV);
	if (err) {
		printf(error_processing(err));
	}
	else {
		printf("\n----- overscanf result -----\n");
		printf("Result: %%d %d, %%u %u, %%f %f, %%lf %f, %%s %s, %%c %c, %%o %o, %%x %x, %%X %X, %%Ro %d, %%Zr %d, %%Cv %d, %%CV %d\n\n", d, u, f, lf, s, c, o, x, X, Ro, Zr, Cv, CV);
	}
	d = 0, u = 0, o = 0, x = 0, X = 0, Ro = 0, Zr = 0, Cv = 0, CV = 0;
	
	// FILE
	if ((document = fopen("task_10.txt", "r")) == NULL) {
		printf("Cannot open file");
		return OPEN_FILE_ERROR;
	}
	err = overfscanf(document, "%d %u %f %lf %s %c %o %x %X %Ro %Zr %Cv %CV", &d, &u, &f, &lf, s, &c, &o, &x, &X, &Ro, &Zr, &Cv, &CV);
	fclose(document);
	if (err) {
		printf(error_processing(err));
	}
	else {
		printf("----- overfscanf result -----\n");
		printf("Result: %%d %d, %%u %u, %%f %f, %%lf %f, %%s %s, %%c %c, %%o %o, %%x %x, %%X %X, %%Ro %d, %%Zr %d, %%Cv %d, %%CV %d\n\n", d, u, f, lf, s, c, o, x, X, Ro, Zr, Cv, CV);
	}
	d = 0, o = 0, u = 0, x = 0, X = 0, Ro = 0, Zr = 0, Cv = 0, CV = 0;

	// STRING
	err = oversscanf(str, "%d %u %f %lf %s %c %o %x %X %Ro %Zr %Cv %CV", &d, &u, &f, &lf, s, &c, &o, &x, &X, &Ro, &Zr, &Cv, &CV);
	if (err) {
		printf(error_processing(err));
	}
	else {
		printf("----- oversscanf result -----\n");
		printf("Result: %%d %d, %%u %u, %%f %f, %%lf %f, %%s %s, %%c %c, %%o %o, %%x %x, %%X %X, %%Ro %d, %%Zr %d, %%Cv %d, %%CV %d", d, u, f, lf, s, c, o, x, X, Ro, Zr, Cv, CV);
	}

	return OK;
}

int overscanf(char* format, ...) {
	int err;
	if (format == NULL) {
		return NULL_POINTER;
	}
	va_list args;
	va_start(args, format);

	err = _preprocessing_scan_data(STDIN_STREAM, (void*)stdin, format, args);

	va_end(args);
	return err;
}

int overfscanf(FILE* stream, char* format, ...) {
	int err;
	if (stream == NULL || format == NULL) {
		return NULL_POINTER;
	}
	va_list args;
	va_start(args, format);

	err = _preprocessing_scan_data(FILE_STREAM, (void*)stream, format, args);

	va_end(args);
	return err;
}

int oversscanf(char* str, char* format, ...) {
	int err;
	char* str_ptr;
	if (str == NULL || format == NULL) {
		return NULL_POINTER;
	}
	va_list args;
	va_start(args, format);
	str_ptr = str;
	err = _preprocessing_scan_data(STR_STREAM, (void*)str_ptr, format, args);

	va_end(args);
	return err;
}

int __scan_from_stream(int stream, void** stream_ptr, char dest[BUFSIZ]) {
	FILE** file_stream;
	char* str_stream;
	char* p_str_stream;
	char* dest_cpy = dest;
	char c = 65;

	if (stream_ptr == NULL || dest == NULL) {
		return NULL_POINTER;
	}

	switch (stream) {
		case STDIN_STREAM: {
			c = fgetc(stdin);
			while ((c == ' ' || c == '\t' || c == '\n') && c != EOF) {
				c = fgetc(stdin);
			}
			while (c != '\0' && c != '\n' && c != ' ' && c != '\t') {
				*dest_cpy++ = c;
				c = fgetc(stdin);
			}
			*dest_cpy = 0;
			break;
		}
		case FILE_STREAM: {
			file_stream = (FILE**)stream_ptr;
			c = fgetc(*file_stream);
			while ((c == ' ' || c == '\t' || c == '\n') && c != EOF) {
				c = fgetc(*file_stream);
			}
			while (c != '\0' && c != '\n' && c != ' ' && c != '\t' && c != EOF) {
				*dest_cpy++ = c;
				c = fgetc(*file_stream);
			}
			*dest_cpy = 0;
			break;
		}
		case STR_STREAM: {
			p_str_stream = *((char**)stream_ptr);
			c = *p_str_stream;
			p_str_stream++;
			while ((c == ' ' || c == '\t' || c == '\n') && c != '\0') {
				c = *p_str_stream;
				p_str_stream++;
			}
			while (c != '\0' && c != '\n' && c != ' ' && c != '\t') {
				*dest_cpy++ = c;
				c = *p_str_stream;
				p_str_stream++;
			}
			*dest_cpy = 0;
			*stream_ptr = p_str_stream;
			break;
		}
		default:
			return INVALID_STREAM_PTR;
			break;
	}

	return OK;
}

int _preprocessing_scan_data(int stream, void* stream_ptr, char* format, va_list args) {
	int err;
	int current_format_index = 0, f_str_len;
	char dest[BUFSIZ];
	char dest_2[BUFSIZ];
	void* variable_ptr;
	int temp_int = 0;
	unsigned int* temp_uint_ptr;
	int i;
	int fib_arr[BUFSIZ];
	int count;

	if (stream_ptr == NULL || format == NULL || args == NULL) {
		return NULL_POINTER;
	}

	f_str_len = strlen(format);

	while (current_format_index < f_str_len) {
		
		if (*format == '%') {
			//printf("%c", *(format + 1));
			switch (*(format+1)) {
				case 'd': {
					err = __scan_from_stream(stream, &stream_ptr, dest);
					if (err) { return err; }
					variable_ptr = va_arg(args, int*);
					err = convert_TO_decimal(&dest, 10, variable_ptr);
					if (err) { return err; }

					current_format_index += 2;
					format += 2;
					break;
				}
				case 'u': {
					err = __scan_from_stream(stream, &stream_ptr, dest);
					if (err) { return err; }
					err = convert_TO_decimal(&dest, 10, va_arg(args,unsigned int*));
					if (err) { return err; }

					current_format_index += 2;
					format += 2;
					break;
				}
				case 'f': {
					err = __scan_from_stream(stream, &stream_ptr, dest);
					if (err) { return err; }
					err = str_TO_float(&dest, va_arg(args, double*));
					if (err) { return err; }

					current_format_index += 2;
					format += 2;
					break;
				}
				case 'l': {
					if (*(format + 2) == 'f') {
						err = __scan_from_stream(stream, &stream_ptr, dest);
						if (err) { return err; }
						err = str_TO_float(&dest, va_arg(args, double*));
						if (err) { return err; }

						current_format_index += 3;
						format += 3;
					}
					else
					{
						return UNKNOWN_FLAG;
					}
					break;
				}
				case 'c': {
					switch (stream)
					{
						case STDIN_STREAM: {
							*va_arg(args, char*) = fgetc(stdin);
							break;
						}
						case FILE_STREAM: {
							*va_arg(args, char*) = fgetc((FILE*)stream_ptr);
							break;
						}
						case STR_STREAM: {
							*va_arg(args, char*) = *((char*)stream_ptr);
							stream_ptr = (char*)stream_ptr + 1;
							break;
						}
						default:
							break;
					}

					current_format_index += 2;
					format += 2;
					break;
				}
				case 's': {
					err = __scan_from_stream(stream, &stream_ptr, dest);
					if (err) { return err; }
					strcpy(va_arg(args, char*), dest);

					current_format_index += 2;
					format += 2;
					break;
				}
				case 'o': {
					err = __scan_from_stream(stream, &stream_ptr, dest);
					if (err) { return err; }
					variable_ptr = va_arg(args, int*);
					err = convert_TO_decimal(&dest, 8, variable_ptr);
					if (err) { return err; }

					current_format_index += 2;
					format += 2;
					break;
				}
				case 'x': {
					err = __scan_from_stream(stream, &stream_ptr, dest);
					if (err) { return err; }
					variable_ptr = va_arg(args, int*);
					err = convert_TO_decimal(&dest, 16, variable_ptr);
					if (err) { return err; }

					current_format_index += 2;
					format += 2;
					break;
				}
				case 'X': {
					err = __scan_from_stream(stream, &stream_ptr, dest);
					if (err) { return err; }
					variable_ptr = va_arg(args, int*);
					err = convert_TO_decimal(&dest, 16, variable_ptr);
					if (err) { return err; }

					current_format_index += 2;
					format += 2;
					break;
				}
				case 'R': {
					if (*(format + 2) == 'o') {
						err = __scan_from_stream(stream, &stream_ptr, dest);
						if (err) { return err; }
						
						err = rome_TO_int(&dest, va_arg(args, int*));
						if (err) { return err; }

						current_format_index += 3;
						format += 3;
					}
					else {
						return UNKNOWN_FLAG;
					}
					break;
				}
				case 'Z': {
					if (*(format + 2) == 'r') {
						err = __scan_from_stream(stream, &stream_ptr, dest);
						if (err) { return err; }
						generate_fib_count(strlen(dest), fib_arr, &count);

						for (i = 0; i < strlen(dest) - 1; i++) {
							if (dest[i] == '1') {
								temp_int += fib_arr[i];
							}
						}
						*va_arg(args, int*) = temp_int;
						temp_int = 0;
						current_format_index += 3;
						format += 3;
					}
					else {
						return UNKNOWN_FLAG;
					}
					break;
				}
				case 'C': {
					if (*(format + 2) == 'v') {
						err = __scan_from_stream(stream, &stream_ptr, dest);
						if (err) { return err; }
						err = __scan_from_stream(stream, &stream_ptr, dest_2);
						if (err) { return err; }
						err = convert_TO_decimal(&dest_2, 10, &temp_int);
						if (err) { return err; }

						if (temp_int < 2 || temp_int > 26) {
							temp_int = 10;
						}

						variable_ptr = va_arg(args, int*);
						err = convert_TO_decimal(&dest, temp_int, variable_ptr);
						if (err) { return err; }

						temp_int = 0;
						current_format_index += 3;
						format += 3;
					}
					else if (*(format + 2) == 'V') {
						err = __scan_from_stream(stream, &stream_ptr, dest);
						if (err) { return err; }
						err = __scan_from_stream(stream, &stream_ptr, dest_2);
						if (err) { return err; }
						err = convert_TO_decimal(&dest_2, 10, &temp_int);
						if (err) { return err; }

						if (temp_int < 2 || temp_int > 26) {
							temp_int = 10;
						}

						variable_ptr = va_arg(args, int*);
						err = convert_TO_decimal(&dest, temp_int, variable_ptr);
						if (err) { return err; }

						current_format_index += 3;
						format += 3;
					}
					else {
						return UNKNOWN_FLAG;
					}
					break;
				}
				default:
					return UNKNOWN_FLAG;
					break;
			}
		}
		else {
			current_format_index += 1;
			format++;
		}
	}

	return OK;
}

char* error_processing(int err) {
	switch (err) {
		case INCORRECT_BASE:
			return "INCORRECT_BASE";
		case MEMORY_ALLOCATE_ERROR:
			return "MEMORY_ALLOCATE_ERROR";
		case UNKNOWN_FLAG:
			return "UNKNOWN_FLAG";
		case NOT_ENOUGH_ARGS:
			return "NOT_ENOUGH_ARGS";
		case OPEN_FILE_ERROR:
			return "OPEN_FILE_ERROR";
		case INCORRECT_ARG:
			return "INCORRECT_ARG";
		case NULL_POINTER:
			return "NULL_POINTER";
		default:
			return "Uncknown error";
	}
}

int convert_TO_decimal(char* n, int base, int* ans) {
	int len = 0, j = 0, sign = 0;

	if (n == NULL || ans == NULL) {
		return NULL_POINTER;
	}
	if (base < 2 || base > 36) {
		return INCORRECT_BASE;
	}
	if (n[0] == '0') {
		*ans = 0;
		return 0;
	}

	len = strlen(n);

	if (n[0] == '-') { sign = 1; }

	for (int i = sign; i < len; i++) {
		if (isdigit(n[i])) {
			(*ans) += (int)((int)n[i] - '0') * pow((double)base, len - i - 1);
		}
		else if (isalpha(n[i])) {
			(*ans) += (int)((int)toupper(n[i]) - 'A' + 10) * pow((double)base, len - i - 1);
		}
	}
	if (sign == 1) { (*ans) *= -1; }
	return 0;
}

void generate_fib_count(int n, int* fib_arr, int* count) {
	int a = 0, b = 1;
	*count = 0;

	if (n >= 1) {
		fib_arr[(*count)++] = a;
	}
	if (n >= 2) {
		fib_arr[(*count)++] = b;
	}

	for (int i = 2; i < n; i++) {
		int next = fib_arr[i - 1] + fib_arr[i - 2];
		fib_arr[(*count)++] = next;
	}
}

int rome_TO_int(char* str, int* ans) {
	int len = strlen(str);
	int i = 0, j = 0;
	char letter_1, letter_2 = NULL;
	int temp_int = 0;

	struct roman_item {
		int val;
		int len;
		char* letter;
	};

	struct roman_item roman_data[] = {
	{1000, 1, "M"}, {900, 2, "CM"}, {500, 1, "D"}, {400, 2, "CD"},
	{100, 1, "C"}, {90, 2, "XC"}, {50, 1, "L"}, {40, 2, "XL"},
	{10, 1, "X"}, {9, 2, "IX"}, {5, 1, "V"}, {4, 2, "IV"},
	{1, 1, "I"} };

	while(i < len) {
		letter_1 = str[i];
		letter_2 = str[i+1];
		for (j = 0; j < 13; j++) {
			if (roman_data[j].len == 1 && roman_data[j].letter[0] == letter_1) {
				temp_int = roman_data[j].val;
			}
			if (roman_data[j].len == 2 && roman_data[j].letter[0] == letter_1 && roman_data[j].letter[1] == letter_2) {
				temp_int = roman_data[j].val;
				i++;
				break;
			}
		}
		(*ans) += temp_int;
		i++;
	}
	return OK;
}

int str_TO_float(char* str, double* ans) {
	int i = 0;
	char* left = str;
	char* right = str;
	int left_int = 0, right_int = 0;

	while (*right++ != '.') { i++; };
	left[i++] = 0;

	convert_TO_decimal(left, 10, &left_int);
	convert_TO_decimal(right, 10, &right_int);

	(*ans) = (double)(right_int / pow(10, strlen(right)));
	(*ans) += left_int;

	return OK;
}