#ifndef UNIDIRECTIONAL_STACK_H_
#define UNIDIRECTIONAL_STACK_H_

#include <stdlib.h>

typedef struct stack_node {
	void* data;
	struct stack_node* next_node;
} stack_node;

typedef struct stack {
	stack_node* first_node;
	void (*destructor)(void*);
	size_t size_of_element;
	int size;
} stack;

stack stack_init(void (*destructor)(void*), size_t size_of_element);
void stack_free(stack* stack_ptr);

int stack_push_back(stack* list_ptr, void* element);
int stack_pop(stack* stack_ptr, void* element);
int stack_get(stack* stack_ptr, void* element);

#endif