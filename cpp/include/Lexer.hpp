#ifndef LEXER_HPP
#define LEXER_HPP

#include <optional>
#include <string>
#include <vector>


namespace cs
{
    enum LexemasToken {
        T_NULL, //              0
        T_TEXT, //any word      1
        T_NUM, // only digits   2
        T_DASH, //"-"           3
        T_SEMI_COLON, // ';'    4
        T_EOF, //on '\0'        5
        T_TRUE, // boolean 1    6
        T_FALSE, // boolean 0   7
        T_UNSIGNED_NUM //       8
    };


    class Lexemas {
        LexemasToken lt;
        std::optional<std::string> var;

        public:
            Lexemas() = delete;
            Lexemas(LexemasToken lt_, std::string var_): lt(lt_), var(var_) {}
            Lexemas(LexemasToken lt_): lt(lt_) {}

            const LexemasToken& getToken() const {return lt;}

            const std::string* getVar() const
            {return var.has_value() ? &var.value() : NULL;}
    };

    using LexemasVctr = std::vector<Lexemas>;

    class Lexer {
        LexemasVctr lxmas;

        std::string input;
        std::string::const_iterator cursor;

        void QuotesTextLoop();
        void LiteralTextLoop();
        void IntegerLoop();
        public:
            Lexer() = delete;
            Lexer(std::string& input) {setInput(input);};

            void clear();
            void setInput(const std::string& input);
            int start();
            std::vector<Lexemas>& getLexemas();
            void newLexemas(LexemasToken lt_, std::string var_);
            void newLexemas(LexemasToken lt_);
    };
}

#endif
