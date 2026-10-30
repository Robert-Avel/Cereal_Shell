#include "Lexer.hpp"


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
            if(Lexer_isDelimiter((unsigned char)*Lexer_char(l))) {
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
        if(Lexer_isDelimiter((unsigned char)*Lexer_char(l))) {
            Lexer_newLexemas(l, T_NUM, l->input + mark, l->cursor - mark);
            return;
        }
        else if(*Lexer_char(l) == 'U') {
            Lexer_newLexemas(l, T_UNSIGNED_NUM, l->input + mark, l->cursor - mark);
            l->cursor++;
            return;
        }
        else if(!isdigit((unsigned char)*Lexer_char(l))) {
            Lexer_newLexemas(l, T_NULL, NULL, 0);
        }
        l->cursor++;
    }
}


int cs::Lexer::start(std::string input) {
    if(l == NULL) {return 1;}
    if(l->cursor != 0) {return 2;}


    do {
        if(*Lexer_char(l) == ' '|| *Lexer_char(l) == '\n') {
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
            break;
        }
        else if(*Lexer_char(l) == '"' || isalpha((unsigned char)*Lexer_char(l))) {
            Lexer_TextLoop(l, *Lexer_char(l) == '"');
            continue;
        }
        else if(isdigit((unsigned char)*Lexer_char(l))) {
            Lexer_IntegerLoop(l);
            continue;
        }
        l->cursor++;
    } while(1);
    return 0;
}
