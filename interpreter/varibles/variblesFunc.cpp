#include "variblesFunc.hpp"

#include <string>
#include <iostream>

namespace interpreter {
    using namespace std;
    using namespace parser;

    double VariblesFunc::doubleValue(Statement &variable) {
        if(variable.mParameters.empty()) { return 0; }
        std::string::size_type st;  
        return stod(variable.mParameters[0].mName, &st);
    }

    char VariblesFunc::charValue(Statement &variable) {
        if(variable.mParameters.empty()) { return 0; }
        std::string::size_type st;
        return stoi(variable.mParameters[0].mName, &st);
    }

    unsigned char VariblesFunc::ucharValue(Statement &variable) {
        if(variable.mParameters.empty()) { return 0; }
        std::string::size_type st;
        return stoi(variable.mParameters[0].mName, &st);
    }

    int VariblesFunc::intValue(Statement &variable) {
        if(variable.mParameters.empty()) { return 0; }
        std::string::size_type st;
        return stoi(variable.mParameters[0].mName, &st);
    }

    unsigned int VariblesFunc::uintValue(Statement &variable) {
        if(variable.mParameters.empty()) { return 0; }
        std::string::size_type st;
        return stol(variable.mParameters[0].mName, &st);
    }
}