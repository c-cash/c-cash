#pragma once

#include "../parser/FunctionDefinition.hpp"
#include "../parser/Statement.hpp"
#include "../parser/Type.hpp"

#include <string>
#include <map>

namespace interpreter {
    using namespace std;
    using namespace parser;

    class Interpreter {
        public:
            void interpreter(map<string, FunctionDefinition> &mFunctions);
        private:
            void executeCommands(FunctionDefinition &commandsFunc);
    };
}