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
//int inverse_matrix(double matrix[MATRIX_MAX][MATRIX_MAX], double inverse[MATRIX_MAX][MATRIX_MAX], int size);
//
//int main() {
//	srand(time(0));
//	double matrix1[MATRIX_MAX][MATRIX_MAX];
//    double inverse[MATRIX_MAX][MATRIX_MAX];
//
//	int size = 0;
//
//	printf("Enter size (1-100) of matrix [format: n]: ");
//	scanf("%d", &size);
//
//	printf("\n");
//
//	generate_matrix(matrix1, size, size, 1);
//
//	printf("Start matrix: \n");
//	print_matrix(matrix1, size, size);
//    
//    if (inverse_matrix(matrix1, inverse, size) == -1) {
//        printf("It is impossible to calculate the determinant of this matrix");
//    }
//    else
//    {
//        /*printf("matrix1: \n");
//        print_matrix(matrix1, size, size);*/
//        printf("Inverse matrix: \n");
//        print_matrix(inverse, size, size);
//    }
//	return 0;
//}
//
//int inverse_matrix(double matrix[MATRIX_MAX][MATRIX_MAX], double inverse[MATRIX_MAX][MATRIX_MAX], int size) {
//    double support_element;
//    int i, j, k = 0;
//
//    for (i = 0; i < size; i++) {
//        for (j = 0; j < size; j++) {
//            if (i == j)
//                inverse[i][j] = 1.0;
//            else
//                inverse[i][j] = 0.0;
//
//        }
//    }
//    
//
//    for (k = 0; k < size; k++) {
//    
//        //Check for divizion by zero
//        if (matrix[k][k] == 0) {
//            printf("Divizion by zero!");
//            return -1;
//        }
//
//        //Make diagonal element (support_element) 1
//        support_element = matrix[k][k];
//        for (j = 0; j < size; j++) {
//            matrix[k][j] /= support_element;
//            inverse[k][j] /= support_element;
//        }
//
//        //Set zero under main diagonal
//        for (i = k + 1; i < size; i++) {
//            support_element = matrix[i][k];
//            for (j = 0; j < size; j++) {
//                matrix[i][j] -= support_element * matrix[k][j];
//                inverse[i][j] -= support_element * inverse[k][j];
//            }
//        }
//    }
//
//    //Set zero above main diagonal: "turn matrix upside down"
//    for (k = size - 1; k >= 0; k--) {
//        for (i = k - 1; i >= 0; i--) {
//            support_element = matrix[i][k];
//            for (j = 0; j < size; j++) {
//                matrix[i][j] -= support_element * matrix[k][j];
//                inverse[i][j] -= support_element * inverse[k][j];
//            }
//        }
//    }
//    
//    return 0;
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
//			printf("%-12.6f", matrix[x][y]);
//		}
//		printf("\n");
//		if (size_x > 10) { printf("\n"); }
//	}
//	printf("\n");
//}