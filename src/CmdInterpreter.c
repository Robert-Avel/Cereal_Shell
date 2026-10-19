#include "CmdIntepeter.h"
#include "Parser.h"
#include <stdlib.h>

struct CmdIntrepeter* CmdIntrepeter_new() {
    struct CmdIntrepeter* ci = malloc(sizeof(struct CmdIntrepeter));
    ci->psr = Parser_new();
    ci ->cmd = NULL;
    return ci;
}
int CmdIntrepeter_end(struct CmdIntrepeter* ci) {
    if (ci == NULL) {return 1;}
    Parser_end(ci->psr);
    return 0;
}

int CmdIntrepeter_parseCommand(struct CmdIntrepeter* ci , const char* text) {
    Parser_setCmd(ci->psr, text);
    Parser_makeArgs(ci->psr);
    return Parser_analyzeArg(ci->psr);
}
