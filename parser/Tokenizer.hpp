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
        POTENTIAL_COMMENT,
        COMMENT
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
        "POTENTIAL_COMMENT",
        "COMMENT"
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
            void endToken(Token &token, vector<Token> &tokens);
    };
}

