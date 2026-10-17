#include "ArgToken.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct PosArg* ArgToken_setPosArg(struct ArgToken* at, int pos, const char* value) {
    if(at == NULL) {return NULL;}

    at->type = POSITIONAL;

    struct PosArg* pa = &at->arg.pa;
    pa->pos = pos;
    pa->value = strdup(value);
    return pa;
}

struct PosArg* ArgToken_setPosArgSized(struct ArgToken* at, int pos, const char* value, size_t value_size) {
    char* buf = malloc(sizeof(char) * value_size + 1);
    strncpy(buf, value, value_size);
    buf[value_size] = '\0';

    struct PosArg* p = ArgToken_setPosArg(at, pos, buf);
    free(buf);
    return p;
}

struct KeyArg* ArgToken_setKeyArg(struct ArgToken* at, const char* key, const char* value) {
    if(at == NULL) {return NULL;}

    at->type = KEY;

    struct KeyArg* ka = &at->arg.ka;
    ka->key = strdup(key);
    ka->value = strdup(value);
    return ka;
}

struct KeyArg* ArgToken_setKeyArgSized(struct ArgToken* at, const char* key, size_t key_size, const char* value, size_t value_size) {
    char* bufk = malloc(sizeof(char) * key_size + 1);
    char* bufv = malloc(sizeof(char) * value_size + 1);
    strncpy(bufk, key, key_size);
    strncpy(bufv, value, value_size);
    bufk[value_size] = '\0';
    bufv[value_size] = '\0';

    struct KeyArg* ka = ArgToken_setKeyArg(at, bufk, bufv);
    free(bufk);
    free(bufv);
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
    switch (at->type) {
        case POSITIONAL:
            free( (void*) at->arg.pa.value);
            break;
        case KEY:
            free( (void*) at->arg.ka.key);
            free( (void*) at->arg.ka.value);
        case NONE:
            break;
    }

    free(at);
    return 0;
}
