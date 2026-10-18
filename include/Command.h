#define MAKE_FUNC_PTR(RETURN_TYPE, NAME, ARGS_TYPES...) \
    RETURN_TYPE (*NAME) (ARGS_TYPES)


#define MAKE_COMMAND(RETURN_TYPE, NAME, ARGS_TYPES...) \
    struct { \
        const char* name; \
        MAKE_FUNC_PTR(RETURN_TYPE, func, ARGS_TYPES); \
    } NAME;

#define CONSTRUCT_INIT_FUUNC(RETURN_TYPE, NAME, FUNCTION_NAME, ARGS_TYPES...) \
    void init() { \
        NAME .name = NAME_MAX; \
        NAME .func = &FUNCTION_NAME; \
    }

CONSTRUCT(int, sum, somar, int);
CONSTRUCT(void, ok, okBro);
