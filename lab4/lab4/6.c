//#include <stdio.h>
//#include <stdarg.h>
//#include "../../libs/error.h"
//
//double calculate_polynomial(double x, int pow, ...);
//
//int main() {
//	double err;
//	err = calculate_polynomial(2.0, 2, 2.0, 2.0, 2.0);
//
//	switch ((int)err)
//	{
//		case NOT_ENOUGH_ARGS:
//			printf("Not enough arguments");
//			break;
//		case 1:
//			printf("The degree must be greater than 1");
//			break;
//		default:
//			printf("Result: %.2f", err);
//			break;
//	}
//}
//
//double calculate_polynomial(double x, int pow, ...) {
//	int i, a_0, a;
//	double res;
//
//	if (pow < 1) {
//		return 1;
//	}
//
//	va_list args;
//	va_start(args, pow);
//
//	a_0 = va_arg(args, double);
//	res = (double) a_0 * x;
//	for (i = 0; i < pow; i++) {
//		if ((a = va_arg(args, double)) == NULL) {
//			return NOT_ENOUGH_ARGS;
//		}
//		res += a;
//		if (pow - i != 1) {
//			res *= x;
//		}		
//	}
//
//	return res;
//}