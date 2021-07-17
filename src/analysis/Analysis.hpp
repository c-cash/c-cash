#pragma once

#include <map>
#include "../parser/FunctionDefinition.hpp"
#include "../parser/ClassDefinition.hpp"

namespace analysis {
    using namespace std;
    using namespace parser;

    class Analysis {
        public:
            void analize(map<string, FunctionDefinition> &aFunctions, map<string, ClassDefinition> &aClasses);
        private:
            void checkStatement(Statement &stmt);
            void checkDeclaration(Statement &statement);
            void checkDeclarationChecks(Statement &stmt);
    };
}