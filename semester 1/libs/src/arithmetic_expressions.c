#include "../../libs/arithmetic_expressions.h"
#include "../../libs/dynamic_array_of_char.h"
#include "../../libs/cust_stack.h"
#include "../../libs/error.h"

#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include <stdarg.h>

int gorner(
    char const* str,
    int base,
    int* result_placement)
{
    int result = 0;
    int is_negative = 0;

    if (str == NULL || !*str || result_placement == NULL) {
        return NULL_POINTER;
    }

    if (*str == '-') {
        ++str;
        is_negative = 1;

        if (!*str) {
            return NULL_POINTER;
        }
    }

    if (base < 2 || base > 36) {
        return INCORRECT_BASE;
    }

    while (*str) {
        if (!isalnum(*str)) {
            return 1;
        }

        result = result * base + (isdigit(*str)
            ? *str - '0'
            : tolower(*str) - 'a' + 10);

        ++str;
    }

    if (is_negative) {
        result = -result;
    }

    *result_placement = result;
    return OK;
}

int compare_operators(void const* first, void const* second) {
    return (*((oper **)first))->representation - (*((oper **)second))->representation;
}

int convert_to_postfix_notation(
    char const* infix_expression,
    int (*lexem_character_selector)(char c),
    int (*priority_mapper)(char c),
    char** postfix_expression)
{
    stack operators;
    size_t result_size = 16;
    dynamic_array_of_char result;
    int for_stack_interaction = '(', err = 0, parenthesis_balance = 0;
    char* ie = infix_expression, prev = ' ', for_dynamic_array_interaction;

    if (infix_expression == NULL || lexem_character_selector == NULL || priority_mapper == NULL) {
        return NULL_POINTER;
    }

    initialize_dynamic_array_of_char(&result);

    operators = stack_init(NULL, sizeof(int));
    stack_push_back(&operators, &for_stack_interaction);

    while (1) {
        if (isdigit(*ie)) {
            add_char_to_end_of_dynamic_array(&result, *ie);
        }
        else {
            if (lexem_character_selector(prev)) {
                add_char_to_end_of_dynamic_array(&result, ' ');
            }

            switch (*ie)
            {
            case '(':
                for_stack_interaction = *ie;
                stack_push_back(&operators, &for_stack_interaction);
                parenthesis_balance++;
                break;
            case ')':
            case '\0':
                if (*ie == ')') parenthesis_balance--;
                if (parenthesis_balance < 0) {
                    err = BALANCE_OF_PARENTHESIS_IS_BROKEN;
                    break;
                }
                while (1) {
                    stack_pop(&operators, &for_stack_interaction);
                    if ((char)for_stack_interaction == '(') {
                        break;
                    }

                    add_char_to_end_of_dynamic_array(&result, (char)for_stack_interaction);
                    add_char_to_end_of_dynamic_array(&result, ' ');
                }
                break;
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            case '^':
                while (1) {
                    stack_get(&operators, &for_stack_interaction);

                    if (priority_mapper((char)for_stack_interaction) < priority_mapper(*ie)) {
                        break;
                    }

                    stack_pop(&operators, &for_stack_interaction);

                    add_char_to_end_of_dynamic_array(&result, (char)for_stack_interaction);
                    add_char_to_end_of_dynamic_array(&result, ' ');
                }
                for_stack_interaction = (int)*ie;
                stack_push_back(&operators, &for_stack_interaction);
                break;
            }
        }

        if (!*ie || err != 0) {
            break;
        }
        prev = *ie;
        ie++;
    }

    *(result.current - 1) = '\0';
    squeeze_dynamic_array_of_char(&result);
    *postfix_expression = result.start;
    result.start = NULL;
    deinitialize_dynamic_array_of_char(&result);

    if (parenthesis_balance != 0 && err == 0) {
        err = BALANCE_OF_PARENTHESIS_IS_BROKEN;
    }

    if (err) {
        stack_free(&operators);
        free(*postfix_expression);
        return err;
    }
    return OK;
}

int calculate_postfix_expression(
    char const* postfix_expression,
    int* expression_result,
    size_t operators_count,
    ...)
{
    va_list arg;
    stack operands;
    int i, left_operand, right_operand, err = 0;
    char const* pe = postfix_expression;
    char buf[BUFSIZ], * b = buf, prev = ' ';
    oper ** operators, current_operator, * current_operator_ptr = &current_operator, * found_operator_ptr = NULL;

    if (postfix_expression == NULL || expression_result == NULL) {
        return NULL_POINTER;
    }

    if (!(operators = (oper **)malloc(sizeof(oper *) * operators_count))) {
        return MEMORY_ALLOCATE_ERROR;
    }

    operands = stack_init(NULL, sizeof(int));
    va_start(arg, operators_count);
    for (i = 0; i < operators_count; ++i) {
        if (!(operators[i] = va_arg(arg, oper *))) {
            free(operators);
            stack_free(&operands);
            return NOT_ENOUGH_ARGS;
        }
    }
    va_end(arg);

    qsort((void*)operators, operators_count, sizeof(oper *), compare_operators);

    while (1) {
        if (isdigit(*pe)) {
            *b++ = *pe;
        }
        else {
            if (isdigit(prev)) {
                *b = 0;
                b = buf;
                err = gorner(buf, 10, &right_operand);
                if (err) {
                    break;
                }

                stack_push_back(&operands, &right_operand);
            }

            found_operator_ptr = NULL;
            for (i = 0; i < operators_count; i++) {
                if (operators[i]->representation - *pe == 0) {
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
            }

        }
        if (!*pe) {
            break;
        }
        prev = *pe;
        pe++;
    }
    stack_pop(&operands, expression_result);
    free(operators);
    stack_free(&operands);
    if (err) { return err; 
    return OK;
}

