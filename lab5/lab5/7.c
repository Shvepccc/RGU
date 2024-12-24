//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <string.h>
//#include "../../libs/cust_string.h"
//#include "../../libs/binary_searsh_tree.h"
//#include "../../libs/error.h"
//#include "../../libs/int_vector.h"
//
//int is_bad_symbol(char symbol, int_vector vec);
//void print_tree(int* data);
//int string_comparer(string* a, string* b);
//void bst_prefix_traversion_inner_to_search_words(
//	bst_item const* subtree,
//	string* min_word, int* min_len,
//	string* max_word, int* max_len);
//
//int main(int argc, char* argv[]) {
//	FILE* data_file;
//	char symbol;
//	char temp_str[BUFSIZ];
//	int i = 0, lexeme_flag = 0, k = 1, read_flag = 0, action = 0, exit_flag = 0;
//	int min_len = INT_MAX, max_len = -1;
//	int* value;
//	string temp_string = string_init();
//	string min_word, max_word;
//	int_vector bad_chars_vector = int_vector_init();
//	binary_search_tree tree;
//	initialize_bst(&tree, string_comparer, 0);
//
//	if (argc < 3) {
//		printf("Not enough arguments");
//		return NOT_ENOUGH_ARGS;
//	}
//
//	for (i = 2; i < argc; i++) {
//		int_vector_push_back(&bad_chars_vector, *argv[i]);
//	}
//	int_vector_push_back(&bad_chars_vector, 10);
//
//	//for (i = 0; i < int_vector_size(bad_chars_vector); i++) {
//	//	printf("'%c'\n", bad_chars_vector[i]);
//	//}
//
//	while (1) {
//		printf("0 - Process a text file\n");
//		printf("1 - Number of occurrences of a certain word\n");
//		printf("2 - Output of the first n most frequently occurring words in a file\n");
//		printf("3 - Output of the longest and shortest word\n");
//		printf("4 - Get the depth of the tree\n");
//		printf("5 - Save tree to file\n");
//		printf("6 - Load tree from file\n");
//		printf("Another value to exit\n");
//		printf("Enter value: ");
//		scanf("%d", &action);
//		system("cls");
//
//		if (read_flag == 1 || action == 0 || action == 6) {
//			switch (action)
//			{
//			case 0:
//				deinitialize_bst(&tree);
//				initialize_bst(&tree, string_comparer, 0);
//				if ((data_file = fopen(argv[1], "r")) == NULL) {
//					return OPEN_FILE_ERROR;
//				}
//
//				printf("Reading data from file...\n");
//				i = 0;
//				while ((symbol = fgetc(data_file)) != EOF) {
//					if (!is_bad_symbol(symbol, bad_chars_vector)) {
//						temp_str[i++] = symbol;
//						lexeme_flag = 1;
//					}
//					else if (lexeme_flag == 1) {
//						temp_str[i++] = 0;
//						temp_string = string_from(temp_str, strlen(temp_str));
//						if (obtain_from_bst(&tree, temp_string, &value) != KEY_NOT_FOUND) {
//							*value += 1;
//						}
//						else {
//							insert_into_bst(&tree, temp_string, &k);
//						}
//						i = 0;
//						lexeme_flag = 0;
//						read_flag = 1;
//						temp_string = string_from("", strlen(""));
//					}
//				}
//				if (strlen(temp_string) != 0) {
//					printf("That's not all!!!\n");
//					temp_str[i++] = 0;
//					temp_string = string_from(temp_str, strlen(temp_str));
//					if (obtain_from_bst(&tree, temp_string, &value) != KEY_NOT_FOUND) {
//						value += 1;
//					}
//					else {
//						insert_into_bst(&tree, temp_string, &k);
//					}
//				}
//				if (read_flag == 0) {
//					printf("File is empty. Programm stopped.");
//					deinitialize_bst(&tree);
//					int_vector_free(bad_chars_vector);
//					return 0;
//				}
//				else {
//					printf("Data read successfully!\n");
//					bst_print_bautiful(&tree);
//				}
//				break;
//			case 1:
//				printf("Enter word to search: ");
//				scanf("%s", temp_str);
//				temp_string = string_from(temp_str, strlen(temp_str));
//				if (obtain_from_bst(&tree, temp_string, &value) != KEY_NOT_FOUND) {
//					printf("The word appeared %d times\n", *value);
//				}
//				else {
//					printf("The word did not occur in the text =( \n");
//				}
//				break;
//			case 2:
//				printf("Enter n: ");
//				scanf("%d", &k);
//				break;
//			case 3:
//				min_word = string_init();
//				max_word = string_init();
//				bst_prefix_traversion_inner_to_search_words(tree.root, &min_word, &min_len, &max_word, &max_len);
//				printf("Smallest word (len %d): %s\n", min_len, min_word);
//				printf("Largest  word (len %d): %s\n", max_len, max_word);
//				min_len = INT_MAX; max_len = 0;
//				string_free(min_word);
//				string_free(max_word);
//				break;
//			case 4:
//				printf("Depth of tree: %d\n", bst_get_depth(&tree));
//				break;
//			case 5:
//				printf("Enter name to save tree: ");
//				scanf("%s", &temp_str);
//				bst_save_to_txt(&tree, &temp_str);
//				printf("Tree saved successfully!\n");
//				break;
//			case 6:
//				printf("Enter name of file to load tree: ");
//				scanf("%s", &temp_str);
//				deinitialize_bst(&tree);
//				initialize_bst(&tree, string_comparer, 0);
//
//				bst_load_from_txt(&temp_str, &tree);
//				bst_print_bautiful(&tree);
//				printf("Tree load successfully!\n");
//				break;
//			default:
//				exit_flag = 1;
//				break;
//			}
//		}
//		else {
//			printf("First you need to download the data!\n");
//		}
//		if (exit_flag) {
//			break;
//		}
//		printf("\n");
//	}
//
//	string_free(temp_string);
//	deinitialize_bst(&tree);
//	int_vector_free(bad_chars_vector);
//	return OK;
//}
//
//void print_tree(int* data) {
//	printf("%d\n", *data);
//}
//
//int is_bad_symbol(char symbol, int_vector vec) {
//	int i;
//	for (i = 0; i < int_vector_size(vec); i++) {
//		if (symbol - vec[i] == 0) {
//			return 1;
//		}
//	}
//	return 0;
//}
//
//int string_comparer(string* a, string* b) {
//	return strcmp(*a, *b);
//}
//
//void bst_prefix_traversion_inner_to_search_words(
//	bst_item const* subtree,
//	string* min_word, int* min_len,
//	string* max_word, int* max_len) 
//{	
//	int len;
//	if (subtree == NULL) {
//		return;
//	}
//
//	len = strlen(subtree->key);
//	if (len < *min_len && len != 0) {
//		*min_word = subtree->key;
//		(*min_len) = len;
//	}
//	if (len > *max_len) {
//		*max_word = subtree->key;
//		(*max_len) = len;
//	}
//
//	 bst_prefix_traversion_inner_to_search_words(subtree->left_subtree, min_word, min_len, max_word, max_len);
//	bst_prefix_traversion_inner_to_search_words(subtree->right_subtree, min_word, min_len, max_word, max_len);
//}