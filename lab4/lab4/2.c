//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <ctype.h>
//#include <math.h>
//#include "../../libs/error.h"
//
//int str_len(char* str);
//char* str_rev(char* str);
//char* str_cat(char* str1, char* str2);
//
//int main(int argc, char* argv[]) {
//	int err = 0;
//	int len = 0;
//	char f;
//	int i = 0, t = 0, seed;
//	char* ptr;
//	int left, right = 0;
//	char change_value;
//	char* str;
//
//	if (argc < 3) { err = NOT_ENOUGH_ARGS;  }
//	else if (argc > 3 && argv[1][1] != 'c') { err = TOO_MANY_ARGS; }
//	else {
//		f = argv[1][1];
//		switch (f) {
//			case 'l': {
//				printf("String len: %d", str_len(argv[2]));
//				break;
//			}
//			case 'r': {
//				printf("Reverse string: %s", str_rev(argv[2]));
//				break;
//			}
//			case 'u': {
//				ptr = argv[2];
//				len = str_len(argv[2]);
//
//				for (i = 0; i < len; ++i) {
//					if (*ptr >= 'a' && *ptr <= 'z' && i % 2 == 0) {
//						*ptr -= 32;
//					}
//					++ptr;
//				}
//
//				printf("Changed string: %s", argv[2]);
//				break;
//			}
//			case 'n': {
//				len = str_len(argv[2]);
//
//				char* str = (char*)malloc(len + 1 * sizeof(char));
//				if (str == NULL) {
//					printf("Memory not allocated =(\n");
//				}
//				memcpy(str, argv[2], len + 1);				
//
//				for (i = 0; i < len; i++) {
//					t = 0;
//					for (t = 0; t < len - 1 - i; t++) {
//
//						if (isdigit(str[t])) { left = 1; }
//						else if (isalpha(str[t])) { left = 2; }
//						else { left = 3; }
//
//						if (isdigit(str[t + 1])) { right = 1; }
//						else if (isalpha(str[t + 1])) { right = 2; }
//						else { right = 3; }
//
//						if (left > right) {
//							change_value = str[t];
//							str[t] = str[1 + t];
//							str[1 + t] = change_value;
//						}
//					}
//				}
//				printf("Changed string: %s", str);
//				free(str);
//				break;
//			}
//			case 'c': {
//				if (argc < 4) {
//					printf("Not enough arguments\n");
//					return 1;
//				}
//
//				//seed = atoi(argv[2]);
//				//srand(seed);
//
//				str = argv[3];
//				for (i = 1; i < argc - 3; i++) {
//					char* str2 = argv[3 + i];
//					str_cat(str, argv[3 + i]);
//				}
//
//				printf("Concatenated string: %s", str);
//				break;
//			}
//			default: {
//				err = UNKNOWN_FLAG;
//				break;
//			}
//		}
//	}
//
//	if (err == NOT_ENOUGH_ARGS) {
//		printf("Not enough arguments");
//	}
//	else if (err == UNKNOWN_FLAG) {
//		printf("Unknown flag");
//	}
//	else if (err == TOO_MANY_ARGS) {
//		printf("Too many arguments");
//	}
//
//	return 0;
//}
//
//int str_len(char* str) {
//	int c = 0;
//
//	while (*str != '\0') {
//		str++;
//		c += 1;
//	}
//
//	return c;
//}
//
//char* str_rev(char* str) {
//	int c = 0;
//	char r, l;
//	char* ptr = str;
//	while (*ptr != '\0') {
//		ptr++;
//		c += 1;
//	}
//	
//	for (int i = 0; i < (c / 2); i++) {
//		l = str[i];
//		r = str[c - i - 1];
//		str[i] = r;
//		str[c - i - 1] = l; 
//	}
//	
//	return str;
//}
//
//char* str_cat(char* str1, char* str2) {
//    int i = 0, j = 0;
//	char* res = str1;
//    while (str1[i] != '\0')
//        i++;
//    while (str2[j] != '\0')
//    {
//        str1[i] = str2[j];
//        j++;
//        i++;
//    }
//    str1[i] = '\0';
//
//    return *str1;
//}