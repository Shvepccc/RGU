//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <time.h>
//#include <math.h>
//#include "../../libs/error.h"
//#include "../../libs/cust_string.h"
//
//typedef struct adress {
//	string city;
//	string street;
//	int house_number;
//	string corpus;
//	int apartment_number;
//	int index;
//} adress;
//
//typedef struct mail {
//	adress mail_adress;
//	double weight;
//	string post_id;
//	string creation_time;
//	string delivery_time;
//} mail;
//
//typedef struct post {
//	adress* post_adress;
//	mail* mail_array;
//} post;
//
//void print_instruction();
//void print_mail(post* main_post, int i);
//int mails_comparer(const mail* val1, const mail* val2);
//
//time_t time_from_str(char const* str);
//int convert_TO_decimal(char* n, int base, int* ans);
//
//int free_adress(adress* adress_ptr) {
//	string_free(adress_ptr->city);
//	string_free(adress_ptr->street);
//	string_free(adress_ptr->corpus);
//	return 0;
//}
//
//int free_mail(mail* mail_ptr) {
//	free_adress(&mail_ptr->mail_adress);
//	string_free(mail_ptr->post_id);
//	string_free(mail_ptr->creation_time);
//	string_free(mail_ptr->delivery_time);
//	return 0;
//}
//
//int main() {
//	int flag = 0;
//	int err = 1;
//	int READ_FILE = 1;
//	FILE* file;
//	int action, i = 0, stop_flag = 0, temp_int;
//	char temp_str[BUFSIZ];
//	char temp_str_2[BUFSIZ];
//	int mail_array_size = 0, mail_array_capacity = 16;
//	mail* temp_mail_arr;
//	string temp_string;
//	time_t mytime;
//
//	char* current_time = (char*)malloc(sizeof(char)*19);
//	if (current_time == NULL) {
//		return MEMORY_ALLOCATE_ERROR;
//	}
//
//	post* main_post = (post*)malloc(sizeof(post));
//	if (main_post == NULL) {
//		free(current_time);
//		return MEMORY_ALLOCATE_ERROR;
//	}
//
//	mail* mail_arr = (mail*)malloc(sizeof(mail) * mail_array_capacity);
//	if (mail_arr == NULL) {
//		free(current_time);
//		free(main_post);
//		return MEMORY_ALLOCATE_ERROR;
//	}
//	main_post->mail_array = mail_arr;
//
//	adress* post_adr = (adress*)malloc(sizeof(adress));
//	if (post_adr == NULL) {
//		free(current_time);
//		free(main_post);
//		free(mail_arr);
//		return MEMORY_ALLOCATE_ERROR;
//	}
//	main_post->post_adress = post_adr;
//
//	char* post_city = "Moscow";
//	char* post_street = "Shabolovskaya";
//	char* post_corpus = "1";
//	main_post->post_adress->city = string_from(post_city, strlen(post_city));
//	main_post->post_adress->street = string_from(post_street, strlen(post_street));
//	main_post->post_adress->house_number = 39;
//	main_post->post_adress->corpus = string_from(post_corpus, strlen(post_corpus));
//	main_post->post_adress->apartment_number = 720;
//	main_post->post_adress->index = 123456;
//
//	print_instruction();
//	while (1) {
//		flag = 0;
//		printf("Enter number (-1 to print instruction): ");
//		scanf("%d", &action);
//		printf("\n");
//		system("cls");
//		switch (action)
//		{
//			case -1:
//				print_instruction();
//				break;
//			case 0:
//				printf(" -- Post adress data -- \ncity: %s, \nstreet: %s, \nhouse number: %d, \ncorpus: %s, \napartment number: %d, \nindex: %d\n",
//					main_post->post_adress->city,
//					main_post->post_adress->street,
//					main_post->post_adress->house_number,
//					main_post->post_adress->corpus,
//					main_post->post_adress->apartment_number,
//					main_post->post_adress->index);
//				printf("Post have %d mails\n\n", mail_array_size);
//				break;
//			case 1:
//			case 11:
//
//				if (mail_array_size == mail_array_capacity) {
//					temp_mail_arr = (mail*)realloc(main_post->mail_array, sizeof(mail) * mail_array_capacity * 2);
//					if (temp_mail_arr == NULL) {
//						free(current_time);
//						free(main_post);
//						free(mail_arr);
//						return MEMORY_ALLOCATE_ERROR;
//					}
//					mail_array_capacity *= 2;
//				}
//
//				if (!READ_FILE || action == 11) {
//					printf(" -- Mail adress data --\n");
//
//					printf("Enter city of recipient: ");
//					scanf("%s", temp_str);
//					main_post->mail_array[mail_array_size].mail_adress.city = string_from(temp_str, strlen(temp_str));
//
//					printf("Enter street of recipient: ");
//					scanf("%s", temp_str);
//					main_post->mail_array[mail_array_size].mail_adress.street = string_from(temp_str, strlen(temp_str));
//
//					printf("Enter house number of recipient: ");
//					scanf("%d", &(main_post->mail_array[mail_array_size].mail_adress.house_number));
//
//					printf("Enter corpus of recipient: ");
//					scanf("%s", temp_str);
//					main_post->mail_array[mail_array_size].mail_adress.corpus = string_from(temp_str, strlen(temp_str));
//
//					printf("Enter apartment number of recipient: ");
//					scanf("%d", &(main_post->mail_array[mail_array_size].mail_adress.apartment_number));
//
//					while (1) {
//						printf("Enter index of recipient [6 symbols]: ");
//						scanf("%d", &temp_int);
//						if (temp_int / 1000000 > 0) {
//							printf("Too many symbols :( Try again\n");
//						}
//						else if (temp_int / 100000 <= 0) {
//							printf("Not enough symbols :( Try again\n");
//						}
//						else {
//							main_post->mail_array[mail_array_size].mail_adress.index = string_from(temp_str, strlen(temp_str));
//							break;
//						}
//					}
//
//					printf("\n -- Mail data --\n");
//
//					printf("Enter weight of package: ");
//					scanf("%lf", &(main_post->mail_array[mail_array_size].weight));
//
//					while (1) {
//						printf("Enter post id of package [14 symbols]: ");
//						scanf("%s", &temp_str);
//						if (strlen(temp_str) > 14) {
//							printf("Too many symbols :( Try again\n");
//						}
//						else if (strlen(temp_str) < 14) {
//							printf("Not enough symbols :( Try again\n");
//						}
//						else {
//							main_post->mail_array[mail_array_size].post_id = string_from(temp_str, strlen(temp_str));
//							break;
//						}
//					}
//
//					printf("Enter creation time [dd:MM:yyyy hh:mm:ss]: ");
//					scanf("%s %s", temp_str, temp_str_2);
//					strcat(temp_str, " ");
//					strcat(temp_str, temp_str_2);
//					main_post->mail_array[mail_array_size].creation_time = string_from(temp_str, strlen(temp_str));
//
//					printf("Enter delivery time [dd:MM:yyyy hh:mm:ss]: ");
//					scanf("%s %s", temp_str, temp_str_2);
//					strcat(temp_str, " ");
//					strcat(temp_str, temp_str_2);
//					main_post->mail_array[mail_array_size].delivery_time = string_from(temp_str, strlen(temp_str));
//				}
//				else {
//					printf("Read data from file ...\n");
//
//					if ((file = fopen("task_3.txt", "r")) == NULL) {
//						free(current_time);
//						free(main_post);
//						free(mail_arr);
//						return OPEN_FILE_ERROR;
//					}
//
//					while (err != -1) {
//
//						if (mail_array_size == mail_array_capacity) {
//							temp_mail_arr = (mail*)realloc(main_post->mail_array, sizeof(mail) * mail_array_capacity * 2);
//							if (temp_mail_arr == NULL) {
//								return MEMORY_ALLOCATE_ERROR;
//							}
//							main_post->mail_array = temp_mail_arr;
//							mail_array_capacity *= 2;
//						}
//
//						err = fscanf(file, "%s", temp_str); 
//						if (err == -1) break;
//						//printf("%s\n", temp_str);
//						main_post->mail_array[mail_array_size].mail_adress.city = string_from(temp_str, strlen(temp_str)); 
//						if (err == -1) break;
//
//						err = fscanf(file, "%s", temp_str);
//						main_post->mail_array[mail_array_size].mail_adress.street = string_from(temp_str, strlen(temp_str)); 
//						if (err == -1) break;
//
//						err = fscanf(file, "%d", &(main_post->mail_array[mail_array_size].mail_adress.house_number));
//						if (err == -1) break;
//
//						err = fscanf(file, "%s", temp_str);
//						main_post->mail_array[mail_array_size].mail_adress.corpus = string_from(temp_str, strlen(temp_str));
//						if (err == -1) break;
//
//						err = fscanf(file, "%d", &(main_post->mail_array[mail_array_size].mail_adress.apartment_number));
//						if (err == -1) break;
//
//						err = fscanf(file, "%d", &(main_post->mail_array[mail_array_size].mail_adress.index));
//						if (err == -1) break;
//
//						err = fscanf(file, "%lf", &(main_post->mail_array[mail_array_size].weight));
//						if (err == -1) break;
//
//						err = fscanf(file, "%s", &temp_str);
//						if (err == -1) break;
//						main_post->mail_array[mail_array_size].post_id = string_from(temp_str, strlen(temp_str));
//
//						err = fscanf(file, "%s %s", temp_str, temp_str_2);
//						if (err == -1) break;
//						strcat(temp_str, " ");
//						strcat(temp_str, temp_str_2);
//						main_post->mail_array[mail_array_size].creation_time = string_from(temp_str, strlen(temp_str));
//
//						err = fscanf(file, "%s %s", temp_str, temp_str_2);
//						if (err == -1) break;
//						strcat(temp_str, " ");
//						strcat(temp_str, temp_str_2);
//						main_post->mail_array[mail_array_size].delivery_time = string_from(temp_str, strlen(temp_str));
//
//						mail_array_size++;
//					}
//					fclose(file);
//					mail_array_size--;
//					if (mail_array_size <= 0) {
//						printf("File is empty. Programm stopped.");
//						free(mail_arr);
//						free(current_time);
//						free(main_post);
//						return EMPTY_FILE;
//					}
//					printf("Reading successfully ended )\n\n");
//				}
//				if (!READ_FILE || action == 11) {
//					mail_array_size++;
//				}
//
//				break;
//			case 2:
//				while (1) {
//					printf("Enter the post id of the mail you want to view [14 symbols] (enter 'e' to exit): ");
//					scanf("%s", &temp_str);
//					if (temp_str[0] == 'e') {
//						flag = 1;
//						break;
//					}
//					else if (strlen(temp_str) > 14) {
//						printf("Too many symbols :( Try again\n");
//					}
//					else if (strlen(temp_str) < 14) {
//						printf("Not enough symbols :( Try again\n");
//					}
//					else {
//						break;
//					}
//				}
//				if (flag) {
//					break;
//				}
//				temp_string = string_from(temp_str, strlen(temp_str));
//				for (i = 0; i < mail_array_size; i++) {
//					if (string_compare(temp_string, main_post->mail_array[i].post_id) == 0) {
//						flag = 1;
//						break;
//					}
//				}
//				if (!flag) {
//					printf("Mail with post id: %s not found\n", temp_str);
//					break;
//				}
//				memcpy(main_post->mail_array + i, main_post->mail_array + i + 1, sizeof(mail) * (mail_array_size - i));
//				mail_array_size--;
//				printf("Mail with post id: %s deleted sucessfully\n", temp_str);
//				break;
//			case 3:
//				while (1) {
//					printf("Enter the post id of the mail you want to view [14 symbols] (enter 'e' to exit): ");
//					scanf("%s", &temp_str);
//					if (temp_str[0] == 'e') {
//						flag = 1;
//						break;
//					}
//					else if (strlen(temp_str) > 14) {
//						printf("Too many symbols :( Try again\n");
//					}
//					else if (strlen(temp_str) < 14) {
//						printf("Not enough symbols :( Try again\n");
//					}
//					else {
//						break;
//					}
//				}
//				if (flag) {
//					break;
//				}
//
//				temp_string = string_from(temp_str, strlen(temp_str));
//				for (i = 0; i < mail_array_size; i++) {
//					if (string_compare(temp_string, main_post->mail_array[i].post_id) == 0) {
//						flag = 1;
//						break;
//					}
//				}
//				if (!flag) {
//					printf("Mail with post id: %s not found\n", temp_str);
//				}
//				else {
//					print_mail(main_post, i);
//				}
//				break;
//			case 4:
//				flag = 0;
//				mytime = time(NULL);
//
//				for (i = 0; i < mail_array_size; i++) {
//					if (difftime(mytime, time_from_str(main_post->mail_array[i].delivery_time)) >= 0) {
//						flag = 1;
//						print_mail(main_post, i);
//					}
//				}
//				if (!flag) {
//					printf("Not a single package has been delivered\n");
//				}
//				break;
//			case 5:
//				qsort(main_post->mail_array, mail_array_size, sizeof(mail), mails_comparer);
//				break;
//			case 6:
//				printf("Post data:\n");
//				for (i = 0; i < mail_array_size; i++) {
//					print_mail(main_post, i);
//				}
//				break;
//			default:
//				stop_flag = 1;
//				break;
//		}
//		if (stop_flag) {
//			break;
//		}
//	}
//	for (i = 0; i < mail_array_size; i++) {
//		free_mail((main_post->mail_array)+i);
//	}
//	free(mail_arr);
//	free(current_time);
//	free_adress(main_post->post_adress);
//	free(main_post);
//	return 0;
//}
//
//void print_instruction() {
//	printf("0 - print information about post\n");
//	printf("1 - append new Mail\n2 - to delete Mail by post id\n3 - show information abour Mail by post id\n");
//	printf("4 - show all Mails which delivered at current time\n");
//	printf("5 - sort Mails (firstly by recepient id, secondly by post id)\n");
//	printf("6 - print all Mails\n");
//	printf("another value - stop process\n");
//}
//
//void print_mail(post* main_post, int i) {
//	printf("================= Mail %d ================= \n", i + 1);
//	printf("adress data: \n\tcity: %s \n\tstreet: %s \n\thouse number: %d \n\tcorpus: %s \n\tapartment number: %d \n\tindex: %d\n",
//		main_post->mail_array[i].mail_adress.city,
//		main_post->mail_array[i].mail_adress.street,
//		main_post->mail_array[i].mail_adress.house_number,
//		main_post->mail_array[i].mail_adress.corpus,
//		main_post->mail_array[i].mail_adress.apartment_number,
//		main_post->mail_array[i].mail_adress.index);
//
//	printf("mail data: \n\tweight: %.4lf \n\tpost id: %s \n\tcreation time: %s \n\tdelivery time: %s\n",
//		main_post->mail_array[i].weight,
//		main_post->mail_array[i].post_id,
//		main_post->mail_array[i].creation_time,
//		main_post->mail_array[i].delivery_time);
//	printf("\n");
//}
//
//int mails_comparer(const mail* val1, const mail* val2) {
//	int res_cmp;
//	if (val1->mail_adress.index != val2->mail_adress.index) {
//		return val1->mail_adress.index - val2->mail_adress.index;
//	}
//	else {
//		return string_compare(val1->post_id, val1->post_id);
//	}
//}
//
//time_t time_from_str(char const * str) {
//	time_t mytime = time(NULL);
//	struct tm* mail_time = localtime(&mytime);
//	//printf("%s\n", str);
//	int i = 0, j = 0, temp_int = 0;
//	int* data = (int*)malloc(sizeof(int) * 6);
//	if (data == NULL) {
//		return NULL;
//	}
//	char* temp_char = (char*)malloc(sizeof(char) * 5);
//	if (temp_char == NULL) {
//		return NULL;
//	}
//
//	char* ptr = str;
//	while (*ptr) {
//		if (i == 0 && *ptr == '0' && *(ptr+1) != '0') {
//			ptr++;
//			continue;
//		}
//		else if (*ptr == ':' || *ptr == '_' || *ptr == ' ') {
//			temp_char[i] = '\0';
//			i = 0;
//			convert_TO_decimal(temp_char, 10, &temp_int);
//			data[j] = temp_int;
//			j++;
//			temp_int = 0;
//		}
//		else {
//			temp_char[i] = *ptr;
//			i++;
//		}
//		ptr++;
//	}
//	if (i != 0) {
//		temp_char[i] = '\0';
//		i = 0;
//		convert_TO_decimal(temp_char, 10, &temp_int);
//		data[j] = temp_int;
//		j++;
//	}
//
//	mail_time->tm_mday = data[0];
//	mail_time->tm_mon = data[1] - 1;
//	mail_time->tm_year = data[2] - 1900;
//	mail_time->tm_hour = data[3];
//	mail_time->tm_min = data[4];
//	mail_time->tm_sec = data[5];
//
//	free(data);
//	free(temp_char);
//	return mktime(mail_time);
//}
//
//int convert_TO_decimal(char* n, int base, int* ans) {
//	int len = 0, j = 0, sign = 0;
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
//			(*ans) += (int)((int)n[i] - '0') * pow((double)base, len - i-1);
//		}
//		else if (isalpha(n[i])) {
//			(*ans) += (int)((int)toupper(n[i]) - 'A' + 10) * pow((double)base, len - i-1);
//		}
//	}
//	if (sign == 1) { (*ans) *= -1; }
//	return 0;
//}