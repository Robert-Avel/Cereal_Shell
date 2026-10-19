#include <stdlib.h>
#include <string.h>
#include "ArgToken2.h"

struct NArgToken* NArgToken_new() {
    struct NArgToken* nat = malloc(sizeof(struct NArgToken));
    nat->arg_type = VOID;
    nat->key_type = NONE;
    return nat;
}

int NArgToken_resetKey(struct NArgToken* nat) {
    if (nat == NULL) {return 2;}
    switch (nat->key_type) {
        case NAMED:
            free((void*) nat->key.named);
            break;

        case NONE:
            return 1;

        case POSITIONAL:
            break;
    }
    nat->key_type = NONE;
    return 0;
}

int NArgToken_setKey(struct NArgToken* nat, const void* key, enum KeyType kt) {
    if (nat == NULL) {return 2;}
    NArgToken_resetKey(nat);

    switch (nat->key_type) {
        case NAMED:
            nat->key.named = strdup((const char*) key);
            break;
        case POSITIONAL:
            nat->key.pos = *(position*) key;
            break;
        case NONE:
        default:
            return 1;
    }
    nat->key_type = kt;
    return 0;
}

int NArgToken_resetArg(struct NArgToken* nat) {
    if (nat == NULL) {return 2;}
    if (nat->arg_type == VOID) {return 1;}
    else if(nat->arg_type == STRING) {free((void*) nat->arg.cc);}

    nat->arg_type = VOID;
    return 0;
}


int NArgToken_setArg(struct NArgToken* nat, const void* arg, enum DataType at) {
    if (nat == NULL) {return 2;}
    NArgToken_resetArg(nat);

    switch (at) {
        case SHORT:
            nat->arg.s = *(short*) arg;
            break;
        case INT:
            nat->arg.i = *(int*) arg;
            break;
        case LONG:
            nat->arg.l = *(long*) arg;
            break;
        case LONG_LONG:
            nat->arg.ll = *(long long*) arg;
            break;
        case UNSIGNED_SHORT:
            nat->arg.us = *(unsigned short*) arg;
            break;
        case UNSIGNED_INT:
            nat->arg.ui = *(unsigned int*) arg;
            break;
        case UNSIGNED_LONG:
            nat->arg.ul = *(unsigned long*) arg;
            break;
        case UNSIGNED_LONG_LONG:
            nat->arg.ull = *(unsigned long long*) arg;
            break;
        case STRING:
            nat->arg.cc = strdup((const char*) arg);
            break;
        case VOID:
        default:
            return 1;
    }
    nat->arg_type = at;
    return 0;
}
int NArgToken_convertArg(struct NArgToken* nat, const char* arg, enum DataType at) {
    if (nat == NULL) {return 2;}
    const char* tmp = strdup(arg);
    if(tmp == NULL) {return 3;}

    NArgToken_resetArg(nat);
    switch (at) {
        case SHORT:
            nat->arg.s = (short) strtol(tmp, NULL, 10);
            break;
        case INT:
            nat->arg.i = (int) strtol(tmp, NULL, 10);
            break;
        case LONG:
            nat->arg.l = (long) strtol(tmp, NULL, 10);
            break;
        case LONG_LONG:
            nat->arg.ll = (long long) strtoll(tmp, NULL, 10);
            break;
        case UNSIGNED_SHORT:
            nat->arg.us = (unsigned short) strtoul(tmp, NULL, 10);
            break;
        case UNSIGNED_INT:
            nat->arg.ui = (unsigned int) strtoul(tmp, NULL, 10);
            break;
        case UNSIGNED_LONG:
            nat->arg.ul = (unsigned long) strtoul(tmp, NULL, 10);
            break;
        case UNSIGNED_LONG_LONG:
            nat->arg.ull = (unsigned long long) strtoull(tmp, NULL, 10);
            break;
        case STRING:
            nat->arg.cc = tmp;
            break;
        case VOID:
        default:
            return 1;
    }
    if (at != STRING) {free( (void*) tmp);}
    return 0;
}


int NArgToken_end(struct NArgToken* nat) {
    if (nat == NULL) {return 1;}
    NArgToken_resetArg(nat);
    NArgToken_resetKey(nat);
    free(nat);
    return 0;
}


_SET_ARG_BODY(short,              SHORT,              s)
_SET_ARG_BODY(int,                INT,                i)
_SET_ARG_BODY(long,               LONG,               l)
_SET_ARG_BODY(long long,          LONG_LONG,          ll)
_SET_ARG_BODY(unsigned short,     UNSIGNED_SHORT,     us)
_SET_ARG_BODY(unsigned int,       UNSIGNED_INT,       ui)
_SET_ARG_BODY(unsigned long,      UNSIGNED_LONG,      ul)
_SET_ARG_BODY(unsigned long long, UNSIGNED_LONG_LONG, ull)
_SET_ARG_BODY(const char*,        STRING,             cc)
_SET_ARG_BODY(bool,               BOOL,               b)
