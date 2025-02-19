//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//
//#define ARRAY_SIZE 10
//
//int main() {
//	int arr[ARRAY_SIZE] = {0,1,2,3,4,5,6,7,8,9};
//	int searching_value = 0;
//	int n, g = 0;
//	printf("Enter searching value (0-9): ");
//	scanf("%d", &searching_value);
//
//	n = ARRAY_SIZE / 2;
//	g = ARRAY_SIZE / 2;
//	while (1) {
//		if (searching_value > arr[n]) {
//			n += ((g / 2) >= 1) ? (g / 2) : 1;
//		}
//		else if (searching_value < arr[n]){
//			n -= ((g / 2) >= 1) ? (g / 2) : 1;
//		}
//		else if (searching_value == arr[n]) {
//			printf("\nValue found! Value: %d, index: %d", arr[n], n);
//			break;
//		}
//
//		if (n < 0 || n > ARRAY_SIZE) {
//			printf("The value is missing from the array =( \n");
//			break;
//		}
//	}
//}