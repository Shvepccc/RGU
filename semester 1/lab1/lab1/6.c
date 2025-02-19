//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//
//#define MATRIX_MAX 101
//#define MAX_RANGE 100
//#define MIN_RANGE -100
//
//void generate_matrix(double matrix[MATRIX_MAX][MATRIX_MAX], int size_x, int size_y, int type);
//void print_matrix(double matrix[MATRIX_MAX][MATRIX_MAX], int size_x, int size_y);
//double determinant_matrix(double matrix[MATRIX_MAX][MATRIX_MAX], int size);
//void find_triangular_matrix(double A[MATRIX_MAX][MATRIX_MAX], double L[MATRIX_MAX][MATRIX_MAX], double U[MATRIX_MAX][MATRIX_MAX], int size);
//
//int main() {
//	srand(time(0));
//	double matrix1[MATRIX_MAX][MATRIX_MAX];
//    double matrixL[MATRIX_MAX][MATRIX_MAX];
//    double matrixU[MATRIX_MAX][MATRIX_MAX];
//	int result_matrix[MATRIX_MAX][MATRIX_MAX];
//	int size = 3;
//
//	printf("Enter size (1-100) of matrix [format: n]: ");
//	scanf("%d", &size);
//
//	printf("\n");
//
//	generate_matrix(matrix1, size, size, 1);
//    generate_matrix(matrixL, size, size, 1);
//    generate_matrix(matrixU, size, size, 1);
//
//	printf("Start matrix: \n");
//	print_matrix(matrix1, size, size);
//
//    find_triangular_matrix(matrix1, matrixL, matrixU, size);
//    
//	printf("Result: %.2f\n", determinant_matrix(matrixU, size));
//
//	return 0;
//}
//
//double determinant_matrix(double matrix[MATRIX_MAX][MATRIX_MAX], int size) {
//	double res = 1;
//    for (int i = 0; i < size; i++) {
//        printf("%.4f\n", matrix[i][i]);
//        res = res * matrix[i][i];
//    }
//	return res;
//}
//
//void generate_matrix(double matrix[MATRIX_MAX][MATRIX_MAX], int size_x, int size_y, int type){
//    int t = 0;
//	for (int x = 0; x < size_x; ++x) {
//		for (int y = 0; y < size_y; ++y) {
//			t = (type != 0) ? (rand() % (MAX_RANGE - MIN_RANGE + 1)) + MIN_RANGE : 0;
//			matrix[x][y] = t;
//		}
//	}
//}
//
//void print_matrix(double matrix[MATRIX_MAX][MATRIX_MAX], int size_x, int size_y) {
//	printf("Matrix:\n");
//
//	for (int x = 0; x < size_x; x++) {
//		for (int y = 0; y < size_y; y++) {
//			printf("%.2f\t", matrix[x][y]);
//		}
//		printf("\n");
//		if (size_x > 10) { printf("\n"); }
//	}
//	printf("\n");
//}
//
//void find_triangular_matrix(double A[MATRIX_MAX][MATRIX_MAX], double L[MATRIX_MAX][MATRIX_MAX], double U[MATRIX_MAX][MATRIX_MAX], int size) {
//    int i, j, k;
//
//    for (i = 0; i < size; i++) {
//        for (j = 0; j < size; j++) {
//            if (i == j)
//                L[i][j] = 1.0;
//            else
//                L[i][j] = 0.0;
//
//            U[i][j] = 0.0;
//        }
//    }
//
//    for (i = 0; i < size; i++) {
//
//        for (j = i; j < size; j++) {
//            U[i][j] = A[i][j];
//            for (k = 0; k < i; k++) {
//                U[i][j] -= L[i][k] * U[k][j];
//            }
//        }
//
//        for (j = i + 1; j < size; j++) {
//            L[j][i] = A[j][i];
//            for (k = 0; k < i; k++) {
//                L[j][i] -= L[j][k] * U[k][i];
//            }
//            L[j][i] /= U[i][i];
//        }
//
//    }
//}