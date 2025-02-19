//#include <stdio.h>
//#include <string.h>
//
//void main() {
//	char str[] = "H8e/l*l$o e5v7e)r-y!o@ne,_t?h/i8s_i~s_m45y_b^l&o5g!";
//	int len = strlen(str);
//	int i, t = 0;
//	int left, right = 0;
//	char change_value;
//
//	for (i = 0; i < len; i++) {
//		t = 0;
//		for (t = 0; t < len - 1 - i; t++) {
//
//			if (isdigit(str[t])) { left = 1; }
//			else if (isalpha(str[t])) { left = 2; }
//			else { left = 3; }
//
//			if (isdigit(str[t+1])) { right = 1; }
//			else if (isalpha(str[t+1])) { right = 2; }
//			else { right = 3; }
//
//			if (left > right) {
//				change_value = str[t];
//				str[t] = str[1 + t];
//				str[1 + t] = change_value;
//			}
//		}
//	}
//	printf("%s", str);
//}