#include "Parser.h"

struct CmdIntrepeter {
    struct Parser* psr;

    int (*Sum) (int, int);
    void (*Ok) ();
};
