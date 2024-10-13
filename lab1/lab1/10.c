//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//
//#define MATRIX_MAX 101
//#define MAX_RANGE 101
//#define MIN_RANGE -100
//
//void generate_matrix(int matrix[MATRIX_MAX][MATRIX_MAX], int size_x, int size_y, int type);
//void print_matrix(int matrix[MATRIX_MAX][MATRIX_MAX], int size_x, int size_y);
//void multiply_matrix(int matrix1[MATRIX_MAX][MATRIX_MAX], int matrix2[MATRIX_MAX][MATRIX_MAX], int res_matrix[MATRIX_MAX][MATRIX_MAX], int size_x, int size_y, int s);
//int main() {
//	//srand(time(0));
//	int matrix1[MATRIX_MAX][MATRIX_MAX];
//	int matrix2[MATRIX_MAX][MATRIX_MAX];
//	int result_matrix[MATRIX_MAX][MATRIX_MAX];
//	int size_1_x, size_1_y, size_2_x, size_2_y = 0;
//
//	printf("Enter size (1-100) of first matrix [format: rows columns]: ");
//	scanf("%d %d", &size_1_x, &size_1_y);
//	printf("Enter size (1-100) of second matrix [format: rows columns]: ");
//	scanf("%d %d", &size_2_x, &size_2_y);
//	printf("\n");
//
//	if (size_1_y != size_2_x) {
//		printf("Two matrices can be multiplied with each other only when \n the number of columns in the first matrix \n matches the number of rows in the second matrix.");
//		return 0;
//	}
//	else {
//		generate_matrix(matrix1, size_1_x, size_1_y, 1);
//		generate_matrix(matrix2, size_2_x, size_2_y, 1);
//		generate_matrix(result_matrix, size_1_x, size_1_y, 0);
//
//		printf("Matrix 1: \n");
//		print_matrix(matrix1, size_1_x, size_1_y);
//		printf("Matrix 2: \n");
//		print_matrix(matrix2, size_2_x, size_2_y);
//		printf("Result matrix: \n");
//		multiply_matrix(matrix1, matrix2, result_matrix, size_1_x, size_2_y, size_1_y);
//		print_matrix(result_matrix, size_1_x, size_2_y);
//	}
//
//	return 0;
//}
//
//
//void generate_matrix(int matrix[MATRIX_MAX][MATRIX_MAX], int size_x, int size_y, int type){
//	int t = 0;
//	for (int x = 0; x < size_x; ++x) {
//		for (int y = 0; y < size_y; ++y) {
//			t = (type != 0) ? (rand() % (MAX_RANGE - MIN_RANGE + 1)) + MIN_RANGE : 0;
//			matrix[x][y] = t;
//		}
//	}
//}
//
//void print_matrix(int matrix[MATRIX_MAX][MATRIX_MAX], int size_x, int size_y) {
//	printf("Matrix:\n");
//
//	for (int x = 0; x < size_x; x++) {
//		for (int y = 0; y < size_y; y++) {
//			printf("%d\t", matrix[x][y]);
//		}
//		printf("\n");
//		if (size_x > 10) { printf("\n"); }
//	}
//	printf("\n");
//}
//void multiply_matrix(int matrix1[MATRIX_MAX][MATRIX_MAX], int matrix2[MATRIX_MAX][MATRIX_MAX], int res_matrix[MATRIX_MAX][MATRIX_MAX], int size_x, int size_y, int s) {
//	for (int x = 0; x < size_x; x++) {
//		for (int y = 0; y < size_y; y++) {
//			for (int g = 0; g < s; g++) {
//				res_matrix[x][y] += matrix1[x][g] * matrix2[g][y];
//			}
//		}
//	}
//}