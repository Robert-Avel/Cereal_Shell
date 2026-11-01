#ifndef PARSER_HPP
#define PARSER_HPP

#include "PStatus.hpp"
#include "Function.hpp"
#include "Lexer.hpp"
#include <list>

namespace cs
{
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
