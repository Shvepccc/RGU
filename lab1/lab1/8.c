//#include <stdio.h>
//
//#define ARRAY_SIZE_S 10
//#define ARRAY_SIZE_E 10
//
//void print_array(int* arr, int t);
//
//int main() {
//	int arr_start[ARRAY_SIZE_S] = { 25, 16, 42, 18, 5, 41, 36, 98, 76, 53 };
//	int arr[ARRAY_SIZE_E];
//	int t = 0;
//
//	for (int i = 0; i < ARRAY_SIZE_S; ++i) {
//		if (arr_start[i] % 2 == 0) {
//			arr[t] = arr_start[i];
//			t++;
//		}
//	}
//
//	printf("Start array: ");
//	print_array(arr_start, ARRAY_SIZE_S);
//	printf("Array: ");
//	print_array(arr, t);
//}
//
//
//void print_array(int* arr, int t) {
//	for (int i = 0; i < t; ++i) {
//		printf("%d ", arr[i]);
//	}
//	printf("\n");
//}