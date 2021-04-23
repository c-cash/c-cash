#pragma once

#include "../parser/Statement.hpp"
#include "../parser/FunctionDefinition.hpp"
#include <iostream>
#include <map>
 
namespace interpreter {
    using namespace std;
    using namespace parser;

    class Functions {
        public:
            void declareVariableFunc(Statement &variable, Functions &func);
            void declareParameter(ParameterDefinition &var, Statement &value, Functions &func);
            void returnFunc(Statement &operations, Functions &func);
            void writeFunc(Statement &operations, Functions &func);
            void readFunc(Statement &operations, Functions &func);
            void changeVarValue(Statement &operations, Functions &func);
            bool startIf(Statement &operations, Functions &func);
        private:
            double startCalculations(Statement &operations, Functions &func);
            double calculating(Statement &operations, Functions &func);
            bool calculateIf(Statement &operations, Functions &func);
            double findVar(Statement &operations, Functions &func);

            map<string, double> doubleVarTab;
            map<string, int> intVarTab;
            map<string, unsigned int> uintVarTab;
            map<string, char> charVarTab;
            map<string, unsigned char> ucharVarTab;
    };

}