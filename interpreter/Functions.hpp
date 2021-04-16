#pragma once

#include "Interpreter.hpp"
 
namespace interpreter {
    using namespace std;
    using namespace parser;

    class Functions {
        public:
            void declareVariableFunc(Statement &variable);
            void writeFunc(Statement &operations);
        private:
            double startCalculations(Statement &operations);
            double calculating(Statement &operations);
    };
}