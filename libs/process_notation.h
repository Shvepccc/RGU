#ifndef CONVERT_NOTATION_H_
#define CONVERT_NOTATION_H_

#include <corecrt.h>

typedef enum operator_type {
    unary,
    binary
} operator_type;

typedef struct oper {
    char representation[7];
    operator_type type;
    int (*func)(int, int);
} oper;

int infix_to_postfix_notation_border(
    char const* src_expression,
    int (*lexem_character_selector)(char c),
    int (*lexems_ignore_selector)(char c),
    int (*priority_mapper)(char* c),
    char** postfix_expression,
    hash_table** operators_ht_ptr,
    int_settings* main_settings);

int calculate_postfix_expression_border(
    char const* postfix_expression,
    int* expression_result,
    int_settings* main_settings,
    hash_table** operators_ht_ptr,
    hash_table** vars_ht_ptr,
    size_t operators_count,
    ...);

#endif 