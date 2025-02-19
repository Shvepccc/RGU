//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//
//#define MATRIX_MAX 101
//#define MAX_RANGE 100
//#define MIN_RANGE -100
//
//void generate_matrix(int matrix[MATRIX_MAX][MATRIX_MAX], int size_x, int size_y, int type);
//void print_matrix(int matrix[MATRIX_MAX][MATRIX_MAX], int size_x, int size_y);
//void transp_matrix(int matrix[MATRIX_MAX][MATRIX_MAX], int size_x, int size_y, int res_matrix[MATRIX_MAX][MATRIX_MAX]);
//
//int main() {
//	srand(time(0));
//	int matrix1[MATRIX_MAX][MATRIX_MAX];
//	int matrix2[MATRIX_MAX][MATRIX_MAX];
//	int result_matrix[MATRIX_MAX][MATRIX_MAX];
//	int size_1_x, size_1_y = 0;
//
//	printf("Enter size (1-100) of matrix [format: rows columns]: ");
//	scanf("%d %d", &size_1_x, &size_1_y);
//
//	printf("\n");
//
//	generate_matrix(matrix1, size_1_x, size_1_y, 1);
//	generate_matrix(result_matrix, size_1_y, size_1_x, 0);
//
//	printf("Start matrix: \n");
//	print_matrix(matrix1, size_1_x, size_1_y);
//	printf("Result matrix: \n");
//	transp_matrix(matrix1, size_1_x, size_1_y, result_matrix);
//	print_matrix(result_matrix, size_1_y, size_1_x);
//
//	return 0;
//}
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
//
//void transp_matrix(int matrix[MATRIX_MAX][MATRIX_MAX], int size_x, int size_y, int res_matrix[MATRIX_MAX][MATRIX_MAX]) {
//	for (int x = 0; x < size_x; x++) {
//		for (int y = 0; y < size_y; y++) {
//			res_matrix[y][x] = matrix[x][y];
//		}
//	}
//}