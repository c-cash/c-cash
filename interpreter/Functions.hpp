#pragma once

#include "../parser/Statement.hpp"

#include <iostream>
#include <map>
 
namespace interpreter {
    using namespace std;
    using namespace parser;

    class Functions {
        public:
            void declareVariableFunc(Statement &variable);
            void writeFunc(Statement &operations);
            void readFunc(Statement &cmd);
            void changeVarValue(Statement &cmd);
            bool startIf(Statement &cmd);
        private:
            double startCalculations(Statement &operations);
            double calculating(Statement &operations);
            bool calculateIf(Statement &cmd1, Statement &cmd2, string log);

            map<string, double> doubleVarTab;
            map<string, int> intVarTab;
            map<string, unsigned int> uintVarTab;
            map<string, char> charVarTab;
            map<string, unsigned char> ucharVarTab;
    };

}