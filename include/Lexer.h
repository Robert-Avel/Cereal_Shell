#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include <stdbool.h>
enum LexerToken {
    T_NULL, //              0
    T_TEXT, //any word      1
    T_NUM, // only digits   2
    T_DASH, //"-"           3
    T_SEMI_COLON, // ';'    4
    T_EOF, //on '\0'        5
    T_TRUE, // boolean 1    6
    T_FALSE // boolean 2    7
};


struct Lexemas {
    enum LexerToken lt;
    const char* var;
};


int Lexemas_init(struct Lexemas* lex, enum LexerToken type, const char* var, size_t size);
int Lexemas_reset(struct Lexemas* lex);


struct Lexer {
    struct Lexemas* lxmas;
    size_t size;
    size_t allocated;

    const char* input;
    size_t cursor;
};


struct Lexer* Lexer_new();
int Lexer_expand(struct Lexer* l);

int Lexer_newLexemas(struct Lexer* l, enum LexerToken type, const char* var, size_t size);

int Lexer_clear(struct Lexer* l);
int Lexer_stepCursor(struct Lexer* l);
int Lexer_setInput(struct Lexer* l, const char* input);

int Lexer_start(struct Lexer* l);

int Lexer_end(struct Lexer* l);

#endif
