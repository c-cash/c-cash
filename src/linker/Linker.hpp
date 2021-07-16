#pragma once

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <exception>

#include "../parser/FunctionDefinition.hpp"
#include "../parser/ClassDefinition.hpp"
#include "../parser/Statement.hpp"
#include "../parser/Tokenizer.hpp"
#include "../parser/Parser.hpp"

namespace linker {
    using namespace std;
    using namespace parser;

    class Linker {
        public:
            void link(map<string, FunctionDefinition> &aFunctions, map<string, ClassDefinition> &aClasses);

        private:
            void linkInclude(Statement &s, map<string, FunctionDefinition> &aFunctions, map<string, ClassDefinition> &aClasses);
            void parseInclude(string &name, map<string, FunctionDefinition> &aFunctions, map<string, ClassDefinition> &aClasses);
            bool isNew(string &name);

            vector<string> includes;
    };
}