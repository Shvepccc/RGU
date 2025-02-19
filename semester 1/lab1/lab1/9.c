//#include <stdio.h>
//
//#define ARRAY_SIZE 10
//
//void print_array(double* arr, int t);
//int contain(double* arr, double t);
//
//int main() {
//	double arr1 [ARRAY_SIZE] = { 25.3, 16.8, 42.6, 18.0, 5.9, 25.3, 16.8, 98.7, 76.35, 18.0 };
//	double arr2 [ARRAY_SIZE];
//	int t = 0;
//	for (int i = 0; i < ARRAY_SIZE; ++i) {
//		if (!contain(arr2, arr1[i])) {
//			arr2[t] = arr1[i];
//			t++;
//		}
//	}
//
//	printf("Start array: ");
//	print_array(arr1, ARRAY_SIZE);
//	printf("Array      : ");
//	print_array(arr2, t);
//}
//
//void print_array(double* arr, int t) {
//	for (int i = 0; i < t; ++i) {
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