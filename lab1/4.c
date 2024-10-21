//#include <stdio.h>
//#include <stdlib.h>
//
//#define ARRAY_SIZE 20
//#define MAX_RANGE 10000
//#define MIN_RANGE -10000
//
//void generate_array(int* arr);
//void print_array(int* arr);
//void max_and_min_array(int* arr);
//
//int main() {
//	
//	int arr[ARRAY_SIZE];
//	generate_array(arr);
//	print_array(arr);
//	max_and_min_array(arr);
//}
//
//void generate_array(int *arr) {
//	for (int i = 0; i < ARRAY_SIZE; ++i) {
//		arr[i] = (rand() % (MAX_RANGE - MIN_RANGE + 1)) + MIN_RANGE;
//	}
//}
//
//void print_array(int *arr) {
//	printf("Array: ");
//	for (int i = 0; i < ARRAY_SIZE; ++i) {
//		printf("%d ", arr[i]);
//	}
//	printf("\n");
//}
//
//void max_and_min_array(int* arr) {
//	int min = 100000;
//	int min_index = 0;
//	int max = -100000;
//	int max_index = 0;
//
//	for (int i = 0; i < ARRAY_SIZE; ++i) {
//		if (arr[i] > max) {
//			max = arr[i];
//			max_index = i;
//		}
//		if (arr[i] < min) {
//			min = arr[i];
//			min_index = i;
//		}
//	}
//	printf("Max value: %d, with index: %d\nMin value: %d with index %d", max, max_index, min, min_index);
//}
