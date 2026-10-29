#include "CerealShell.h"
#include "Function.h"
#include "Lexer.h"
#include "Parser.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>




static void printError(int err) {
    switch (err) {
        case FAIL:
            printf(FG_RED CEREAL_INPUT_HEAD" Fatal Error" RESET "\n");
            break;
        case EXPECTED_NAME_FUNCTION:
            printf(FG_RED CEREAL_INPUT_HEAD" Syntax Error: Expected Name Function" RESET "\n");
            break;
        case LEXER_ERROR:
            printf(FG_RED CEREAL_INPUT_HEAD" Lexer Error: Invalid Expression" RESET "\n");
            break;
        case EXPECTED_SEMI_COLON:
            printf(FG_RED CEREAL_INPUT_HEAD" Syntax Error: Expected Semi Colon" RESET "\n");
            break;
        case KW_ARGS_ONLY_ERROR:
            printf(FG_RED CEREAL_INPUT_HEAD" Syntax Error: Positional Args after Named Args are not allowed" RESET "\n");
            break;
        case EXPECTED_EXPRESSION:
            printf(FG_RED CEREAL_INPUT_HEAD" Syntax Error: Expected Expression" RESET "\n");
            break;
        case DASH_WITH_UNSIGNED:
            printf(FG_RED CEREAL_INPUT_HEAD" Syntax Error: Cannot convert in negative number with unsigned" RESET "\n");
            break;
        case 10:
            printf(FG_RED CEREAL_INPUT_HEAD" Function Not Found in System" RESET "\n");
            break;
        case 11:
            printf(FG_RED CEREAL_INPUT_HEAD" Function Found, but not exectuted! (NULL function)" RESET "\n");
            break;
        case 12:
            printf(FG_RED CEREAL_INPUT_HEAD" No input! "RESET "\n");
            break;
        case CONTINUE:
        case SUCCESS:
            break;
    }
}


static int Map_init(struct Map* mp, const char* key, WrapperFunction f) {
    if(mp == NULL) {return 1;}

    mp->key = strdup(key);
    mp->wf = f;
    return 0;
}

static int Map_end(struct Map* mp, bool allocated_ptr) {
    if(mp == NULL) {return 1;}

    free( (void*) mp->key);
    if(allocated_ptr) {free(mp);}

    mp->key = NULL;
    mp->wf = NULL;
    return 0;
}


struct CerealShell* CerealShell_new() {
    struct CerealShell* cs = malloc(sizeof(struct CerealShell));
    if (cs == NULL) {return NULL;}

    cs->lxr = Lexer_new();
    if(cs->lxr == NULL) {return NULL;}
    cs->psr = Parser_new();
    if(cs->psr == NULL) {return NULL;}

    cs->allocated = 0;
    cs->size = 0;
    return cs;
}

int CerealShell_end(struct CerealShell* cs) {
    if(cs == NULL) {return 1;}

    for(size_t i = 0; i < cs->size; i++) {
        Map_end(cs->reg_fun + i, false);
    }
    Lexer_end(cs->lxr);
    Parser_end(cs->psr);
    free(cs);
    return 0;
}


static int CerealShell_expand(struct CerealShell* cs) {
    if(cs == NULL) {return 1;}

    struct Map* d;
    if(cs->reg_fun == NULL)
    {
        d = malloc(sizeof(struct Map) * 10);
        if(d == NULL) {return 2;}
        cs->reg_fun = d;
    } else {
        d = realloc(cs->reg_fun, sizeof(struct Map) * (10 + cs->allocated));
        if(d == NULL) {return 2;}
        cs->reg_fun = d;
    }
    cs->allocated += 10;
    return 0;
}


int CerealShell_newFunction(struct CerealShell* cs, const char* key, WrapperFunction f) {
    if(cs == NULL) {return 1;}

    if(cs->size + 1 > cs->allocated) {
        if(CerealShell_expand(cs) != 0) {return 2;}
    }

    struct Map* n_l = &cs->reg_fun[cs->size];
    int r = {0};
    r = Map_init(n_l, key, f);
    if(r != 0) {return 0;}

    cs->size += 1;
    return r;
}


const struct Map* CerealShell_getFunction(struct CerealShell* cs, const char* key) {
    if(cs == NULL) {return NULL;}

    for(size_t i = 0; i < cs->size; i++) {
        if (strcmp(key, cs->reg_fun[i].key) == 0) {
            return cs->reg_fun + i;
        }
    }
    return NULL;
}


int CerealShell_reset(struct CerealShell* cs) {
    if(cs == NULL) {return 1;}

    Parser_end(cs->psr);
    cs->psr = Parser_new();
    Lexer_end(cs->lxr);
    cs->lxr = Lexer_new();
    return 0;
}


static int __CerealShell_start(struct CerealShell* cs, const char* input) {
    int status;
    status = Lexer_setInput(cs->lxr, input);
    if(status != 0) {return status;}

    status = Lexer_start(cs->lxr);
    if(status != 0) {return status;}

    status = Parser_Start(cs->psr, cs->lxr);
    if(status != 0) {return status;}

    struct Function* fncs = cs->psr->functions;
    for(size_t i = 0; i < cs->psr->size; i++) {
        const struct Map* fmap = CerealShell_getFunction(cs, fncs[i].name);
        if(fmap != NULL) {
            if(fmap->wf == NULL) {return 11;}
            fmap->wf(fncs + i);
        }
        else {return 10;}
    }
    return 0;
}

int CerealShell_start(struct CerealShell* cs, const char* input) {
    int status;
    if(strlen(input) == 0) {return 12;}
    status = __CerealShell_start(cs, input);
    CerealShell_reset(cs);
    return status;
}


void CerealShell_initLoop(struct CerealShell* cs) {
    size_t ss = 0;
    char* inp = NULL;
    while (1) {
        printf(FG_BLUE CEREAL_INPUT_HEAD RESET);
        getline(&inp, &ss, stdin);

        int end = CerealShell_start(cs, inp);
        if(end > 0) {printError(end);}

        if(strcmp(inp, "quit") == 0) {return;}
        free(inp);
        inp = NULL;
    }
}
