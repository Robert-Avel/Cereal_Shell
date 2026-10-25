#include "Lexer.h"
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>


int Lexemas_init(struct Lexemas* lex, enum LexerToken type, const char* var, size_t size) {
    if(lex == NULL) {return 1;}

    if(var != NULL) {
        if(size == 0) {
            lex->var = strdup(var);
            if(lex->var == NULL) {return 2;}
        } else {
            lex->var = malloc(sizeof(char) * size + 1);
            if(lex->var == NULL) {return 2;}
            strncpy((char*) lex->var, var, size);
        }
    }
    lex->lt = type;
    return 0;
}


int Lexemas_reset(struct Lexemas* lex) {
    if (lex == NULL) {return 1;}

    free( (void*) lex->var);
    lex->var = NULL;
    lex->lt = T_NULL;
    return 0;
}


struct Lexer* Lexer_new() {
    struct Lexer* l = malloc(sizeof(struct Lexer));
    if (l == NULL) {return NULL;}

    l->lxmas = NULL;
    l->allocated = 0;
    l->size = 0;

    l->input = NULL;
    l->cursor = 0;
    return l;
}


int Lexer_expand(struct Lexer* l) {
    if(l == NULL) {return 1;}

    struct Lexemas* d;
    if(l->lxmas == NULL)
    {
        d = malloc(sizeof(struct Lexemas) * 10);
        if(d == NULL) {return 2;}
        l->lxmas = d;
    } else {
        d = realloc(l->lxmas, sizeof(struct Lexemas) * (10 + l->allocated));
        if(d == NULL) {return 2;}
        l->lxmas = d;
    }
    l->allocated += 10;
    return 0;
}


int Lexer_newLexemas(struct Lexer* l, enum LexerToken type, const char* var, size_t size) {
    if(l == NULL) {return 1;}

    if(l->size + 1 > l->allocated) {
        if(Lexer_expand(l) != 0) {return 2;}
    }

    struct Lexemas* n_l = &l->lxmas[l->size];
    int r = {0};
    r = Lexemas_init(n_l, type, var, size);
    if(r != 0) {return 0;}

    l->size += 1;
    return r;
}


int Lexer_end(struct Lexer* l) {
    if(l == NULL) {return 1;}

    for(size_t i = 0; i < l->size; i++) {
        Lexemas_reset(&l->lxmas[i]);
    }
    free(l->lxmas);
    free(l);
    return 0;
}

int Lexer_clear(struct Lexer* l) {
    if(l == NULL) {return 1;}

    for(size_t i = 0; i < l->size; i++) {
        Lexemas_reset(&l->lxmas[i]);
    }
    if(l->lxmas != NULL) {free(l->lxmas);}
    l->allocated = 0;
    l->size = 0;

    if(l->input != NULL) {free((char*) l->input);}
    l->cursor = 0;
    return 0;
}

int Lexer_stepCursor(struct Lexer* l) {
    if (l == NULL) {return 1;}
    if(l->input == NULL) {return 2;}

    l->cursor++;
    return 0;
}


int Lexer_setInput(struct Lexer* l, const char* input) {
    if (l == NULL) {return 1;}
    if(l->input != NULL) {free((char*) l->input);}

    l->input = strdup(input);
    if(l->input == NULL) {return 2;}

    l->cursor = 0;
    return 0;
}

const char* Lexer_char(struct Lexer* l) {
    return l->input + l->cursor;
}


void Lexer_TextLoop(struct Lexer* l, bool in_quotes) {
    if(in_quotes) {l->cursor++;}
    size_t mark = l->cursor;

    while(1) {
        if(in_quotes) {
            if (*Lexer_char(l) == '"') {
                Lexer_newLexemas(l, T_TEXT, l->input + mark, l->cursor - mark);
                l->cursor++;
                return;
            }
            else if (*Lexer_char(l) == '\0') {
                Lexer_newLexemas(l, T_NULL, NULL, 0);
                return;
            }
        }
        else {
            if(*Lexer_char(l) == '\0' || *Lexer_char(l) == ' ' || *Lexer_char(l) == ';'|| *Lexer_char(l) == '-') {
                if(l->cursor - mark == 4 && strncmp(l->input + mark, "true", 4) == 0) {
                    Lexer_newLexemas(l, T_TRUE, NULL, 0);
                }
                else if(l->cursor - mark == 5 && strncmp(l->input + mark, "false", 5) == 0) {
                    Lexer_newLexemas(l, T_FALSE, NULL, 0);
                }
                else Lexer_newLexemas(l, T_TEXT, l->input + mark, l->cursor - mark);
                return;
            }
            else if (*Lexer_char(l) == '"') {
                Lexer_newLexemas(l, T_NULL, NULL, 0);
                l->cursor++;
                return;
            }
        }
        l->cursor++;
    }
}


void Lexer_IntegerLoop(struct Lexer* l) {
    size_t mark = l->cursor;

    while(1) {
        if(*Lexer_char(l) == ' ' || *Lexer_char(l) == '\0' || *Lexer_char(l) == ';') {
            Lexer_newLexemas(l, T_NUM, l->input + mark, l->cursor - mark);
            return;
        }
        if(!isdigit(*Lexer_char(l))) {
            Lexer_newLexemas(l, T_NULL, NULL, 0);
        }
        l->cursor++;
    }
}


int Lexer_start(struct Lexer* l) {
    if(l == NULL) {return 1;}
    if(l->cursor != 0) {return 2;}


    while(*Lexer_char(l) != '\0') {
        if(*Lexer_char(l) == ' ') {
            l->cursor++;
            continue;
        }
        if(*Lexer_char(l) == '-') {
            Lexer_newLexemas(l, T_DASH, NULL, 0);
        }
        else if(*Lexer_char(l) == ';') {
            Lexer_newLexemas(l, T_SEMI_COLON, NULL, 0);
        }
        else if (*Lexer_char(l) == '\0') {
            Lexer_newLexemas(l, T_EOF, NULL, 0);
        }
        else if(*Lexer_char(l) == '"' || isalpha(*Lexer_char(l))) {
            Lexer_TextLoop(l, *Lexer_char(l) == '"');
            continue;
        }
        else if(isdigit(*Lexer_char(l))) {
            Lexer_IntegerLoop(l);
            continue;
        }
        l->cursor++;
    }
    return 0;
}
