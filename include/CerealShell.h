#include "Lexer.h"
#include "Parser.h"
#include "Function.h"


typedef void (*WrapperFunction) (struct Function*);


struct Map {
    const char* key;
    WrapperFunction wf;
};


struct CerealShell {
    struct Lexer* lxr;
    struct Parser* psr;

    struct Map regfun;
};


struct CerealShell* CerealShell_new();
int CerealShell_end(struct CerealShell* cs);
