#pragma once

#include <string>
#include <vector>
#include "Type.hpp"

//#include "Parser.hpp"

namespace parser {
    using namespace std;

    enum struct StatementKind {
        VARIABLE_DECLARATION,
        MULTIPLE_VARIABLE_DECLARATION,
        FUNCTION_CALL,
        LITERAL,
        OPERATOR_CALL,
        VARIABLE,
        VARIABLE_CALL,
        LOGIC_CALL,
        ARRAY_DECLARATION,
        MULTIPLE_ARRAY_DECLARATION,
        ARRAY,
        ARRAY_ELEMENT,
        ARRAY_CALL,
        NAMESPACE,
        INCREMENTATION,
        DECREMENTATION,
        TERNARY
    };

    static const char* sStatementKindStrings[] = {
        "VARIABLE_DECLARATION",
        "MULTIPLE_VARIABLE_DECLARATION",
        "FUNCTION_CALL",
        "LITTERAL",
        "OPERATOR_CALL",
        "VARIABLE",
        "VARIABLE_CALL",
        "LOGIC_CALL",
        "ARRAY_DECLARATION",
        "MULTIPLE_ARRAY_DECLARATION",
        "ARRAY",
        "ARRAY_ELEMENT",
        "ARRAY_CALL",
        "NAMESPACE",
        "INCREMENTATION",
        "DECREMENTATION,",
        "TERNARY"
    };

    class Statement {
        public:
            string mName;
            Type mType{Type("func", FUNC)};
            vector<Statement> mStatements;
            StatementKind mKind{StatementKind::FUNCTION_CALL};
            size_t mLine {0};

            void DebugPrint(size_t indent);
    };
}