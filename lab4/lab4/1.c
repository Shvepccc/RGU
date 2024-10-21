//#include <stdio.h>
//#include <math.h>
//#include <stdlib.h>
//
//char* str_rev(char* str);
//
//int main() {
//	int n ,r;
//	char* str;
//	printf("Enter number: ");
//	scanf("%d", &n);
//	printf("Enter base (2^r - enter r): ");
//	scanf("%d", &r);
//
//	printf("Result:")
//}
//
//char* convert_FROM_decimal_bin(int n, int base, char** res) {
//
//	char* res;
//	int i = 0, j = 0, sign = 0;
//
//	if (n == 0) {
//		res = (char*)malloc(2 * sizeof(char));
//		res[i++] = '0';
//	}
//	if (n < 0) { sign = -1; n = -n; }
//
//	res = (char*)malloc((int)log2(n)+1 * sizeof(char));
//	while (n > 0) {
//		j = (int)n & (base - 1);
//		if (j < 10) {
//			res[i] = j + '0';
//		}
//		else {
//			res[i] = j + 'A' - 10;
//		}
//
//		n >>= base;
//		i++;
//	}
//
//	if (sign == -1) res[i++] = '-';
//	res[i++] = 0;
//	res = str_rev(res);
//	return res;
//}
//
//char* str_rev(char* str) {
//	int c = 0;
//	char r, l;
//	char* ptr = str;
//	while (*ptr != '\0') {
//		ptr++;
//		c += 1;
//	}
//
//	for (int i = 0; i < (c / 2); i++) {
//		l = str[i];
//		r = str[c - i - 1];
//		str[i] = r;
//		str[c - i - 1] = l;
//	}
//
//	return str;
//}