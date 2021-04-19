#pragma once

#include "../parser/FunctionDefinition.hpp"
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
            void interpreter(map<string, FunctionDefinition> &mFunctions);
        private:
            void executeCommands(FunctionDefinition &commandsFunc, map<string, parser::FunctionDefinition> &mFunctions);
            void executeCommand(Functions &func, Statement cmd, map<string, parser::FunctionDefinition> &mFunctions);
    };
}