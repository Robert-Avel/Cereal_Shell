#ifndef NARGTOKEM_HPP
#define NARGTOKEM_HPP

#include <stdbool.h>
#include <stdlib.h>

#define _SET_ARG_SIGN(type, func_ext)                                \
    type* NArgToken_getArg_##func_ext (const struct NArgToken* nat)


#define _SET_ARG_BODY(type, enum_type, union_var)                    \
    _SET_ARG_SIGN(type, union_var) {                                 \
        if(nat == NULL || nat->arg_type != enum_type) {return NULL;} \
        return &nat->arg.union_var;                                  \
    }

typedef unsigned int position;

enum KeyType {
    NONE,
    POSITIONAL,
    NAMED
};

union Key {
    position pos;
    const char* named;
};

enum DataType {
    SHORT,
    INT,
    LONG,
    LONG_LONG,
    UNSIGNED_SHORT,
    UNSIGNED_INT,
    UNSIGNED_LONG,
    UNSIGNED_LONG_LONG,
    STRING,
    BOOL,
    VOID
};


union DynamicArg {
    short                    s;
    int                      i;
    long                     l;
    long long               ll;
    unsigned short          us;
    unsigned int            ui;
    unsigned long           ul;
    unsigned long long     ull;
    const char*             cc;
    bool                     b;
};

struct NArgToken {
    enum KeyType      key_type;
    union Key              key;

    enum DataType     arg_type;
    union DynamicArg       arg;
};


struct NArgToken* NArgToken_new();

int NArgToken_resetKey(struct NArgToken* nat);
int NArgToken_setKey(struct NArgToken* nat, enum KeyType kt, const void* key);
int NArgToken_setKeyString(struct NArgToken* nat, const char* key);
int NArgToken_setConvertKey(struct NArgToken* nat, enum KeyType kt, const char* key);

const char** NArgToken_getKeyNamed(struct NArgToken* nat);
position* NArgToken_getKeyPositional(struct NArgToken* nat);

int NArgToken_resetArg(struct NArgToken* nat);
int NArgToken_setArg(struct NArgToken* nat, enum DataType at, const void* arg);
int NArgToken_setString(struct NArgToken* nat, const char* arg);
int NArgToken_setConvertArg(struct NArgToken* nat, enum DataType at, const char* arg);


int NArgToken_setAllConvert(struct NArgToken* nat, enum KeyType kt, const char* key, enum DataType at, const char* arg);

struct NArgToken* NArgToken_newConstruct(enum KeyType kt, const void* key, enum DataType at, const void* arg);
int NArgToken_end(struct NArgToken* nat);


_SET_ARG_SIGN(const short,              s);
_SET_ARG_SIGN(const int,                i);
_SET_ARG_SIGN(const long,               l);
_SET_ARG_SIGN(const long long,          ll);
_SET_ARG_SIGN(const unsigned short,     us);
_SET_ARG_SIGN(const unsigned int,       ui);
_SET_ARG_SIGN(const unsigned long,      ul);
_SET_ARG_SIGN(const unsigned long long, ull);
_SET_ARG_SIGN(const char* const,        cc);
_SET_ARG_SIGN(const bool,               b);

#endif
