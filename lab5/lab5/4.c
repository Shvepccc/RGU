//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "../../libs/cust_string.h"
//#include "../../libs/error.h"
//
//typedef struct student {
//	size_t id;
//	string name;
//	string surname;
//	string group;
//	unsigned int* score;
//} student;
//
//void print_student(student* student_array, int i);
//int compare_student_id(const student* val1, const student* val2);
//int compare_student_name(const student* val1, const student* val2);
//int compare_student_surname(const student* val1, const student* val2);
//int compare_student_group(const student* val1, const student* val2);
//
//int save_data_to_file(FILE* file, const student st, int argc);
//
//int free_student(student* student_ptr) {
//	string_free(student_ptr->name);
//	string_free(student_ptr->surname);
//	string_free(student_ptr->group);
//	free(student_ptr->score);
//	return 0;
//}
//
//int main(int argc, char* argv[]) {
//	int i, j, err = 0, action, flag = 0, stop_flag = 0;
//	int student_array_grades_sum = 0, student_array_grades_count = 0;
//	double student_array_grades_average;
//	FILE* file;
//	FILE* tracing_file = NULL;
//	char temp_str[BUFSIZ];
//	size_t temp_size_t;
//	int student_array_size = 0, student_array_capacity = 16;
//	double mean_data;
//	unsigned int sum = 0;
//
//	if (argc < 2) {
//		return NOT_ENOUGH_ARGS;
//	}
//
//	student* student_array = (student*)malloc(sizeof(student) * student_array_capacity);
//	if (student_array == NULL) {
//		return MEMORY_ALLOCATE_ERROR;
//	}
//	for (i = 0; i < student_array_capacity; i++) {
//		student_array[i].score = (unsigned int*)malloc(sizeof(unsigned int) * 5);
//		if (student_array[i].score == NULL) {
//			for (j = 0; j < i; j++) {
//				free(student_array[i].score);
//			}
//			free(student_array);
//			return MEMORY_ALLOCATE_ERROR;
//		}
//	}
//
//	if ((file = fopen(argv[1], "r")) == NULL) {
//		return OPEN_FILE_ERROR;
//	}
//	if (argc == 3) {
//		if ((tracing_file = fopen(argv[2], "w")) == NULL) {
//			fclose(file);
//			for (j = 0; j < student_array_capacity; j++) {
//				free(student_array[i].score);
//			}
//			free(student_array);
//			return OPEN_FILE_ERROR;
//		}
//	}
//
//	while (1) {
//
//		if (fscanf(file, "%zu", &(student_array[student_array_size].id)) == EOF) break;
//		
//		if (fscanf(file, "%s", &temp_str) == EOF) break;
//		student_array[student_array_size].name = string_from(temp_str, strlen(temp_str));
//
//		if (fscanf(file, "%s", &temp_str) == EOF) break;
//		student_array[student_array_size].surname = string_from(temp_str, strlen(temp_str));
//
//		if (fscanf(file, "%s", &temp_str) == EOF) break;
//		student_array[student_array_size].group = string_from(temp_str, strlen(temp_str));
//
//		for (i = 0; i < 5; i++) {
//			if (fscanf(file, "%u", &(student_array[student_array_size].score[i])) == EOF) break;
//			student_array_grades_sum += student_array[student_array_size].score[i];
//		}
//		student_array_grades_count += 5;
//		student_array_size++;
//	}
//	fclose(file);
//	if (student_array_size == 0) {
//		printf("File is empty. Programm stopped.");
//		return EMPTY_FILE;
//	}
//	student_array_grades_average = student_array_grades_sum / student_array_grades_count;
//
//	while (1) {
//		printf("1 - find students by key\n2 - sort students by key\n3 - print all students\n");
//		printf("4 - save to a trace file students whose average score for all exams is higher \nthan the average score for all exams for all students read from the file\n");
//		printf("another value - to exit\n");
//		printf("Enter number: ");
//		scanf("%d", &action);
//		printf("\n");
//		system("cls");
//
//		switch (action)
//		{
//			case 1:
//				printf("1 - find student by id\n");
//				printf("2 - find student by name\n");
//				printf("3 - find student by surname\n");
//				printf("4 - find student by group\n");
//				printf("Enter value: ");
//				scanf("%d", &action);
//				system("cls");
//				switch (action)
//				{
//					case 1:
//						printf("Enter id of student: ");
//						scanf("%zu", &temp_size_t);
//						for (i = 0; i < student_array_size; i++) {
//							if (temp_size_t == student_array[i].id) {
//								print_student(student_array, i);
//								save_data_to_file(tracing_file, student_array[i], argc);
//								flag = 1;
//								break;
//							}
//						}
//						if (!flag) {
//							printf("There is no such student\n");
//						}
//						flag = 0;
//						break;
//					case 2:
//						printf("Enter name of student: ");
//						scanf("%s", &temp_str);
//						for (i = 0; i < student_array_size; i++) {
//							if (string_compare(string_from(temp_str, strlen(temp_str)), student_array[i].name) == 0) {
//								print_student(student_array, i);
//								flag = 1;
//							}
//						}
//						if (!flag) {
//							printf("There is no such student\n");
//						}
//						flag = 0;
//						break;
//					case 3:
//						printf("Enter surname of student: ");
//						scanf("%s", &temp_str);
//						for (i = 0; i < student_array_size; i++) {
//							if (string_compare(string_from(temp_str, strlen(temp_str)), student_array[i].surname) == 0) {
//								print_student(student_array, i);
//								flag = 1;
//							}
//						}
//						if (!flag) {
//							printf("There is no such student\n");
//						}
//						flag = 0;
//						break;
//					case 4:
//						printf("Enter group of student: ");
//						scanf("%s", &temp_str);
//						for (i = 0; i < student_array_size; i++) {
//							if (string_compare(string_from(temp_str, strlen(temp_str)), student_array[i].group) == 0) {
//								print_student(student_array, i);
//								flag = 1;
//							}
//						}
//						if (!flag) {
//							printf("There is no such student\n");
//						}
//						flag = 0;
//						break;
//					default:
//						break;
//				}
//				printf("\n");
//				break;
//			case 2:
//				printf("1 - sort student by id\n");
//				printf("2 - sort student by name\n");
//				printf("3 - sort student by surname\n");
//				printf("4 - sort student by group\n");
//				printf("Enter value: ");
//				scanf("%d", &action);
//				system("cls");
//
//				switch (action)
//				{
//					case 1:
//						qsort(student_array, student_array_size, sizeof(student), compare_student_id);
//						break;
//					case 2:
//						qsort(student_array, student_array_size, sizeof(student), compare_student_name);
//						break;
//					case 3:
//						qsort(student_array, student_array_size, sizeof(student), compare_student_surname);
//						break;
//					case 4:
//						qsort(student_array, student_array_size, sizeof(student), compare_student_group);
//						break;
//					default:
//						break;
//				}
//				break;
//			case 3:
//				printf("Student data\n");
//				for (i = 0; i < student_array_size; i++) {
//					print_student(student_array, i);
//					printf("\n");
//				}
//				break;
//			case 4:
//				printf("Students whose average score is higher than the overall average score:\n");
//				for (i = 0; i < student_array_size; i++) {
//					for (j = 0; j < 5; j++) {
//						sum += student_array[i].score[j];
//					}
//					mean_data = sum / 5;
//					if (mean_data > student_array_grades_average) {
//						fprintf(tracing_file, "name: %s,\nsurname: %s\n",
//							student_array[i].name,
//							student_array[i].surname);
//					}
//					printf("name: %s,\nsurname: %s\n",
//						student_array[i].name,
//						student_array[i].surname);
//				}
//				break;
//			default:
//				stop_flag = 1;
//				break;
//		}
//		if (stop_flag == 1) {
//			break;
//		}
//	}
//	fclose(tracing_file);
//	for (j = 0; j < student_array_capacity; j++) {
//		//free(student_array[i].score);
//		free_student(&student_array[i]);
//	}
//	free(student_array);
//	return OK;
//}
//
//void print_student(student* student_array, int i) {
//	int j;
//	printf("%d Student: \n\tid: %zu \n\tname: %s\n\tsurname: %s\n\tgroup: %s\n\t", i + 1,
//		student_array[i].id,
//		student_array[i].name,
//		student_array[i].surname,
//		student_array[i].group);
//	printf("Score: ");
//	for (j = 0; j < 5; j++) {
//		printf("%u ", student_array[i].score[j]);
//	}
//	printf("\n");
//}
//
//int compare_student_id(const student* val1, const student* val2) {
//	return val1->id - val2->id;
//}
//int compare_student_name(const student* val1, const student* val2) {
//	return string_compare(val1->name, val2->name);
//}
//int compare_student_surname(const student* val1, const student* val2) {
//	return string_compare(val1->surname, val2->surname);
//}
//int compare_student_group(const student* val1, const student* val2) {
//	return string_compare(val1->group, val2->group);
//}
//
//int save_data_to_file(FILE* file, const student st, int argc) {
//	int j;
//	double mean_data;
//	unsigned int sum = 0;
//	if (argc == 3) {
//		for (j = 0; j < 5; j++) {
//			sum += st.score[j];
//		}
//		mean_data = sum / 5.0;
//		fprintf(file, "%zu: %s %s %.2lf\n",
//			st.id,
//			st.name,
//			st.surname,
//			mean_data);
//	}
//}
