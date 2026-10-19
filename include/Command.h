#include <stddef.h>

#define MAKE_FUNC_TYPE(RETURN_TYPE, NAME, ARGS_TYPES...) \
    typedef RETURN_TYPE (*NAME) (ARGS_TYPES);


typedef void* (*PreCommand) (const char*, size_t) ;


struct CmdArray {
    PreCommand* cmds;
    size_t size;
};
