//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <math.h>
//#include <ctype.h>
//#include "../../libs/error.h"
//#include "../../libs/u_list.h"
//#include "../../libs/cust_string.h"
//
//typedef struct node {
//	int ratio;
//	int pow;
//} node;
//
//int polynomial_add(u_list* list_ptr_1, u_list* list_ptr_2, u_list* result_ptr);
//int polynomial_sub(u_list* list_ptr_1, u_list* list_ptr_2, u_list* result_ptr);
//int polynomial_mult(u_list* list_ptr_1, u_list* list_ptr_2, u_list* result_ptr);
//int polynomial_div(u_list* list_ptr_1, u_list* list_ptr_2, u_list* result_ptr);
//int polynomial_eval(u_list* list_ptr, int value, int* result);
//int polynomial_diff(u_list* list_ptr_1, u_list* result_ptr);
//int polynomial_cmps(u_list* list_ptr_1, u_list* list_ptr_2, u_list* result_ptr);
//
//
//int standart_appearance(u_list* source_ptr);
//int print_polynomial(u_list* list_ptr);
//int convert_TO_decimal(char* n, int base, int* ans);
//
//int comparator_by_pow(u_list_node* a, u_list_node* b) {
//	node* n1, * n2;
//	n1 = a->data;
//	n2 = b->data;
//	return n2->pow - n1->pow;
//}
//
//int main() {
//	int i, comment_flag = 0, is_expression = 0, res = 0;
//	int polynomial_flag = 0, ratio_flag = 0, pow_flag = 0, line = 0, read_flag = 0, is_eval = 0;
//	FILE* data_file;
//	string temp_string, action_string, number_str;
//	temp_string = string_init();
//	char temp_str[BUFSIZ];
//	char actions[][10] = {"Add\0", "Sub\0", "Mult\0", "Div\0", "Mod\0", "Eval\0", "Diff\0", "Cmps\0"};
//	char* symbol;
//	u_list list_summator = u_list_init(NULL, sizeof(node));
//	node* _node = (node*)malloc(sizeof(node));
//	if (_node = NULL) {
//		string_free(temp_string);
//		return MEMORY_ALLOCATE_ERROR;
//	}
//	_node->pow = 0;
//	_node->ratio = 0;
//	u_list list_1 = u_list_init(NULL, sizeof(node));
//	u_list list_2 = u_list_init(NULL, sizeof(node));
//	
//	char last_char;
//	action_string = string_init();
//	number_str = string_init();
//	if ((data_file = fopen("task_6.txt", "r")) == NULL) {
//		return OPEN_FILE_ERROR;
//	}
//
//	while ((fgets(temp_str, BUFSIZ, data_file)) != NULL) {
//		temp_string = string_from(temp_str, strlen(temp_str));
//		read_flag = 1;
//		i = 0;
//		while (temp_string[i] != '\0') {
//			if (temp_string[i] == '%') {
//				break;
//			}
//			else if (temp_string[i] == '[' || comment_flag == 1) {
//				if (comment_flag == 1 && temp_string[i] == ']') {
//					comment_flag = 0;
//				}
//				else {
//					comment_flag = 1;
//				}
//				last_char = temp_string[i];
//				i++;
//				continue;
//			}
//
//			if (comment_flag == 0) {
//				if (temp_string[i] == ';' || temp_string[i] == ' ' || temp_string[i] == '\n') {
//					last_char = temp_string[i];
//					i++;
//					continue;
//				}
//				else if (temp_string[i] != '(' && temp_string[i] != ')' && polynomial_flag == 0) {
//					string_push_back(&action_string, temp_string[i]);
//				}
//				else if (temp_string[i] == '(') {
//					polynomial_flag = 1;
//					ratio_flag = 1;
//				}
//				else if (temp_string[i] == ')') {
//					if (string_size(number_str) != 0) {
//						if (ratio_flag == 1 && pow_flag == 0) {
//							_node->pow = 0;
//							convert_TO_decimal(number_str, 10, &(_node->ratio));
//							if (_node->ratio == 0) _node->ratio = 1;
//							string_free(number_str);
//							number_str = string_init();
//						}
//						else {
//							convert_TO_decimal(number_str, 10, &(_node->pow));
//							string_free(number_str);
//							number_str = string_init();
//						}
//						if (polynomial_flag == 1) {
//							u_list_push_back(&list_1, _node);
//						}
//						else {
//							u_list_push_back(&list_2, _node);
//						}
//						_node->ratio = 0;
//						_node->pow = 0;
//					}
//
//					if (polynomial_flag == 1) {
//						list_2 = list_summator;
//						//printf("*** Getting data from summator ***\n");
//					}
//
//					printf("Action: %s\n", action_string);
//					printf("polynomial 1: ");
//					print_polynomial(&list_1);
//					if (action_string[2] != 'f') {
//						printf("polynomial 2: ");
//						print_polynomial(&list_2);
//					}
//
//					if (string_flexographic_compare_char(action_string, &actions[0]) == 0) {
//						//Add
//						polynomial_add(&list_1, &list_2, &list_summator);
//					}
//					else if (string_flexographic_compare_char(action_string, &actions[1]) == 0) {
//						//Sub
//						polynomial_sub(&list_1, &list_2, &list_summator);
//					}
//					else if (string_flexographic_compare_char(action_string, &actions[2]) == 0) {
//						//Mult
//						polynomial_mult(&list_1, &list_2, &list_summator);
//					}
//					else if (string_flexographic_compare_char(action_string, &actions[3]) == 0) {
//						//Div
//						polynomial_div(&list_1, &list_2, &list_summator);
//					}
//					else if (string_flexographic_compare_char(action_string, &actions[4]) == 0) {
//						//Mod
//						polynomial_mod(&list_1, &list_2, &list_summator);
//					}
//					else if (string_flexographic_compare_char(action_string, &actions[5]) == 0) {
//						//Eval
//						is_eval = 1;
//						if (polynomial_flag == 1) {
//							_node = list_1.first_node->data;
//							polynomial_eval(&list_2, _node->ratio, &res);
//						}
//						else {
//							_node = list_2.first_node->data;
//							polynomial_eval(&list_1, _node->ratio, &res);
//						}
//						printf("Result: %d\n", res);
//					}
//					else if (string_flexographic_compare_char(action_string, &actions[6]) == 0) {
//						//Diff
//						if (polynomial_flag == 0) {
//							polynomial_diff(&list_summator, &list_summator);
//						}
//						else {
//							polynomial_diff(&list_1, &list_summator);
//						}
//					}
//					else if (string_flexographic_compare_char(action_string, &actions[7]) == 0) {
//						//Cmps
//						polynomial_cmps(&list_1, &list_2, &list_summator);
//					}
//					else {
//						printf("WARNING: Unknown command in line: %d.\nThe program continues to run without executing this command.\n", line);
//					}
//
//					if (is_eval != 1) {
//						printf("Result: ");
//						print_polynomial(&list_summator);
//					}
//					printf("--------------------------------- \n");
//
//					if (list_1.first_node != NULL) u_list_free(&list_1);
//					if (action_string[2] != 'f') u_list_free(&list_2);
//
//					list_1 = u_list_init(NULL, sizeof(node));
//					list_2 = u_list_init(NULL, sizeof(node));
//					string_free(action_string);
//					action_string = string_init();
//					polynomial_flag = 0;
//					is_eval = 0;
//					res = 0;
//					_node->ratio = 0;
//					_node->pow = 0;
//				}
//				else if (temp_string[i] == ',') {
//					if (string_size(number_str) != 0) {
//						if (ratio_flag == 1 && pow_flag == 0) {
//							_node->pow = 0;
//							convert_TO_decimal(number_str, 10, &(_node->ratio));
//							if (_node->ratio == 0) _node->ratio = 1;
//							string_free(number_str);
//							number_str = string_init();
//						}
//						else {
//							convert_TO_decimal(number_str, 10, &(_node->pow));
//							string_free(number_str);
//							number_str = string_init();
//						}
//						u_list_push_back(&list_1, _node);
//						_node->ratio = 0;
//						_node->pow = 0;
//					}
//					polynomial_flag = 2;
//				}
//				else {
//					if (temp_string[i] == 'x' || ((temp_string[i] == '+' || temp_string[i] == '-') && pow_flag == 0 && last_char != ',')) {
//						if (last_char == '+' || last_char == '-') {
//							string_push_back(&number_str, '1');
//						}
//						convert_TO_decimal(number_str, 10, &(_node->ratio));
//						if (_node->ratio == 0) _node->ratio = 1;
//						ratio_flag = 0;
//						pow_flag = 1;
//						string_free(number_str);
//						number_str = string_init();
//
//						if (temp_string[i] == '+' || temp_string[i] == '-') {
//							_node->pow = 0;
//						}
//					}
//					else if (temp_string[i] == '^') {
//						last_char = temp_string[i];
//						i++;
//						continue;
//					}
//					else if (ratio_flag == 1) {
//						string_push_back(&number_str, temp_string[i]);
//					}
//					else if (pow_flag == 1) {
//						if (temp_string[i] == '-' || temp_string[i] == '+') {
//							if (last_char == 'x') {
//								_node->pow = 1;
//							}
//							convert_TO_decimal(number_str, 10, &(_node->pow));
//							string_free(number_str);
//							number_str = string_init();
//							ratio_flag = 1;
//							pow_flag = 0;
//
//							if (temp_string[i] == '-') {
//								string_push_back(&number_str, temp_string[i]);
//							}
//
//							if (polynomial_flag == 1) {
//								u_list_push_back(&list_1, _node);
//							}
//							else if (polynomial_flag == 2) {
//								u_list_push_back(&list_2, _node);
//							}
//							_node->ratio = 0;
//							_node->pow = 0;
//						}
//						else {
//							string_push_back(&number_str, temp_string[i]);
//						}
//					}
//				}
//				last_char = temp_string[i];
//			}
//			i++;
//		}
//		line++;
//	}
//	free(_node);
//	if (read_flag == 0) {
//		printf("File is empty. Programm stopped.");
//		return EMPTY_FILE;
//	}
//	else {
//		string_free(temp_string);
//	}
//	fclose(data_file);
//	u_list_free(&list_1);
//	u_list_free(&list_2);
//	u_list_free(&list_summator);
//	string_free(action_string);
//	string_free(number_str);
//	return 0;
//}
//
//int polynomial_add(u_list* list_ptr_1, u_list* list_ptr_2, u_list* result_ptr) {
//	u_list_node* temp_ptr_1, *temp_ptr_2;
//	u_list temp_list = u_list_init(NULL, sizeof(node));
//	node* n1 = NULL, * n2 = NULL, * temp_node;
//	int flag_1 = 0, flag_2 = 0;
//
//	if (list_ptr_1 == NULL || list_ptr_2 == NULL || result_ptr == NULL) {
//		return NULL_POINTER;
//	}
//
//	standart_appearance(list_ptr_1);
//	standart_appearance(list_ptr_2);
//
//	temp_node = (node*)malloc(sizeof(node));
//	if (temp_node == NULL) {
//		return MEMORY_ALLOCATE_ERROR;
//	}
//
//	temp_ptr_1 = list_ptr_1->first_node;
//	temp_ptr_2 = list_ptr_2->first_node;
//	while (temp_ptr_1 != NULL || temp_ptr_2 != NULL) {
//		if (temp_ptr_1 != NULL) n1 = temp_ptr_1->data;
//		if (temp_ptr_2 != NULL) n2 = temp_ptr_2->data;
//
//		if (n1->pow == n2->pow) {
//			if (n1->ratio + n2->ratio != 0) {
//				temp_node->pow = n1->pow;
//				temp_node->ratio = n1->ratio + n2->ratio;
//				u_list_push_back(&temp_list, temp_node);
//			}
//			flag_1 = 1;
//			flag_2 = 1;
//		}
//		else {
//			if (n1->pow > n2->pow) {
//				u_list_push_back(&temp_list, n1);
//				flag_1 = 1;
//			}
//			else {
//				u_list_push_back(&temp_list, n2);
//				flag_2 = 1;
//			}
//		}
//
//		if (temp_ptr_1 != NULL && flag_1) temp_ptr_1 = temp_ptr_1->next_node;
//		if (temp_ptr_2 != NULL && flag_2) temp_ptr_2 = temp_ptr_2->next_node;
//		flag_1 = 0; flag_2 = 0;
//	}
//	if (temp_list.first_node == NULL) {
//		temp_node->pow = 0;
//		temp_node->ratio = 0;
//		u_list_push_back(&temp_list, temp_node);
//	}
//	*result_ptr = temp_list;
//	free(temp_node);
//	return 0;
//}
//
//int polynomial_sub(u_list* list_ptr_1, u_list* list_ptr_2, u_list* result_ptr) {
//	u_list_node* temp_ptr_1, * temp_ptr_2;
//	u_list temp_list = u_list_init(NULL, sizeof(node));
//	node* n1 = NULL, * n2 = NULL, * temp_node;
//	int flag_1 = 0, flag_2 = 0;
//
//	if (list_ptr_1 == NULL || list_ptr_2 == NULL || result_ptr == NULL) {
//		return NULL_POINTER;
//	}
//
//	temp_node = (node*)malloc(sizeof(node));
//	if (temp_node == NULL) {
//		return MEMORY_ALLOCATE_ERROR;
//	}
//
//	temp_ptr_1 = list_ptr_1->first_node;
//	temp_ptr_2 = list_ptr_2->first_node;
//	while (temp_ptr_1 != NULL || temp_ptr_2 != NULL) {
//		if (temp_ptr_1 != NULL) {
//			n1 = temp_ptr_1->data;
//		}
//		else {
//			temp_node->pow = 0;
//			temp_node->ratio = 0;
//			n1 = temp_node;
//		}
//		if (temp_ptr_2 != NULL) {
//			n2 = temp_ptr_2->data;
//		}
//		else {
//			temp_node->pow = 0;
//			temp_node->ratio = 0;
//			n2 = temp_node;
//		}
//
//		if (n1->pow == n2->pow) {
//			if (n1->ratio - n2->ratio != 0) {
//				temp_node->pow = n1->pow;
//				temp_node->ratio = n1->ratio - n2->ratio;
//				u_list_push_back(&temp_list, temp_node);
//			}
//			flag_1 = 1;
//			flag_2 = 1;
//		}
//		else {
//			if (n1->pow > n2->pow) {
//				u_list_push_back(&temp_list, n1);
//				flag_1 = 1;
//			}
//			else {
//				u_list_push_back(&temp_list, n2);
//				flag_2 = 1;
//			}
//		}
//
//		if (temp_ptr_1 != NULL && flag_1) temp_ptr_1 = temp_ptr_1->next_node;
//		if (temp_ptr_2 != NULL && flag_2) temp_ptr_2 = temp_ptr_2->next_node;
//		flag_1 = 0; flag_2 = 0;
//	}
//
//	if (temp_list.first_node == NULL) {
//		temp_node->pow = 0;
//		temp_node->ratio = 0;
//		u_list_push_back(&temp_list, temp_node);
//	}
//	*result_ptr = temp_list;
//	free(temp_node);
//	return 0;
//}
//
//int polynomial_mult(u_list* list_ptr_1, u_list* list_ptr_2, u_list* result_ptr) {
//	u_list_node* temp_ptr_1, * temp_ptr_2;
//	u_list temp_list = u_list_init(NULL, sizeof(node));
//	node* n1 = NULL, * n2 = NULL, * temp_node;
//
//	if (list_ptr_1 == NULL || list_ptr_2 == NULL || result_ptr == NULL) {
//		return NULL_POINTER;
//	}
//
//	standart_appearance(list_ptr_1);
//	standart_appearance(list_ptr_2);
//
//	temp_node = (node*)malloc(sizeof(node));
//	if (temp_node == NULL) {
//		return MEMORY_ALLOCATE_ERROR;
//	}
//
//	temp_ptr_1 = list_ptr_1->first_node;
//	while (temp_ptr_1 != NULL) {
//		n1 = temp_ptr_1->data;
//
//		temp_ptr_2 = list_ptr_2->first_node;
//		while (temp_ptr_2 != NULL) {
//			n2 = temp_ptr_2->data;
//
//			temp_node->pow = n1->pow + n2->pow;
//			temp_node->ratio = n1->ratio * n2->ratio;
//			u_list_push_back(&temp_list, temp_node);
//
//			temp_ptr_2 = temp_ptr_2->next_node;
//		}
//		temp_ptr_1 = temp_ptr_1->next_node;
//	}
//	standart_appearance(&temp_list);
//	*result_ptr = temp_list;
//	free(temp_node);
//	return 0;
//}
//
//int polynomial_div(u_list* list_ptr_1, u_list* list_ptr_2, u_list* result_ptr) {
//	u_list_node* temp_ptr_1, * temp_ptr_2;
//	u_list temp_list_result = u_list_init(NULL, sizeof(node));
//	u_list temp_list_multiplier = u_list_init(NULL, sizeof(node));
//	node* n1 = NULL, * n2 = NULL, * temp_node;
//
//	if (list_ptr_1 == NULL || list_ptr_2 == NULL || result_ptr == NULL) {
//		return NULL_POINTER;
//	}
//
//	standart_appearance(list_ptr_1);
//	standart_appearance(list_ptr_2);
//
//	temp_node = (node*)malloc(sizeof(node));
//	if (temp_node == NULL) {
//		return MEMORY_ALLOCATE_ERROR;
//	}
//	
//	temp_ptr_1 = list_ptr_1->first_node;
//	temp_ptr_2 = list_ptr_2->first_node;
//	while (temp_ptr_1 != NULL) {
//		n1 = temp_ptr_1->data;
//		n2 = temp_ptr_2->data;
//
//		if (n1->pow == 0 && n1->ratio == 0) {
//			break;
//		}
//		if (n1->pow < n2->pow) {
//			free(temp_node);
//			return BAD_DIVISION;
//		}
//
//		temp_node->pow = n1->pow - n2->pow;
//		temp_node->ratio = n1->ratio / n2->ratio;
//
//		u_list_push_back(&temp_list_result, temp_node);
//		u_list_push_back(&temp_list_multiplier, temp_node);
//
//		polynomial_mult(&temp_list_multiplier, list_ptr_2, &temp_list_multiplier);
//		polynomial_sub(list_ptr_1, &temp_list_multiplier, list_ptr_1);
//
//		temp_ptr_1 = list_ptr_1->first_node;
//		temp_ptr_2 = list_ptr_2->first_node;
//		u_list_free(&temp_list_multiplier);
//	}
//	*result_ptr = temp_list_result;
//	free(temp_node);
//	return 0;
//}
//
//int polynomial_mod(u_list* list_ptr_1, u_list* list_ptr_2, u_list* result_ptr) {
//	u_list_node* temp_ptr_1, * temp_ptr_2;
//	u_list temp_list_result = u_list_init(NULL, sizeof(node));
//	u_list temp_list_multiplier = u_list_init(NULL, sizeof(node));
//	node* n1 = NULL, * n2 = NULL, * temp_node;
//
//	if (list_ptr_1 == NULL || list_ptr_2 == NULL || result_ptr == NULL) {
//		return NULL_POINTER;
//	}
//
//	standart_appearance(list_ptr_1);
//	standart_appearance(list_ptr_2);
//
//	temp_node = (node*)malloc(sizeof(node));
//	if (temp_node == NULL) {
//		return MEMORY_ALLOCATE_ERROR;
//	}
//
//	temp_ptr_1 = list_ptr_1->first_node;
//	temp_ptr_2 = list_ptr_2->first_node;
//	while (temp_ptr_1 != NULL) {
//		n1 = temp_ptr_1->data;
//		n2 = temp_ptr_2->data;
//
//		if (n1->pow == 0 && n1->ratio == 0) {
//			break;
//		}
//		if (n1->pow < n2->pow) {
//			u_list_free(result_ptr);
//			*result_ptr = u_list_init(NULL, sizeof(node));
//			temp_ptr_1 = list_ptr_1->first_node;
//			while (temp_ptr_1 != NULL) {
//				u_list_push_back(result_ptr, temp_ptr_1);
//				temp_ptr_1 = temp_ptr_1->next_node;
//			}
//			free(temp_node);
//			return 0;
//		}
//
//		temp_node->pow = n1->pow - n2->pow;
//		temp_node->ratio = n1->ratio / n2->ratio;
//
//		u_list_push_back(&temp_list_multiplier, temp_node);
//
//		polynomial_mult(&temp_list_multiplier, list_ptr_2, &temp_list_multiplier);
//		polynomial_sub(list_ptr_1, &temp_list_multiplier, list_ptr_1);
//
//		temp_ptr_1 = list_ptr_1->first_node;
//		temp_ptr_2 = list_ptr_2->first_node;
//		u_list_free(&temp_list_multiplier);
//	}
//	u_list_free(result_ptr);
//	*result_ptr = u_list_init(NULL, sizeof(node));
//	temp_ptr_1 = list_ptr_1->first_node;
//	while (temp_ptr_1 != NULL) {
//		u_list_push_back(result_ptr, temp_ptr_1);
//		temp_ptr_1 = temp_ptr_1->next_node;
//	}
//	free(temp_node);
//	return 0;
//}
//
//int polynomial_eval(u_list* list_ptr, int value, int* result) {
//	u_list_node* temp_ptr_1;
//	node* n1 = NULL;
//
//	if (list_ptr == NULL || result == NULL) {
//		return NULL_POINTER;
//	}
//
//	temp_ptr_1 = list_ptr->first_node;
//	while (temp_ptr_1 != NULL ) {
//		n1 = temp_ptr_1->data;
//		(*result) += (pow(value, n1->pow) * n1->ratio);
//		temp_ptr_1 = temp_ptr_1->next_node;
//	}
//	return 0;
//}
//
//int polynomial_diff(u_list* list_ptr_1, u_list* result_ptr) {
//	u_list_node* temp_ptr_1;
//	u_list temp_list = u_list_init(NULL, sizeof(node));
//	node* n1 = NULL, * n2 = NULL, * temp_node;
//
//	if (list_ptr_1 == NULL || result_ptr == NULL) {
//		return NULL_POINTER;
//	}
//
//	standart_appearance(list_ptr_1);
//
//	temp_node = (node*)malloc(sizeof(node));
//	if (temp_node == NULL) {
//		return MEMORY_ALLOCATE_ERROR;
//	}
//
//	temp_ptr_1 = list_ptr_1->first_node;
//	while (temp_ptr_1 != NULL) {
//		n1 = temp_ptr_1->data;
//
//		if (n1->pow == 1) {
//			temp_node->pow = 0;
//			temp_node->ratio = n1->ratio;
//		}
//		else {
//			temp_node->pow = n1->pow - 1;
//			temp_node->ratio = n1->pow * n1->ratio;
//		}
//
//		if (n1->pow != 0) u_list_push_back(&temp_list, temp_node);
//
//		temp_ptr_1 = temp_ptr_1->next_node;
//	}
//	
//	*result_ptr = temp_list;
//	free(temp_node);
//	return 0;
//}
//
//int polynomial_cmps(u_list* list_ptr_1, u_list* list_ptr_2, u_list* result_ptr) {
//	u_list_node* temp_ptr_1, * temp_ptr_2;
//	u_list temp_list = u_list_init(NULL, sizeof(node));
//	u_list temp_list_result = u_list_init(NULL, sizeof(node));
//	u_list temp_list_multiplier = u_list_init(NULL, sizeof(node));
//	node* n1 = NULL, * n2 = NULL, * temp_node;
//	int i = 0;
//
//	if (list_ptr_1 == NULL || list_ptr_2 == NULL || result_ptr == NULL) {
//		return NULL_POINTER;
//	}
//
//	standart_appearance(list_ptr_1);
//	standart_appearance(list_ptr_2);
//
//	temp_node = (node*)malloc(sizeof(node));
//	if (temp_node == NULL) {
//		return MEMORY_ALLOCATE_ERROR;
//	}
//
//	temp_ptr_1 = list_ptr_1->first_node;
//	while (temp_ptr_1 != NULL) {
//		temp_list_multiplier = u_list_init(NULL, sizeof(node));
//		temp_list = u_list_init(NULL, sizeof(node));
//		n1 = temp_ptr_1->data;
//		if (n1->pow > 1) {
//			for (i = 0; i < (n1->pow - 1); i++) {
//				if (i == 0) {
//					polynomial_mult(list_ptr_2, list_ptr_2, &temp_list_multiplier);
//				}
//				else {
//					polynomial_mult(&temp_list_multiplier, list_ptr_2, &temp_list_multiplier);
//				}
//			}
//			temp_node->pow = 0;
//			temp_node->ratio = n1->ratio;
//			u_list_push_back(&temp_list, temp_node);
//			polynomial_mult(&temp_list_multiplier, &temp_list, &temp_list_multiplier);
//		}
//		else if (n1->pow == 1) {
//			temp_node->pow = 0;
//			temp_node->ratio = n1->ratio;
//			u_list_push_back(&temp_list, temp_node);
//			polynomial_mult(list_ptr_2, &temp_list, &temp_list_multiplier);
//		}
//		else {
//			u_list_push_back(&temp_list_multiplier, n1);
//		}
//
//		temp_ptr_2 = temp_list_multiplier.first_node;
//		while (temp_ptr_2 != NULL) {
//			n2 = temp_ptr_2->data;
//			u_list_push_back(&temp_list_result, n2);
//			temp_ptr_2 = temp_ptr_2->next_node;
//		}
//		temp_ptr_1 = temp_ptr_1->next_node;
//		u_list_free(&temp_list_multiplier);
//		u_list_free(&temp_list);
//	}
//	standart_appearance(&temp_list_result);
//	*result_ptr = temp_list_result;
//	free(temp_node);
//	return 0;
//}
//
//
//int standart_appearance(u_list* source_ptr) {
//	int flag = 0;
//	u_list_node* temp_ptr, * last_ptr = NULL;
//	node* n1, * last_data;
//	u_list destination_ptr = u_list_init(NULL, sizeof(node));
//
//	last_data = (node*)malloc(sizeof(node));
//	if (!last_data) {
//		return MEMORY_ALLOCATE_ERROR;
//	}
//	last_data->pow = INT_MIN;
//	last_data->ratio = 0;    
//
//	u_list_mergeSort(source_ptr, comparator_by_pow);
//
//	temp_ptr = source_ptr->first_node;
//	while (temp_ptr != NULL) {
//		n1 = temp_ptr->data;
//
//		if (last_data->pow == n1->pow) {
//			last_data->ratio += n1->ratio;
//		}
//		else {
//			if (last_data->pow != INT_MIN) {
//				u_list_push_back(&destination_ptr, last_data);
//				last_data = (node*)malloc(sizeof(node));
//				if (!last_data) return MEMORY_ALLOCATE_ERROR;
//			}
//			*last_data = *n1;
//		}
//		temp_ptr = temp_ptr->next_node;
//	}
//
//	if (last_data->pow != INT_MIN) {
//		u_list_push_back(&destination_ptr, last_data);
//	}
//	free(last_data);
//	u_list_free(source_ptr);
//	*source_ptr = destination_ptr;
//	return 0;
//}
//
//int print_polynomial(u_list* list_ptr) {
//	u_list_node* temp_ptr;
//	node* N;
//	int i = 0;
//
//	if (list_ptr == NULL) {
//		return NULL_POINTER;
//	}
//
//	temp_ptr = list_ptr->first_node;
//	while (temp_ptr != NULL) {
//		N = temp_ptr->data;
//		if (N->ratio == 1) {
//			if (i == 0) {
//				if (N->pow == 1) {
//					printf("x");
//				}
//				else if (N->pow == 0) {
//					printf("");
//				}
//				else {
//					printf("x^%d", N->pow);
//				}
//			}
//			else {
//				if (N->pow == 1) {
//					printf("+x");
//				}
//				else if (N->pow == 0) {
//					printf("+1");
//				}
//				else {
//					printf("+x^%d", N->pow);
//				}
//			}
//		}
//		else if (N->ratio == -1) {
//			if (N->pow == 1) {
//				printf("-x");
//			}
//			else if (N->pow == 0) {
//				printf("-1");
//			}
//			else {
//				printf("-x^%d", N->pow);
//			}
//		}
//		else {
//			if (i == 0) {
//				if (N->pow == 1) {
//					printf("%dx", N->ratio);
//				}
//				else if (N->pow == 0) {
//					printf("%d", N->ratio);
//				}
//				else {
//					printf("%dx^%d", N->ratio, N->pow);
//				}
//			}
//			else {
//				if (N->pow == 1) {
//					printf("%+dx", N->ratio);
//				}
//				else if (N->pow == 0) {
//					printf("%+d", N->ratio);
//				}
//				else {
//					printf("%+dx^%d", N->ratio, N->pow);
//				}
//			}
//		}
//		temp_ptr = temp_ptr->next_node;
//		i++;
//	}
//	printf("\n");
//	return 0;
//}
//
//int convert_TO_decimal(char* n, int base, int* ans) {
//	int len = 0, j = 0, sign = 0;
//
//	if (n == NULL) {
//		return NULL_POINTER;
//	}
//
//	if (base < 2 || base > 36) {
//		return INCORRECT_BASE;
//	}
//	if (n[0] == '0') {
//		*ans = 0;
//		return 0;
//	}
//
//	len = strlen(n);
//
//	if (n[0] == '-') { sign = 1; }
//
//	for (int i = sign; i < len; i++) {
//		if (isdigit(n[i])) {
//			(*ans) += (int)((int)n[i] - '0') * pow((double)base, len - i - 1);
//		}
//		else if (isalpha(n[i])) {
//			(*ans) += (int)((int)toupper(n[i]) - 'A' + 10) * pow((double)base, len - i - 1);
//		}
//	}
//	if (sign == 1) { (*ans) *= -1; }
//	return 0;
//}