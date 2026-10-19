#include "Parser.h"
#include <stddef.h>

struct CmdIntrepeter {
    struct Parser* psr;

    void* cmd;
};



struct CmdIntrepeter* CmdIntrepeter_new();
int CmdIntrepeter_end(struct CmdIntrepeter* ci);

int CmdIntrepeter_parseCommand(struct CmdIntrepeter* ci , const char* text);
int CmdIntrepeter_execCommand(const char* cmd);
