#include "PStatus.hpp"
#include <unordered_map>
#include <string>
#include "Wrapper.hpp"
#include "Function.hpp"

#define CEREAL_INPUT_HEAD "<Cereal_Shell_> "

#define CS_BOOL BOOL
#define CS_STRING STRING
#define CS_LONG LONG
#define CS_UNSIGNED_LONG UNSIGNED_LONG


namespace cs
{
    typedef void (*WrapperFunction) (const cs::Function&);
    using FunctionMap = std::unordered_map<std::string, WrapperFunction> ;

    class CerealShell {
        FunctionMap reg_fun;

        public:
            CerealShell() = default;

            WrapperFunction* getFunction(const std::string& name);
            bool newFunction(std::string name, WrapperFunction f);
            cs::PStatus CerealShell_start(std::string input);
            void initLoop();
    };
};
