//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <string.h>
//
//#define MAX_LENGTH_FILE_NAME 201
//#define BUZZ_SIZE 1024
//
//void print_array(int* arr, int arr_size);
//void max_and_min_array(int* arr, int arr_size);
//int read_data(char* data);
//
//int main() {
//
//    char data[BUZZ_SIZE];
//	int arr[BUZZ_SIZE];
//	char* token;
//	int top = -1;
//
//    read_data(data);
//	printf("%s \n", data);
//
//	token = strtok(data, " ");
//	while (token != NULL)
//	{
//		top++;
//		arr[top] = atoi(token);
//		token = strtok(NULL, " ");
//	}
//
//	print_array(arr, top);
//	max_and_min_array(arr, top);
//}
//
//void print_array(int* arr, int arr_size) {
//	printf("Array: ");
//	for (int i = 0; i < arr_size; ++i) {
//		printf("%d ", arr[i]);
//	}
//	printf("\n");
//}
//
//void max_and_min_array(int* arr, int arr_size) {
//	int min = 10000000;
//	int min_index = 0;
//	int max = -10000000;
//	int max_index = 0;
//	
//	for (int i = 0; i < arr_size; ++i) {
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
//
//int read_data(char* data) {
//	FILE* data_file;
//    char filename[MAX_LENGTH_FILE_NAME];
//    printf("Enter data file name (max length of name 200 chars): ");
//    scanf("%200s", filename);
//
//    data_file = fopen(filename, "r");
//
//    if (data_file == NULL)
//    {
//        printf("Error: could not open file %s\n", filename);
//        return 1;
//    }
//    
//    fgets(data, BUZZ_SIZE, data_file);
//
//	fclose(data_file);
//    return 0;
//}