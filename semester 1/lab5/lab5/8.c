//#define _CRT_SECURE_NO_WARNINGS
//
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <math.h>
//#include <ctype.h>
//#include "../../libs/error.h"
//#include "../../libs/u_list.h"
//#include "../../libs/cust_string.h"
//
////task_8.txt /trace trace.txt
//
//typedef struct variable {
//	char name;
//	int value;
//} variable;
//
//int convert_TO_decimal(char* n, int base, int* ans);
//int convert_FROM_decimal(int n, int base, char** ans);
//
//int print_list(u_list_node* node) {
//	if (node == NULL) {
//		return NULL_POINTER;
//	}
//	variable* temp_variable = node->data;
//	printf("Variable: %c, value: %d\n", temp_variable->name, temp_variable->value);
//}
//
//int logs(char* str, int mode, FILE* file) {
//	if (str == NULL || file == NULL) {
//		return NULL_POINTER;
//	}
//	if (mode == 1) {
//		printf("%s", str);
//		fprintf(file, "%s", str);
//	}
//	else {
//		printf("%s", str);
//	}
//}
//
//int main(int argc, char* argv[]) {
//	int i, k, comment_flag = 0, MODE = 0;
//	int line = 0, read_flag = 0, rw_flag = 0, ready_flag = 0, base = 0, is_there_flag = 0, value = 0, error_flag = 0;
//	FILE* data_file;
//	FILE* trace_file = NULL;
//	string temp_string, action_string, number_str;
//	temp_string = string_init();
//	action_string = string_init();
//	number_str = string_init();
//	char temp_str[BUFSIZ];
//	char temp_str_2[BUFSIZ];
//	char print_str[BUFSIZ];
//	char variable_1, variable_2 = '0', variable_3;
//	char* ans;
//	variable temp_variable;
//	variable* temp_variable_ptr;
//	variable* variable_ptr_1, * variable_ptr_2, * variable_ptr_3;
//	char action[][10] = {"read\0", "write\0", "+\0", "&\0", "->\0", "<-\0", "~\0", "<>\0", "+>\0", "?\0", "!\0", "/trace\0"};
//	char last_char;
//	u_list variables_list = u_list_init(NULL, sizeof(variable));
//	u_list_node* temp_ptr, * temp_ptr_1, * temp_ptr_2;
//
//	if ((data_file = fopen("task_8.txt", "r")) == NULL) {
//		string_free(&action_string);
//		string_free(&number_str);
//		u_list_free(&variables_list);
//		return OPEN_FILE_ERROR;
//	}
//
//	if (argc > 1) {
//		if (strcmp(action[11], argv[2]) == 0) {
//			if ((trace_file = fopen(argv[3], "w")) == NULL) {
//				printf("Open trace file error =(\n");
//				string_free(&action_string);
//				string_free(&number_str);
//				u_list_free(&variables_list);
//				fclose(data_file);
//				return OPEN_FILE_ERROR;
//			}
//			MODE = 1;
//		}
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
//			else if (temp_string[i] == '{' || comment_flag == 1) {
//				if (comment_flag == 1 && temp_string[i] == '}') {
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
//				if ((temp_string[i] == ';' || temp_string[i] == ' ' || temp_string[i] == '\n' || temp_string[i] == '=') && ready_flag == 0) {
//					last_char = temp_string[i];
//					i++;
//					continue;
//				}
//				else if (temp_string[i] != '(' && temp_string[i] != ')' && 
//					temp_string[i] != ':' && temp_string[i] != '=' && 
//					rw_flag == 0) 
//				{
//					string_push_back(&action_string, temp_string[i]);
//				}
//				else if (temp_string[i] == '(') {
//					rw_flag = 1;
//				}
//				else if (temp_string[i] == ':') {
//					variable_1 = action_string[0];
//					string_free(&action_string);
//					action_string = string_init();
//					rw_flag = -1;
//				}
//				else if (temp_string[i] == ')' || ready_flag != 0) {
//					if (ready_flag == 1) {
//						sprintf(print_str, "Action: %c := %c %s %c\n", variable_1, variable_2, action_string, variable_3);
//						logs(print_str, MODE, trace_file);
//
//						error_flag = 0;
//						variable_ptr_1 = NULL; variable_ptr_2 = NULL; variable_ptr_3 = NULL;
//						// find second and third variables
//						temp_ptr = variables_list.first_node;
//						while (temp_ptr != NULL && is_there_flag != 2) {
//							temp_variable_ptr = temp_ptr->data;
//
//							if (temp_variable_ptr->name == variable_2) {
//								variable_ptr_2 = temp_variable_ptr;
//								is_there_flag += 1;
//							}
//							if (temp_variable_ptr->name == variable_3) {
//								variable_ptr_3 = temp_variable_ptr;
//								is_there_flag += 1;
//							}
//							temp_ptr = temp_ptr->next_node;
//						}
//						if (is_there_flag != 2) {
//							if (variable_ptr_2 == NULL) {
//								sprintf(print_str, "Line: %d - The variable %c does not exist. First, create a variable.\n", line, variable_2);
//								logs(print_str, MODE, trace_file);
//							}
//							if (variable_ptr_3 == NULL) {
//								sprintf(print_str, "Line: %d - The variable %c does not exist. First, create a variable.\n", line, variable_3);
//								logs(print_str, MODE, trace_file);
//							}
//							string_free(&action_string);
//							action_string = string_init();
//							rw_flag = 0;
//							ready_flag = 0;
//							sprintf(print_str, "\n");
//							logs(print_str, MODE, trace_file);
//							last_char = temp_string[i];
//							i++;
//							continue;
//						}
//
//						// find first variable, if it not there - made it
//						temp_ptr = variables_list.first_node;
//						while (temp_ptr != NULL && is_there_flag != 1) {
//							temp_variable_ptr = temp_ptr->data;
//
//							if (temp_variable_ptr->name == variable_1) {
//								variable_ptr_1 = temp_variable_ptr;
//								is_there_flag = 1;
//								break;
//							}
//							temp_ptr = temp_ptr->next_node;
//						}
//						if (!is_there_flag) {
//							variable_ptr_1 = &temp_variable;
//						}
//
//						if (string_flexographic_compare_char(action_string, &action[2]) == 0) {
//							variable_ptr_1->value = variable_ptr_2->value | variable_ptr_3->value;
//						}
//						else if (string_flexographic_compare_char(action_string, &action[3]) == 0) {
//							variable_ptr_1->value = variable_ptr_2->value & variable_ptr_3->value;
//						}
//						else if (string_flexographic_compare_char(action_string, &action[4]) == 0) {
//							variable_ptr_1->value = (~variable_ptr_2->value) | variable_ptr_3->value;
//						}
//						else if (string_flexographic_compare_char(action_string, &action[5]) == 0) {
//							variable_ptr_1->value = (~variable_ptr_3->value) | variable_ptr_2->value;
//						}
//						else if (string_flexographic_compare_char(action_string, &action[6]) == 0) {
//							variable_ptr_1->value = ~(variable_ptr_2->value ^ variable_ptr_3->value);
//						}
//						else if (string_flexographic_compare_char(action_string, &action[7]) == 0) {
//							variable_ptr_1->value = variable_ptr_2->value ^ variable_ptr_3->value;
//						}
//						else if (string_flexographic_compare_char(action_string, &action[8]) == 0) {
//							variable_ptr_1->value = ~(variable_ptr_2->value & variable_ptr_3->value);
//						}
//						else if (string_flexographic_compare_char(action_string, &action[9]) == 0) {
//							variable_ptr_1->value = ~(variable_ptr_2->value & variable_ptr_3->value);
//						}
//						else if (string_flexographic_compare_char(action_string, &action[10]) == 0) {
//							variable_ptr_1->value = ~(variable_ptr_2->value | variable_ptr_3->value);
//						}
//						else {
//							sprintf(print_str, "Line %d - Unknown operator (%s)", line, action_string);
//							logs(print_str, MODE, trace_file);
//							error_flag = 1;
//						}
//
//						if (!is_there_flag && error_flag == 0) {
//							u_list_push_back(&variables_list, variable_ptr_1);
//						}
//					}
//					else if (ready_flag == 2) {
//						sprintf(print_str, "Action: %c := \\%c\n", variable_1, variable_2);
//						logs(print_str, MODE, trace_file);
//						temp_ptr_2 = variables_list.first_node;
//						while (temp_ptr_2 != NULL && is_there_flag != 1) {
//							temp_variable_ptr = temp_ptr_2->data;
//
//							if (temp_variable_ptr->name == variable_2) {
//								value = temp_variable_ptr->value;
//								is_there_flag = 1;
//							}
//							temp_ptr_2 = temp_ptr_2->next_node;
//						}
//						if (!is_there_flag) {
//							sprintf(print_str, "There are not variable %c. It will be made with value 0.\n", variable_2);
//							logs(print_str, MODE, trace_file);
//							temp_variable.name = variable_2;
//							temp_variable.value = 0;
//							value = 0;
//							u_list_push_back(&variables_list, (void *)& temp_variable);
//						}
//
//						is_there_flag = 0;
//						temp_ptr_1 = variables_list.first_node;
//						while (temp_ptr_1 != NULL && is_there_flag != 1) {
//							temp_variable_ptr = temp_ptr_1->data;
//
//							if (temp_variable_ptr->name == variable_1) {
//								temp_variable_ptr->value = ~value;
//								is_there_flag = 1;
//							}
//							temp_ptr_1 = temp_ptr_1->next_node;
//						}
//						if (!is_there_flag) {
//							temp_variable.name = variable_1;
//							temp_variable.value = ~value;
//							u_list_push_back(&variables_list, (void *)& temp_variable);
//						}
//					}
//					else {
//						temp_variable.name = '0';
//						temp_variable.value = 0;
//						sprintf(print_str, "Action: %s\n", action_string);
//						logs(print_str, MODE, trace_file);
//						convert_TO_decimal(number_str, 10, &base);
//						sprintf(print_str, "var: %c, base: %d\n", variable_1, base);
//						logs(print_str, MODE, trace_file);
//
//						if (string_flexographic_compare_char(action_string, &action[0]) == 0) {
//							//read
//							is_there_flag = 0;
//							value = 0;
//
//							sprintf(print_str, "Enter value: ");
//							logs(print_str, MODE, trace_file);
//							scanf("%s", temp_str_2);
//							convert_TO_decimal(&temp_str_2, base, &value);
//							if (MODE == 1) fprintf(trace_file, "%d\n", value);
//
//							temp_ptr = variables_list.first_node;
//							while (temp_ptr != NULL && is_there_flag != 1) {
//								temp_variable_ptr = temp_ptr->data;
//
//								if (temp_variable_ptr->name == variable_1) {
//									temp_variable_ptr->value = value;
//									is_there_flag = 1;
//								}
//								temp_ptr = temp_ptr->next_node;
//							}
//							if (!is_there_flag) {
//								temp_variable.name = variable_1;
//								temp_variable.value = value;
//								u_list_push_back(&variables_list, (void *)& temp_variable);
//							}
//						}
//						else if (string_flexographic_compare_char(action_string, &action[1]) == 0) {
//							//write
//							temp_ptr = variables_list.first_node;
//							while (temp_ptr != NULL) {
//								temp_variable_ptr = temp_ptr->data;
//
//								if (temp_variable_ptr->name == variable_1) {
//									convert_FROM_decimal(temp_variable_ptr->value, base, &ans);
//									sprintf(print_str, "Variable: %c, value: %s\n", variable_1, ans);
//									logs(print_str, MODE, trace_file);
//									free(ans);
//									break;
//								}
//								temp_ptr = temp_ptr->next_node;
//							}
//						}
//						else {
//							sprintf(print_str, "Unknown action");
//							logs(print_str, MODE, trace_file);
//						}
//						string_free(&number_str);
//						number_str = string_init();
//						base = 0;
//					}
//
//					string_free(&action_string);
//					action_string = string_init();
//					rw_flag = 0;
//					ready_flag = 0;
//					sprintf(print_str, "\n");
//					logs(print_str, MODE, trace_file);
//				}
//				else if (temp_string[i] == ',') {
//					variable_1 = last_char;
//					rw_flag = 2;
//				}
//				else if (temp_string[i] == '\\') {
//					string_push_back(&action_string, temp_string[i]);
//					rw_flag = -1;
//				}
//				else {
//					if (rw_flag == -1) {
//						variable_2 = temp_string[i];
//						rw_flag = -2;
//						if (action_string[0] == '\\') {
//							ready_flag = 2;
//						}
//					}
//					else if (rw_flag == -2 && ready_flag != 2) {
//						string_push_back(&action_string, temp_string[i]);
//						rw_flag = -3;
//					}
//					else if (rw_flag == -3) {
//						variable_3 = temp_string[i];
//						ready_flag = 1;
//					}
//					else if (rw_flag == 2) {
//						string_push_back(&number_str, temp_string[i]);
//					}
//				}
//				last_char = temp_string[i];
//			}
//			i++;
//		}
//		string_free(&temp_string);
//		line++;
//	}
//	if (read_flag == 0) {
//		sprintf(print_str, "File is empty. Programm stopped.");
//		logs(print_str, MODE, trace_file);
//		return EMPTY_FILE;
//	}
//	if (trace_file != NULL) {
//		fclose(trace_file);
//	}
//	fclose(data_file);
//	string_free(&action_string);
//	string_free(&number_str);
//	u_list_free(&variables_list);
//
//	_CrtDumpMemoryLeaks();
//
//	return OK;
//}
//
//int convert_TO_decimal(char* n, int base, int* ans) {
//	int len = 0, j = 0, sign = 0;
//
//	if (n == NULL || ans == NULL) {
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
//
//int convert_FROM_decimal(int n, int base, char** ans) {
//	char* res;
//	int len = 0, j = 0, sign = 0;
//
//	if (ans == NULL) {
//		return NULL_POINTER;
//	}
//	if (base < 2 || base > 36) {
//		return INCORRECT_BASE;
//	}
//	if (n == 0) {
//		*ans = (char*)malloc(2 * sizeof(char));
//		if (*ans == NULL) { return MEMORY_ALLOCATE_ERROR; }
//		(*ans)[0] = '0';
//		(*ans)[1] = '\0';
//		return 0;
//	}
//
//	len = (log(abs(n)) / log(base)) + 1;
//
//	if (n < 0) { sign = 1; len++; n = -n; }
//
//	*ans = (char*)malloc((len + 1) * sizeof(char));
//	if (*ans == NULL) { return MEMORY_ALLOCATE_ERROR; }
//	res = *ans;
//
//	if (sign) { *res = '-'; res++; }
//
//	res += len - sign;
//	*res-- = 0;
//	
//	while (n > 0) {
//		j = (int)n % base;
//		if (j < 10) {
//			*res = j + '0';
//		}
//		else {
//			*res = j + 'a' - 10;
//		}
//
//		n /= base;
//		res--;
//	}
//
//	return 0;
//}