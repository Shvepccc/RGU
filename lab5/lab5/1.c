//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include "../../libs/error.h"
//
//typedef struct employee {
//	int id;
//	char* name;
//	char* surname;
//	double salary;
//} employee;
//
//int funccmp(const employee* val1, const employee* val2);
//int funccmp_reverse(const employee* val1, const employee* val2);
//
//int main(int argc, char* argv[]) {
//
//	if (argc < 4) {
//		printf("Not enough args");
//		return NOT_ENOUGH_ARGS;
//	}
//
//	FILE* input_file;
//	FILE* output_file;
//	employee* employee_arr;
//	int employee_arr_size = 0, employee_arr_capacity = 16;
//	int i, k = 0, index_to_write_employee_data = 0;
//	if ((input_file = fopen(argv[1], "r")) == NULL) {
//		return OPEN_FILE_ERROR;
//	}
//
//	employee_arr = (employee*)malloc(sizeof(employee) * employee_arr_capacity);
//	if (employee_arr == NULL) {
//		printf("Memory allocate error :(");
//		fclose(input_file);
//		return MEMORY_ALLOCATE_ERROR;
//	}
//
//	char buff_str[BUFSIZ];
//	while (fgets(buff_str, BUFSIZ, input_file) != NULL) {
//		
//		if (employee_arr_size == employee_arr_capacity) {
//			employee_arr = (employee*)realloc(employee_arr, sizeof(employee) * employee_arr_capacity * 2);
//			if (employee_arr == NULL) {
//				printf("Memory allocate error :(");
//				fclose(input_file);
//				return MEMORY_ALLOCATE_ERROR;
//			}
//			employee_arr_capacity *= 2;
//		}
//
//		employee_arr[employee_arr_size].name = malloc(50 * sizeof(char));
//		employee_arr[employee_arr_size].surname = malloc(50 * sizeof(char));
//
//		sscanf(buff_str, "%d %s %s %lf",
//			&(employee_arr[employee_arr_size].id),
//			employee_arr[employee_arr_size].name,
//			employee_arr[employee_arr_size].surname,
//			&(employee_arr[employee_arr_size].salary));
//
//		employee_arr_size++;
//	}
//	fclose(input_file);
//
//	if (argv[2][1] == 'a') {
//		qsort(employee_arr, employee_arr_size, sizeof(employee), funccmp);
//	}
//	else if (argv[2][1] == 'd') {
//		qsort(employee_arr, employee_arr_size, sizeof(employee), funccmp_reverse);
//	}
//	else {
//		printf("Unknown flag");
//		return UNKNOWN_FLAG;
//	}
//
//	if ((output_file = fopen(argv[3], "w")) == NULL) {
//		return OPEN_FILE_ERROR;
//	}
//
//	for (i = 0; i < employee_arr_size; i++) {
//		fprintf(output_file, "%d %s %s %.2lf\n",
//			employee_arr[i].id,
//			employee_arr[i].name,
//			employee_arr[i].surname,
//			employee_arr[i].salary);
//		printf("%d %s %s %.2lf\n",
//			employee_arr[i].id,
//			employee_arr[i].name,
//			employee_arr[i].surname,
//			employee_arr[i].salary);
//	}
//	fclose(output_file);
//
//	return 0;
//}
//
//int funccmp(const employee* val1, const employee* val2) {
//
//	if (val1->salary != val2->salary) {
//		return val1->salary - val2->salary;
//	}
//	else {
//		if (val1->surname != val2->surname) {
//			return val1->surname != val2->surname;
//		}
//		else {
//			if (val1->name != val2->name) {
//				return val1->name != val2->name;
//			}
//			else {
//				if (val1->id != val2->id) {
//					return val1->id != val2->id;
//				}
//				else {
//					return 0;
//				}
//			}
//		}
//	}
//
//}
//
//int funccmp_reverse(const employee* val1, const employee* val2) {
//	employee* temp_employee;
//	temp_employee = val1;
//	val1 = val2;
//	val2 = temp_employee;
//
//	if (val1->salary != val2->salary) {
//		return val1->salary - val2->salary;
//	}
//	else {
//		if (val1->surname != val2->surname) {
//			return val1->surname != val2->surname;
//		}
//		else {
//			if (val1->name != val2->name) {
//				return val1->name != val2->name;
//			}
//			else {
//				if (val1->id != val2->id) {
//					return val1->id != val2->id;
//				}
//				else {
//					return 0;
//				}
//			}
//		}
//	}
//
//}
