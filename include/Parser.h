#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <stdbool.h>
#include "Lexer.h"
#include "ArgToken2.h"


enum __Status {
    CONTINUE = -1,
    SUCCESS,
    FAIL,
    EXPECTED_NAME_FUNCTION,
    LEXER_ERROR,
    EXPECTED_SEMI_COLON,
    KW_ARGS_ONLY_ERROR,
    EXPECTED_EXPRESSION,
};


struct Parser {
    size_t allocated;
    size_t size;
    struct Function* functions;
};


struct Parser* Parser_new();

int Parser_expand(struct Parser* p);
int Parser_append(struct Parser* fm, struct Function* f);
int _Parser_PosArgAnalyze(struct Function* fn, struct Lexemas* lxm, size_t* cursor, position* i_p);
int _Parser_KeyArgAnalyze(struct Function* fn, struct Lexemas* lxm, size_t* cursor, const char* key);
void _Parser_NegativeNum(struct Function* fn, struct Lexemas* lxm, size_t* cursor, position* i_p);
int _Parser_FunctionLoop(struct Parser* p, struct Lexer* lx, size_t* cursor);
int Parser_Start(struct Parser* p, struct Lexer* lx);
enum LexerToken peek(const struct Lexemas* lxm);
enum LexerToken peekNext(const struct Lexemas* lxm);
int Parser_end(struct Parser* p);

#endif
