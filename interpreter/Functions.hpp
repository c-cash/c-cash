#pragma once

#include "Interpreter.hpp"
#include "Functions.hpp"

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
        private:
            double startCalculations(Statement &operations);
            double calculating(Statement &operations);

            map<string, double> doubleVarTab;
            map<string, int> intVarTab;
            map<string, unsigned int> uintVarTab;
            map<string, char> charVarTab;
            map<string, unsigned char> ucharVarTab;
    };

}