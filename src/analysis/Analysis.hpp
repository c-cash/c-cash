#pragma once

#include <map>
#include <vector>
#include <algorithm>
#include <variant>
#include <string>
#include "../parser/FunctionDefinition.hpp"
#include "../parser/ClassDefinition.hpp"

typedef std::variant<int, long long, double, char, bool, void*, unsigned long long> dataType;

namespace analysis {
    using namespace std;
    using namespace parser;

    struct varClass {
        BUILTIN_TYPE type;
        string name;
        double val;
    };

    class Analysis {
        public:
            void analize(map<string, FunctionDefinition> &aFunctions, map<string, ClassDefinition> &aClasses);
        private:
            void checkStatement(Statement &stmt);
            void checkDeclaration(Statement &statement);
            void checkVariblesOutOfRange(Statement &stmt, varClass &var);
            void checkVaribleCall(Statement &statement);

            vector<varClass> varibles;
    };
}