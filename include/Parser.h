#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ArgList.h"

#define ARG_PREFIX '-'

struct Parser {
    const char* commad;
    size_t cursor;

    struct ArgList* argl;
};


struct Parser* Parser_new();
int Parser_setCmd(struct Parser* p, char* cmd);
char Parser_stepChar(struct Parser* p);
int Parser_end(struct Parser* p);
