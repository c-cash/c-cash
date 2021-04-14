#pragma once

#include <string>
#include <vector>
#include "Type.hpp"

namespace parser {
    using namespace std;

    enum class StatementKind {
        VARIBLE_DECLARATION,
        FUNCTION_CALL,
        LITTERAL
    };

    static const char* sStatementKindStrings[] = {
        "VARIBLE_DECLARATION",
        "FUNCTION_CALL",
        "LITTERAL"
    };

    class Statement {
        public:
            string mName;
            Type mType{Type("void", VOID)};
            vector<Statement> mParameters;
            StatementKind mKind{StatementKind::FUNCTION_CALL};

            void DebugPrint();
    };
}