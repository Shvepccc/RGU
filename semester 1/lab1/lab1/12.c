//#include <stdio.h>
//
//int main() {
//	char str1[] = "Hello world!";
//	char str2[] = "world";
//	char* istr;
//
//	printf("First string: %s\n", str1);
//	printf("Second string: %s\n", str2);
//	istr = strstr(str1, str2);
//
//	if (istr == NULL)
//		printf("String not found\n");
//	else
//		printf("Sting start with char %d \n", istr - str1 + 1);
//
//	return 0;
//}
//
//char* search_str(const char* str, const char* s_str) {
//	char* n = NULL;
//
//	while (1) {
//		if (*str == *s_str) {
//			n = *str;
//			++str;
//			++s_str;
//		}
//		else if (n != NULL) {
//			return n;
//		}
//		else {
//			++str;
//		}
//	}
//	return NULL;
//}