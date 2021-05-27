#pragma once

#include "../parser/Statement.hpp"
#include "../parser/FunctionDefinition.hpp"
#include <iostream>
#include <map>
#include <string>

#include "../variables/Object.hpp"
#include "../variables/SpecialObject.hpp"
#include "Namespace.hpp"
#include "../variables/Array.hpp"
 

namespace interpreter {
    using namespace std;
    using namespace parser;
/*
    template <typename T, std::enable_if_t<std::is_same_v<T, double> || std::is_same_v<T, std::string> || std::is_same_v<T, bool>, bool> = true>
    struct findVarStruct {
        T data;
    };
*/
    typedef variable::Object*(*builtinF)(std::vector<variable::Object*> args);

    class Scope {
        public:
            map<string, variable::Object*> varTab;
            map<string, builtinF> functions;
            map<string, Namespace*> namespaces;
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
            static variable::Object* evaluateArrayDeclaration(Statement &stmt, Scope &scope);
            static variable::Object* evaluateArrayCreation(Statement &stmt, Scope &scope);
            static variable::Object* evaluateArrayElement(Statement &stmt, Scope &scope);
            static variable::Object* evaluateVariableCall(Statement &stmt, Scope &scope); 
            static variable::Object* specialVariable(Statement &stmt, Scope &scope);
            static void includeLibrary(FunctionDefinition &func, Scope &scope);

        private:
            static variable::Object* evaluateLoop(Statement &stmt, Scope &scope);
            static variable::Object* useSpecial(SpecialObject* s, string place);
    };
}