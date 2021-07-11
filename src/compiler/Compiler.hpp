#pragma once

#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <stack>
#include <string>

#include "../parser/Parser.hpp"
#include "../parser/FunctionDefinition.hpp"
#include "../parser/Statement.hpp"

namespace compiler {
    using namespace std;
    using namespace parser;

    struct Stack {
        std::stack<int> elements;
        std::map<string, string> varTypes;
        std::map<string, int> values;

        std::pair<int, int> pop2() {
            pair<int, int> p;
            p.first = elements.top();
            elements.pop();
            p.second = elements.top();
            elements.pop();
            return p;
        }
        int pop() {
            int t = elements.top();
            elements.pop();
            return t;
        }
    };

    class Compiler {
        public:
            void compile(map<string, FunctionDefinition> &mFunctions, ofstream &out);

            void compileCode(vector<Statement> &statements, Stack &s);
            void compileStatement(Statement &stmt, Stack &s);
            void compileVariableDeclaration(Statement &stmt, Stack &s);
            void compileLitteral(Statement &stmt, Stack &s);
            void compileMathOperation(Statement &stmt, Stack &s);
            void compileVariableCall(Statement &stmt, Stack &s);
            
            uint getNumericalName(string name);
            void writeInteger(int n);

        private:
            map<string, uint> numerical_names;
            uint nextNumericalName = 0;
            ofstream* out;
    };

    enum BytecodeInstructions {
        ICONST = (char) 0x0,
        ISTORE = (char) 0x1,
        IASTORE = (char) 0x2,
        ILOAD = (char) 0x3,
        IALOAD = (char) 0x4,
        IADD = (char) 0x5,
        ISUB = (char) 0x6,
        IMUL = (char) 0x7,
        IDIV = (char) 0x8
        // TODO: Add more instructions
    };
}