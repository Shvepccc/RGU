//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <stdarg.h>
//#include "../../libs/error.h"
//
//int hren(char* str, ...);
//
//int main() {
//	int i = 0;
//	int err;
//	int** ans;
//	int data[4] = { 0, 0, 0, 0 };
//
//	ans = (int**)malloc(BUFSIZ * sizeof(int*));
//	if (!ans) {
//		printf("Memory not allocated \n");
//	}
//	else {
//		for (int j = 0; j < BUFSIZ; j++) {
//			ans[j] = (int*)malloc(4 * sizeof(int));
//			if (!ans[j]) {
//				for (int k = 0; k < BUFSIZ; k++) {
//					free(ans[k]);
//				}
//				free(ans);
//				printf("Memory not allocated \n");
//			}
//		}
//	}
//
//	err = hren("hello", &ans, "4_1.txt", "4_2.txt", "4_3.txt", NULL);
//
//	switch (err)
//	{
//		case OPEN_FILE_ERROR:
//			printf("File can't be opened \n");
//			break;
//		default: {
//			while (ans[i][1] != -2){
//
//				if (ans[i][1] == -1) {
//					printf("File: %d, I have no occupation there ._. \n", ans[i][0]);
//				}
//				else {
//					printf("File: %d, occurrence %d on the string: %d, with index: %d \n", ans[i][0], ans[i][1], ans[i][2], ans[i][3]);
//				}
//				if (ans[i+1][0] != ans[i][0]) { printf("\n"); }
//				i++;
//			}
//			break;
//		}
//	}
//	for (int k = 0; k < BUFSIZ; k++) {
//		free(ans[k]);
//	}
//	free(ans);
//}
//
//int hren(char* str, ...) {
//	FILE* fptr;
//	char* ptr;
//	char buff[BUFSIZ];
//	int i, g, j, k, index, str_count, occur_count, total_count, file_count;
//	char sptr;
//	int*** res;
//	int data[4] = {0, 0, 0, 0};
//	
//	va_list args;
//	va_start(args, str);
//
//	res = va_arg(args, void***);
//	
//	file_count = 1;
//	total_count = 0;
//	while ((ptr = va_arg(args, void*)) != NULL) {
//
//		fptr = fopen(ptr, "r");
//		if (fptr == NULL) {
//			va_end(args);
//			return OPEN_FILE_ERROR;
//		}
//		
//		str_count = 1;
//		occur_count = 0;
//		while (fgets(buff, BUFSIZ, fptr)) {
//			
//			for (i = 0; buff[i] != '\0'; i++) {
//				for (j = i, k = 0; str[k] != '\0' && buff[j] == str[k]; j++, k++);
//				if (k > 0 && str[k] == '\0')
//				{
//					occur_count++;
//					data[0] = file_count;
//					data[1] = occur_count;
//					data[2] = str_count;
//					data[3] = i + 1;
//					for (g = 0; g < 4; g++) {
//						(*res)[total_count][g] = data[g];
//					}
//					total_count++;
//					//break;
//				}
//			}
//			str_count++;
//
//		}
//		if (feof(fptr)) {
//			fclose(fptr);
//			if (occur_count == 0) {
//				(*res)[total_count][0] = file_count;
//				(*res)[total_count][1] = -1;
//				total_count++;
//			}
//		}
//		file_count++;
//	}
//	(*res)[total_count][1] = -2;
//	total_count++;
//  va_end(args);
//	return 0;
//}