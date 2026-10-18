#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "Parser.h"
#include "ArgList.h"
#include "StrArray.h"
#include <stdio.h>

enum Status {
    SUCCESS,
    FAIL,
    WHITESPACE_OR_SEMICOLON_EXPECTED,
};

struct Parser* Parser_new() {
    struct Parser* psr = malloc(sizeof(struct Parser));
    psr->commad = NULL;
    psr->argl = ArgList_new();
    psr->stra = StrArray_new();
    return psr;
}

int Parser_setCmd(struct Parser* p, const char* cmd) {
    if(p == NULL) {return FAIL;}
    p->commad = cmd;
    return SUCCESS;
}

int Parser_slip(struct Parser* p) {
    size_t csize = strlen(p->commad) + 1;

    size_t marker = 0;
    bool on_quotes = false;
    bool marking = false;

    #define CHAR_PTR p->commad[i]
    for(size_t i = 0; i < csize; i++) {
        if(CHAR_PTR == '"') {on_quotes = !on_quotes;}
        if(CHAR_PTR != ' ' && !marking && CHAR_PTR != '"') {
            marker = i;
            marking = true;
            continue;
        }
        else if(marking && !on_quotes && (CHAR_PTR == ' ' || CHAR_PTR == '"' || CHAR_PTR == '\0')) {
            StrArray_append(p->stra, p->commad + marker, i - marker - 1);
            marking = false;
            continue;
        }
    }
    return 0;
}

int Parser_makeArgs(struct Parser* p) {
    if(p == NULL) {return FAIL;}

    const char** str_arr = p->stra->array;

    for (int i = 0; i < p->stra->size; i++) {
        if(str_arr[i][0] == ARG_PREFIX) {
            if(str_arr[i + 1] == NULL) {return 1;}
            ArgList_appendManualKeyArg(p->argl, str_arr[i+1], str_arr[i]);
            i++;
        }
        else {
            ArgList_appendManualPosArg(p->argl, str_arr[i], i);
        }
        continue;
    }
    StrArray_end(p->stra);
    p->stra = NULL;
    return 0;
}


int Parser_end(struct Parser* p) {
    if(p == NULL) {return FAIL;}

    ArgList_end(p->argl);
    StrArray_end(p->stra);
    free(p);
    return SUCCESS;
}
