//#define _CRT_SECURE_NO_WARNINGS
//#define MAX_LENGTH_FILE_NAME 201
//#define MAX_LENGTH_KEY 101
//#include <stdio.h>
//
//int read_txt(FILE** start_file, FILE** result_file);
//int encryption_data(FILE* start_file, FILE* result_file, char key[]);
//
//int main() {
//    while (1) {
//        char key[MAX_LENGTH_KEY];
//        FILE* start_file;
//        FILE* result_file;
//        read_txt(&start_file, &result_file);
//        printf("Enter key (max length of key 100 chars): ");
//        scanf("%s", key);
//        encryption_data(start_file, result_file, key);
//        fclose(start_file);
//        fclose(result_file);
//        printf("File was encrypted/decrypted successfully\nPress any key to resume\n");
//        _getch();
//        system("cls");
//    }
//    return 0;
//}
//
//int read_txt(FILE** start_file, FILE** result_file) {
//    char filename[MAX_LENGTH_FILE_NAME];
//    char result_filename[MAX_LENGTH_FILE_NAME];
//    printf("Enter data file name (max length of name 200 chars): ");
//    scanf("%200s", &filename);
//
//    *start_file = fopen(filename, "r");
//
//    if (*start_file == NULL)
//    {
//        printf("Error: could not open file %s\n", filename);
//        return 1;
//    }
//
//    printf("Enter result file name (max length of name 200 chars): ");
//    scanf("%s", &result_filename);
//    *result_file = fopen(result_filename, "w");
//
//    return 0;
//}
//
//int encryption_data(FILE* start_file, FILE* result_file, char key[]) {
//    char c, res;
//    int len_key = 3;
//    int i = 0;
//    int index = 0;
//
//    while ((c = fgetc(start_file)) != EOF) {
//        index = i % len_key;
//        res = c ^ key[index];
//        fputc(res, result_file);
//        i++;
//    }
//    return 0;
//}