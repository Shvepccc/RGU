//#include <stdio.h>
//#include "../../libs/cust_string.h"
//
//int main() {
//	string str1 = string_init();
//	printf("Result of init empty string (str1): '%s'\n", str1);
//
//	char str_dest[4] = "1234";
//	string str2 = string_from(str_dest, 4);
//	printf("Result of getting string from char's array (str2): %s\n", str2);
//
//	string_cpy(&str2, &str1);
//	printf("Result of copying str2 to str1: %s\n", str1);
//
//	string str3 = string_cpy_new(str1);
//	printf("Result of copying str1 to str3: %s\n", str3);
//
//	string_cat(&str3, &str1);
//	printf("Result of concatenation str3 with str1: %s\n", str3);
//
//	string str4 = string_cpy_new(str3);
//
//	string_push_back(&str3, 'a');
//	printf("Result of push back symbol 'a' to str3: %s\n", str3);
//
//	printf("Result of compare (flexographic) str3:(%s) with str4:(%s) : %d\n", str3, str4, string_flexographic_compare(str3, str4));
//	printf("Result of compare (length + flexographic) str3:(%s) with str4:(%s) : %d\n", str3, str4, string_compare(str3, str4));
//
//	string_free(str1);
//	string_free(str2);
//	string_free(str3);
//	string_free(str4);
//	return 0;
//}