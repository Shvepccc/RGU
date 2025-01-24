#include "../../libs/arithmetic_expressions.h"
#include "../../libs/dynamic_array_of_char.h"
#include "../../libs/cust_stack.h"
#include "../../libs/error.h"
#include "../../libs/cust_string.h"
#include "../../libs/hash_table.h"
#include "../../libs/parse_config.h"

#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include <stdarg.h>

int compare_operators(void const* first, void const* second) {
    return (*((oper**)first))->representation - (*((oper**)second))->representation;
}

int cust_cpy_for_search_ordo_2(void* dst, void* src) {
    if (dst == NULL || src == NULL) {
        return NULL_POINTER;
    }
    *(string*)dst = string_cpy_new(*(string*)src);
    return OK;
}

int convert_to_postfix_notation_border(
    char const* src_expression,
    int (*lexem_character_selector)(char c),
    int (*priority_mapper)(char* c),
    char** postfix_expression,
    hash_table** operators_ht_ptr,
    int_settings* main_settings)
{
    stack operators;
    size_t result_size = 16;
    int symbol_count = 0,err = 0, parenthesis_balance = 0, bracket_flag = 0, collect_all_flag = 0;
    char* se = src_expression, prev = 'a', for_dynamic_array_interaction;
    string temp_string, temp_string_2, for_stack_interaction, result_string;

    if (src_expression == NULL || lexem_character_selector == NULL) {
        return NULL_POINTER;
    }

    temp_string = string_init();
    temp_string_2 = string_init();
    for_stack_interaction = string_from_l("(");
    result_string = string_init();

    operators = stack_init(NULL, sizeof(string));
    stack_push_back_cust_cpy(&operators, &for_stack_interaction, cust_cpy_for_search_ordo_2);

    while (1) {
        if (!lexem_character_selector(*se) && !bracket_flag && collect_all_flag == 0) {
            if (*se != ' ') string_push_back(&temp_string, *se);
        }
        else if ((lexem_character_selector(*se) != lexem_character_selector(prev)) || bracket_flag || collect_all_flag == 1 || prev == '(' || prev == ')') {
            if (bracket_flag == 1) {
                if (prev != ' ') string_push_back(&temp_string, prev);
            }
            else if (symbol_count == 0 || prev == '(' || prev == ')') { 
                string_push_back(&temp_string, *se);
            }

            printf("*** (%2.d) '%s'\n", string_size(temp_string), temp_string);

            if (temp_string[0] == '(') {
                for_stack_interaction = string_from_l("(");
                stack_push_back_cust_cpy(&operators, &for_stack_interaction, cust_cpy_for_search_ordo_2);
                parenthesis_balance++;
                string_free(&for_stack_interaction);
            }
            else if (temp_string[0] == ')' || temp_string[0] == '\0') {

                if (temp_string[0] == ')') parenthesis_balance--;
                if (parenthesis_balance < 0) {
                    err = BALANCE_OF_PARENTHESIS_IS_BROKEN;
                    break;
                }

                while (1) {
                    stack_pop_cust_cpy(&operators, &for_stack_interaction, cust_cpy_for_search_ordo_2);
                    if (for_stack_interaction[0] == '(') {
                        break;
                    }

                    string_cat(&result_string, &for_stack_interaction);
                    string_push_back(&result_string, ' ');
                    string_free(&for_stack_interaction);
                }
            }
            else if (ht_search_cust_cpy(*operators_ht_ptr, &temp_string, &temp_string_2, cust_cpy_for_search_ordo_2) == OK) {
                while (1) {
                    stack_get(&operators, &for_stack_interaction);

                    if (priority_mapper(for_stack_interaction) < priority_mapper(temp_string)) {
                        break;
                    }

                    stack_pop_cust_cpy(&operators, &for_stack_interaction, cust_cpy_for_search_ordo_2);

                    string_cat(&result_string, &for_stack_interaction);
                    string_push_back(&result_string, ' ');
                    string_free(&for_stack_interaction);
                }
                if (main_settings->operator_type == 1) {
                    string_cat(&result_string, &temp_string);
                    string_push_back(&result_string, ' ');
                } 
                else {
                    stack_push_back_cust_cpy(&operators, &temp_string, cust_cpy_for_search_ordo_2);
                }
                string_free(&temp_string_2);
            }
            else {
                string_cat(&result_string, &temp_string);
                string_push_back(&result_string, ' ');
            }

            string_free(&temp_string);
            temp_string = string_init();

            if ((*se == '(' || *se == ')') && symbol_count != 0 && prev != ')' && prev != '(') {
                bracket_flag = 1;
            }
            else if (bracket_flag == 1) {
                if (*se != ' ') string_push_back(&temp_string, *se);
                bracket_flag = 0;
            }
        }
        
        if (!*se || err != 0) {
            if (operators.size != 0 && err == 0) {
                collect_all_flag = 1;
            }
            else {
                break;
            }
        }

        prev = *se;
        se++;
        symbol_count++;
    }

    result_string[string_size(result_string) - 1] = '\0';
    *postfix_expression = result_string;

    if (parenthesis_balance != 0 && err == 0) {
        err = BALANCE_OF_PARENTHESIS_IS_BROKEN;
    }

    if (err) {
        stack_free(&operators);
        return err;
    }
    return OK;
}