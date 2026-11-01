#ifndef PARSER_HPP
#define PARSER_HPP

#include "Function.hpp"
#include "Lexer.hpp"
#include <list>

namespace cs
{

    enum PStatus {
        CONTINUE = -1, //           -1
        SUCCESS, //                  0
        FAIL, //                     1
        EXPECTED_NAME_FUNCTION, //   2
        LEXER_ERROR, //              3
        EXPECTED_SEMI_COLON, //      4
        KW_ARGS_ONLY_ERROR, //       5
        EXPECTED_EXPRESSION, //      6
        DASH_WITH_UNSIGNED, //        7
        NOT_FOUND,
        FUNCTION_NULL,
        NO_INPUT
    };

    class Parser {
        std::list<cs::Function> functions;

        cs::PStatus loopPosArg(cs::LexemasVctr& lxr, cs::LexemasVctr::const_iterator& cursor, cs::Function& fn);
        cs::PStatus loopKeyArg(cs::LexemasVctr& lxr, cs::LexemasVctr::const_iterator& cursor, cs::Function& fn, const std::string* key);
        void loopNegativeNum(cs::LexemasVctr::const_iterator& cursor, cs::Function& fn);
        cs::PStatus loopFunction(std::string& name, cs::LexemasVctr& lxr, cs::LexemasVctr::const_iterator& cursor);
        public:
            Parser() = default;
            void appendFunction(cs::Function& f);
            cs::PStatus start(cs::LexemasVctr& lxms);

             std::list<cs::Function> getFunctions() const {return functions;}
    };

}

#endif
