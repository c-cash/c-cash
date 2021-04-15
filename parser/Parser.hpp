#pragma once

#include "Tokenizer.hpp"
#include "Type.hpp"
#include "FunctionDefinition.hpp"
#include "Statement.hpp"

#include "Type.hpp"
#include <optional>
#include <string>
#include <map>
#include <vector>

namespace parser {
    using namespace std;


    class Parser {
        public:
            Parser();
        
            void parse(vector<Token> &tokens);

            void DebugPrint() const;

        private:
            vector<Token>::iterator mCurrentToken;
            vector<Token>::iterator mEndToken;
            map<string, Type> mTypes;
            map<string, FunctionDefinition> mFunction;

            optional<Type> expectType();
            //! Empty string means match any identifier.
            optional<Token> expectIdentifier(const string& name = string());
            //! Empty string means match any operator.
            optional<Token> expectOperator(const string& name = string());

            bool expectFunctionDefinition();
            optional<vector<Statement>> parseFunctionBody();
            optional<Statement> expectOneValue();
            optional<Statement> expectStatement();
            optional<Statement> expectVariableDeclaration();
            optional<Statement> expectFunctionCall();
            optional<Statement> expectExpression();

            size_t operatorPrecedence(const string &operatorName);
            Statement * findRightmostStatement(Statement *lhs, size_t rhsPrecedence);
    };
}
