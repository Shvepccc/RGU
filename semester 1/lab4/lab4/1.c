//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <math.h>
//#include <stdlib.h>
//#include "../../libs/error.h"
//
//int convert_FROM_decimal_bin(int n, int r, char** str);
//
//int main() {
//	int n ,r;
//	char* ans;
//	int err;
//	printf("Enter number: ");
//	scanf("%d", &n);
//	printf("Enter base (2^r - enter r): ");
//	scanf("%d", &r);
//
//	err = convert_FROM_decimal_bin(n, r, &ans);
//	switch (err)
//	{
//	case 0:
//		printf("Result: %s", ans);
//		break;
//	case INCORRECT_BASE: 
//		printf("Incorrect base \n");
//		break;
//	case MEMORY_ALLOCATE_ERROR:
//		printf("Memory not allocated \n");
//	default:
//		break;
//	}
//	free(ans);
//
//	return 0;
//}
//
//int convert_FROM_decimal_bin(int n, int r, char** str) {
//
//	int base = 1 << r;
//	char* res;
//	int len = 0, j = 0, sign = 0;
//
//	if (base < 2 || base > 32) {
//		return INCORRECT_BASE;
//	}
//
//	if (n == 0) {
//		*str = (char*)malloc(sizeof(char) << 1);
//		if (*str == NULL) { return MEMORY_ALLOCATE_ERROR; }
//		(*str)[0] = '0';
//		(*str)[1] = '\0';
//		return 0;
//	}
//
//	len = (log(abs(n)) / log(base)) + 1;
//
//	if (n < 0) { sign = 1; len++; n = -n; }
//
//	*str = (char*)malloc((len + 1) * sizeof(char));
//	if (*str == NULL) { return MEMORY_ALLOCATE_ERROR; }
//	res = *str;
//
//	if (n < 0) { *res = '-'; }
//
//	res += len - sign;
//	*res-- = 0;
//	
//	while (n > 0) {
//		j = n & (base - 1);
//		if (j < 10) {
//			*res = j + '0';
//		}
//		else {
//			*res = j + 'A' - 10;
//		}
//
//		n >>= r;
//		res--;
//	}
//
//	return 0;
//}