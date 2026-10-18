#ifndef ARGTOKEM_HPP
#define ARGTOKEM_HPP

#include <stdlib.h>

typedef enum {
    NONE,
    POSITIONAL,
    KEY
} ArgType;


struct KeyArg {
    const char* key;
    const char* value;
};


struct PosArg {
    int pos;
    const char* value;
};

struct ArgToken {
    ArgType type;
    union {
        struct PosArg pa;
        struct KeyArg ka;
    } arg;
};



struct ArgToken* ArgToken_new();
int ArgToken_end(struct ArgToken* at);

struct PosArg* ArgToken_setPosArg(struct ArgToken* at, int pos, const char* value);
struct PosArg* ArgToken_setPosArgSized(struct ArgToken* at, int pos, const char* value, size_t value_size);

struct KeyArg* ArgToken_setKeyArg(struct ArgToken* at, const char* key, const char* value);
struct KeyArg* ArgToken_setKeyArgSized(struct ArgToken* at, const char* key, size_t key_size, const char* value, size_t value_size);

const struct PosArg* ArgToken_getPosArg(const struct ArgToken* at);
const struct KeyArg* ArgToken_getKeyArg(const struct ArgToken* at);

#endif
