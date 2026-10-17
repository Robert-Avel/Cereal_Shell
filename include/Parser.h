#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ArgList.h"

#define ARG_PREFIX '-'


struct Parser {
    const char* commad;
    const char* main_cmd;
    size_t cursor;

    struct ArgList* argl;
};


struct Parser* Parser_new();
int Parser_setCmd(struct Parser* p, const char* cmd);
char Parser_stepChar(struct Parser* p);
int Parser_end(struct Parser* p);

int Parser_checkChar(struct Parser* p, const char c);

int Parser_stepMainCommand(struct Parser* p);

int Parser_start(struct Parser* p);
