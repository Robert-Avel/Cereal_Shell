#include "Lexer.h"
#include "Parser.h"
#include "Function.h"
#include <stdbool.h>
#include <stddef.h>

#define CEREAL_INPUT_HEAD "Cereal_Shell >"


#define CS_BOOL BOOL
#define CS_STRING STRING
#define CS_LONG LONG
#define CS_UNSIGNED_LONG UNSIGNED_LONG


typedef void (*WrapperFunction) (const struct Function*);


struct Map {
    const char* key;
    WrapperFunction wf;
};


int Map_init(struct Map* mp, const char* key, WrapperFunction f);
int Map_end(struct Map* mp, bool allocated_ptr);


struct CerealShell {
    struct Lexer* lxr;
    struct Parser* psr;

    struct Map* reg_fun;
    size_t allocated;
    size_t size;
};


struct CerealShell* CerealShell_new();
int CerealShell_end(struct CerealShell* cs);
int CerealShell_newFunction(struct CerealShell* cs, const char* key, WrapperFunction f);
const struct Map* CerealShell_getFunction(struct CerealShell* cs, const char* key);
int CerealShell_start(struct CerealShell* cs, const char* input);
void CerealShell_initLoop(struct CerealShell* cs);
