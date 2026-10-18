#ifndef PARSER_HPP
#define PARSER_HPP

#include <stddef.h>
#include <stdbool.h>
#include "ArgList.h"
#include "StrArray.h"

#define ARG_PREFIX '-'


struct Parser {
    const char* commad;

    struct StrArray* stra;
    struct ArgList* argl;
};


struct Parser* Parser_new();

int Parser_setCmd(struct Parser* p, const char* cmd);
int Parser_slip(struct Parser* p);
int Parser_makeArgs(struct Parser* p);
int Parser_analyzeArg(struct Parser* p);

int Parser_end(struct Parser* p);

#endif
