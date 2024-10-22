/* Дан массив целых чисел типа int. Реализовать алгоритм двоичного поиска элемента,
значение которого запрашивается у пользователя из стандартного потока ввода */

//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//
//#define ARRAY_SIZE 10
//
//int main() {
//	int arr[ARRAY_SIZE] = {1,2,3,4,5,6,7,8,9};
//	int searching_value = 0;
//	int n = 0;
//	printf("Enter searching value: ");
//	scanf("%d", &searching_value);
//
//	n = ARRAY_SIZE / 2;
//	while (1) {
//		if (searching_value > arr[n]) {
//			n += n / 2;
//		}
//		else if (searching_value < arr[n]){
//			n -= n / 2;
//		}
//		else if (searching_value == arr[n]) {
//			printf("\nValue found! Value: %d, index: %d", arr[n], n);
//			break;
//		}
//
//		if (n < 1 || n > ARRAY_SIZE) {
//			printf("The value is missing from the array =( \n");
//			break;
//		}
//	}
//}