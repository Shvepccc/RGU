//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <math.h>
//#include "../../libs/error.h"
//#include "../../libs/memory.h"
//
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
//int fib(int n) {
//	if (n <= 1) { return 1; }
//	else { return fib(n - 1) + fib(n - 2); }
//}
//
//int le_fib(int limit, int n, int last_t) {
//	int t = fib(n - 1) + fib(n - 2);
//	if (t >= limit) {
//		return last_t;
//	}
//	le_fib(limit, n + 1, t);
//}
//
//int find_zeckendorf(int target, int result, char* ans, int current_index, int size_str) {
//	int t = le_fib(target, 1, 1);
//	int err;
//	int cur_len;
//
//	if (current_index >= size_str) {
//		err = cust_realloc((void**)&ans, size_str*2);
//		if (err) {
//			return err;
//		}
//		size_str *= 2;
//	}
//
//	char* temp_str;
//	convert_FROM_decimal(t, 10, &temp_str);
//	cur_len = strlen(temp_str);
//	memcpy(ans + current_index, temp_str, cur_len);
//	current_index += cur_len;
//	free(temp_str);
//
//	if (cur_len != NULL) {
//		ans[current_index] = ' ';
//		current_index++;
//	}
//
//	if (target - t <= 0) {
//		ans[current_index] = 0;
//		return 0;
//	}
//	find_zeckendorf(target - t, 0, ans, current_index, size_str);
//}
//
//int main2222() {
//	char* ans = (char*)malloc(BUFSIZ * sizeof(char));
//	if (ans == NULL) {
//		return MEMORY_ALLOCATE_ERROR;
//	}
//	find_zeckendorf(8, 0, ans, 0, BUFSIZ);
//	printf("%s", ans);
//}