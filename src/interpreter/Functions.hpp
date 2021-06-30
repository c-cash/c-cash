#pragma once

#include "../parser/Statement.hpp"
#include "../parser/FunctionDefinition.hpp"
#include "../variables/Object.hpp"
#include "../variables/SpecialObject.hpp"
#include "../variables/Array.hpp"
 
#include <iostream>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>

namespace interpreter {
    using namespace std;
    using namespace parser;
    using namespace variable;

    typedef variable::Object*(*builtinF)(std::vector<variable::Object*> &args);
    typedef variable::Object*(*objectF)(variable::Object* t, std::vector<variable::Object*> &args);

    class Scope {
        public:
            unordered_map<string, variable::Object*> varTab;
            unordered_map<string, builtinF> functions;
            unordered_map<string, Scope*> namespaces;
            vector<const char*>* varCache {nullptr};
            vector<variable::Object*> memoryClean;

            Scope* parent {nullptr};
            Scope* nsparent {nullptr};

            bool isPreviousIf = false;
            bool previousResult = false;

            Scope() {};
            Scope(Scope &b);
            Scope(Scope &b, Scope* n);

            void reset();
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
            static variable::Object* evaluateTernaryOperator(Statement &stmt, Scope &scope);
            static variable::Object* specialVariable(Statement &stmt, Scope &scope);
            static void includeLibrary(FunctionDefinition &func, Scope &scope);

            static variable::Object* findVariable(string name, Scope &scope);
            static vector<const char*>* getNearestCache(Scope &scope);
            static builtinF findFunction(string name, Scope &scope);
            static Scope* findNamespace(string name, Scope &scope);

        private:

            static variable::Object* evaluateLoop(Statement &stmt, Scope &scope);
            static variable::Object* useSpecial(SpecialObject* s, string place);
    };
}