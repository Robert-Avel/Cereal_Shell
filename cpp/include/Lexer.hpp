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
        T_FALSE, // boolean 0    7
        T_UNSIGNED_NUM
    };


    class Lexemas {
        LexemasToken lt;
        std::optional<std::string> var;

        public:
            Lexemas() = delete;
            Lexemas(LexemasToken lt_, std::string var_): lt(lt_), var(var_) {}
            Lexemas(LexemasToken lt_): lt(lt_) {}

            const LexemasToken& getToken() {return lt;}

            const std::string* getVar()
            {return var.has_value() ? &var.value() : NULL;}
    };


    class Lexer {
        std::vector<Lexemas> lxmas;

        public:
            Lexer();
            Lexer(std::string input) {start(input);};

            void clear();
            int start(std::string input);
            const std::vector<Lexemas>& getLexemas();
    };
}
