//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <string.h>
//
//char* convert_FROM_decimal(int n, int base);
//void reverse(char* str);
//
//int main() {
//	int number, base;
//	printf("Enter number: ");
//	scanf("%d", &number);
//	printf("Enter base: ");
//	scanf("%d", &base);
//
//	if (base < 2 || base > 36) {
//		printf("Incorrect base");
//	}
//	else {
//		printf("Result: %s", convert_FROM_decimal(number, base));
//	}
//}
//
//char* convert_FROM_decimal(int n, int base) {
//
//	char res[1024];
//	int i = 0, j = 0, sign = 0;
//
//	if (n == 0) res[i++] = '0';
//	if (n < 0) { sign = -1; n = -n; }
//
//	while (n > 0) {
//		j = (int)n % base;
//		if (j < 10) {
//			res[i] = j + '0';
//		}
//		else {
//			res[i] = j + 'A' - 10;
//		}
//
//		n /= base;
//		i++;
//	}
//
//
//	if (sign == -1) res[i++] = '-';
//	res[i++] = 0;
//	reverse(res);
//	return res;
//}
//
//void reverse(char* str) {
//	int len = strlen(str);
//	char l, r;
//	for (int i = 0; i < (len / 2); i++) {
//		l = str[i];
//		r = str[len - i - 1];
//		str[i] = r;
//		str[len - i - 1] = l;
//	}
//}
