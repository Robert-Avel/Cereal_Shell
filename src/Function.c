#include "Function.h"
#include "ArgList.h"
#include <stdlib.h>
#include <string.h>


struct Function* Function_new(const char* name) {
    if (name == NULL) {return NULL;}
    struct Function* _nf = malloc(sizeof(struct Function));
    _nf->name = strdup(name);
    _nf->args = ArgList_new();
    return _nf;
}

int Function_init(struct Function* f, const char* name) {
    if(f == NULL) {return 1;}

    f->name = strdup(name);
    f->args = ArgList_new();
    return 1;
}


int Function_end(struct Function* f, bool allocated_ptr) {
    if (f == NULL) {return 1;}
    free((char*) f->name);
    ArgList_end(f->args);
    if(allocated_ptr) {free(f);}
    return 0;
}
