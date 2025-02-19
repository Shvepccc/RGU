//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <math.h>
//
//int ones_line(int n) {
//	int i, res = 0;
//
//	for (i = 0; i < n; ++i) {
//		res += pow(2, i);
//	}
//	return res;
//}
//
//void printb(int n) {
//	for (int i = 31; i >= 0; i--) {
//		int bit = (n >> i) & 1;
//		printf("%d", bit);
//		if (i % 8 == 0) printf("%s", " ");
//	}
//	printf("\n");
//}
//
//int numPlaces(int n) {
//	int retVal = 0;
//
//	while (n) {
//		retVal += 1;
//		n >>= 1;
//	}
//
//	return retVal;
//}
//
//int main() {
//
//	int n, i, len, r, l, res = 0;
//	printf("Enter number (decimal): ");
//	scanf("%d", &n);
//	printf("Enter i: ");
//	scanf("%d", &i);
//	len = numPlaces(n);
//
//	r = ones_line(i) & n;
//	l = (ones_line(i) & (n >> (len - i))) << i;
//	res = r | l;
//	printf("The start number (binary): ");
//	printb(n);
//	printf("Result                   : ");
//	printb(res);
//	return 0;
//}