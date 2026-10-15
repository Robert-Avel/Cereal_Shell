typedef enum {
    NONE,
    POSITIONAL,
    KEY
} ArgType;


typedef struct {
    char* key;
    char* value;
} KeyArg;


typedef struct {
    int pos;
    char* value;
} PosArg;

typedef struct {
    ArgType type;
    union {
        PosArg pa;
        KeyArg ka;
    } arg;
} ArgToken;



ArgToken* ArgToken_new();
int ArgToken_end(ArgToken* at);

PosArg* ArgToken_setPosArg(ArgToken* at, int pos, char* value);
KeyArg* ArgToken_setKeyArg(ArgToken* at, char* key, char* value);

PosArg* ArgToken_getPosArg(ArgToken* at);
KeyArg* ArgToken_getKeyArg(ArgToken* at);
