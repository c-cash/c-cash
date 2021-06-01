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
#include <iostream>

namespace parser {
    using namespace std;

    class Parser {
        public:
            Parser();
        
            void parse(vector<Token> &tokens);
            void DebugPrint() const;
            map<string, FunctionDefinition> mFunction;
   
        private:
            vector<Token>::iterator mCurrentToken;
            vector<Token>::iterator mEndToken;
            map<string, Type> mTypes;

            optional<Type> expectType();
            //! Empty string means match any identifier.
            optional<Token> expectIdentifier(const string& name = string());
            //! Empty string means match any operator.
            optional<Token> expectOperator(const string& name = string());
            optional<Token> expectAssignmentOperator(const string &name = string());
            optional<Token> expectIncDecOperator(const string &name = string());
            optional<Token> expectFuncOperator(const string &name = string());
            optional<Token> expectIdentifierIf(const string &name = string());
            optional<Statement> expectOneValue();
            optional<Statement> expectStatement();
            optional<Statement> expectVariableDeclaration();
            optional<Statement> expectFunctionCall();
            optional<Statement> expectExpression();
            optional<Statement> expectVariableCall();
            optional<Statement> expectExpressionFunc();
            optional<Statement> expectOneValueFunc();
            optional<Statement> expectArrayDeclaration();
            optional<Statement> expectLogicExpression();
            optional<Token> expectLogic(const string &name = string());
            bool isDeclaration();
            bool expectFunctionDefinition();
            bool nextTokenIsAlias();

            optional<Statement> parseNamespaceAlias();
            optional<Statement> parseLoopStatement();
            optional<vector<Statement>> parseFunctionBody();
            optional<Statement> parseIfStatement();

            size_t operatorPrecedence(const string &operatorName);
            size_t logicPrecedence(const string &operatorName);
            Statement * findRightmostStatement(Statement *lhs, size_t rhsPrecedence);
            FunctionDefinition includes;

            struct OperatorEntry {
                string mName;
                size_t mPrecedence;
            };
            
            map<string, OperatorEntry> sOperators {
                {"+", OperatorEntry{"+", 1}}, 
                {"-", OperatorEntry{"-", 1}}, 
                {"*", OperatorEntry{"*", 10}}, 
                {"/", OperatorEntry{"/", 10}},
                {"%", OperatorEntry{"%", 10}} 
            };

            map<string, OperatorEntry> sLogics {
                {"<", OperatorEntry{"<", 10}}, 
                {">", OperatorEntry{">", 10}}, 
                {"==", OperatorEntry{"==", 10}}, 
                {"!=", OperatorEntry{"!=", 10}},
                {"<=", OperatorEntry{"<=", 10}}, 
                {">=", OperatorEntry{">=", 10}}, 
                {"and", OperatorEntry{"and", 10}}, 
                {"or", OperatorEntry{"or", 10}} 
            };
    };
}
