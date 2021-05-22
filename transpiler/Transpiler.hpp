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

            string getAutoName();
            string getNextName(string current);
            string getNextRandom(string l);

            void researchVariables(vector<Statement> &stmt);

            Statement* findFirstTyped(Statement &stmt);
            Statement* findFirstReturn(vector<Statement> &stmt);

            int replace(string &s, string from, string to);
            bool replaceOne(string &s, string from, string to);
            template<typename T>
            string connect(vector<T> &v, string c);
            string getEnd(Statement &stmt);
            void unescape(string &s);
    };
}