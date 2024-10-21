#include <stdio.h>
#include <stdlib.h>

#define MATRIX_1_X 3
#define MATRIX_1_Y 3
#define MATRIX_2_X 3
#define MATRIX_2_Y 1
#define MAX_RANGE 100
#define MIN_RANGE -100

void generate_matrix(int matrix[MATRIX_1_X][MATRIX_1_Y]);
void print_matrix(int matrix[MATRIX_1_X][MATRIX_1_Y]);


int main() {
	srand(time(0));
	int matrix1[MATRIX_1_X][MATRIX_1_Y];
	generate_matrix(matrix1);
	print_matrix(matrix1);

	return 0;
}


void generate_matrix(int matrix[MATRIX_1_X][MATRIX_1_Y]){
	int t = 0;
	for (int x = 0; x < MATRIX_1_X; ++x) {
		for (int y = 0; y < MATRIX_1_Y; ++y) {
			t = (rand() % (MAX_RANGE - MIN_RANGE + 1)) + MIN_RANGE;
			//printf("%d ", t);
			matrix[x][y] = t;
		}
	}
}

void print_matrix(int matrix[MATRIX_1_X][MATRIX_1_Y]) {
	printf("Matrix:\n");

	for (int x = 0; x < MATRIX_1_X; x++) {
		for (int y = 0; y < MATRIX_1_Y; y++) {
			printf("%d\t", matrix[x][y]);
		}
		printf("\n");
	}
	printf("\n");
}
