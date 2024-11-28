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