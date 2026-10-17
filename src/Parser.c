#include <stddef.h>
#include <stdlib.h>
#include "Parser.h"

enum Status {
    SUCCESS,
    FAIL,
    WHITESPACE_EXPECTED,
};

struct Parser* Parser_new() {
    struct Parser* psr = malloc(sizeof(struct Parser));
    psr->commad = NULL;
    psr->cursor = 0;
    psr->argl = ArgList_new();
    return psr;
}

int Parser_setCmd(struct Parser* p, const char* cmd) {
    if(p == NULL) {return FAIL;}
    p->commad = cmd;
    p->cursor = 0;
    return SUCCESS;
}

char Parser_stepChar(struct Parser* p) {
    if(p == NULL) {return '\0';}
    return p->commad[p->cursor++];
}

int Parser_end(struct Parser* p) {
    if(p == NULL) {return FAIL;}

    ArgList_end(p->argl);
    free(p);
    return SUCCESS;
}


int Parser_stepMainCommand(struct Parser* p) {
    if(p->cursor != 0) {return FAIL;}
    char buffer[100];


    for(size_t i = 0; ;i++) {
        buffer[i] = Parser_stepChar(p);
        if (buffer[i] == '\0' || buffer[i] == ' ') {
            break;
        }
    }

}



int Parser_start(struct Parser* p) {
    if(p->commad == NULL) {return 1;}
    Parser_stepMainCommand(p);
}
