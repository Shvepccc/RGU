//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//
//int xor_all_digits(int n) {
//
//	int res = n % 2;
//	n /= 2;
//	while (n > 0) {
//		res = res ^ (n % 2);
//		n /= 2;
//	}
//	return res;
//}
//
//int main() {
//	int n = 0;
//	printf("Enter number (decimal): ");
//	scanf("%d", &n);
//	printf("Result: %d\n", xor_all_digits(n));
//	return 0;
//}