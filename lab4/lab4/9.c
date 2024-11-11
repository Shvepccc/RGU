//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdarg.h>
//#include <stdlib.h>
//#include <string.h>
//#include <math.h>
//#include <ctype.h>
//#include "../../libs/error.h"
//#include "../../libs/memory.h"
//
//
//int _preprocessing_print_data(char** ans, char* format, va_list args);
//void _update_data_in_preprocessing_print_data(int clean, int flag_len, int* current_ans_index, int* current_format_index, char** proc_str, char** temp_str, char** format);
//
//int overprintf(char* format, ...);
//int overfprintf(FILE* file,char* format, ...);
//int oversprintf(char* buf, char* format, ...);
//
//int convert_FROM_decimal(int num, int base, char** ans);
//int convert_TO_decimal(char* n, int base, int* ans);
//int float_to_str(float n, char** ans);
//int int_TO_roman(int n, char** ans);
//void generate_fib_limit(int limit, int* fib_arr, int* count);
//int find_zeckendorf(int target, int* ans, int* count);
//
//char* error_processing(int err);
//
//int main() {
//	FILE* document;
//	char* arr[BUFSIZ];
//	int err;
//
//	char* format_str = "Default flags: %%d %d, %%u %u, %%f %f, %%lf %lf, %%c %c, %%s %s, %%o %o, %%x %x, %%X %X \n\
//Custom flags: %%Ro %Ro, %%Zr %Zr, %%Cv %Cv, %%CV %CV, %%to %to, %%TO %TO, \n%%mi %mi, \n%%mu %mu, \n%%md %md, \n%%mf %mf\n\n";
//
//	// TERMINAL
//	printf("----- overprintf result -----\n");
//	err = overprintf(format_str, 15, -35, 1.3333, 12.65, 'h', "Hello world!", 789, 123, 123, 
//					132, 100, 789, 16, 789, 16, "aaa", 12, "AAA", 12, 16, -16, 52.52, 12.34);
//	if (err) {
//		printf(error_processing(err));
//	}
//
//	// FILE
//	printf("\n----- overfprintf result -----\n");
//	if ((document = fopen("task_9.txt", "w")) == NULL) {
//		printf("Cannot open file");
//		return OPEN_FILE_ERROR;
//	}
//	err = overfprintf(document, format_str, 15, -35, 1.3333, 12.65, 'h', "Hello world!", 789, 123, 123, 
//					132, 100, 789, 16, 789, 16, "aaa", 12, "AAA", 12, 16, -16, 52.52, 12.34);
//	fclose(document);
//	if (err) {
//		printf(error_processing(err));
//	}
//	else {
//		printf("Data was written to file successfully! :D\n\n");
//	}
//
//	// ARRAY
//	printf("\n----- oversprintf result -----\n");
//	err = oversprintf(arr, format_str, 15, -35, 1.3333, 12.65, 'h', "Hello world!", 789, 123, 123, 
//					132, 100, 789, 16, 789, 16, "aaa", 12, "AAA", 12, 16, -16, 52.52, 12.34);
//	if (err) {
//		printf(error_processing(err));
//	}
//	else {
//		printf("Data was written to array successfully! :D\n");
//		printf("Result:\n%s", arr);
//	}
//
//	return 0;
//}
//
//int overprintf(char* format, ...) {
//	int err;
//	char* str_ans, *str_ans_cpy;
//	va_list args;
//	va_start(args, format);
//
//	err = _preprocessing_print_data(&str_ans, format, args);
//	if (err) {
//		return err;
//	}
//
//	str_ans_cpy = str_ans;
//	while (*str_ans_cpy) {
//		fputc(*str_ans_cpy++, stdout);
//	}
//
//	free(str_ans);
//	va_end(args);
//	return err;
//}
//
//int overfprintf(FILE* file, char* format, ...) {
//	int err;
//	char* str_ans, * str_ans_cpy;
//	va_list args;
//	va_start(args, format);
//
//	if (file == NULL) {
//		return OPEN_FILE_ERROR;
//	}
//	err = _preprocessing_print_data(&str_ans, format, args);
//	if (err) {
//		return err;
//	}
//
//	str_ans_cpy = str_ans;
//	while (*str_ans_cpy) {
//		fputc(*str_ans_cpy++, file);
//	}
//
//	free(str_ans);
//	va_end(args);
//	return err;
//}
//
//int oversprintf(char* buf, char* format, ...) {
//	int err;
//	char* str_ans, * str_ans_cpy;
//	va_list args;
//	va_start(args, format);
//
//	if (buf == NULL) {
//		return INCORRECT_ARG;
//	}
//	err = _preprocessing_print_data(&str_ans, format, args);
//	if (err) {
//		return err;
//	}
//
//	strcpy(buf, str_ans);
//	free(str_ans);
//	va_end(args);
//	return err;
//}
//
//int _preprocessing_print_data(char** ans, char* format, va_list args) {
//	//*ans = format;
//	char* proc_str = NULL;
//	char* temp_str = NULL;
//	char* str_num;
//	int err, num, base, i;
//	int* num_ptr;
//	int current_ans_index = 0, current_format_index = 0;
//	int f_str_len = strlen(format) - 1;
//	int total_len = BUFSIZ;
//	unsigned char* uc_ptr;
//	double double_num;
//	int ans_zr[BUFSIZ];
//	int count = 0;
//	
//	if ((proc_str = (char*)malloc(BUFSIZ * sizeof(char))) == NULL) {
//		return MEMORY_ALLOCATE_ERROR;
//	}
//	
//	while (current_format_index <= f_str_len) {
//		if (current_ans_index >= total_len) {
//			err = cust_realloc((void**)&proc_str, total_len * 2);
//			if (err) {
//				return err;
//			}
//			total_len *= 2;
//		}
//
//		if (*format == '%') {
//			switch (*(format+1))
//			{	
//				// %d, %u, %f, %lf, %c, %s, %o, %x, %X
//				case 'd': {
//					err = convert_FROM_decimal(va_arg(args, int), 10, &temp_str);
//					if (err) {
//						free(temp_str);
//						return err;
//					}
//					_update_data_in_preprocessing_print_data(1, 2, &current_ans_index, &current_format_index, &proc_str, &temp_str, &format);
//					break;
//				}
//				case 'u': {
//					err = convert_FROM_decimal((int)va_arg(args, unsigned int), 10, &temp_str);
//					if (err) {
//						free(temp_str);
//						return err;
//					}
//					_update_data_in_preprocessing_print_data(1, 2, &current_ans_index, &current_format_index, &proc_str, &temp_str, &format);
//					break;
//				}
//				case 'f': {
//					err = float_to_str(va_arg(args, double), &temp_str);
//					if (err) {
//						free(temp_str);
//						return err;
//					}
//					_update_data_in_preprocessing_print_data(1, 2, &current_ans_index, &current_format_index, &proc_str, &temp_str, &format);
//					break;
//				}
//				case 'l': {
//					if (*(format + 2) == 'f') {
//						err = float_to_str(va_arg(args, double), &temp_str);
//						if (err) {
//							free(temp_str);
//							return err;
//						}
//						_update_data_in_preprocessing_print_data(1, 2, &current_ans_index, &current_format_index, &proc_str, &temp_str, &format);
//						break;
//					}
//					else {
//						return UNKNOWN_FLAG;
//					}
//				}
//				case 'c': {
//					proc_str[current_ans_index] = va_arg(args, char);
//					current_ans_index += 1;
//					current_format_index += 2;
//					format += 2;
//					break;
//				}
//				case 's': {
//					temp_str = va_arg(args, char*);
//					_update_data_in_preprocessing_print_data(0, 2, &current_ans_index, &current_format_index, &proc_str, &temp_str, &format);
//					break;
//				}
//				case 'o': {
//					err = convert_FROM_decimal((int)va_arg(args, unsigned int), 8, &temp_str);
//					if (err) {
//						free(temp_str);
//						return err;
//					}
//					_update_data_in_preprocessing_print_data(1, 2, &current_ans_index, &current_format_index, &proc_str, &temp_str, &format);
//					break;
//				}
//				case 'x': {
//					err = convert_FROM_decimal((int)va_arg(args, unsigned int), 16, &temp_str);
//					if (err) {
//						free(temp_str);
//						return err;
//					}
//					_update_data_in_preprocessing_print_data(1, 2, &current_ans_index, &current_format_index, &proc_str, &temp_str, &format);
//					break;
//				}
//				case 'X': {
//					err = convert_FROM_decimal((int)va_arg(args, unsigned int), 16, &temp_str);
//					for (i = 0; temp_str[i] != '\0'; i++) {
//						temp_str[i] = toupper(temp_str[i]);
//					}
//					if (err) {
//						free(temp_str);
//						return err;
//					}
//					_update_data_in_preprocessing_print_data(1, 2, &current_ans_index, &current_format_index, &proc_str, &temp_str, &format);
//					break;
//				}
//				case 'R': {
//					err = int_TO_roman(va_arg(args, int), &temp_str);
//					if (err) {
//						free(temp_str);
//						return err;
//					}
//					_update_data_in_preprocessing_print_data(1, 3, &current_ans_index, &current_format_index, &proc_str, &temp_str, &format);
//					break;
//				}
//				case 'Z': {
//					/*temp_str = (char*)malloc(BUFSIZ * sizeof(char));
//					if (temp_str == NULL) {
//						return MEMORY_ALLOCATE_ERROR;
//					}
//					err = find_zeckendorf(va_arg(args, unsigned int), 0, temp_str, 0, BUFSIZ);
//					if (err) {
//						free(temp_str);
//						return err;
//					}
//					_update_data_in_preprocessing_print_data(1, 3, &current_ans_index, &current_format_index, &proc_str, &temp_str, &format);*/
//
//					err = find_zeckendorf(va_arg(args, unsigned int), ans_zr, &count);
//					if (err) {
//						free(temp_str);
//						return err;
//					}
//
//					temp_str = (char*)malloc((count+1) * sizeof(char));
//					if (temp_str == NULL) {
//						return MEMORY_ALLOCATE_ERROR;
//					}
//					for (i = 0; i < count; i++) {
//						if (i == 0) {
//							temp_str[i] = 48;
//						}
//						else {
//							temp_str[i] = ans_zr[i] + 48;
//						}
//					}
//					temp_str[i] = '\0';
//
//					_update_data_in_preprocessing_print_data(1, 3, &current_ans_index, &current_format_index, &proc_str, &temp_str, &format);
//					break;
//				}
//				case 'C': {
//					if (*(format + 2) == 'v') {
//						num = va_arg(args, int);
//						base = va_arg(args, int);
//						if (base < 2 || base > 36) {
//							base = 10;
//						}
//						err = convert_FROM_decimal(num, base, &temp_str);
//						if (err) {
//							free(temp_str);
//							return err;
//						}
//						_update_data_in_preprocessing_print_data(1, 3, &current_ans_index, &current_format_index, &proc_str, &temp_str, &format);
//					}
//					else if (*(format + 2) == 'V') {
//						num = va_arg(args, int);
//						base = va_arg(args, int);
//						if (base < 2 || base > 36) {
//							base = 10;
//						}
//						err = convert_FROM_decimal(num, base, &temp_str);
//						if (err) {
//							free(temp_str);
//							return err;
//						}
//						for (int i = 0; temp_str[i] != '\0'; i++) {
//							temp_str[i] = toupper(temp_str[i]);
//						}
//						_update_data_in_preprocessing_print_data(1, 3, &current_ans_index, &current_format_index, &proc_str, &temp_str, &format);
//					}
//					else {
//						return UNKNOWN_FLAG;
//					}
//					break;
//				}
//				case 't': {
//					if (*(format + 2) == 'o') {
//						num = 0;
//						str_num = va_arg(args, char*);
//						base = va_arg(args, int);
//						if (base < 2 || base > 36) {
//							base = 10;
//						}
//						err = convert_TO_decimal(str_num, base, &num);
//						if (err) {
//							return err;
//						}
//						err = convert_FROM_decimal(num, 10, &temp_str);
//						if (err) {
//							free(temp_str);
//							return err;
//						}
//						_update_data_in_preprocessing_print_data(1, 3, &current_ans_index, &current_format_index, &proc_str, &temp_str, &format);
//					}
//					else {
//						return UNKNOWN_FLAG;
//					}
//					break;
//				}
//				case 'T': {
//					if (*(format + 2) == 'O') {
//						num = 0;
//						str_num = va_arg(args, char*);
//						base = va_arg(args, int);
//						if (base < 2 || base > 36) {
//							base = 10;
//						}
//						err = convert_TO_decimal(str_num, base, &num);
//						if (err) {
//							return err;
//						}
//						err = convert_FROM_decimal(num, 10, &temp_str);
//						if (err) {
//							free(temp_str);
//							return err;
//						}
//						_update_data_in_preprocessing_print_data(1, 3, &current_ans_index, &current_format_index, &proc_str, &temp_str, &format);
//					}
//					else {
//						return UNKNOWN_FLAG;
//					}
//					break;
//				}
//				case 'm': {
//					switch (*(format+2))
//					{
//						case 'i': {
//							num = va_arg(args, int);
//							uc_ptr = (unsigned char*)&num;
//							for (i = 0; i < sizeof(int); i++) {
//								err = convert_FROM_decimal((*uc_ptr)++, 2, &temp_str);
//								if (err) {
//									free(temp_str);
//									return err;
//								}
//
//								memcpy(proc_str + current_ans_index, temp_str, strlen(temp_str));
//								current_ans_index += strlen(temp_str);
//								free(temp_str);
//
//								if (i != sizeof(int)-1) {
//									proc_str[current_ans_index] = ' ';
//									current_ans_index += 1;
//								}
//							}
//							uc_ptr = NULL;
//							current_format_index += 3;
//							format += 3;
//							num = 0;
//							break;
//						}
//						case 'u': {
//							num = (int)va_arg(args, unsigned int);
//							uc_ptr = (unsigned char*)&num;
//							for (i = 0; i < sizeof(int); i++) {
//								err = convert_FROM_decimal((*uc_ptr)++, 2, &temp_str);
//								if (err) {
//									free(temp_str);
//									return err;
//								}
//
//								memcpy(proc_str + current_ans_index, temp_str, strlen(temp_str));
//								current_ans_index += strlen(temp_str);
//								free(temp_str);
//
//								if (i != sizeof(int) - 1) {
//									proc_str[current_ans_index] = ' ';
//									current_ans_index += 1;
//								}
//							}
//							uc_ptr = NULL;
//							current_format_index += 3;
//							format += 3;
//							num = 0;
//							break;
//						}
//						case 'd': {
//							double_num = va_arg(args, double);
//							uc_ptr = (unsigned char*)&double_num;
//							for (i = 0; i < sizeof(double); i++) {
//								err = convert_FROM_decimal((*uc_ptr)++, 2, &temp_str);
//								if (err) {
//									free(temp_str);
//									return err;
//								}
//
//								memcpy(proc_str + current_ans_index, temp_str, strlen(temp_str));
//								current_ans_index += strlen(temp_str);
//								free(temp_str);
//
//								if (i != sizeof(double) - 1) {
//									proc_str[current_ans_index] = ' ';
//									current_ans_index += 1;
//								}
//							}
//							uc_ptr = NULL;
//							current_format_index += 3;
//							format += 3;
//							double_num = 0.0;
//							break;
//						}
//						case 'f': {
//							double_num = va_arg(args, double);
//							uc_ptr = (unsigned char*)&double_num;
//							for (i = 0; i < sizeof(float); i++) {
//								err = convert_FROM_decimal((*uc_ptr)++, 2, &temp_str);
//								if (err) {
//									free(temp_str);
//									return err;
//								}
//
//								memcpy(proc_str + current_ans_index, temp_str, strlen(temp_str));
//								current_ans_index += strlen(temp_str);
//								free(temp_str);
//
//								if (i != sizeof(float) - 1) {
//									proc_str[current_ans_index] = ' ';
//									current_ans_index += 1;
//								}
//							}
//							uc_ptr = NULL;
//							current_format_index += 3;
//							format += 3;
//							double_num = 0.0;
//							break;
//						}
//						default:
//							break;
//					}
//					break;
//				}
//				case '%': {
//					proc_str[current_ans_index] = '%';
//					current_ans_index += 1;
//					current_format_index += 2;
//					format += 2;
//					break;
//				}
//				default:
//					break;
//			}
//		}
//		else {
//			proc_str[current_ans_index] = *format;
//			current_ans_index += 1;
//			current_format_index += 1;
//			format++;
//		}
//	}
//	proc_str[current_ans_index] = '\0';
//	*ans = proc_str;
//	return 0;
//}
//
//void _update_data_in_preprocessing_print_data(int clean, int flag_len, int* current_ans_index, int* current_format_index, char** proc_str, char** temp_str, char** format) {
//	memcpy((*proc_str) + (*current_ans_index), *temp_str, strlen(*temp_str));
//	(*current_ans_index) += strlen(*temp_str);
//	if (clean) {
//		free(*temp_str);
//	}
//	*temp_str = NULL;
//	(*current_format_index) += flag_len;
//	*format += flag_len;
//}
//
//void generate_fib_limit(int limit, int* fib_arr, int* count) {
//	int a = 0, b = 1;
//	*count = 0;
//
//	fib_arr[(*count)++] = a;
//	while (b < limit) {
//		fib_arr[(*count)++] = b;
//		int next = a + b;
//		a = b;
//		b = next;
//	}
//}
//
//int find_zeckendorf(int target, int* ans, int* count) {
//
//	int fib_arr[BUFSIZ];
//	int i;
//
//	generate_fib_limit(target, fib_arr, count);
//
//	ans[*count] = 1;
//	for (i = *count-1; i >= 0; i--) {
//		if (target - fib_arr[i] >= 0) {
//			target -= fib_arr[i];
//			ans[i] = 1;
//		}
//		else {
//			ans[i] = 0;
//		}
//	}
//	(*count)++;
//	return 0;
//}
//
//int int_TO_roman(int n, char** ans) {
//	int digit = 0;
//	int i = 0, err, len = 0;
//
//	if (ans == NULL) {
//		return NULL_POINTER;
//	}
//
//	struct roman_item {
//		int val;
//		int len;
//		char* letter;
//	};
//
//	struct roman_item roman_data[] = {
//		{1000, 1, "M"}, {900, 2, "CM"}, {500, 1, "D"}, {400, 2, "CD"},
//		{100, 1, "C"}, {90, 2, "XC"}, {50, 1, "L"}, {40, 2, "XL"},
//		{10, 1, "X"}, {9, 2, "IX"}, {5, 1, "V"}, {4, 2, "IV"},
//		{1, 1, "I"} };
//
//	while (n > 0) {
//		while (n >= roman_data[i].val) {
//			err = cust_realloc((void**)&(*ans), (len + roman_data[i].len) * sizeof(char));
//			if (err) {
//				return err;
//			}
//
//			memcpy(*ans + len, roman_data[i].letter, roman_data[i].len);
//			len += roman_data[i].len;
//			n -= roman_data[i].val;
//		}
//		err = cust_realloc((void**)&(*ans), (len + 1) * sizeof(char));
//		if (err) {
//			return err;
//		}
//		i++;
//	}
//	(*ans)[len] = '\0';
//	return 0;
//}
//
//char* error_processing(int err) {
//	switch (err) {
//		case INCORRECT_BASE:
//			return "INCORRECT_BASE";
//		case MEMORY_ALLOCATE_ERROR:
//			return "MEMORY_ALLOCATE_ERROR";
//		case UNKNOWN_FLAG:
//			return "UNKNOWN_FLAG";
//		case NOT_ENOUGH_ARGS:
//			return "NOT_ENOUGH_ARGS";
//		case OPEN_FILE_ERROR:
//			return "OPEN_FILE_ERROR";
//		case INCORRECT_ARG:
//			return "INCORRECT_ARG";
//		case NULL_POINTER:
//			return "NULL_POINTER";
//		default:
//			return "Uncknown error";
//	}
//}
//
//int convert_FROM_decimal(int n, int base, char** ans) {
//	char* res;
//	int len = 0, j = 0, sign = 0;
//
//	if (ans == NULL) {
//		return NULL_POINTER;
//	}
//	if (base < 2 || base > 36) {
//		return INCORRECT_BASE;
//
//	}
//	if (n == 0) {
//		*ans = (char*)malloc(2 * sizeof(char));
//		if (*ans == NULL) { return MEMORY_ALLOCATE_ERROR; }
//		(*ans)[0] = '0';
//		(*ans)[1] = '\0';
//		return 0;
//	}
//
//	len = (log(abs(n)) / log(base)) + 1;
//
//	if (n < 0) { sign = 1; len++; n = -n; }
//
//	*ans = (char*)malloc((len + 1) * sizeof(char));
//	if (*ans == NULL) { return MEMORY_ALLOCATE_ERROR; }
//	res = *ans;
//
//	if (n < 0) { *res = '-'; }
//
//	res += len - sign;
//	*res-- = 0;
//	
//	while (n > 0) {
//		j = (int)n % base;
//		if (j < 10) {
//			*res = j + '0';
//		}
//		else {
//			*res = j + 'a' - 10;
//		}
//
//		n /= base;
//		res--;
//	}
//
//	return 0;
//}
//
//int convert_TO_decimal(char* n, int base, int* ans) {
//	int len = 0, j = 0, sign = 0;
//
//	if (n == NULL || ans == NULL) {
//		return NULL_POINTER;
//	}
//	if (base < 2 || base > 36) {
//		return INCORRECT_BASE;
//	}
//	if (n[0] == '0') {
//		*ans = 0;
//		return 0;
//	}
//
//	len = strlen(n);
//
//	if (n[0] == '-') { sign = 1; }
//
//	for (int i = sign; i < len; i++) {
//		if (isdigit(n[i])) {
//			(*ans) += (int)((int)n[i] - '0') * pow((double)base, len - i-1);
//		}
//		else if (isalpha(n[i])) {
//			(*ans) += (int)((int)toupper(n[i]) - 'A' + 10) * pow((double)base, len - i-1);
//		}
//	}
//	if (sign == 1) { (*ans) *= -1; }
//	return 0;
//}
//
//int float_to_str(float n, char** ans) {
//	char* str_num = NULL;
//	int integer_part, err, len, i, int_end;
//	float float_part;
//
//	if (ans == NULL) {
//		return NULL_POINTER;
//	}
//	integer_part = (int)n;
//	float_part = n - integer_part;
//
//	err = convert_FROM_decimal(integer_part, 10, &str_num);
//	if (err) {
//		return err;
//	}
//	if (integer_part == 0) {
//		int_end = 1;
//	}
//	else {
//		int_end = (log(abs(n)) / log(10)) + 1;
//	}
//	len = int_end + 1 + 6; 
//	err = cust_realloc((void**)&str_num, len + 1);
//	if (err) {
//		return err;
//	}
//	str_num[len] = 0;
//	str_num[int_end] = '.';
//
//	for (i = int_end + 1; i < len; ++i) {
//		float_part *= 10;
//		str_num[i] = ((int)float_part) + '0';
//		float_part = float_part - (int)float_part;
//	}
//	*ans = str_num;
//
//	return 0;
//}
