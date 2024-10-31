//#include <stdio.h>
//#include <stdarg.h>
//#include "../../libs/error.h"
//
//int is_convex_polygon(int n, ...);
//int cross_product(int* o, int* a, int* b);
//
//int main() {
//	int err;
//	err = is_convex_polygon(4, 0, 0, 2, 0, 2, 2, 0, 2);
//	//err = is_convex_polygon(5, 0, 0, 2, 0, 1, 1, 2, 2, 0, 2);
//
//	switch (err)
//	{
//		case NOT_ENOUGH_ARGS:
//			printf("Not enough arguments");
//			break;
//		default:{
//			if (err == 1) {
//				printf("The polygon is convex");
//			}
//			else {
//				printf("The polygon is NOT convex");
//			}
//		break;
//		}
//	}
//  return 0;
//}
//
//int is_convex_polygon(int n, ...) {
//	int i, j, x, y, sign, crs_prod, current_sign;
//	int data[3][2] = { {0,0} ,{0,0}, {0,0} };
//
//	if (n < 3) {
//		return NOT_ENOUGH_ARGS;
//	}
//
//	va_list args;
//	va_start(args, n);
//	
//	for (i = 0; i < 3; i++) {
//		data[i][0] = va_arg(args, int);
//		data[i][1] = va_arg(args, int);
//	}
//
//	sign = 0;
//	for (i = 0; i < (n * 2); i++) {
//
//		crs_prod = cross_product(data[0], data[1], data[2]);
//		if (crs_prod != 0) {
//			current_sign = (crs_prod > 0) ? 1 : - 1;
//			if (sign == 0) {
//				sign = current_sign;
//			}
//			else if (sign != current_sign)
//			{
//				va_end(args);
//				return 0;
//			}
//		}
//		for (j = 0; j < 2; j++) {
//			data[j][0] = data[j+1][0];
//			data[j][1] = data[j+1][1];
//		}
//		if ((data[2][0] = va_arg(args, int) == NULL)) {
//			va_end(args);
//			return NOT_ENOUGH_ARGS;
//		}
//		else {
//			data[2][1] = va_arg(args, int);
//		}
//	}
//  va_end(args);
//	return 1;
//}
//
//int cross_product(int* o, int* a, int* b) {
//	return (a[0] - o[0]) * (b[1] - o[1]) - (a[1] - o[1]) * (b[0] - o[0]);
//}