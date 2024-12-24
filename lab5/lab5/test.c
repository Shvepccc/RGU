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