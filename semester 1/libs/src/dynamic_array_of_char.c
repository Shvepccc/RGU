#include "../../libs/dynamic_array_of_char.h"
#include "../../libs/error.h"
#include <stdlib.h>

int initialize_dynamic_array_of_char(p_dynamic_array_of_char target) {
    if (!target) {
        return NULL_POINTER;
    }

    target->current_degree_of_2 = 4;

    target->start = (char*)malloc(sizeof(char) * (1 << target->current_degree_of_2));
    if (!target->start) {
        return MEMORY_ALLOCATE_ERROR;
    }

    target->current = target->start;
    return OK;
}

int add_char_to_end_of_dynamic_array(p_dynamic_array_of_char target, char c) {
    char* reallocated;

    if (!target) {
        return NULL_POINTER;
    }

    if ((target->current - target->start) == (1 << target->current_degree_of_2)) {

        reallocated = (char*)realloc(target->start, sizeof(char) * (1 << (target->current_degree_of_2 + 1)));
        if (!reallocated) {
            return MEMORY_ALLOCATE_ERROR;
        }

        target->current = (target->start = reallocated) + sizeof(char) * (1 << target->current_degree_of_2++);
    }

    *(target->current)++ = c;
    return OK;
}

int squeeze_dynamic_array_of_char(p_dynamic_array_of_char target) {
    char* reallocated = (char*)realloc(target->start, sizeof(char) * (target->current - target->start));
    if (!reallocated) {
        return MEMORY_ALLOCATE_ERROR;
    }

    target->start = reallocated;
    return OK;
}

int deinitialize_dynamic_array_of_char(p_dynamic_array_of_char target) {
    free(target->start);
    return OK;
}