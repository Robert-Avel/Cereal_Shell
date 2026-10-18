#include "StrArray.h"
#include <stdlib.h>
#include <string.h>

struct StrArray* StrArray_new() {
    struct StrArray* sa = malloc(sizeof (struct StrArray));
    sa->array = NULL;
    sa->allocated = 0;
    sa->size = 0;
    return sa;
}

int StrArray_expand(struct StrArray* sa) {
    if (sa->array == NULL) {
        sa->array = malloc(sizeof(char*) * 10);
        if(sa->array == NULL) {return 1;}
    } else {
        const char** n_c = realloc(sa->array, sa->allocated + 10);
        if(n_c == NULL) {return 1;}
        sa->array = n_c;
    }
    sa->allocated += 10;
    return 0;
}

int StrArray_append(struct StrArray* sa, const char* str, size_t str_size) {
    if (sa->size + 1 > sa->allocated) {
        StrArray_expand(sa);
    }
    char* n_str = malloc(sizeof(char) * str_size + 1);
    n_str[str_size] = '\0';
    strncpy(n_str, str, str_size + 1);
    sa->array[sa->size] = n_str;
    sa->size += 1;
    return 0;
}

int StrArray_end(struct StrArray* sa) {
    if(sa == NULL) {return 1;}

    for (int i = 0; i < sa->size; i++) {
        free((void*) sa->array[i]);
    }
    free(sa->array);
    return 0;
}
