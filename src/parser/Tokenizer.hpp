#pragma once

#include <vector>
#include <string>
#include <iostream>

namespace parser {
    using namespace std;

    enum TokenType {
        WHITESPACE,
        IDENTIFIER,
        INTEGER_LITERAL,
        STRING_LITERAL,
        DOUBLE_LITERAL,
        OPERATOR,
        STRING_ESCAPE_SEQUENCE,
        POTENTIAL_DOUBLE,
        COMMENT,
        LOGIC,
        POTENTIAL_LOGIC,
        BLOCK_COMMENT,
        COMPOUND_ASSIGNMENT_OPERATOR,
        INC_DEC_OPERATOR,
        POTENTIAL_SPECIFIC_OPERATOR,
        NAMESPACE_ALIAS,
        POTENTIAL_NAMESPACE_ALIAS
    };

    static const char *sTokenTypeStrings[] = {
        "WHITESPACE",
        "IDENTIFIER",
        "INTEGER_LITERAL",
        "STRING_LITERAL",
        "DOUBLE_LITERAL",
        "OPERATOR",
        "STRING_ESCAPE_SEQUENCE",
        "POTENTIAL_DOUBLE",
        "COMMENT",
        "LOGIC",
        "POTENTIAL_LOGIC",
        "BLOCK_COMMENT",
        "COMPOUND_ASSIGNMENT_OPERATOR",
        "INC_DEC_OPERATOR",
        "POTENTIAL_SPECIFIC_OPERATOR",
        "NAMESPACE_ALIAS",
        "POTENTIAL_NAMESPACE_ALIAS"
    };

    class Token {
        public:
            enum TokenType mType{WHITESPACE};
            string mText;
            size_t mLine{0};

            void DebugPrint() const;
    };

    class Tokenaizer {
        public:
            vector<Token> parse(const string &filename);
        private:
            static void endToken(Token &token, vector<Token> &tokens);
    };
}

