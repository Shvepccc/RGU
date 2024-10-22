//#include <stdio.h>
//
//#define ARRAY_SIZE 10
//
//void print_array(double* arr);
//int contain(double* arr, double t);
//
//int main() {
//	double arr1 [ARRAY_SIZE] = { 25.3, 16.8, 42.6, 18.0, 5.9, 25.3, 16.8, 98.7, 76.35, 53.48 };
//	double arr2 [ARRAY_SIZE];
//
//	for (int i = 0; i < ARRAY_SIZE; ++i) {
//		arr2[i] = contain(arr2, arr1[i]) ? 0 : arr1[i];
//	}
//
//	print_array(arr2);
//}
//
//void print_array(double* arr) {
//	printf("Array: ");
//	for (int i = 0; i < ARRAY_SIZE; ++i) {
//		printf("%.2f ", arr[i]);
//	}
//	printf("\n");
//}
//
//int contain(double* arr, double t) {
//	for (int i = 0; i < ARRAY_SIZE; ++i) {
//		if (arr[i] == t) {
//			return 1;
//		}
//	}
//	return 0;
//}