//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//
//int change_bytes(int n, int i, int j) {
//	char* bytes_array = (char*)&n;
//	char buf = bytes_array[i];
//	bytes_array[i] = bytes_array[j];
//	bytes_array[j] = buf;
//	return n;
//}
//
//int main() {
//	int i, j, n = 0;
//	printf("Enter number: ");
//	scanf("%d", &n);
//	printf("Enter number of i bit (0-3): ");
//	scanf("%d", &i);
//	printf("Enter number of j bit (0-3): ");
//	scanf("%d", &j);
//
//	printf("\nStart number: %d\n", n);
//	n = change_bytes(n, i, j);
//	printf("Processed number: %d\n", n);
//
//	return 0;
//}