//#include <stdio.h>
//
//#define ARRAY_SIZE 10
//
//void print_array(int* arr);
//
//int main() {
//	int arr[ARRAY_SIZE] = { 3, 1, 9, 6, 8, 2, 4, 0, 5, 7 };
//
//	int change_value = 0;
//	for (int i = 0; i < ARRAY_SIZE-1; ++i) {
//		int t = 0;
//		for (int t = 0; t < ARRAY_SIZE-1-i; ++t) {
//			if (arr[0+t] > arr[0 + 1+t]) {
//				change_value = arr[0+t];
//				arr[0+t] = arr[0 + 1+t];
//				arr[0 + 1+t] = change_value;
//			}
//		}
//	}
//
//	print_array(arr);
//	system("pause");
//}
//
//void print_array(int *arr) {
//	printf("Array: ");
//	for (int i = 0; i < ARRAY_SIZE; ++i) {
//		printf("%d ", arr[i]);
//	}
//	printf("\n");
//}