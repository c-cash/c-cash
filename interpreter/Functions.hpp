#pragma once

#include "../parser/Statement.hpp"
#include "../parser/FunctionDefinition.hpp"
#include <iostream>
#include <map>
 
namespace interpreter {
    using namespace std;
    using namespace parser;

    class Scope {
        public:
            map<string, double> doubleVarTab;
            map<string, int> intVarTab;
            map<string, unsigned int> uintVarTab;
            map<string, char> charVarTab;
            map<string, unsigned char> ucharVarTab;
            map<string, string> stringVarTab;
    };

    class Functions {
        public:
            void declareVariableFunc(Statement &variable, Scope &scope);
            void declareParameter(ParameterDefinition &var, Statement &value, Scope &scope);
            void returnFunc(Statement &operations, Scope &scope);
            void writeFunc(Statement &operations, Scope &scope);
            void readFunc(Statement &operations, Scope &scope);
            void changeVarValue(Statement &operations, Scope &scope);
            bool startIf(Statement &operations, Scope &scope);
        private:
            double startCalculations(Statement &operations, Scope &scope);
            double calculating(Statement &operations, Scope &scope);
            bool calculateIf(Statement &operations, Scope &scope);
            double findVar(Statement &operations, Scope &scope);
    };
}