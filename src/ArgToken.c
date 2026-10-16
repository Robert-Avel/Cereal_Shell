#include "ArgToken.h"
#include <stdlib.h>

struct PosArg* ArgToken_setPosArg(struct ArgToken* at, int pos, char* value) {
    if(at == NULL) {return NULL;}

    at->type = POSITIONAL;

    struct PosArg* pa = &at->arg.pa;
    pa->pos = pos;
    pa->value = value;
    return pa;
}

struct KeyArg* ArgToken_setKeyArg(struct ArgToken* at, char* key, char* value) {
    if(at == NULL) {return NULL;}

    at->type = KEY;

    struct KeyArg* ka = &at->arg.ka;
    ka->key = key;
    ka->value = value;
    return ka;
}



const struct PosArg* ArgToken_getPosArg(const struct ArgToken* at) {
    if(at == NULL || at->type != POSITIONAL) {return NULL;}

    return &at->arg.pa;
}

const struct KeyArg* ArgToken_getKeyArg(const struct ArgToken* at) {
    if(at == NULL || at->type != KEY) {return NULL;}

    return &at->arg.ka;
}


struct ArgToken* ArgToken_new() {
    struct ArgToken* at = (struct ArgToken*) malloc(sizeof(struct ArgToken));
    at->type = NONE;
    return at;
}

int ArgToken_end(struct ArgToken* at) {
    if(at == NULL) {return 1;}
    free(at);
    return 0;
}
