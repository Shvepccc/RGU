//#include <stdio.h>
//
//#define ARRAY_SIZE_S 10
//#define ARRAY_SIZE_E 5
//
//void print_array(int* arr, int type);
//
//int main() {
//	int arr_start[ARRAY_SIZE_S] = {25, 16, 42, 18, 5, 41, 36, 98, 76, 53};
//	int arr[ARRAY_SIZE_E];
//
//	for (int i = 0; i < ARRAY_SIZE_S; ++i) {
//		if (i % 2 == 0) {
//			arr[i / 2] = arr_start[i];
//		}
//	}
//	printf("Start array: ");
//	print_array(arr_start, 1);
//	printf("Array: ");
//	print_array(arr, 2);
//}
//
//void print_array(int *arr, int type) {
//	if (type == 1) {
//		for (int i = 0; i < ARRAY_SIZE_S; ++i) {
//			printf("%d ", arr[i]);
//		}
//	}
//	else {
//		for (int i = 0; i < ARRAY_SIZE_E; ++i) {
//			printf("%d ", arr[i]);
//		}
//	}
//	printf("\n");
//}