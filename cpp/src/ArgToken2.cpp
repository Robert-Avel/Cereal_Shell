#include "ArgToken2.hpp"
#include <string>
#include <variant>

#define _GETTTER_BODY(type)                     \
    if(std::holds_alternative<type>(value)) {          \
        return &std::get<type>(value);                 \
    }                                                        \
    else return NULL;


void cs::ArgToken::setVar(cs::ArgVar v) {value = v;}
const long* cs::ArgToken::getArg_l () const {
    _GETTTER_BODY(long)
}
const unsigned long* cs::ArgToken::getArg_ul () const {
    _GETTTER_BODY(unsigned long)
}
const std::string* cs::ArgToken::getArg_cc () const {
    _GETTTER_BODY(std::string)
}
const bool* cs::ArgToken::getArg_b () const {
    _GETTTER_BODY(bool)
}


void cs::NamedArgToken::setKey(std::string k) {key = k;}
const std::string& cs::NamedArgToken::getKey() const {return key;}

void cs::PositionalArgToken::setKey(position k) {key = k;}
const cs::position& cs::PositionalArgToken::getKey() const {return key;}
