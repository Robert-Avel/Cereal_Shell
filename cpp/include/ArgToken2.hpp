#ifndef NARGTOKEM_HPP
#define NARGTOKEM_HPP

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace cs {
    using position = unsigned int;
    using ArgVar = std::variant<long, unsigned long, std::string, bool>;
}

namespace cs {
    class ArgToken {
        cs::ArgVar value;

        public:
            ArgToken() = delete;
            ArgToken(cs::ArgVar value_): value(value_) {}
            void setVar(cs::ArgVar v);

            long* getArg_l ();
            unsigned long* getArg_ul ();
            std::string* getArg_cc ();
            bool* getArg_b ();
    };


    class NamedArgToken: public cs::ArgToken {
        std::string key;

        public:
            NamedArgToken() = delete;
            NamedArgToken(std::string key_, cs::ArgVar value_):
            ArgToken(value_), key(key_) {}

            void setKey(std::string k);
            const std::string& getKey();
    };


    class PositionalArgToken: public cs::ArgToken {
        position key;

        public:
            PositionalArgToken() = delete;
            PositionalArgToken(cs::position key_, cs::ArgVar value_):
            ArgToken(value_), key(key_) {}

            void setKey(cs::position k);
            const cs::position& getKey();
    };

    using PositionalArgList = std::vector<cs::PositionalArgToken>;
    using NamedArgList = std::unordered_map<std::string, cs::NamedArgToken>;
}

#endif
