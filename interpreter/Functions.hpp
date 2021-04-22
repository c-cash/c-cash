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
            void declareVariableFunc(Statement &variable);
            void declareParameter(ParameterDefinition &var, Statement &value);
            void returnFunc(Statement &operations);
            void writeFunc(Statement &operations);
            void readFunc(Statement &operations);
            void changeVarValue(Statement &operations);
            bool startIf(Statement &operations);
        private:
            double startCalculations(Statement &operations);
            double calculating(Statement &operations);
            bool calculateIf(Statement &operations);
            double findVar(Statement &operations);

            map<string, double> doubleVarTab;
            map<string, int> intVarTab;
            map<string, unsigned int> uintVarTab;
            map<string, char> charVarTab;
            map<string, unsigned char> ucharVarTab;
    };

}