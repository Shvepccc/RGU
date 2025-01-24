#include "../../libs/dynamic_array_of_char.h"
#include "../../libs/cust_stack.h"
#include "../../libs/error.h"
#include "../../libs/cust_string.h"
#include "../../libs/hash_table.h"
#include "../../libs/parse_config.h"
#include "../../libs/convertors.h"
#include "../../libs/process_notation.h"

#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include <stdarg.h>
#include <string.h>

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
int cust_cpy_stack_int_ordo(void* dst, void* src) {
    if (dst == NULL || src == NULL) {
        return NULL_POINTER;
    }
    memcpy(dst, src, sizeof(int));
    return OK;
}

int infix_to_postfix_notation_border(
    char const* src_expression,
    int (*lexem_character_selector)(char c),
    int (*lexems_ignore_selector)(char c),
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
        if (lexems_ignore_selector(*se)) {
            se++;
            continue;
        }

        if (lexem_character_selector(*se)) {
            if (*se != '\0') {
                string_push_back(&temp_string, *se);
            }
            se++;
        }
        else {
            int j = 0;
            while (*se && !lexem_character_selector(*se) && !lexems_ignore_selector(*se)) {
                string_push_back(&temp_string, *se);
                se++;
            }
        }

        //printf("*** (%2.d) '%s'\n", string_size(temp_string), temp_string);

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
            stack_push_back_cust_cpy(&operators, &temp_string, cust_cpy_for_search_ordo_2);
            string_free(&temp_string_2);
        }
        else {
            string_cat(&result_string, &temp_string);
            string_push_back(&result_string, ' ');
        }

        string_free(&temp_string);
        temp_string = string_init();

        if (!*se || err != 0) {
            if (operators.size != 0 && err == 0) {
                collect_all_flag = 1;
            }
            else {
                break;
            }
        }
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

int cust_cpy_for_search_ordo_process_notation(void* dst, void* src) {
    if (dst == NULL || src == NULL) {
        return NULL_POINTER;
    }
    *(string*)dst = string_cpy_new(*(string*)src);
    return OK;
}

int calculate_postfix_expression_border(
    char const* postfix_expression,
    int* expression_result,
    int_settings* main_settings,
    hash_table** operators_ht_ptr,
    hash_table** vars_ht_ptr,
    size_t operators_count,
    ...)
{
    va_list arg;
    stack operands;
    int i, left_operand = 0, right_operand = 0, err = 0, some_flag = 0, collect_all_flag = 0, temp_int = 0;
    char const* pe = postfix_expression;
    char buf[BUFSIZ], * b = buf, prev = ' ';
    oper** operators, current_operator, * current_operator_ptr = &current_operator, * found_operator_ptr = NULL;
    string temp_string, temp_string_2;

    if (postfix_expression == NULL || expression_result == NULL) {
        return NULL_POINTER;
    }

    if (!(operators = (oper**)malloc(sizeof(oper*) * operators_count))) {
        return MEMORY_ALLOCATE_ERROR;
    }

    operands = stack_init(NULL, sizeof(int));
    va_start(arg, operators_count);
    for (i = 0; i < operators_count; ++i) {
        if (!(operators[i] = va_arg(arg, oper*))) {
            free(operators);
            stack_free(&operands);
            return NOT_ENOUGH_ARGS;
        }
    }
    va_end(arg);

    temp_string = string_init();
    
    qsort((void*)operators, operators_count, sizeof(oper*), compare_operators);

    while (1) {
        if (!isspace(*pe) && collect_all_flag != 1) {
            string_push_back(&temp_string, *pe);
        }
        else {
            some_flag = 0;
            temp_int = 0;
            
            //try to find in operators
            err = ht_search_cust_cpy(*operators_ht_ptr, &temp_string, &temp_string_2, cust_cpy_for_search_ordo_2);
            if (!err) {
                found_operator_ptr = NULL;
                for (i = 0; i < operators_count; i++) {
                    if (strcmp(operators[i]->representation, temp_string_2) == 0) {
                        found_operator_ptr = operators[i];
                        break;
                    }
                }
                if (found_operator_ptr != NULL) {
                    switch (found_operator_ptr->type)
                    {
                    case unary:
                        stack_pop(&operands, &left_operand);
                        left_operand = found_operator_ptr->func(left_operand, 0);
                        stack_push_back(&operands, &left_operand);
                        break;
                    case binary:
                        stack_pop(&operands, &right_operand);
                        stack_pop(&operands, &left_operand);
                        left_operand = found_operator_ptr->func(left_operand, right_operand);
                        stack_push_back(&operands, &left_operand);
                        break;
                    }
                    some_flag = 1;
                    string_free(&temp_string_2);
                }
            }
            else {
                //try to find in vars
                err = ht_search(*vars_ht_ptr, &temp_string, &temp_int);
                if (err) {
                    // i don't find it in vars; try convert to digit
                    err = convert_TO_decimal(temp_string, main_settings->base_assign, &temp_int);
                    if (!err) {
                        some_flag = 1;
                    }
                }
                else {
                    //i find it in vars and get variabel's value
                    some_flag = 1;
                }
                if (some_flag) {
                    stack_push_back(&operands, &temp_int);
                }
            }

            if (some_flag != 1) {
                err = INVALID_INSTRUCTION;
                break;
            }
            err = 0;

            string_free(&temp_string);
            temp_string = string_init();
        }
        if (!*pe) {
            if (string_size(temp_string) != 0) {
                collect_all_flag = 1;
            }
            else {
                break;
            }
        }
        else {
            prev = *pe;
            pe++;
        }
    }
    string_free(&temp_string);
    stack_pop(&operands, expression_result);
    free(operators);
    stack_free(&operands);
    if (err) { return err; }
    return OK;
}