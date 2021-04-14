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
            optional<Type> expectType();

            //! Empty string means match any identifier.
            optional<Token> exceptIdentifier(const string& name = string());
            //! Empty string means match any operator.
            optional<Token> exceptOperator(const string& name = string());

            bool expectFunctionDefinition();
            optional<vector<Statement>> parseFunctionBody();
            optional<Statement> exceptOneStatement();
            optional<Statement> expectVariableDeclaration();
            optional<Statement> expectFunctionCall();

            vector<Token>::iterator mCurrentToken;
            vector<Token>::iterator mEndToken;
            map<string, Type> mTypes;
            map<string, FunctionDefinition> mFunction;
    };
}
