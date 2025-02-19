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
//int le_fib(int limit, int n, int last_t, int* fib_arr) {
//	int t = fib(n - 1) + fib(n - 2);
//	//printf("%d\n", t);
//	if (t >= limit) {
//		return last_t;
//	}
//	le_fib(limit, n + 1, t, fib_arr);
//}
//
//int find_zeckendorf2(int target, int result, char* ans, int current_index, int size_str) {
//	int fib_arr[10];
//	int t = le_fib(target, 1, 1, fib_arr);
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
//	char* temp_str = "1";
//	//convert_FROM_decimal(t, 10, &temp_str);
//	cur_len = strlen(temp_str);
//	memcpy(ans + current_index, temp_str, cur_len);
//	current_index += cur_len;
//
//	if (cur_len != NULL) {
//		ans[current_index] = ' ';
//		current_index++;
//	}
//
//	if (target - t <= 0) {
//		char* temp_str = "1";
//		cur_len = strlen(temp_str);
//		memcpy(ans + current_index, temp_str, cur_len);
//		current_index += cur_len;
//
//		ans[current_index] = 0;
//		return 0;
//	}
//	find_zeckendorf2(target - t, 0, ans, current_index, size_str);
//}
//
//void generate_fib(int limit, int* fib_arr, int* count) {
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
//	generate_fib(target, fib_arr, count);
//
//	ans[*count] = 1;
//	for (i = *count-1; i > 0; i--) {
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
//int main() {
//
//	int ans[BUFSIZ];
//	int count = 0;
//	int i = 0;
//	int target = 100;
//
//	int fib_arr[BUFSIZ];
//	generate_fib(target, fib_arr, &count);
//	for (i = 1; i < count; i++) {
//		printf("\t%d ", fib_arr[i]);
//	}
//	count = 0;
//	printf("\n");
//	find_zeckendorf(target, ans, &count);
//
//	for (i = 1; i < count; i++) {
//		printf("\t%d ", ans[i]);
//	}
//
//	char* ans_str = (char*)malloc((count + 1) * sizeof(char));
//	if (ans_str == NULL) {
//		return MEMORY_ALLOCATE_ERROR;
//	}
//	for (i = 1; i < count; i++) {
//		ans_str[i-1] = ans[i] + 48;
//	}
//	ans_str[i-1] = '\0';
//	printf("\n%s", ans_str);
//	return 0;
//}

#include <stdio.h>
#include "../../libs/int_vector.h"

//int main() {
//	int element = 0;
//	int i;
//	int arr[] = { 1, 2, 3, 4, 5 };
//	int_vector vec = int_vector_from(arr, 5);
//	int_vector_insert(&vec, 6, 5);
//	int_vector_push_back(&vec, 7);
//	int_vector_push_back(&vec, 8);
//	int_vector_pop(&vec, &element, 6);
//	printf("%d\n", element);
//	int_vector_pop_back(&vec, &element);
//	printf("%d\n", element);
//	for (i = 0; i < int_vector_size(vec); ++i) {
//		printf("%d ", vec[i]);
//	}
//	
//	int_vector_free(vec);
//
//	int_vector vec2 = int_vector_init();
//	int_vector_get_back(&vec2, &i);
//	printf("Back data; %d", i);
//	return 0;
//}

//#include <stdio.h>
//#include "u_list.h"
//int main() {
//	int i = 0;
//	u_list list = u_list_init(NULL);
//	for (i = 0; i < 10; i++) {
//		u_list_insert(&list, &i, 0);
//	}
//	i = 15;
//	printf("%d", *(int*)(list.first_node->data));
//	u_list_free(&list);
//	return 0;
//}