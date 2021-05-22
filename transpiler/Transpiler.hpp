#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "../parser/Statement.hpp"
#include "../parser/FunctionDefinition.hpp"

namespace transpiler {

    using namespace std;
    using namespace parser;

    class Transpiler {
        public:
            void transpile(string path, map<string, FunctionDefinition> &mFunctions);
            string discoverType(FunctionDefinition &func);

        private:
            string transpileStatement(Statement &stmt);
            string transpileFunctionCall(Statement &stmt);
            string transpileLitteral(Statement &stmt);
            string transpileOperator(Statement &stmt);
            string transpileVariableDeclaration(Statement &stmt);
            string transpileLoop(Statement &stmt);

            int replace(string &s, string from, string to);
            template<typename T>
            string connect(vector<T> &v, string c);
    };
}