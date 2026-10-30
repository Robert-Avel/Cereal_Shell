#include "ArgToken2.hpp"
#include <string>
#include <variant>

#define _GETTTER_BODY(type)                     \
    if(std::holds_alternative<type>(value)) {          \
        return &std::get<type>(value);                 \
    }                                                        \
    else return NULL;


void cs::ArgToken::setVar(cs::ArgVar v) {value = v;}
long* cs::ArgToken::getArg_l () {
    _GETTTER_BODY(long)
}
unsigned long* cs::ArgToken::getArg_ul () {
    _GETTTER_BODY(unsigned long)
}
std::string* cs::ArgToken::getArg_cc () {
    _GETTTER_BODY(std::string)
}
bool* cs::ArgToken::getArg_b () {
    _GETTTER_BODY(bool)
}


void cs::NamedArgToken::setKey(std::string k) {key = k;}
const std::string& cs::NamedArgToken::getKey() {return key;}

void cs::PositionalArgToken::setKey(position k) {key = k;}
const cs::position& cs::PositionalArgToken::getKey() {return key;}
