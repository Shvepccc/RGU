//#include <stdio.h>
//#include "../../libs/u_list.h"
//
//int traverce_function(u_list_node const* list_node) {
//	list_node->next_node == NULL ? (printf("%c\n", *(int*)list_node->data)) : 
//		(printf("%c ", *(int*)list_node->data));
//	return 0;
//}
//
//
//int main() {
//	int i = 0;
//	u_list list = u_list_init(NULL, sizeof(char));
//	for (i = 65; i < 65+6; i++) {
//		u_list_insert(&list, &i, i-65);
//	}
//	u_list_traverse(&list, traverce_function);
//	u_list_delete_by_index(&list, &i, 3);
//	u_list_traverse(&list, traverce_function);
//	u_list_delete_by_index(&list, &i, 0);
//	u_list_traverse(&list, traverce_function);
//	u_list_get_by_index(&list, &i, 2);
//	printf("%c\n", i);
//	u_list_push_back(&list, &i);
//	u_list_traverse(&list, traverce_function);
//	u_list_free(&list);
//	return 0;
//}

//#include <stdio.h>
//#include "../../libs/cust_stack.h"
//
//int main() {
//	int i = 0, t = 0;
//	stack st = stack_init(NULL, sizeof(int));
//	for (i = 1; i < 6; i++) {
//		stack_push_back(&st, &i);
//	}
//	for (i = 0; i < 6; i++) {
//		stack_pop(&st, &t);
//		printf("%d\n", t);
//	}
//
//	stack_free(&st);
//	return 0;
//}

//#include <stdio.h>
//#include <string.h>
//#include "../../libs/cust_string.h"
//#include "../../libs/binary_searsh_tree.h"
//
//int int_cmp(int a, int b) {
//	return a - b;
//}
//
//void print_tree(int* data) {
//	printf("%d\n", *data);
//}
//
//int main() {
	//string str;
	//int k;
	//binary_search_tree tree;
	//initialize_bst(&tree, strcmp, 0);

	//k = 1;
	//str = string_from("hello", strlen("hello"));
	//insert_into_bst(&tree, str, &k);

	//k = 2;
	//str = string_from("goodbye", strlen("goodbye"));
	//insert_into_bst(&tree, str, &k);

	//bst_prefix_traversion(&tree, print_tree);

	//deinitialize_bst(&tree);
	//return 0;
//}




//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//
//#include <stdio.h>
//#include <string.h>
//#include "../../libs/hash_table.h"
//#include "../../libs/error.h"
//
//#include "../../libs/cust_string.h"
//
//#define HASHSIZE 128
//
//unsigned long hash_function(char* str) {
//	unsigned long i = 0;
//	for (int j = 0; str[j]; j++)
//		i += str[j];
//	//printf("*** hash function result - %s = %d\n", str, i % HASHSIZE);
//	return i;
//}
//
//void print_search(hash_table* table, char* key) {
//	char* val = (char*)malloc(50);
//	
//	if (ht_search(table, key, val) == KEY_NOT_FOUND) {
//		printf("Key: %s does not exist\n", key);
//	}
//	else {
//		printf("Key: %s, Value: %s\n", key, val);
//	}
//	free(val);
//}
//
//void print_function(int i, void* key, void* value) {
//	char* _key = key;
//	char* _value = value;
//	printf("Index: %d \tKey: %s  \tValue: %s\n", i, _key, _value);
//}
//
//void ht_destructor(hash_table_item* item) {
//	free(item->key);
//	free(item->value);
//	free(item);
//}
//
//int main() {
//	hash_table* ht = ht_init(128, sizeof(char)*10, sizeof(char)*50, hash_function, ht_destructor, strcmp);
//
//	ht_insert(ht, "1", "First address");
//	ht_insert(ht, "2", "Second address");
//	ht_insert(ht, "Hel", "Third address");
//	ht_insert(ht, "Cau", "Fourth address");
//	
//	print_search(ht, "1");
//	print_search(ht, "2");
//	print_search(ht, "3");
//	print_search(ht, "Hel");
//	print_search(ht, "Cau");  // Collision!
//	
//	ht_print(ht, print_function);
//	
//	ht_delete(ht, "1");
//	ht_delete(ht, "Cau");
//	
//	ht_print(ht, print_function);
//	ht_free(ht);
//
//	_CrtDumpMemoryLeaks();
//
//	return 0;
//}
