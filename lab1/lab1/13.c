/* »меетс€ строка. »спользу€ указатель типа char *, преобразовать каждый элемент, сто€щий
на нечЄтной позиции, в верхний регистр.*/

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
//void eee(char* str) {
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
//
//	int i, len;
//	len = my_strlen(str);
//	printf("%d %s \n", len, str);
//
//	for (i = 0; i < len; ++i) {
//		if ((i % 2 == 0)) { // && (96 < *str) && (*str < 123)) {
//			*str -= 32;
//		}
//	}
//}
//
//int main() {
//	char* str = "Hello world!\0";
//	printf("Start string: %s\n", str);
//	eee(str);
//	//change_elements(str);
//	printf("New string: %s\n", str);
//}
