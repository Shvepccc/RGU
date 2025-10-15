#include "../include/error.h"
#include <stdio.h>
#include "../include/u_list.h"
#include <ctype.h>
#include <string.h>

// + uniq elements 
// + несуществующее множество
// + добавление в несуществующее множество
// + лексикографический порядок вывода (обход через 2 указателя)
// + изменить вывод размеров в =
// + проверить =

// равно

void clear_stdin()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int print_char_node(const u_list_node* node)
{
    if (node && node->data) {
        printf("%c ", *(char*)node->data);
    }
    return 0;
}

int compare_elements(const void* a, const void* b)
{
    return *(char*)a - *(char*)b;
}

int compare_elements_merge(u_list_node* a, u_list_node* b)
{
    return *(char*)a->data - *(char*)b->data;
}


int main(int argc, char *argv[])
{
    char com[4];
    char str[4];
    char a_set, b_set, op, c, el;
    u_list temp_list;
    u_list sets[26];
    u_list_node* temp_ptr, *temp_ptr_1, *temp_ptr_2;
    int i = 0, j = 0, contains, flag, err = 0, cmp;

    for (i = 0; i < 26; i++) {
        sets[i] = u_list_init(NULL, sizeof(char));
    }

    while (1)
    {
        printf("Enter command: ");
        scanf("%s %s", com, str);
        if (islower(com[0]))
        {
        a_set = str[0] == '[' ? str[1] : str[0];

        if ((sets[a_set - 'A'].first_node == NULL) && com[0] != 'n')
        {
            printf("Unknown set. Try again.\n");
            clear_stdin();
            continue;
        }

        switch (com[0])
        {
            case 'n':
            if (sets[a_set - 'A'].first_node != NULL)
            {
                printf("This set is already in use. Create a new one or clear the current one.\n");
            }
            else
            {
                u_list_push_back(&sets[a_set - 'A'], &a_set);
            }
            break;

            case 'd':
                u_list_free(&(sets[a_set - 'A']), 0);
                sets[a_set - 'A'] = u_list_init(NULL, sizeof(char));
            break;

            case 'a':
                scanf(" %c", &el);
                err = u_list_push_back(&sets[a_set - 'A'], &el);
                if (err == VALUE_ALREADY_EXISTS)
                {
                    printf("Value already exists.\n");
                }
                else
                {
                    u_list_mergeSort(&sets[a_set - 'A'], compare_elements_merge, 1);
                }
            break;

            case 'r':
                scanf(" %c", &el);
                u_list_delete_by_value(&sets[a_set - 'A'], &el, compare_elements);
                break;

            case 'p':
                printf("P(%c) = { ", a_set);
                printf("{} ");
                
                size_t set_size = u_list_size(&sets[a_set - 'A']) - 1;
                
                if (set_size > 0) {
                    char* elements = malloc(set_size * sizeof(char));
                    temp_ptr = sets[a_set - 'A'].first_node->next_node;
                    for (i = 0; i < set_size; i++)
                    {
                    if (temp_ptr != NULL)
                    {
                        elements[i] = *(char*)temp_ptr->data;
                        temp_ptr = temp_ptr->next_node;
                    }
                    }
                    
                    int total_subsets = 1 << set_size;
                    
                    for (int mask = 1; mask < total_subsets; mask++)
                    {
                    printf("{ ");
                    for (i = 0; i < set_size; i++)
                    {
                        if (mask & (1 << i))
                        {
                        printf("%c ", elements[i]);
                        }
                    }
                    printf("} ");
                    }
                    
                    free(elements);
                }
                printf("}\n");
            break;

            case 's':
                if (a_set == ']')
                {
                    for(i = 0; i < 26; i++)
                    {
                    if (sets[i].first_node != NULL)
                    {
                        printf("%c ", *(char*)sets[i].first_node->data);
                    }
                    }
                    printf("\n");
                }
                else
                {
                    printf("%c = { ", *(char*)sets[a_set - 'A'].first_node->data);
                    temp_ptr = (&sets[a_set - 'A'])->first_node->next_node;
                    while (temp_ptr != NULL) {
                    print_char_node(temp_ptr);
                    temp_ptr = temp_ptr->next_node;
                    }
                    printf("}\n");
                }
            break;

            default:
                printf("Invalid command =(\n");
            break;
        }
        }
        else {
            scanf(" %c", &b_set);
            op = str[0];
            a_set = com[0];

            if ((sets[a_set - 'A'].first_node == NULL) || (sets[b_set - 'A'].first_node == NULL))
            {
                printf("Unknown set. Try again.\n");
                clear_stdin();
                continue;
            }

            switch (op)
            {
                case '+':
                printf("%c + %c = { ", a_set, b_set);
                
                temp_ptr_1 = sets[a_set - 'A'].first_node->next_node;
                temp_ptr_2 = sets[b_set - 'A'].first_node->next_node;
                
                while (temp_ptr_1 != NULL || temp_ptr_2 != NULL)
                {
                    if (temp_ptr_1 != NULL && temp_ptr_2 != NULL)
                    {
                    cmp = memcmp(temp_ptr_1->data, temp_ptr_2->data, sizeof(char*));
                    
                    if (cmp == 0)
                    {
                        print_char_node(temp_ptr_1);
                        temp_ptr_1 = temp_ptr_1->next_node;
                        temp_ptr_2 = temp_ptr_2->next_node;
                    }
                    else if (cmp < 0)
                    {
                        print_char_node(temp_ptr_1);
                        temp_ptr_1 = temp_ptr_1->next_node;
                    }
                    else
                    {
                        print_char_node(temp_ptr_2);
                        temp_ptr_2 = temp_ptr_2->next_node;
                    }
                    }
                    else if (temp_ptr_1 != NULL)
                    {
                    print_char_node(temp_ptr_1);
                    temp_ptr_1 = temp_ptr_1->next_node;
                    }
                    else
                    {
                    print_char_node(temp_ptr_2);
                    temp_ptr_2 = temp_ptr_2->next_node;
                    }
                }
                printf("}\n");
                break;

                case '&':
                printf("%c & %c = { ", a_set, b_set);
                
                temp_ptr_1 = sets[a_set - 'A'].first_node->next_node;
                temp_ptr_2 = sets[b_set - 'A'].first_node->next_node;
                
                while (temp_ptr_1 != NULL && temp_ptr_2 != NULL)
                {
                    cmp = (*(char*)temp_ptr_1->data) - (*(char*)temp_ptr_2->data);//memcmp(temp_ptr_1->data, temp_ptr_2->data, sizeof(char*));
                    
                    if (cmp == 0)
                    {
                    print_char_node(temp_ptr_1);
                    temp_ptr_1 = temp_ptr_1->next_node;
                    temp_ptr_2 = temp_ptr_2->next_node;
                    }
                    else if (cmp < 0)
                    {
                    temp_ptr_1 = temp_ptr_1->next_node;
                    }
                    else
                    {
                    temp_ptr_2 = temp_ptr_2->next_node;
                    }
                }
                printf("}\n");
                break;

                case '-':
                printf("%c - %c = { ", a_set, b_set);
                
                temp_ptr_1 = sets[a_set - 'A'].first_node->next_node;
                temp_ptr_2 = sets[b_set - 'A'].first_node->next_node;
                
                while (temp_ptr_1 != NULL && temp_ptr_2 != NULL)
                {
                    int cmp = memcmp(temp_ptr_1->data, temp_ptr_2->data, sizeof(char*));
                    
                    if (cmp == 0)
                    {
                        temp_ptr_1 = temp_ptr_1->next_node;
                        temp_ptr_2 = temp_ptr_2->next_node;
                    }
                    else if (cmp < 0)
                    {
                        print_char_node(temp_ptr_1);
                        temp_ptr_1 = temp_ptr_1->next_node;
                    }
                    else
                    {
                        temp_ptr_2 = temp_ptr_2->next_node;
                    }
                }
                
                while (temp_ptr_1 != NULL)
                {
                    print_char_node(temp_ptr_1);
                    temp_ptr_1 = temp_ptr_1->next_node;
                }
                printf("}\n");
                break;

                case '<':
                temp_ptr_1 = sets[a_set - 'A'].first_node->next_node;
                temp_ptr_2 = sets[b_set - 'A'].first_node->next_node;
                flag = 1;
                
                while (temp_ptr_1 != NULL && temp_ptr_2 != NULL && flag)
                {
                    int cmp = memcmp(temp_ptr_1->data, temp_ptr_2->data, sizeof(char*));
                    
                    if (cmp == 0)
                    {
                        temp_ptr_1 = temp_ptr_1->next_node;
                        temp_ptr_2 = temp_ptr_2->next_node;
                    }
                    else if (cmp > 0)
                    {
                        temp_ptr_2 = temp_ptr_2->next_node;
                    }
                    else
                    {
                        flag = 0;
                    }
                }
                
                if (flag && temp_ptr_1 == NULL)
                {
                    printf("%c is a subset of %c\n", a_set, b_set);
                }
                else
                {
                    printf("%c is not a subset of %c\n", a_set, b_set);
                }
                break;

                case '=':
                flag = 1;
                size_t size_a = u_list_size(&sets[a_set - 'A']);
                size_t size_b = u_list_size(&sets[b_set - 'A']);
                
                if (size_a != size_b) {
                    printf("%c is not equal to %c\n", a_set, b_set);
                    break;
                }
                
                temp_ptr_1 = sets[a_set - 'A'].first_node->next_node;
                temp_ptr_2 = sets[b_set - 'A'].first_node->next_node;
                while (temp_ptr_1 != NULL && temp_ptr_2 != NULL)
                {
                    if (memcmp(temp_ptr_1->data, temp_ptr_2->data, sizeof(char*)) != 0)
                    {
                        printf("%c is not equal to %c\n", a_set, b_set);
                        flag = 0;
                        break;
                    }
                    temp_ptr_1 = temp_ptr_1->next_node;
                    temp_ptr_2 = temp_ptr_2->next_node;
                }
                if (flag)
                {
                    printf("%c is equal to %c\n", a_set, b_set);
                }
                break;
            }
        }
    }

    return 0;
}