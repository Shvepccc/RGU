//#include <stdio.h>
//#include <stdarg.h>
//#include "../../libs/error.h"
//#include <string.h>
//#include <stdlib.h>
//#include <ctype.h>
//
//int sum_numbers(int base, int count, ...);
//int sum_base(int base, char* num1, char* num2, char* result);
//
//int main() {
//	char* result;
//	int err;
//	err = sum_numbers(2, 3, &result, "101", "1010", "100");
//	//err = sum_numbers(16, 3, &result, "1", "A", "FF");
//
//	switch (err)
//	{
//		case MEMORY_ALLOCATE_ERROR:
//			printf("Memory allocate error");
//			break;
//		case INCORRECT_BASE:
//			printf("Incorrect base. Base must be from 2 to 36");
//			break;
//		case INCORRECT_ARG:
//			printf("Incorrect argument. All arguments must be greater than 0 and include only numbers");
//			break;
//		default:
//			printf("Result: %s", result);
//			break;
//	}
//	free(result);
//	result = NULL;
//	return 0;
//}
//
//int sum_numbers(int base, int count,  ...) {
//	int i, data;
//	char* ptr;
//	char** result;
//	char* a;
//
//	if (base < 2 || base > 36) {
//		return INCORRECT_BASE;
//	}
//
//	va_list args;
//	va_start(args, base);
//
//	if ((result = va_arg(args, char**)) == NULL) {
//		return INCORRECT_ARG;
//		//TODO: good error
//	}
//
//	for (i = 0; i < count; i++) {
//		if ((ptr = va_arg(args, char*)) == NULL) {
//			return INCORRECT_ARG;
//		}
//		else if (ptr[0] == '-') {
//			return INCORRECT_ARG;
//		}
//		else {
//			if (i == 0) {
//				a = "0";
//			}
//			else{
//				a = *result;
//			}
//			data = sum_base(base, a, ptr, result);
//			switch (data)
//			{
//				case INCORRECT_ARG:
//					return INCORRECT_ARG;
//					break;
//				case MEMORY_ALLOCATE_ERROR:
//					return MEMORY_ALLOCATE_ERROR;
//					break;
//				default:
//					break;
//			}
//			//printf("Result inner: %s\n", (*result));
//		}
//	}
//	return 0;
//}
//
//int charToDigit(char c, int base) {
//	c = toupper(c);
//	if (c >= '0' && c <= '9') return c - '0';
//	else if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
//	else return -1;
//}
//
//char digitToChar(int digit) {
//	if (digit >= 0 && digit <= 9) return '0' + digit;
//	else return 'A' + (digit - 10);
//}
//
//int sum_base(int base, char* num1, char* num2, char** result) {
//	int len1 = strlen(num1);
//	int len2 = strlen(num2);
//	int max_len = (len1 > len2 ? len1 : len2);
//	int carry = 0, i;
//
//	if (((*result) = (char*)malloc((max_len+2))) == NULL) {
//		return MEMORY_ALLOCATE_ERROR;
//	}
//	
//	for (i = 0; i < max_len || carry; i++) {
//		int digit1, digit2, data;
//
//		if (i < len1) {
//			if ((data = charToDigit(num1[len1 - i - 1], base)) == -1) {
//				free(*result);
//				(*result) = NULL;
//				return INCORRECT_ARG;
//			}
//			digit1 = data;
//		}
//		else { digit1 = 0; }
//
//		if (i < len2) {
//			if ((data = charToDigit(num2[len2 - i - 1], base)) == -1) {
//				free(*result);
//				(*result) = NULL;
//				return INCORRECT_ARG;
//			}
//			digit2 = data;
//		}
//		else { digit2 = 0; }
//		//int digit1 = (i < len1) ? charToDigit(num1[len1 - i - 1], base) : 0;
//		//int digit2 = (i < len2) ? charToDigit(num2[len2 - i - 1], base) : 0;
//
//		int sum = digit1 + digit2 + carry;
//		(*result)[i] = digitToChar(sum % base);
//		carry = sum / base;
//	}
//
//	for (int j = 0; j < i / 2; j++) {
//		char temp = (*result)[j];
//		(*result)[j] = (*result)[i - j - 1];
//		(*result)[i - j - 1] = temp;
//	}
//	(*result)[i] = '\0';
//	return 0;
//}