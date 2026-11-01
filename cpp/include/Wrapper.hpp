#define FG_RED            "\033[31m"
#define RESET             "\033[0m"
#define FG_BLUE           "\033[34m"
#define FG_BRIGHT_RED     "\033[31m"


#define CAT(a, b) a##b
#define CAT2(a, b) CAT(a, b)


#define _BOOL_E bool
#define _STRING_E std::string
#define _LONG_E long
#define _UNSIGNED_LONG_E unsigned long


#define _BOOL_S b
#define _STRING_S cc
#define _LONG_S l
#define _UNSIGNED_LONG_S ul

#define _BOOL_F "%b"
#define _STRING_F "%s"
#define _LONG_F "%ld"
#define _UNSIGNED_LONG_F "%lu"


#define _TYPE_EXTEND_ENUM(type) _##type##_E
#define _TYPE_COLAPSE_ENUM(type) _##type##_S
#define _TYPE_FORMAT_ENUM(type) _##type##_F


#define _TOSTR(n) #n


#define _ARGS_CHECHER( n_args ) \
    if(f.size() != n_args)    \
    {std::cout << FG_BRIGHT_RED << "ARGS DIVERGENCES\n" << RESET; return;}

#define _ARG_POSITIONAL(pos) \
    const cs::PositionalArgToken* a##pos = f.getPositionalArg(pos); \
    if( a##pos == nullptr ) {std::cout << FG_BRIGHT_RED << "ARG " << _TOSTR(pos) << " NOT FOUND" << RESET << "\n"; return;} \


#define _ARG_NAMED(name) \
    const cs::NamedArgToken* a##name = f.getNamedArg( #name ); \
    if( a##name == nullptr ) {std::cout << FG_BRIGHT_RED << "ARG " << #name << " NOT FOUND" << RESET << "\n"; return;} \


#define _VAR_NAMED(name, type) \
    const _TYPE_EXTEND_ENUM(type) * b##name = CAT2( a##name ->getArg_ , _TYPE_COLAPSE_ENUM(type)) (); \
    if( b##name == nullptr ) {std::cout << FG_BRIGHT_RED << "ARG " << #name << " TYPE INVALID" << RESET << "\n"; return;} \
    const _TYPE_EXTEND_ENUM(type) c##name = *b##name;

#define _VAR_POSITIONAL(pos, type) \
    const _TYPE_EXTEND_ENUM(type) * b##pos = CAT2( a##pos ->getArg_ , _TYPE_COLAPSE_ENUM(type)) (); \
    if( b##pos == nullptr ) {std::cout << FG_BRIGHT_RED << "ARG " << _TOSTR(pos) << " TYPE INVALID" << RESET << "\n"; return;} \
    const _TYPE_EXTEND_ENUM(type) c##pos = *b##pos;


#define _RETURNER_VOID(function_name, caller) \
    function_name caller; std::cout << "OK, No Return!\n";  \


#define _RETURNER(type, function_name, caller) \
    const _TYPE_EXTEND_ENUM(type) r = function_name caller ;  \
    std::cout << r << '\n'; \


#define CPP_ARG_ASSEMBLY_NAMED(name, type) \
    _ARG_NAMED(name) \
    _VAR_NAMED(name, type) \


#define CPP_ARG_ASSEMBLY_POS(pos, type) \
    _ARG_POSITIONAL(pos) \
    _VAR_POSITIONAL(pos, type) \


#define CPP_FUNCTION_NEW(function_name, return_type, n_args, caller, args...) \
    void function_name##__outCShell(const cs::Function& f) { \
    _ARGS_CHECHER( n_args) \
    args \
    _RETURNER(return_type, function_name, caller); \
    }


#define CPP_VOID_FUNCTION_NEW(function_name, n_args, caller, args...) \
    void function_name##__outCShell(const cs::Function& f) { \
    _ARGS_CHECHER( n_args) \
    args \
    _RETURNER_VOID(function_name, caller) \
    }
