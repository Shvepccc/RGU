#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main() {
	int k = 0;
	printf("Number: ");
	scanf("%d", &k);
	printf("The lowest bit of the number %d = %d ", k, k % 2);
	return 0;
}