#ifndef STRARRAY_HPP
#define STRARRAY_HPP

#include <stddef.h>

struct StrArray {
    const char** array;
    size_t allocated;
    size_t size;
};


struct StrArray* StrArray_new();
int StrArray_end(struct StrArray* sa);
int StrArray_expand(struct StrArray* sa);

int StrArray_append(struct StrArray* sa, const char* str, size_t str_size);

#endif
