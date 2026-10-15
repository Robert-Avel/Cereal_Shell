#include "cmd_args.h"
#include <stddef.h>


typedef struct {
    ArgToken* all_args;
    size_t size;
    size_t allocated;
} ArgsBlock;


ArgsBlock* ArgsBlock_new();
int ArgsBlock_end(ArgsBlock* ab);
