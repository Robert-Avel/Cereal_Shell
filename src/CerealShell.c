#include "CerealShell.h"
#include "Lexer.h"
#include "Parser.h"
#include <stdlib.h>


struct CerealShell* CerealShell_new() {
    struct CerealShell* cs = malloc(sizeof(struct CerealShell));
    if (cs == NULL) {return NULL;}

    cs->lxr = Lexer_new();
    if(cs->lxr == NULL) {return NULL;}
    cs->psr = Parser_new();
    if(cs->psr == NULL) {return NULL;}
    return cs;
}

int CerealShell_end(struct CerealShell* cs) {
    if(cs == NULL) {return 1;}

    Lexer_end(cs->lxr);
    Parser_end(cs->psr);
    return 0;
}
