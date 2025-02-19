//#include <stdio.h>
//#include <string.h>
//
//char* string_cat(char* str1, char* str2);
//
//void main(void)
//{
//    char str1[101], str2[101];
//    char istr;
//    printf("Enter first string (max length 100 char): ");
//    gets(str1);
//    printf("Enter second string (max length 100 char): ");
//    gets(str2);
//    
//    istr = string_cat(str1, str2);
//    printf("Concatenated string: %s", str1);
//}
//
//char* string_cat(char* str1, char* str2) {
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