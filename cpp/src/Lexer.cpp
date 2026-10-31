#include "Lexer.hpp"
#include <string>


std::vector<cs::Lexemas>& cs::Lexer::getLexemas() {
    return lxmas;
}


void cs::Lexer::setInput(const std::string& input_) {
    lxmas.clear();
    this->input = input_;
    this->cursor = this->input.cbegin();
}


static const char ichar(std::string::const_iterator& cursor) {
    return *cursor.base();
}


void cs::Lexer::newLexemas(LexemasToken lt_, std::string var_) {
    this->lxmas.push_back({lt_, var_});
}

void cs::Lexer::newLexemas(LexemasToken lt_) {
    this->lxmas.push_back({lt_});
}


void cs::Lexer::QuotesTextLoop() {
    cursor++;
    auto mark = cursor;

    while (cursor != input.cend()) {
        if (ichar(cursor) == '"') {
            newLexemas(T_TEXT, {mark, cursor});
            cursor++;
            return;
        }
        else if (ichar(cursor) == '\0') {
            newLexemas(T_NULL);
            return;
        }
        cursor++;
    }
}


void cs::Lexer::LiteralTextLoop() {
    auto mark = cursor;

    while (cursor != input.cend()) {
        switch (ichar(cursor)) {
            case '"':
                newLexemas(T_NULL);
                cursor++;
                return;
            case ' ':
            case '\n':
            case '-':
            case ';':
            case '\0':
                std::string tmp = {mark, cursor};
                if( tmp == "true") {
                    newLexemas(T_TRUE);
                    return;
                } else if(tmp == "false") {
                    newLexemas(T_FALSE);
                    return;
                } else {
                    newLexemas(T_TEXT, tmp);
                    return;
                }
        }
        cursor++;
    }
}

void cs::Lexer::IntegerLoop() {
    auto mark = cursor;

    while (cursor != input.cend()) {
        switch (ichar(cursor)) {
            case ' ':
            case '\n':
            case '-':
            case ';':
            case '\0':
            case '"':
                newLexemas(LexemasToken::T_NUM, {mark, cursor});
                return;
            case 'U':
                newLexemas(LexemasToken::T_UNSIGNED_NUM, {mark, cursor});
                return;
            default:
                if(!isdigit(ichar(cursor))) {
                    newLexemas(LexemasToken::T_NULL);
                    return;
                }
        }
        cursor++;
    }
}


int cs::Lexer::start() {
    if(cursor != input.cbegin()) {return -1;}

    while(cursor != input.cend()) {
        switch (ichar(cursor)) {
            case ' ':
            case '\n':
                break;
            case '-':
                newLexemas(T_DASH);
                break;
            case ';':
                newLexemas(T_SEMI_COLON);
                break;
            case '"':
                QuotesTextLoop();
                continue;
            default:
                if(isalpha(ichar(cursor))) {
                    LiteralTextLoop();
                    continue;
                }
                else if (isdigit(ichar(cursor))) {
                    IntegerLoop();
                    continue;
                }
                else continue;
        }
        cursor++;
    };

    newLexemas(T_EOF);
    return 0;
}
