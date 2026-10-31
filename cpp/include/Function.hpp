#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "ArgToken2.hpp"
#include <string>


namespace cs {
    class Function {
        std::string name;
        cs::PositionalArgList posal;
        cs::NamedArgList namal;

        public:
            Function() = delete;
            Function(std::string& name_): name(name_) {}

            void newPositionalArg(cs::ArgVar var);
            bool newNamedArg(std::string& name, cs::ArgVar var);
            bool newNamedArg(const std::string* name, cs::ArgVar var);

            const std::string& getName() const {return name;}
            const cs::PositionalArgList& getPosal() const {return posal;}
            const cs::NamedArgList& getNamal() const {return namal;}
            const cs::PositionalArgToken* getPositionalArg(cs::position pos) const;
            const cs::NamedArgToken* getNamedArg(std::string& name) const;
    };
}

#endif
