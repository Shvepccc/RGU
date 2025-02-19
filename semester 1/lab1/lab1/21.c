//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <string.h>
//#include <math.h>
//
//int convert_TO_decimal(char* n, int base);
//
//int main() {
//	char number[1024];
//	int base;
//	printf("Enter number: ");
//	scanf("%s", &number);
//	printf("Enter base: ");
//	scanf("%d", &base);
//
//	if (base < 2 || base > 36) {
//		printf("Incorrect base");
//	}
//	else {
//		printf("Result: %d", convert_TO_decimal(number, base));
//	}
//}
//
//int convert_TO_decimal(char* n, int base) {
//	int res = 0;
//	int len = strlen(n)-1;
//
//	for (int i = 0; i < len; i++) {
//		if (isdigit(n[i])) {
//			res += (int) ((int)n[i] - '0')*pow((double)base, len-i);
//		}
//		else if(isalpha(n[i])) {
//			res += (int) ((int)toupper(n[i]) - 'A' + 10)*pow((double)base, len-i);
//		}
//	}
//
//	return res;
//}
