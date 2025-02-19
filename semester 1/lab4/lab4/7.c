//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdarg.h>
//#include "../../libs/error.h"
//#include "../../libs/memory.h"
//#include <string.h>
//#include <math.h>
//#include <ctype.h>
//#include <stdlib.h>
//
//int find_Kaprekara(int base, ...);
//int convert_FROM_decimal(int n, int base, char** ans);
//int convert_TO_decimal(char* n, int base, int* ans);
//
//int main() {
//	char** data;
//	int err, i = 0;
//	err = find_Kaprekara(10, &data, "-1", "5", "9", "40", "45", NULL);
//	//err = find_Kaprekara(16, &data, "6", "9", "A", "C", "F", NULL);
//
//	switch (err)
//	{
//		case NOT_ENOUGH_ARGS:
//			printf("Not enough arguments");
//			break;
//		case MEMORY_ALLOCATE_ERROR:
//			printf("Memory allocate error");
//			break;
//		case INCORRECT_BASE:
//			printf("Incorrect base. Base must be from 2 to 36");
//			break;
//		case INCORRECT_ARG:
//			printf("Incorrect argument. All arguments must be greater than 0");
//			break;
//		default: {
//			printf("These are Kaprekar's numbers: ");
//			while (data[i] != NULL) {
//				printf("%s ", data[i]);
//				i++;
//			}
//			break;
//		}
//	}
//	free(data);
//	data = NULL;
//	return 0;
//}
//
//int find_Kaprekara(int base, ...) {
//	char* ptr;
//	char* ans;
//	char*** data;
//	int decimal = 0;
//	char* l_str;
//	char* r_str;
//	int len, goal, i, r, l, total_count = 0, size = 100;
//	int res;
//	char* dest;
//
//	char* prom_res;
//
//	va_list args;
//	va_start(args, base);
//
//	if (base < 2 || base > 36) {
//		return INCORRECT_BASE;
//	}
//	if ((data = va_arg(args, void***)) == NULL){
//		va_end(args);
//		return NOT_ENOUGH_ARGS;
//	}
//
//	if (((*data) = (char*)malloc(size*sizeof(char))) == NULL) {
//		va_end(args);
//		return MEMORY_ALLOCATE_ERROR;
//	}
//
//	while ((ptr = va_arg(args, char*)) != NULL) {
//
//		ans = NULL;
//		decimal = 0;
//		convert_TO_decimal(ptr, base, &decimal);
//
//		if (decimal < 0) {
//			return INCORRECT_ARG;
//		}
//
//		decimal = pow(decimal, 2.0);
//
//		if (convert_FROM_decimal(decimal, base, &ans) == MEMORY_ALLOCATE_ERROR) {
//			free(data);
//			data = NULL;
//			va_end(args);
//			return MEMORY_ALLOCATE_ERROR;
//		}
//
//		len = strlen(ans);
//		for (i = 0; i < len; i++) {
//			l = 0; r = 0;
//
//			l_str = NULL;
//			if ((l_str = (char*)malloc((len*2+1) * sizeof(char))) == NULL) {
//				free(data);
//				data = NULL;
//				va_end(args);
//				return MEMORY_ALLOCATE_ERROR;
//			}
//			memcpy(l_str, ans, i + 1);
//			l_str[i + 1] = '\0';
//			convert_TO_decimal(l_str, base, &l);
//			
//			r_str = NULL;
//			if ((r_str = (char*)malloc((len-i+1) * sizeof(char))) == NULL) {
//				free(data);
//				free(l_str);
//				data = NULL;
//				l_str = NULL;
//				va_end(args);
//				return MEMORY_ALLOCATE_ERROR;
//			}
//			memcpy(r_str, ans + i + 1, len - i+ 1);
//			r_str[len - i] = '\0';
//			convert_TO_decimal(r_str, base, &r);
//			
//			if ((convert_FROM_decimal(r + l, base, &prom_res)) == MEMORY_ALLOCATE_ERROR) {
//				free(r_str);
//				free(l_str);
//				free(data);
//				data = NULL;
//				l_str = NULL;
//				r_str = NULL;
//				va_end(args);
//				return MEMORY_ALLOCATE_ERROR;
//			}
//			if (strcmp(prom_res, ptr) == 0) {
//				if (total_count > size) {
//					if (cust_realloc(&data, size * 2 * sizeof(int))) {
//						free(l_str);
//						free(r_str);
//						free(data);
//						data = NULL;
//						l_str = NULL;
//						r_str = NULL;
//						va_end(args);
//						return MEMORY_ALLOCATE_ERROR;
//					}
//					size *= 2;
//				}
//				else {
//					(*data)[total_count] = ptr;
//					total_count++;
//				}
//			}
//			free(l_str);
//			free(r_str);
//			l_str = NULL;
//			r_str = NULL;
//		}
//		free(ans);
//		ans = NULL;
//	}
//	(*data)[total_count] = NULL;
//	va_end(args);
//	return 0;
//}
//
//int convert_TO_decimal(char* n, int base, int* ans) {
//	int len = 0, j = 0, sign = 0;
//
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
//int convert_FROM_decimal(int n, int base, char** ans) {
//	char* res;
//	int len = 0, j = 0, sign = 0;
//
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
//			*res = j + 'A' - 10;
//		}
//
//		n /= base;
//		res--;
//	}
//
//	return 0;
//}