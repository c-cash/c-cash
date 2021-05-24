#pragma once

#include "../parser/Statement.hpp"
#include "../parser/FunctionDefinition.hpp"
#include <iostream>
#include <map>
#include <string>

#include "../variables/Object.hpp"
#include "Namespace.hpp"
 
namespace interpreter {
    using namespace std;
    using namespace parser;
/*
    template <typename T, std::enable_if_t<std::is_same_v<T, double> || std::is_same_v<T, std::string> || std::is_same_v<T, bool>, bool> = true>
    struct findVarStruct {
        T data;
    };
*/
    class Scope {
        public:
            map<string, variable::Object*> varTab;
            bool isPreviousIf = false;
            bool previousResult = false;

            Scope() {};
            Scope(Scope &b) {
                for (pair<string, variable::Object*> p : b.varTab) {
                    varTab[p.first] = p.second; // create clone of current scope
                }
            }
    };

    class Functions {
        public:
            static variable::Object* evaluateMath(Statement &stmt, Scope &scope);
            static variable::Object* evaluateLogic(Statement &stmt, Scope &scope);
            static variable::Object* evaluateFunctionCall(Statement &stmt, Scope &scope);

        private:
            static variable::Object* evaluateLoop(Statement &stmt, Scope &scope);
            static variable::Object* useSpecial(Object* s, string place);
    };
}