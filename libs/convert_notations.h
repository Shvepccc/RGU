#ifndef CONVERT_NOTATION_H_
#define CONVERT_NOTATION_H_

int convert_to_postfix_notation_border(
    char const* src_expression,
    int (*lexem_character_selector)(char c),
    int (*priority_mapper)(char* c),
    char** postfix_expression,
    hash_table** operators_ht_ptr,
    int_settings* main_settings);

#endif 