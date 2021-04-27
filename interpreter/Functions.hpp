#pragma once

#include "../parser/Statement.hpp"
#include "../parser/FunctionDefinition.hpp"
#include <iostream>
#include <map>
#include <variant>
 
namespace interpreter {
    using namespace std;
    using namespace parser;
/*
    template <typename T, std::enable_if_t<std::is_same_v<T, double> || std::is_same_v<T, std::string>, bool> = true>
    struct findVarStruct {
        T data;
    };
*/
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
            variant<double, string, int, unsigned int, char, unsigned char> startCalculations(Statement &operations, Scope &scope);
            variant<double, string, int, unsigned int, char, unsigned char> calculating(Statement &operations, Scope &scope);
            bool calculateIf(Statement &operations, Scope &scope);
            variant<double, string, int, unsigned int, char, unsigned char> findVar(Statement &operations, Scope &scope);
    };
}