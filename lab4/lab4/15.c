//#include <stdio.h>
//#include <stdlib.h>
//#define MEMORY_ALLOCATE_ERROR 6
//#define SIZE 10
//typedef int tvalue;
//
//int find_saddle_points(tvalue const* const* matrix, size_t matrix_rows_count, size_t
//	matrix_columns_count, size_t*** found_saddle_points_storage, size_t
//	* found_saddle_points_count_storage, int (*comparer)(tvalue const*, tvalue const*), 
//    int is_comparison_is_strict);
//
//int comparer_function(tvalue const* a, tvalue const* b);
//
//int main() {
//    int i, j;
//    size_t count_saddle_points = 0;
//    tvalue matrix_data[SIZE][SIZE] = {
//        {15,  2,  30,  40,  50,  6,  70,  80,  90,  100},
//        {10,  9,  10,  10,  10,  10, 10,  10,  10,  10},
//        {5,   1,  15,  20,  25,  3,  35,  40,  45,  50},
//        {20,  2,  30,  35,  40,  4,  50,  55,  60,  65}, 
//        {1,   3,  5,   7,   9,   1,  13,  15,  17,  19},
//        {100, 9,  60,  40,  20,  7,  20,  40,  60,  800},
//        {10,  2,  30,  40,  50,  6,  70,  80,  90,  100},
//        {5,   1,  25,  35,  45,  5,  65,  75,  85,  95},
//        {0,   -1, -20, -30, -40, -5, -60, -70, -80, -90},
//        {25,  3,  35,  40,  45,  5,  55,  60,  65,  70}
//    };
//    int** matrix = (int**)malloc(sizeof(int*) * SIZE);
//    if (matrix == NULL) {
//        return MEMORY_ALLOCATE_ERROR;
//    }
//    for (i = 0; i < SIZE; i++) {
//        matrix[i] = (int*)malloc(sizeof(int) * SIZE);
//        if (matrix[i] == NULL) {
//            return MEMORY_ALLOCATE_ERROR;
//        }
//        for (j = 0; j < SIZE; j++) {
//            matrix[i][j] = matrix_data[i][j];
//        }
//    }
//    size_t** result;
//
//    find_saddle_points(matrix, SIZE, SIZE, &result, &count_saddle_points, comparer_function, 0);
//    printf("Result of processing matrix with parametr is_comparison_is_strict = 0\n");
//    for (i = 0; i < count_saddle_points; i++) {
//        printf("Saddle point at (%d, %d)\n", result[i][0], result[i][1]);
//    }
//    printf("\n");
//
//    count_saddle_points = 0;
//    find_saddle_points(matrix, SIZE, SIZE, &result, &count_saddle_points, comparer_function, 1);
//    printf("Result of processing matrix with parametr is_comparison_is_strict = 1\n");
//    for (i = 0; i < count_saddle_points; i++) {
//        printf("Saddle point at (%d, %d)\n", result[i][0], result[i][1]);
//    }
//}
//
//int comparer_function(tvalue const* a, tvalue const* b) {
//    if (*a == *b) {
//        return 0;
//    }
//    return (*a) - (*b);
//}
//
//int find_saddle_points(tvalue const* const* matrix, size_t matrix_rows_count, size_t
//	matrix_columns_count, size_t*** found_saddle_points_storage, size_t
//	* found_saddle_points_count_storage, int (*comparer)(tvalue const*, tvalue const*), 
//    int is_comparison_is_strict) {
//	
//    int i, j, k, isSaddle;
//    int size = 0, capacity = 16;
//
//    if (matrix == NULL) {
//        return 1;
//    }
//    else {
//        for (i = 0; i < matrix_rows_count; i++) {
//            if (matrix[i] == NULL) {
//                return 2;
//            }
//        }
//    }
//    if (found_saddle_points_storage == NULL) {
//        return 3;
//    }
//    if (found_saddle_points_count_storage == NULL) {
//        return 4;
//    }
//    if (comparer == NULL) {
//        return 5;
//    }
//
//    *found_saddle_points_storage = (size_t**)malloc(sizeof(size_t*) * capacity);
//    if (*found_saddle_points_storage == NULL) {
//        return MEMORY_ALLOCATE_ERROR;
//    }
//    for (i = 0; i < capacity; i++) {
//        (*found_saddle_points_storage)[i] = (size_t*)malloc(sizeof(size_t) * 2);
//        if ((*found_saddle_points_storage)[i] == NULL) {
//            return MEMORY_ALLOCATE_ERROR;
//        }
//    }
//
//    for (i = 0; i < matrix_rows_count; i++) {
//        for (j = 0; j < matrix_columns_count; j++) {
//            isSaddle = 1;
//
//            // Check row
//            for (k = 0; k < matrix_columns_count; k++) {
//                if (is_comparison_is_strict == 0 ? (comparer(&(matrix[i][k]), &(matrix[i][j])) < 0) : (comparer(&(matrix[i][k]), &(matrix[i][j])) <= 0)) {
//                    isSaddle = 0;
//                    break;
//                }
//            }
//            
//            // Check column
//            for (k = 0; k < matrix_rows_count; k++) {
//                if (is_comparison_is_strict == 0 ? (comparer(&(matrix[k][j]), &(matrix[i][j])) > 0) : (comparer(&(matrix[k][j]), &(matrix[i][j])) >= 0)) {
//                    isSaddle = 0;
//                    break;
//                }
//            }
//
//            if (isSaddle) {
//                *found_saddle_points_count_storage += 1;
//                if (size == capacity) {
//                    *found_saddle_points_storage = (size_t**)realloc(found_saddle_points_storage, sizeof(size_t*) * capacity * 2);
//                    if (*found_saddle_points_storage == NULL) {
//                        return MEMORY_ALLOCATE_ERROR;
//                    }
//                    capacity *= 2;
//                    for (i = 0; i < capacity; i++) {
//                        (*found_saddle_points_storage)[i] = (size_t*)malloc(sizeof(size_t) * 2);
//                        if ((*found_saddle_points_storage)[i] == NULL) {
//                            return MEMORY_ALLOCATE_ERROR;
//                        }
//                    }
//                }
//                (*found_saddle_points_storage)[size][0] = i;
//                (*found_saddle_points_storage)[size][1] = j;
//                size++;
//                //printf("Saddle point at (%d, %d) with value %d\n", i, j, matrix[i][j]);
//            }
//        }
//    }
//	return 0;
//}