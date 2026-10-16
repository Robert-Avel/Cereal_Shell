typedef enum {
    NONE,
    POSITIONAL,
    KEY
} ArgType;


struct KeyArg {
    char* key;
    char* value;
};


struct PosArg {
    int pos;
    char* value;
};

struct ArgToken {
    ArgType type;
    union {
        struct PosArg pa;
        struct KeyArg ka;
    } arg;
};



struct ArgToken* ArgToken_new();
int ArgToken_end(struct ArgToken* at);

struct PosArg* ArgToken_setPosArg(struct ArgToken* at, int pos, char* value);
struct KeyArg* ArgToken_setKeyArg(struct ArgToken* at, char* key, char* value);

const struct PosArg* ArgToken_getPosArg(const struct ArgToken* at);
const struct KeyArg* ArgToken_getKeyArg(const struct ArgToken* at);
