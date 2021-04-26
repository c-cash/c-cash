#pragma once

#include "../parser/Statement.hpp"
#include "../parser/Type.hpp"
#include "Functions.hpp"

#include <string>
#include <map>

namespace interpreter {
    using namespace std;
    using namespace parser;

    class Interpreter {
        public:       
            void interpreter(map<string, FunctionDefinition> &mFunctionss);
            bool executeCommands(FunctionDefinition &commandsFunc, vector<Statement> &args, Scope &scope);
        private:
            bool executeCommand(Scope &scope, Statement &cmd);

    };
}