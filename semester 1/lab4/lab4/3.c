//#include <stdio.h>
//#include <stdarg.h>
//#include <stdlib.h>
//
//void free_resources(char flag, ...);
//
//int main() {
//	int* A_ptr;
//	A_ptr = (int*)malloc(2*sizeof(int));
//	if (!A_ptr) {
//		printf("Memory not allocated");
//	}
//	A_ptr[0] = 0;
//	A_ptr[1] = 1;
//
//	FILE* f_ptr = fopen("3.txt", "w");
//
//	free_resources('f', f_ptr, 'm', A_ptr, NULL);
//	printf("Resources were free secsessfully");
//    return 0;
//}
//
//void free_resources(char flag, ...) {
//	char* ptr;
//	FILE* fptr;
//
//	va_list args;
//	va_start(args, flag);
//	while(1) {
//		
//		if (flag == 'f') {
//			ptr = va_arg(args, void*);
//			fclose(ptr);
//		}
//		else if (flag == 'm') {
//			ptr = va_arg(args, void*);
//			free(ptr);
//			ptr = NULL;
//		}
//		else if (flag == NULL) {
//			break;
//		}
//		flag = va_arg(args, char);
//
//		if (flag != 'f' && flag != 'm') {
//			printf("Incorrect flag");
//			break;
//		}
//
//	}
//	va_end(args);
//}