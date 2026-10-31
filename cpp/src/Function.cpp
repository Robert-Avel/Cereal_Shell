#include "Function.hpp"
#include "ArgToken2.hpp"


void cs::Function::newPositionalArg(cs::ArgVar var) {
    position pos = posal.size();
    posal.push_back({pos, var});
}

bool cs::Function::newNamedArg(std::string& name, cs::ArgVar var) {
    if(namal.find(name) == namal.end()) {
        namal.insert({name, {name, var}});
        return true;
    } else return false;
}

bool cs::Function::newNamedArg(const std::string* name, cs::ArgVar var) {
    if(name == nullptr) {return false;}
    if(namal.find(*name) == namal.end()) {
        namal.insert({*name, {*name, var}});
        return true;
    } else return false;
}

const cs::PositionalArgToken* cs::Function::getPositionalArg(cs::position pos) const {
    if(posal.size() < pos) {return nullptr;}
    return &posal[pos];
}

const cs::NamedArgToken* cs::Function::getNamedArg(std::string& name) const {
    if(namal.find(name) == namal.end()) {
        return &namal.at(name);
    } else return nullptr;
}
