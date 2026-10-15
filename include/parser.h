#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    char* commad;
    size_t cursor;

    bool inited;
} Parser;


Parser* Parser_new();
int Parser_setCmd(Parser* p, char* cmd);
char Parser_stepChar(Parser* p);
int Parser_end(Parser* p);
