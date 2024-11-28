#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "../../libs/cust_string.h"
#include "../../libs/error.h"
#include "../../libs/u_list.h"

typedef struct citizen {
	string surname;
	string name;
	string lastname;
	string date_of_birth;
	char gender;
	double monthly_income;
} citizen;

int print_citizen_data(u_list_node const* list_node);

int time_from_str(char const* str, time_t* result);
int convert_TO_decimal(char* n, int base, int* ans);

int main() {
	time_t time_1;
	time_t time_2;
	double temp_double;
	u_list_node* temp_ptr;
	int stop_flag = 0, action, flag = 0, i;
	citizen* temp_citizen, * temp_citizen_2 = NULL;
	char temp_str[BUFSIZ];
	FILE* data_file;
	FILE* result_file;
	u_list citizen_array = u_list_init(NULL, sizeof(citizen));

	if ((data_file = fopen("task_5.txt", "r")) == NULL) {
		return OPEN_FILE_ERROR;
	}

	temp_citizen = (citizen*)malloc(sizeof(citizen));
	if (temp_citizen == NULL) {
		fclose(data_file);
		return MEMORY_ALLOCATE_ERROR;
	}
	char temp_cahr;
	while (1) {

		if (fscanf(data_file, "%s", &temp_str) == EOF) break;
		temp_citizen->surname = string_from(temp_str, strlen(temp_str));

		if (fscanf(data_file, "%s", &temp_str) == EOF) break;
		temp_citizen->name = string_from(temp_str, strlen(temp_str));

		if (fscanf(data_file, "%s", &temp_str) == EOF) break;
		temp_citizen->lastname = string_from(temp_str, strlen(temp_str));

		if (fscanf(data_file, "%s", &temp_str) == EOF) break;
		temp_citizen->date_of_birth = string_from(temp_str, strlen(temp_str));

		if (fscanf(data_file, "%s", &temp_str) == EOF) break;
		temp_citizen->gender = temp_str[0];

		if (fscanf(data_file, "%lf", &(temp_citizen->monthly_income)) == EOF) break;

		i = 0;
		temp_ptr = citizen_array.first_node;
		while (temp_ptr != NULL) {
			temp_citizen_2 = temp_ptr->data;
			time_from_str(temp_citizen->date_of_birth, &time_1);
			time_from_str(temp_citizen_2->date_of_birth, &time_2);

			if (difftime(time_1, time_2) <= 0) {
				break;
			}
			temp_ptr = temp_ptr->next_node;
			i++;
		}

		u_list_insert(&citizen_array, temp_citizen, i);
	}
	fclose(data_file);

	while (1) {
		printf("1 - search citizen by key\n2 - change citizen data by key\n3 - print all citizens\n");
		printf("4 - to save all citizens data to file\n");
		printf("another value - to exit\n");
		printf("Enter number: ");
		scanf("%d", &action);
		printf("\n");
		system("cls");

		switch (action)
		{
			case 1:
				printf("If you don't need any parameter, then enter '-'\n");
				printf("Enter citizen surname: ");
				scanf("%s", &temp_str);
				temp_citizen->surname = string_from(temp_str, strlen(temp_str));

				printf("Enter citizen name: ");
				scanf("%s", &temp_str);
				temp_citizen->name = string_from(temp_str, strlen(temp_str));

				printf("Enter citizen lastname: ");
				scanf("%s", &temp_str);
				temp_citizen->lastname = string_from(temp_str, strlen(temp_str));

				printf("Enter citizen date of birth: ");
				scanf("%s", &temp_str);
				temp_citizen->date_of_birth = string_from(temp_str, strlen(temp_str));

				printf("Enter citizen gender: ");
				scanf("%s", &temp_str);
				temp_citizen->gender = temp_str[0];

				printf("Enter citizen monthly income: ");
				if (scanf("%lf", &temp_double) != EOF) { temp_citizen->monthly_income = temp_double; }
				else { temp_citizen->monthly_income = -1.0;  }

				temp_ptr = citizen_array.first_node;
				while (temp_ptr->next_node != NULL) {
					temp_citizen_2 = temp_ptr->data;

					if ((temp_citizen->surname[0] != '-' ? (string_compare(temp_citizen->surname, temp_citizen_2->surname) == 0) : (1)) &&
						(temp_citizen->lastname[0] != '-' ? (string_compare(temp_citizen->lastname, temp_citizen_2->lastname) == 0) : (1)) &&
						(temp_citizen->name[0] != '-' ? (string_compare(temp_citizen->name, temp_citizen_2->name) == 0) : (1)) &&
						(temp_citizen->date_of_birth[0] != '-' ? (string_compare(temp_citizen->date_of_birth, temp_citizen_2->date_of_birth) == 0) : (1)) &&
						(temp_citizen->gender != '-' ? (string_compare(temp_citizen->gender, temp_citizen_2->gender) == 0) : (1)) &&
						(temp_citizen->monthly_income >= 0 ? ((temp_citizen->monthly_income - temp_citizen_2->monthly_income) == 0) : (1))) {
						
						flag = 1;
						print_citizen_data(temp_ptr);
						//break;
					}
					temp_ptr = temp_ptr->next_node;
				}
				if (!flag) {
					printf("There is no such citizen\n");
				}

				break;
			case 2:
				printf("Enter citizen surname: ");
				scanf("%s", &temp_str);
				temp_citizen->surname = string_from(temp_str, strlen(temp_str));

				printf("Enter citizen name: ");
				scanf("%s", &temp_str);
				temp_citizen->name = string_from(temp_str, strlen(temp_str));

				printf("Enter citizen lastname: ");
				scanf("%s", &temp_str);
				temp_citizen->lastname = string_from(temp_str, strlen(temp_str));

				temp_ptr = citizen_array.first_node;
				while (temp_ptr->next_node != NULL) {
					temp_citizen_2 = temp_ptr->data;
					if (string_compare(temp_citizen->surname, temp_citizen_2->surname) == 0 &&
						string_compare(temp_citizen->lastname, temp_citizen_2->lastname) == 0 &&
						string_compare(temp_citizen->name, temp_citizen_2->name) == 0) {

						flag = 1;
						print_citizen_data(temp_ptr);

						printf("Enter new surname: ");
						scanf("%s", &temp_str);
						temp_citizen_2->surname = string_from(temp_str, strlen(temp_str));

						printf("Enter new name: ");
						scanf("%s", &temp_str);
						temp_citizen_2->name = string_from(temp_str, strlen(temp_str));

						printf("Enter new lastname: ");
						scanf("%s", &temp_str);
						temp_citizen_2->lastname = string_from(temp_str, strlen(temp_str));

						printf("Enter new date of birth: ");
						scanf("%s", &temp_str);
						temp_citizen_2->date_of_birth = string_from(temp_str, strlen(temp_str));

						printf("Enter new gender: ");
						scanf("%s", &temp_str);
						temp_citizen_2->gender = temp_str[0];

						printf("Enter new monthly income: ");
						scanf("%lf", &(temp_citizen_2->monthly_income));

						break;
					}
					temp_ptr = temp_ptr->next_node;
				}
				if (!flag) {
					printf("There is no such citizen\n");
				}
				break;
			case 3:
				printf("========== Citizen data ==========\n\n");
				u_list_traverse(&citizen_array, print_citizen_data);
				break;
			case 4:
				printf("Enter path to file to save all citizens data: ");
				scanf("%s", &temp_str);

				if (strlen(temp_str) == 1) {
					if ((result_file = fopen("task_5_res.txt", "w")) == NULL) {
						free(temp_citizen);
						free(temp_citizen_2);
						u_list_free(&citizen_array);
						return OPEN_FILE_ERROR;
					}
				}
				else {
					if ((result_file = fopen(temp_str, "w")) == NULL) {
						free(temp_citizen);
						free(temp_citizen_2);
						u_list_free(&citizen_array);
						return OPEN_FILE_ERROR;
					}
				}

				temp_ptr = citizen_array.first_node;
				while (temp_ptr->next_node != NULL) {
					fprintf(result_file, "Surname: %s \nName: %s \nLastname: %s \nDate of birth: %s \nGender: %c \nMonthly income: %.2lf\n\n",
						temp_citizen->surname,
						temp_citizen->name,
						temp_citizen->lastname,
						temp_citizen->date_of_birth,
						temp_citizen->gender,
						temp_citizen->monthly_income);
					temp_ptr = temp_ptr->next_node;
				}
				fclose(result_file);
				printf("Citiizen data saved succesfully\n\n");
				break;
			default:
				stop_flag = 1;
				break;
		}
		if (stop_flag == 1) {
			break;
		}
	}
	free(temp_citizen);
	free(temp_citizen_2);
	u_list_free(&citizen_array);
	return OK;
}

int print_citizen_data(u_list_node const* list_node) {
	citizen* temp_citizen = list_node->data;
	printf("Surname:%s \nName:%s \nLastname: %s \nDate of birth: %s \nGender: %c \nMonthly income: %.2lf\n\n", 
		temp_citizen->surname, 
		temp_citizen->name,
		temp_citizen->lastname,
		temp_citizen->date_of_birth,
		temp_citizen->gender,
		temp_citizen->monthly_income);
	return 0;
}

int time_from_str(char const * str, time_t* result) {
	time_t mytime;
	int i = 0, j = 0, temp_int = 0;
	int* data = (int*)malloc(sizeof(int) * 3);
	if (data == NULL) {
		return NULL;
	}
	char* temp_char = (char*)malloc(sizeof(char) * 5);
	if (temp_char == NULL) {
		return NULL;
	}

	struct tm* mail_time = (struct tm*)malloc(sizeof(struct tm));
	if (mail_time == NULL) {
		return MEMORY_ALLOCATE_ERROR;
	}

	char* ptr = str;
	while (*ptr) {
		if (i == 0 && *ptr == '0' && *(ptr+1) != '0') {
			ptr++;
			continue;
		}
		else if (*ptr == ':' || *ptr == '_' || *ptr == ' ' || *ptr == '.') {
			temp_char[i] = '\0';
			i = 0;
			convert_TO_decimal(temp_char, 10, &temp_int);
			data[j] = temp_int;
			j++;
			temp_int = 0;
		}
		else {
			temp_char[i] = *ptr;
			i++;
		}
		ptr++;
	}
	if (i != 0) {
		temp_char[i] = '\0';
		i = 0;
		convert_TO_decimal(temp_char, 10, &temp_int);
		data[j] = temp_int;
		j++;
	}
	mail_time->tm_hour = 0;
	mail_time->tm_min = 0;
	mail_time->tm_sec = 0;
	mail_time->tm_wday = 0;
	mail_time->tm_yday = 0;
	mail_time->tm_isdst = 0;
	mail_time->tm_mday = data[0];
	mail_time->tm_mon = data[1] - 1;
	mail_time->tm_year = data[2] - 1900;

	mytime = mktime(mail_time);
	memcpy(result, &mytime, sizeof(time_t));
	free(data);
	free(temp_char);
	free(mail_time);
	return 0;
}

int convert_TO_decimal(char* n, int base, int* ans) {
	int len = 0, j = 0, sign = 0;

	if (base < 2 || base > 36) {
		return INCORRECT_BASE;
	}
	if (n[0] == '0') {
		*ans = 0;
		return 0;
	}

	len = strlen(n);

	if (n[0] == '-') { sign = 1; }

	for (int i = sign; i < len; i++) {
		if (isdigit(n[i])) {
			(*ans) += (int)((int)n[i] - '0') * pow((double)base, len - i - 1);
		}
		else if (isalpha(n[i])) {
			(*ans) += (int)((int)toupper(n[i]) - 'A' + 10) * pow((double)base, len - i - 1);
		}
	}
	if (sign == 1) { (*ans) *= -1; }
	return 0;
}