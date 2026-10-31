#include "Parser.hpp"
#include "Lexer.hpp"
#include <string>

void cs::Parser::appendFunction(cs::Function& f) {
    functions.push_back(f);
}

inline static cs::LexemasToken Ltoken(cs::LexemasVctr::const_iterator& lxm) {return lxm->getToken();}
inline static cs::LexemasToken LtokenNext(cs::LexemasVctr::const_iterator& lxm) {return (lxm + 1)->getToken();}


cs::PStatus cs::Parser::loopPosArg(cs::LexemasVctr& lxr, cs::LexemasVctr::const_iterator& cursor, cs::Function& fn) {

    unsigned long vul;
    long vl;
    bool vb;
    switch (Ltoken(cursor)) {
        case T_UNSIGNED_NUM:;
            vul = strtoul(cursor->getVar()->c_str(), NULL, 10);
            fn.newPositionalArg(vul);
            break;
        case T_TEXT:
            fn.newPositionalArg(*cursor->getVar());
            break;
        case T_NUM:;
            vl = strtol(cursor->getVar()->c_str(), NULL, 10);
            fn.newPositionalArg(vl);
            break;
        case T_TRUE:
        case T_FALSE:
            vb = Ltoken(cursor) == T_TRUE;
            fn.newPositionalArg(vb);
            break;
        default:
            return FAIL;
    }
    cursor++;
    return CONTINUE;
}


cs::PStatus cs::Parser::loopKeyArg(cs::LexemasVctr& lxr, cs::LexemasVctr::const_iterator& cursor, cs::Function& fn, const std::string* key) {
    cursor++;

    unsigned long vul;
    long vl;
    bool vb;
    switch (Ltoken(cursor)) {
        case T_NUM:
            vl = std::strtol(cursor->getVar()->c_str(), NULL, 10);
            fn.newNamedArg(key, vl);
            break;
        case T_UNSIGNED_NUM:;
            vul = std::strtoul(cursor->getVar()->c_str(), NULL, 10);
            fn.newNamedArg(key, vul);
            break;
        case T_TEXT:
            fn.newNamedArg(key, *cursor->getVar());
            break;

        case T_TRUE:
        case T_FALSE:
            vb = Ltoken(cursor) == T_TRUE;
            fn.newNamedArg(key, vb);
            break;
        case T_DASH:
            if(LtokenNext(cursor) == T_NUM) {
                vl = std::strtol(cursor->getVar()->c_str(), NULL, 10);
                vl *= -1;
                fn.newNamedArg(key, vl);
                cursor++;
                break;
            }
        case T_NULL:
            return LEXER_ERROR;
        case T_SEMI_COLON:
        case T_EOF:
            return EXPECTED_EXPRESSION;
    }
    cursor++;
    return CONTINUE;
}



void cs::Parser::loopNegativeNum(cs::LexemasVctr::const_iterator& cursor, cs::Function& fn) {
    long var = std::strtol(cursor->getVar()->c_str(), NULL, 10);
    var *= -1;
    fn.newPositionalArg(var);
    cursor++;
}


cs::PStatus cs::Parser::loopFunction(std::string& name, cs::LexemasVctr& lxr, cs::LexemasVctr::const_iterator& cursor) {
    cs::Function f(name);
    cursor++;

    cs::PStatus r = cs::CONTINUE;
    bool only_kargs = false;
    while (1) {
        switch (Ltoken(cursor)) {
            case cs::T_TEXT:
            case cs::T_NUM:
            case cs::T_UNSIGNED_NUM:
            case cs::T_TRUE:
            case cs::T_FALSE:
                if(only_kargs) {r = cs::KW_ARGS_ONLY_ERROR; break;}
                r = loopPosArg(lxr, cursor, f);
                break;
            case cs::T_DASH:
                if (LtokenNext(cursor) == cs::T_NUM) {
                    if(only_kargs) {r = cs::KW_ARGS_ONLY_ERROR; break;}

                    cursor++;
                    loopNegativeNum(cursor, f);
                }
                else if (LtokenNext(cursor) == cs::T_TEXT) {
                    cursor++;
                    r = loopKeyArg(lxr, cursor, f, cursor->getVar());
                    only_kargs = true;
                }
                else if (LtokenNext(cursor) == cs::T_UNSIGNED_NUM) {
                    r = cs::DASH_WITH_UNSIGNED;
                    break;
                }
                else {r = cs::EXPECTED_EXPRESSION;}
                break;
            case cs::T_NULL:
                r = cs::LEXER_ERROR;
                break;
            case cs::T_SEMI_COLON:
                this->appendFunction(f);
                r = cs::SUCCESS;
                break;
            case cs::T_EOF:
                r = cs::EXPECTED_SEMI_COLON;
                break;
        }
        if(r >= 0) {
            cursor++;
            break;
        }
    }
    return r;
}


cs::PStatus cs::Parser::start(cs::LexemasVctr& lxms) {
    if(lxms.empty()) {return cs::FAIL;}
    functions.clear();

    auto cursor = lxms.cbegin();

    while (cursor != lxms.cend()) {
        if(Ltoken(cursor) == T_TEXT) {
            if(cursor->getVar() == nullptr) {return EXPECTED_NAME_FUNCTION;}
            auto n = *cursor->getVar();
            auto r = loopFunction(n, lxms, cursor);
            if(r != 0) {return r;}
        } else if (Ltoken(cursor) == T_EOF) {
            break;
        }
        else return EXPECTED_NAME_FUNCTION; //Syntax Error
    }
    return SUCCESS;
}
