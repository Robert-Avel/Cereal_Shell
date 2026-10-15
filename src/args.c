#include "cmd_args.h"
#include <stdlib.h>

PosArg* ArgToken_setPosArg(ArgToken* at, int pos, char* value) {
    if(at == NULL) {return NULL;}

    at->type = POSITIONAL;

    PosArg* pa = &at->arg.pa;
    pa->pos = pos;
    pa->value = value;
    return pa;
}

KeyArg* ArgToken_setKeyArg(ArgToken* at, char* key, char* value) {
    if(at == NULL) {return NULL;}

    at->type = KEY;

    KeyArg* ka = &at->arg.ka;
    ka->key = key;
    ka->value = value;
    return ka;
}



PosArg* ArgToken_getPosArg(ArgToken* at) {
    if(at == NULL || at->type != POSITIONAL) {return NULL;}

    return &at->arg.pa;
}

KeyArg* ArgToken_getKeyArg(ArgToken* at) {
    if(at == NULL || at->type != KEY) {return NULL;}

    return &at->arg.ka;
}


ArgToken* ArgToken_new() {
    ArgToken* at = (ArgToken*) malloc(sizeof(ArgToken));
    at->type = NONE;
    return at;
}

int ArgToken_end(ArgToken* at) {
    if(at == NULL) {return 1;}
    free(at);
    return 0;
}
