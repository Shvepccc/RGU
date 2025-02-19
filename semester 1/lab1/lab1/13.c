//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//
//int my_strlen(char* str) {
//	int count = 0;
//
//	while (*str != '\0') {
//		++str;
//		++count;
//	}
//	return count;
//}
//
//void change_register(char* str) {
//
//	char* ptr = str;
//
//	while (*ptr != '\0') {
//		if (96 < *ptr && *ptr < 123) {
//			*ptr = *ptr - 32;
//		}
//		++ptr;
//	}
//}
//
//void change_elements(char* str) {
//	char* ptr = str;
//	int i, len;
//	len = my_strlen(str);
//
//	for (i = 0; i < len; ++i) {
//		if (*ptr > 'a' && *ptr < 'z') {
//			*ptr -= 32;
//		}
//		++ptr;
//	}
//}
//
//int main() {
//	char str[] = "Hello world!";
//	printf("Start string: %s\n", str);
//	change_elements(str);
//	printf("New string: %s\n", str);
//}
