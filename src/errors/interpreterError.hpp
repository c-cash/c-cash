#pragma once

#include "ccashError.hpp"

#include <map>
#include <string>
#include <vector>

namespace error {
    enum InterpreterErrorType {
        NO_MAIN,
        INVALID_ARGUMENT_COUNT,
        INVALID_ARGUMENT_COUNT_LINE,
        EXPECTED_ARRAY,
        VARIABLE_ALREADY_DEFINED,
        EXPECTED_EXPRESSION,
        NO_FUNCTION,
        OUTSIDE_SCOPE,
        INVALID_ARGUMENTS,
        NO_LIBRARY,
        NO_VARIABLE
    };

    static std::map<InterpreterErrorType, string> interpreterErrors = {
        {InterpreterErrorType::NO_MAIN, "program must have a 'main' function"},
        {InterpreterErrorType::INVALID_ARGUMENT_COUNT, "function '{A_0}' takes exactly {A_1} arguments but got {A_2}"},
        {InterpreterErrorType::INVALID_ARGUMENT_COUNT_LINE, "function '{A_0}' takes exactly {A_1} arguments but got {A_2} (line {A_3})"},
        {InterpreterErrorType::EXPECTED_ARRAY, "argument '{A_0}' should be array but got {A_1}"},
        {InterpreterErrorType::VARIABLE_ALREADY_DEFINED, "variable '{A_0}' in line {A_1} is already defined"},
        {InterpreterErrorType::EXPECTED_EXPRESSION, "'{A_0}' must have an expression"},
        {InterpreterErrorType::NO_FUNCTION, "cannot find function '{A_0}' in line {A_1}"},
        {InterpreterErrorType::OUTSIDE_SCOPE, "you can only use '{A_0}' outside of scope (line {A_1})"},
        {InterpreterErrorType::INVALID_ARGUMENTS, "invalid arguments for function '{A_0}' in line {A_1}"},
        {InterpreterErrorType::NO_LIBRARY, "cannot find library '{A_0}' in line {A_1}"},
        {InterpreterErrorType::NO_VARIABLE, "cannot find variable '{A_0}' in line {A_1}"}
    };

    // error(..., "a", "b", "c")
    static std::string getInterpreterError(InterpreterErrorType type, vector<string> args) {
        std::string e = interpreterErrors[type];
        for (uint32_t i{0}; i<args.size(); ++i) error::replacePart(e, "{A_" + to_string(i) + "}", args[i]);
        return e;
    }
}