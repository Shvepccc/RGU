#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../libs/error.h"
#include "../../libs/u_list.h"
#include "../../libs/cust_string.h"

typedef struct node {
	int ratio;
	int pow;
} node;

int polynomial_add(u_list* list_ptr_1, u_list* list_ptr_2);
int polynomial_sub(u_list* list_ptr_1, u_list* list_ptr_2);
int polynomial_mult(u_list* list_ptr_1, u_list* list_ptr_2);
int polynomial_div(u_list* list_ptr_1, u_list* list_ptr_2);


int standart_appearance(u_list* source_ptr);
int print_polynomial(u_list* list_ptr);

int comparator_by_pow(u_list_node* a, u_list_node* b) {
	node* n1, * n2;
	n1 = a->data;
	n2 = b->data;
	return n2->pow - n1->pow;
}

int main() {
	int i, comment_flag = 0, is_expression = 0;
	FILE* data_file;
	string str = string_init();
	string temp_string;
	char temp_str[BUFSIZ];
	char* symbol;
	u_list list_summator = u_list_init(NULL, sizeof(node));

	node* _node = (node*)malloc(sizeof(node));
	
	u_list list_1 = u_list_init(NULL, sizeof(node));
	_node->ratio = 1;
	_node->pow = 0;
	u_list_push_back(&list_1, _node);
	_node->ratio = 4;
	_node->pow = 0;
	u_list_push_back(&list_1, _node);
	_node->ratio = 2;
	_node->pow = 1;
	u_list_push_back(&list_1, _node);
	_node->ratio = 2;
	_node->pow = 2;
	u_list_push_back(&list_1, _node);
	_node->ratio = 4;
	_node->pow = 3;
	u_list_push_back(&list_1, _node);
	_node->ratio = 10;
	_node->pow = 2;
	u_list_push_back(&list_1, _node);
	//print_polynomial(&list_1);

	u_list list_2 = u_list_init(NULL, sizeof(node));
	_node->ratio = 3;
	_node->pow = 1;
	u_list_push_back(&list_2, _node);
	_node->ratio = -2;
	_node->pow = 0;
	u_list_push_back(&list_2, _node);
	//print_polynomial(&list_2);

	u_list list_3 = u_list_init(NULL, sizeof(node));
	_node->ratio = 2;
	_node->pow = 0;
	u_list_push_back(&list_3, _node);
	//print_polynomial(&list_3);

	printf("add: ");
	polynomial_add(&list_1, &list_2);
	print_polynomial(&list_1);
	
	printf("sub: ");
	polynomial_sub(&list_1, &list_2);
	print_polynomial(&list_1);

	printf("mult: ");
	polynomial_mult(&list_1, &list_2);
	print_polynomial(&list_1);

	
	/*if ((data_file = fopen("task_6.txt", "r")) == NULL) {
		return OPEN_FILE_ERROR;
	}

	while ((fgets(temp_str, BUFSIZ, data_file)) != NULL) {
		temp_string = string_from(temp_str, strlen(temp_str));

		i = 0;
		while (temp_string[i] != '\0') {
			if (temp_string[i] == '%') {
				break;
			}
			else if (temp_string[i] == '[' || comment_flag == 1) {
				if (comment_flag == 1 && temp_string[i] == ']') {
					comment_flag = 0;
				}
				else {
					comment_flag = 1;
				}
				i++;
				continue;
			}

			if (comment_flag == 0) {

				printf("%c", temp_string[i]);
				string_push_back(&temp_string, temp_string[i]);
			}
			i++;
		}

	}*/
	return 0;
}

int polynomial_add(u_list* list_ptr_1, u_list* list_ptr_2) {
	u_list_node* temp_ptr_1, *temp_ptr_2;
	u_list temp_list = u_list_init(NULL, sizeof(node));
	node* n1 = NULL, * n2 = NULL, * temp_node;
	int flag_1 = 0, flag_2 = 0;

	standart_appearance(list_ptr_1);
	standart_appearance(list_ptr_2);

	//printf("\n-- Standart appearance --\n");
	//print_polynomial(list_ptr_1);
	//print_polynomial(list_ptr_2);
	//printf("-------------------------\n\n");

	temp_node = (node*)malloc(sizeof(node));

	temp_ptr_1 = list_ptr_1->first_node;
	temp_ptr_2 = list_ptr_2->first_node;
	while (temp_ptr_1 != NULL || temp_ptr_2 != NULL) {
		if (temp_ptr_1 != NULL) n1 = temp_ptr_1->data;
		if (temp_ptr_2 != NULL) n2 = temp_ptr_2->data;

		if (n1->pow == n2->pow) {
			if (n1->ratio + n2->ratio != 0) {
				temp_node->pow = n1->pow;
				temp_node->ratio = n1->ratio + n2->ratio;
				u_list_push_back(&temp_list, temp_node);
			}
			flag_1 = 1;
			flag_2 = 1;
		}
		else {
			if (n1->pow > n2->pow) {
				u_list_push_back(&temp_list, n1);
				flag_1 = 1;
			}
			else {
				u_list_push_back(&temp_list, n2);
				flag_2 = 1;
			}
		}

		if (temp_ptr_1 != NULL && flag_1) temp_ptr_1 = temp_ptr_1->next_node;
		if (temp_ptr_2 != NULL && flag_2) temp_ptr_2 = temp_ptr_2->next_node;
		flag_1 = 0; flag_2 = 0;
	}
	if (temp_list.first_node == NULL) {
		temp_node->pow = 0;
		temp_node->ratio = 0;
		u_list_push_back(&temp_list, temp_node);
	}
	*list_ptr_1 = temp_list;
	free(temp_node);
	return 0;
}

int polynomial_sub(u_list* list_ptr_1, u_list* list_ptr_2) {
	u_list_node* temp_ptr_1, * temp_ptr_2;
	u_list temp_list = u_list_init(NULL, sizeof(node));
	node* n1 = NULL, * n2 = NULL, * temp_node;
	int flag_1 = 0, flag_2 = 0;

	temp_node = (node*)malloc(sizeof(node));

	temp_ptr_1 = list_ptr_1->first_node;
	temp_ptr_2 = list_ptr_2->first_node;
	while (temp_ptr_1 != NULL || temp_ptr_2 != NULL) {
		if (temp_ptr_1 != NULL) n1 = temp_ptr_1->data;
		if (temp_ptr_2 != NULL) n2 = temp_ptr_2->data;

		if (n1->pow == n2->pow) {
			if (n1->ratio - n2->ratio != 0) {
				temp_node->pow = n1->pow;
				temp_node->ratio = n1->ratio - n2->ratio;
				u_list_push_back(&temp_list, temp_node);
			}
			flag_1 = 1;
			flag_2 = 1;
		}
		else {
			if (n1->pow > n2->pow) {
				u_list_push_back(&temp_list, n1);
				flag_1 = 1;
			}
			else {
				u_list_push_back(&temp_list, n2);
				flag_2 = 1;
			}
		}

		if (temp_ptr_1 != NULL && flag_1) temp_ptr_1 = temp_ptr_1->next_node;
		if (temp_ptr_2 != NULL && flag_2) temp_ptr_2 = temp_ptr_2->next_node;
		flag_1 = 0; flag_2 = 0;
	}

	if (temp_list.first_node == NULL) {
		temp_node->pow = 0;
		temp_node->ratio = 0;
		u_list_push_back(&temp_list, temp_node);
	}
	*list_ptr_1 = temp_list;
	free(temp_node);
	return 0;
}

int polynomial_mult(u_list* list_ptr_1, u_list* list_ptr_2) {
	u_list_node* temp_ptr_1, * temp_ptr_2;
	u_list temp_list = u_list_init(NULL, sizeof(node));
	node* n1 = NULL, * n2 = NULL, * temp_node;
	int flag_1 = 0, flag_2 = 0;

	standart_appearance(list_ptr_1);
	standart_appearance(list_ptr_2);

	temp_node = (node*)malloc(sizeof(node));

	temp_ptr_1 = list_ptr_1->first_node;
	temp_ptr_2 = list_ptr_2->first_node;
	while (temp_ptr_1 != NULL || temp_ptr_2 != NULL) {
		if (temp_ptr_1 != NULL) n1 = temp_ptr_1->data;
		if (temp_ptr_2 != NULL) n2 = temp_ptr_2->data;

		if (n1->pow == n2->pow) {
			if (n1->ratio * n2->ratio != 0) {
				temp_node->pow = n1->pow;
				temp_node->ratio = n1->ratio * n2->ratio;
				u_list_push_back(&temp_list, temp_node);
			}
			flag_1 = 1;
			flag_2 = 1;
		}
		else {
			if (n1->pow > n2->pow) {
				u_list_push_back(&temp_list, n1);
				flag_1 = 1;
			}
			else {
				u_list_push_back(&temp_list, n2);
				flag_2 = 1;
			}
		}

		if (temp_ptr_1 != NULL && flag_1) temp_ptr_1 = temp_ptr_1->next_node;
		if (temp_ptr_2 != NULL && flag_2) temp_ptr_2 = temp_ptr_2->next_node;
		flag_1 = 0; flag_2 = 0;
	}
	if (temp_list.first_node == NULL) {
		temp_node->pow = 0;
		temp_node->ratio = 0;
		u_list_push_back(&temp_list, temp_node);
	}
	*list_ptr_1 = temp_list;
	free(temp_node);
	return 0;
}

int standart_appearance(u_list* source_ptr) {
	int flag = 0;
	u_list_node* temp_ptr, * last_ptr = NULL;
	node* n1, * last_data;
	u_list destination_ptr = u_list_init(NULL, sizeof(node));

	last_data = (node*)malloc(sizeof(node));
	if (!last_data) return MEMORY_ALLOCATE_ERROR;
	last_data->pow = INT_MIN;
	last_data->ratio = 0;    

	u_list_mergeSort(source_ptr, comparator_by_pow);

	temp_ptr = source_ptr->first_node;
	while (temp_ptr != NULL) {
		n1 = temp_ptr->data;

		if (last_data->pow == n1->pow) {
			last_data->ratio += n1->ratio;
		}
		else {
			if (last_data->pow != INT_MIN) {
				u_list_push_back(&destination_ptr, last_data);
				last_data = (node*)malloc(sizeof(node));
				if (!last_data) return MEMORY_ALLOCATE_ERROR;
			}
			*last_data = *n1;
		}
		temp_ptr = temp_ptr->next_node;
	}

	if (last_data->pow != INT_MIN) {
		u_list_push_back(&destination_ptr, last_data);
	}
	free(last_data);
	u_list_free(source_ptr);
	*source_ptr = destination_ptr;
	return 0;
}

int print_polynomial(u_list* list_ptr) {
	u_list_node* temp_ptr;
	node* N;
	int i = 0;

	temp_ptr = list_ptr->first_node;
	while (temp_ptr != NULL) {
		N = temp_ptr->data;
		if (i == 0) {
			if (N->pow == 1) {
				printf("%dx", N->ratio);
			}
			else if (N->pow == 0) {
				printf("%d", N->ratio);
			}
			else {
				printf("%dx^%d", N->ratio, N->pow);
			}
		}
		else {
			if (N->pow == 1) {
				printf("%+dx", N->ratio);
			}
			else if (N->pow == 0) {
				printf("%+d", N->ratio);
			}
			else {
				printf("%+dx^%d", N->ratio, N->pow);
			}
		}
		temp_ptr = temp_ptr->next_node;
		i++;
	}
	printf("\n");
	return 0;
}