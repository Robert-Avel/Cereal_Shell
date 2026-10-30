#ifndef FUNCTION_H
#define FUNCTION_H

#include "ArgToken2.h"
#include <ArgList.h>
#include <stddef.h>

struct Function {
    const char* name;
    struct ArgList* args;
};


struct Function* Function_new(const char* name);
int Function_init(struct Function* f, const char* name);
int Function_end(struct Function* f, bool allocated_ptr);


const struct NArgToken* Function_getArgPositional(const struct Function* f, position p);
const struct NArgToken* Function_getArgNamed(const struct Function* f, const char* p);


#endif
