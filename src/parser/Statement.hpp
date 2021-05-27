#pragma once

#include <string>
#include <vector>
#include "Type.hpp"

namespace parser {
    using namespace std;

    enum class StatementKind {
        VARIABLE_DECLARATION,
        FUNCTION_CALL,
        LITERAL,
        OPERATOR_CALL,
        VARIABLE,
        VARIABLE_CALL,
        LOGIC_CALL,
        ARRAY_DECLARATION,
        ARRAY,
        ARRAY_ELEMENT,
        ARRAY_CALL
    };

    static const char* sStatementKindStrings[] = {
        "VARIABLE_DECLARATION",
        "FUNCTION_CALL",
        "LITTERAL",
        "OPERATOR_CALL",
        "VARIABLE",
        "VARIABLE_CALL",
        "LOGIC_CALL",
        "ARRAY_DECLARATION",
        "ARRAY",
        "ARRAY_ELEMENT",
        "ARRAY_CALL"
    };

    class Statement {
        public:
            string mName;
            Type mType{Type("func", FUNC)};
            vector<Statement> mStatements;
            StatementKind mKind{StatementKind::FUNCTION_CALL};

            void DebugPrint(size_t indent);
    };
}