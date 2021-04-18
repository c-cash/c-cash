#pragma once

#include <string>
#include <vector>
#include "Type.hpp"

namespace parser {
    using namespace std;

    enum class StatementKind {
        VARIABLE_DECLARATION,
        FUNCTION_CALL,
        LITTERAL,
        OPERATOR_CALL,
        VARIABLE,
        VARIABLE_CALL,
        VARIBLE_CALL_FUNC
    };

    static const char* sStatementKindStrings[] = {
        "VARIABLE_DECLARATION",
        "FUNCTION_CALL",
        "LITTERAL",
        "OPERATOR_CALL",
        "VARIABLE",
        "VARIABLE_CALL",
        "VARIBLE_CALL_FUNC"
    };

    class ParameterDefinitionIf {
        public:
            string mName; //Emty -> no name given
            Type mType;

            void DebugPrint(size_t indent) const;
    };

    class Statement {
        public:
            string mName;
            Type mType{Type("void", VOID)};
            vector<Statement> mStatements;
            vector<ParameterDefinitionIf> mParameters;
            StatementKind mKind{StatementKind::FUNCTION_CALL};

            void DebugPrint(size_t indent);
    };
}