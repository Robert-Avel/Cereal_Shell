#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "Parser.h"
#include "ArgList.h"
#include "ArgToken2.h"
#include "StrArray.h"
#include <stdio.h>

enum Status {
    SUCCESS,
    FAIL,
    CRITICAL,
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


bool checkString(const char* cc, size_t size) {
    bool is_valid = false;
    for(size_t i = 0; i < size; i++) {
        if(cc[i] == ' ' || cc[i] == '\n' || cc[i] == '\0')
        {} else is_valid = true;
    }
    return is_valid;
}


int Parser_slip(struct Parser* p) {
    size_t csize = strlen(p->commad) + 1;

    size_t marker = 0;
    bool on_quotes = false;
    bool marking = false;

    #define CHAR_PTR p->commad[i]
    for(size_t i = 0; i < csize; i++) {
        if(CHAR_PTR == '\n') {continue;}
        if(CHAR_PTR == '"') {on_quotes = !on_quotes;}
        if(CHAR_PTR != ' ' && !marking && CHAR_PTR != '"') {
            marker = i;
            marking = true;
        }
        else if(marking && !on_quotes && (CHAR_PTR == ' ' || CHAR_PTR == '"' || CHAR_PTR == '\0')) {
            if(checkString(p->commad + marker, i - marker - 1)) {
                StrArray_append(p->stra, p->commad + marker, i - marker - 1);
                marking = false;
            }
        }
    }
    return 0;
}

int Parser_makeArgs(struct Parser* p) {
    if(p == NULL) {return FAIL;}

    const char** str_arr = p->stra->array;

    for (int i = 0; i < p->stra->size; i++) {
        if(str_arr[i][0] == ARG_PREFIX) {
            if(str_arr[i + 1] == NULL) {return FAIL;}
            ArgList_appendManual(p->argl, NAMED, &str_arr[i], STRING, &str_arr[i+1]);
            i++;
        }
        else {
            ArgList_appendManual(p->argl, POSITIONAL, &i, STRING, &str_arr[i]);
        }

        continue;
    }

    //StrArray_end(p->stra);
    //p->stra = NULL;
    return 0;
}


int Parser_analyzeArg(struct Parser* p) {
    if(p == NULL) {return FAIL;}
    struct ArgListIterator ali = {0};
    ArgListIterator_init(&ali, p->argl);

    bool only_keys = false;
    do { //Checa argumentos posicionais depois de argumentos nomeados
        if(ali.iterator->at->key_type == NAMED) {
            only_keys = true;
        }
        if(ali.iterator->at->key_type == POSITIONAL && only_keys) {
            Parser_end(p);
            return CRITICAL;
        }
    } while(ArgListIterator_jump(&ali) != 5);
    return SUCCESS;
}


int Parser_start(struct Parser* p, const char* cmd) {
    ArgList_end(p->argl);
    StrArray_end(p->stra);

    p->argl = ArgList_new();
    p->stra = StrArray_new();

    Parser_setCmd(p, cmd);
    Parser_slip(p);
    Parser_makeArgs(p);
    return Parser_analyzeArg(p);
}


int Parser_end(struct Parser* p) {
    if(p == NULL) {return FAIL;}

    ArgList_end(p->argl);
    StrArray_end(p->stra);
    free(p);
    return SUCCESS;
}
