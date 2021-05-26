#pragma once

#include "../parser/Statement.hpp"
#include "../parser/Type.hpp"
#include "Functions.hpp"
#include "../variables/Object.hpp"
#include "Namespace.hpp"

#include <string>
#include <map>
#include <vector>
#include "../libraries/Library.hpp"

typedef std::vector<variable::Object*>(*builtinF)(std::vector<variable::Object*> args);

namespace interpreter {
    using namespace std;
    using namespace parser;
    using namespace variable;

    class Interpreter {
        public:

            static map<string, builtinF> globalBuiltins;
            static map<string, FunctionDefinition> definitions;
            static Scope* includes;

            static void interpret(map<string, FunctionDefinition> &mFunctions);
            static Object* evaluateFunction(FunctionDefinition &func, vector<Object*> &args);
            static Object* evaluateStatement(Statement &stmt, Scope &scope);

            static void addBuiltin(string name, builtinF); // normal builtin
            static void addBuiltin(string ns, string name, builtinF); // namespaced builtin

            static void addDefaultBuiltins();
    };
}