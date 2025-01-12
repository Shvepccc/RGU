#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "../../libs/arithmetic_expressions.h"
#include "../../libs/hash_table.h"
#include "../../libs/cust_string.h"
#include "../../libs/error.h"

// task_10.txt --calculate
int priorities(char operator);
int arithmetic_errors_to_file(int err, int line, char* temp_str, char* src_file_name);

int plus_func(int a, int b) {
    return a + b;
}
int minus_func(int a, int b) {
    return a - b;
}
int mult_func(int a, int b) {
    return a * b;
}
int div_func(int a, int b) {
    return a / b;
}
int mod_func(int a, int b) {
    return a % b;
}
int pow_func(int a, int b) {
    return (int)pow((double)a, b);
}

int main(int argc, char* argv[]) {

    oper plus = {'+', binary, plus_func};
    oper minus = { '-', binary, minus_func };
    oper mult = { '*', binary, mult_func };
    oper div = { '/', binary, div_func };
    oper mod = { '%', binary, mod_func };
    oper pow_op = { '^', binary, pow_func };

    int i, j, err, math_result = 0, line = 0;
    FILE* data_file;
    char temp_str[BUFSIZ];
    char* postfix_expression;

	for (i = 1; i < argc-1; i++) {
		if (string_flexographic_compare_char_char(argv[i + 1], "--calculate") == 0) {

            if ((data_file = fopen(argv[i], "r")) == NULL) {
                return OPEN_FILE_ERROR;
            }

            printf(" ----- FILE: %s -----\n", argv[i]);

            while ((fgets(temp_str, BUFSIZ, data_file)) != NULL) {
                
                if (temp_str[strlen(temp_str)-1] == '\n') temp_str[strlen(temp_str) - 1] = 0;

                err = convert_to_postfix_notation(temp_str, isdigit, priorities, &postfix_expression);
                if (err) {
                    printf("ERROR - output of error data to a file\n");
                    err = arithmetic_errors_to_file(err, line, temp_str, argv[i]);
                    if (err) { return err; }
                }
                else {
                    calculate_postfix_expression(postfix_expression, &math_result, 6, plus, minus, mult, div, mod, pow_op);

                    printf("line: %d\n", line);
                    printf("(inf) %s\n", temp_str);
                    printf("(postf) %s\n", postfix_expression);
                    printf("Result of calculation: %d\n", math_result);
                    printf("\n");
                    free(postfix_expression);
                    math_result = 0;
                }
                line++;
            }
		
		}
		else if (string_flexographic_compare_char_char(argv[i+1], "--table") == 0) {
		
		}
		else {
			printf("ERROR: Invalid flag: %s\n", argv[i+1]);
		}
	}

    _CrtDumpMemoryLeaks();

    return OK;
}

int priorities(char operator) {
    // ^ - 2
    // *, /, % - 1
    // +, - - 0
    switch (operator)
    {
    case '+':
    case '-':
        return 0;
    case '*':
    case '/':
    case '%':
        return 1;
    case '^':
        return 2;
    case '(':
        return INT_MIN;
    default:
        return -1;
    }
}

int arithmetic_errors_to_file(int err, int line, char* temp_str, char* src_file_name) {
    FILE* error_file;
    char name_of_file[BUFSIZ];
    int len_of_name, j;

    char* nn = "arithmetic_errors.txt";

    if ((error_file = fopen(nn, "w")) == NULL) {
        return OPEN_FILE_ERROR;
    }
    fprintf(error_file, "line: %d\n", line);
    fprintf(error_file, "(inf) %s\n", temp_str);
    if (err == BALANCE_OF_PARENTHESIS_IS_BROKEN) {
        fprintf(error_file, "ERROR: Balance of parenthesis is broken\n");
    }
    else if (err) {
        fprintf(error_file, "ERROR with code: %d\n", err);
    }
    fclose(error_file);
    return OK;
}