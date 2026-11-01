#include "Function.hpp"
#include "CerealShell.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include <iostream>
#include <string>
#include "Wrapper.hpp"


static void printError(cs::PStatus err) {
    switch (err) {
        case cs::FAIL:
            printf(FG_RED CEREAL_INPUT_HEAD" Fatal Error" RESET "\n");
            break;
        case cs::EXPECTED_NAME_FUNCTION:
            printf(FG_RED CEREAL_INPUT_HEAD" Syntax Error: Expected Name Function" RESET "\n");
            break;
        case cs::LEXER_ERROR:
            printf(FG_RED CEREAL_INPUT_HEAD" Lexer Error: Invalid Expression" RESET "\n");
            break;
        case cs::EXPECTED_SEMI_COLON:
            printf(FG_RED CEREAL_INPUT_HEAD" Syntax Error: Expected Semi Colon" RESET "\n");
            break;
        case cs::KW_ARGS_ONLY_ERROR:
            printf(FG_RED CEREAL_INPUT_HEAD" Syntax Error: Positional Args after Named Args are not allowed" RESET "\n");
            break;
        case cs::EXPECTED_EXPRESSION:
            printf(FG_RED CEREAL_INPUT_HEAD" Syntax Error: Expected Expression" RESET "\n");
            break;
        case cs::DASH_WITH_UNSIGNED:
            printf(FG_RED CEREAL_INPUT_HEAD" Syntax Error: Cannot convert in negative number with unsigned" RESET "\n");
            break;
        case cs::NOT_FOUND:
            printf(FG_RED CEREAL_INPUT_HEAD" Function Not Found in System" RESET "\n");
            break;
        case cs::FUNCTION_NULL:
            printf(FG_RED CEREAL_INPUT_HEAD" Function Found, but not exectuted! (NULL function)" RESET "\n");
            break;
        case cs::NO_INPUT:
            printf(FG_RED CEREAL_INPUT_HEAD" No input!" RESET "\n");
            break;
        case cs::CONTINUE:
        case cs::SUCCESS:
            break;
    }
}



cs::WrapperFunction* cs::CerealShell::getFunction(const std::string& name) {
    if(this->reg_fun.find(name) != this->reg_fun.end()) {
        return &this->reg_fun.at(name);
    } else return nullptr;
}


bool cs::CerealShell::newFunction(std::string name, cs::WrapperFunction f) {
    if(reg_fun.find(name) == reg_fun.end()) {
        reg_fun.insert({name, f});
        return true;
    } else return false;
}

cs::PStatus cs::CerealShell::CerealShell_start(std::string input) {
    if(input.empty()) {return cs::NO_INPUT;}

    cs::Lexer lxr(input);
    lxr.start();

    cs::Parser psr;
    auto r = psr.start(lxr.getLexemas());
    if(r != cs::SUCCESS) {return r;}

    for(const cs::Function& f: psr.getFunctions()) {
        cs::WrapperFunction* wf = this->getFunction(f.getName());
        if (wf == nullptr) {return cs::NOT_FOUND;}
        if(*wf == nullptr) {return cs::FUNCTION_NULL;}
        (*wf)(f);
    }
    return cs::SUCCESS;
}

void cs::CerealShell::initLoop() {
    std::string in;
    while (1) {
        printf(FG_BLUE CEREAL_INPUT_HEAD RESET);
        std::getline(std::cin, in);

        auto end = this->CerealShell_start(in);
        if(end != cs::SUCCESS) {printError(end);}
    }
}
