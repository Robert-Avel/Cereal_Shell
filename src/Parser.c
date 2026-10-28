#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "Parser.h"
#include "ArgList.h"
#include "ArgToken2.h"
#include <stdio.h>
#include "Function.h"
#include "Lexer.h"

struct Parser* Parser_new() {
    struct Parser* psr = malloc(sizeof(struct Parser));
    psr->functions = NULL;
    psr->allocated = 0;
    psr->size = 0;
    return psr;
}


int Parser_expand(struct Parser* p) {
    if (p->functions == NULL) {
        p->functions = malloc(sizeof(struct Function) * 10);
        if(p->functions == NULL) {return 1;}
    } else {
        struct Function* n_c = realloc(p->functions, sizeof(struct Function) * p->allocated + 10);
        if(n_c == NULL) {return 1;}
        p->functions = n_c;
    }
    p->allocated += 10;
    return 0;
}

int Parser_append(struct Parser* fm, struct Function* f) {
    if (fm->size + 1 > fm->allocated) {
        if(Parser_expand(fm) != 0) {return 1;}
    }

    fm->functions[fm->size] = *f;
    fm->size += 1;
    return 0;
}


enum LexerToken peek(const struct Lexemas* lxm) {
    return lxm->lt;
}

enum LexerToken peekNext(const struct Lexemas* lxm) {
    return (lxm + 1)->lt;
}


int _Parser_PosArgAnalyze(struct Function* fn, struct Lexemas* lxm, size_t* cursor, position* i_p) {
    switch (peek(lxm)) {
        case T_TEXT:
            ArgList_appendManual(fn->args, POSITIONAL, i_p, STRING, &lxm->var);
            break;
        case T_NUM:;
            unsigned long var = strtol(lxm->var, NULL, 10);
            ArgList_appendManual(fn->args, POSITIONAL, i_p, UNSIGNED_LONG, &var);
            break;
        case T_TRUE:
        case T_FALSE:
            bool v = lxm->lt == T_TRUE;
            ArgList_appendManual(fn->args, POSITIONAL, i_p, BOOL, &v);
            break;
        default:
            return FAIL;
    }
    if(cursor != NULL) {*cursor += 1;}
    *i_p += 1;
    return CONTINUE;
}


int _Parser_KeyArgAnalyze(struct Function* fn, struct Lexemas* lxm, size_t* cursor, const char* key) {
    switch (peek(lxm)) {
        case T_TEXT:
            ArgList_appendManual(fn->args, NAMED, &key, STRING, &lxm->var);
            break;
        case T_NUM:;
            unsigned long var = strtoul(lxm->var, NULL, 10);
            ArgList_appendManual(fn->args, NAMED, &key, UNSIGNED_LONG, &var);
            break;
        case T_TRUE:
        case T_FALSE:
            bool v = peek(lxm) == T_TRUE;
            ArgList_appendManual(fn->args, NAMED, &key, BOOL, &v);
            break;
        case T_DASH:
            if(peekNext(lxm) == T_NUM) {
                long var = strtol(lxm->var, NULL, 10);
                var *= -1;
                ArgList_appendManual(fn->args, NAMED, &key, LONG, &var);
                if(cursor != NULL) {*cursor += 1;}
                break;
            }
        case T_NULL:
            return LEXER_ERROR;
        case T_SEMI_COLON:
        case T_EOF:
            return EXPECTED_EXPRESSION;
    }
    if(cursor != NULL) {*cursor += 3;}
    return CONTINUE;
}



void _Parser_NegativeNum(struct Function* fn, struct Lexemas* lxm, size_t* cursor, position* i_p) {
    long var = strtol(lxm->var, NULL, 10);
    var *= -1;
    ArgList_appendManual(fn->args, POSITIONAL, i_p, LONG, &var);
    if(cursor != NULL) {*cursor += 2;}
    *i_p += 1;
}

int _Parser_FunctionLoop(struct Parser* p, struct Lexer* lx, size_t* cursor) {
    struct Function fn;
    Function_init(&fn, (lx->lxmas + *cursor)->var);
    *cursor += 1;

    int r = -1;
    position i_p = 0;
    bool only_kargs = false;

    while (1) {
        struct Lexemas* pointed_lxm = lx->lxmas + *cursor;
        switch (peek(pointed_lxm)) {
            case T_TEXT:
            case T_NUM:
            case T_TRUE:
            case T_FALSE:
                if(only_kargs) {return KW_ARGS_ONLY_ERROR;}
                r = _Parser_PosArgAnalyze(&fn, pointed_lxm, cursor, &i_p);
                break;
            case T_DASH:
                if (peekNext(pointed_lxm) == T_NUM) {
                    if(only_kargs) {return r = KW_ARGS_ONLY_ERROR; break;}

                    _Parser_NegativeNum(&fn, pointed_lxm + 1, cursor, &i_p);
                }
                else if (peekNext(pointed_lxm) == T_TEXT) {
                    r = _Parser_KeyArgAnalyze(&fn, pointed_lxm + 2, cursor, (pointed_lxm + 1)->var);
                    only_kargs = true;
                }
                else {r = EXPECTED_EXPRESSION;}
                break;
            case T_NULL:
                r = LEXER_ERROR;
                break;
            case T_SEMI_COLON:
                Parser_append(p, &fn);
                r = SUCCESS;
                break;
            case T_EOF:
                r = EXPECTED_SEMI_COLON;
                break;
        }
        if(r > 0) {
            Function_end(&fn, false);
            break;
        } else if(r == 0) {
            *cursor += 1;
            break;
        }
    }
    return r;
}

int Parser_reset(struct Parser* p) {
    if(p == NULL) {return FAIL;}
    for(size_t i = 0; i < p->size; i++) {
        Function_end((p + i)->functions, false);
    }
    free(p->functions);

    p->functions = NULL;
    p->allocated = 0;
    p->size = 0;
    return 0;
}

int Parser_Start(struct Parser* p, struct Lexer* lx) {
    if(lx == NULL) {return FAIL;}
    Parser_reset(p);

    size_t cursor = 0;
    while (1) {
        struct Lexemas* lxm = lx->lxmas + cursor;

        if(lxm->lt == T_TEXT) {
            if(lxm->var == NULL) {return EXPECTED_NAME_FUNCTION;}
            int r = _Parser_FunctionLoop(p, lx, &cursor);
            if(r != 0) {return r;}
        }

        else if(lxm->lt == T_EOF) {
            break;
        } else return EXPECTED_NAME_FUNCTION; //Syntax Error
    }

    return SUCCESS;
}


int Parser_end(struct Parser* p) {
    if(p == NULL) {return FAIL;}

    for(size_t i = 0; i < p->size; i++) {
        Function_end((p + i)->functions, false);
    }
    free(p->functions);
    free(p);
    return SUCCESS;
}
