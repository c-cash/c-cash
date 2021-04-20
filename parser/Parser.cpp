#include "Parser.hpp"
#include <iostream>

namespace parser {
    using namespace std;

	struct OperatorEntry {
		string mName;
		size_t mPrecedence;
	};
	
    static map<string, OperatorEntry> sOperators {
		{"+", OperatorEntry{"+", 1}}, 
		{"-", OperatorEntry{"-", 1}}, 
		{"*", OperatorEntry{"*", 10}}, 
		{"/", OperatorEntry{"/", 10}} 
	};

    static map<string, OperatorEntry> sLogics {
		{"<", OperatorEntry{"<", 10}}, 
		{">", OperatorEntry{">", 10}}, 
		{"==", OperatorEntry{"==", 10}}, 
		{"!=", OperatorEntry{"!=", 10}},
        {"<=", OperatorEntry{"<=", 10}}, 
		{">=", OperatorEntry{">=", 10}}, 
		{"and", OperatorEntry{"and", 100}}, 
		{"or", OperatorEntry{"or", 100}} 
	};

	bool Parser::expectFunctionDefinition() {
		vector<Token>::iterator parseStart = mCurrentToken;
		optional<Type> possibleType = expectType();
        if(possibleType.has_value()){ //Value
            optional<Token> possibleName = expectIdentifier();
            if(possibleName.has_value()){ //Name
                optional<Token> possibleOperator = expectOperator("("); 
                if(possibleOperator.has_value()){ //Function or varible
				FunctionDefinition func;
					func.mName = possibleName->mText;

                    while (!expectOperator(")").has_value()) {
                        optional<Type> possibleParamType = expectType();
                        if(!possibleParamType.has_value()){
                            throw runtime_error("Expected a type at start of argument list.");
                        }

                        optional<Token> possibleVaribleName = expectIdentifier();

                        ParameterDefinition param;
                        param.mType = possibleParamType->mName;
                        if(possibleVaribleName.has_value()){
                            param.mName = possibleVaribleName->mText;
                        }
                        func.mParameters.push_back(param);

                        if(expectOperator(")").has_value()) break;
                        if(!expectOperator(",").has_value()) {
                            throw runtime_error("Expected ',' to separate parametrs or ')' to indicate end of argument list.");
                        }
                    }

                    optional<vector<Statement>> statements = parseFunctionBody();
                    if(!statements.has_value()){
                       mCurrentToken = parseStart;
                       return false;
                    }
                    func.mStatements.insert(func.mStatements.begin(), statements->begin(), statements->end());
                    
                    mFunction[func.mName] = func;
                        
                    return true;
                }  else {
                    mCurrentToken = parseStart;
                }
            }  else {
                mCurrentToken = parseStart;
            }
        }
		return false;
	}

    void Parser::parse(vector<Token> &tokens){
        mCurrentToken = tokens.begin();
        mEndToken = tokens.end();

        while (mCurrentToken != mEndToken) {
            if(expectFunctionDefinition()) {
				
			} else {
                cerr << "Unknow indentifier " << mCurrentToken->mText << endl;
                ++mCurrentToken;
            }
        }
		
    }

	Parser::Parser() {
		mTypes["void"] = Type("void", VOID);
        mTypes["int"] = Type("signed int", INT32);
        mTypes["uint"] = Type("unsigned int", UINT32);
        mTypes["char"] = Type("signed char", INT8);
        mTypes["uchar"] = Type("unsigned char", UINT8);
        mTypes["double"] = Type("double", DOUBLE);
    }

    optional<Token> Parser::expectIdentifier(const string &name) {
        if(mCurrentToken == mEndToken) { return nullopt; }
        if(mCurrentToken->mType != IDENTIFIER) {return nullopt;}
        if(!name.empty() && mCurrentToken->mText != name) { return nullopt; }

        Token returnToken = *mCurrentToken;
        ++mCurrentToken;
        return returnToken;
    }

    optional<Token> Parser::expectIdentifierIf(const string &name) {
        if(mCurrentToken == mEndToken) { return nullopt; }
        if(mCurrentToken->mType == OPERATOR) {return nullopt;}

        Token returnToken = *mCurrentToken;
        ++mCurrentToken;
        return returnToken;
    }

    optional<Token> Parser::expectOperator(const string &name) {
        if(mCurrentToken == mEndToken) { return nullopt; }
        if(mCurrentToken->mType != OPERATOR ) {return nullopt;}
        if(!name.empty() && mCurrentToken->mText != name) { return nullopt; }

        Token returnToken = *mCurrentToken;
        ++mCurrentToken;
        return returnToken;
    }
    
    optional<Type> Parser::expectType() {
		optional<Token> possibleType = expectIdentifier();
		if(!possibleType) { return nullopt; }

		map<string, Type>::iterator foundType = mTypes.find(possibleType->mText);
		
        if(foundType == mTypes.end()) {
			--mCurrentToken;
			return nullopt; 
		}

		return foundType->second; 
	}

	optional<vector<Statement>> Parser::parseFunctionBody() {
		if(!expectOperator("{").has_value()) return nullopt;

        vector<Statement> statements;
        bool ifStatement;
        
        while (!expectOperator("}").has_value()){
            ifStatement = false;
            if(mCurrentToken->mText == "if" || mCurrentToken->mText == "elif" || mCurrentToken->mText == "else") ifStatement = true;
            optional<Statement> statement = expectStatement();
            if(statement.has_value()) { statements.push_back(statement.value()); }

            if(ifStatement == false) { if(!expectOperator(";").has_value()) { cout << mCurrentToken->mText << " "; throw runtime_error("Expected ';' at end of statement."); }}
        }

        return statements;
	}

	void Parser::DebugPrint() const {
		for(auto funcPair : mFunction) {
			funcPair.second.DebugPrint();
		}
    }

    optional<Statement> Parser::expectOneValue() {
        optional<Statement> result;
        if(mCurrentToken != mEndToken && mCurrentToken->mType == DOUBLE_LITERAL){
            Statement doubleLiteralStatement;
            doubleLiteralStatement.mKind = StatementKind::LITTERAL;
            doubleLiteralStatement.mName = mCurrentToken->mText;
            doubleLiteralStatement.mType =  Type("double", DOUBLE);
            result = doubleLiteralStatement;
            ++mCurrentToken;
        } else if(mCurrentToken != mEndToken && mCurrentToken->mType == INTEGER_LITERAL){
            Statement integerLiteralStatement;
            integerLiteralStatement.mKind = StatementKind::LITTERAL;
            integerLiteralStatement.mName = mCurrentToken->mText;
            integerLiteralStatement.mType =  Type("signed integer", INT32);
            result = integerLiteralStatement;
            ++mCurrentToken;
        } else if(mCurrentToken != mEndToken && mCurrentToken->mType == STRING_LITERAL){
            Statement stringLiteralStatement;
            stringLiteralStatement.mKind = StatementKind::LITTERAL;
            stringLiteralStatement.mName = mCurrentToken->mText;
            stringLiteralStatement.mType =  Type("string", UINT8);
            result = stringLiteralStatement;
            ++mCurrentToken; 
        } else if (expectOperator("(").has_value()) {
            result = expectExpression();
            if (!expectOperator(")").has_value()) {
                throw runtime_error("Unbalanced '(' in parenthesized expression.");
            }
        }

        if(!result.has_value()){
            result = expectFunctionCall();
        }

        return result;
    }

    optional<Statement> Parser::expectOneValueFunc() {
        optional<Statement> result;
        if(mCurrentToken != mEndToken && mCurrentToken->mType == DOUBLE_LITERAL){
            Statement doubleLiteralStatement;
            doubleLiteralStatement.mKind = StatementKind::LITTERAL;
            doubleLiteralStatement.mName = mCurrentToken->mText;
            doubleLiteralStatement.mType =  Type("double", DOUBLE);
            result = doubleLiteralStatement;
            ++mCurrentToken;
        } else if(mCurrentToken != mEndToken && mCurrentToken->mType == INTEGER_LITERAL){
            Statement integerLiteralStatement;
            integerLiteralStatement.mKind = StatementKind::LITTERAL;
            integerLiteralStatement.mName = mCurrentToken->mText;
            integerLiteralStatement.mType =  Type("signed integer", INT32);
            result = integerLiteralStatement;
            ++mCurrentToken;
        } else if(mCurrentToken != mEndToken && mCurrentToken->mType == STRING_LITERAL){
            Statement stringLiteralStatement;
            stringLiteralStatement.mKind = StatementKind::LITTERAL;
            stringLiteralStatement.mName = mCurrentToken->mText;
            stringLiteralStatement.mType =  Type("string", UINT8);
            result = stringLiteralStatement;
            ++mCurrentToken; 
        } else if(mCurrentToken != mEndToken && mCurrentToken->mType == IDENTIFIER){
            Statement variableCallStatement;
            variableCallStatement.mKind = StatementKind::VARIBLE_CALL_FUNC;
            variableCallStatement.mName = mCurrentToken->mText;
            variableCallStatement.mType =  Type("string", UINT8);
            result = variableCallStatement;
            ++mCurrentToken; 
        } else if (expectOperator("(").has_value()) {
            result = expectExpression();
            if (!expectOperator(")").has_value()) {
                throw runtime_error("Unbalanced '(' in parenthesized expression.");
            }
        }

        if(!result.has_value()){
            result = expectFunctionCall();
        }

        return result;
    }

    optional<Statement> Parser::expectVariableCall(){
        Statement statment;

        statment.mKind = StatementKind::VARIABLE_CALL;

        optional<Token> possibleVaribleName = expectIdentifier();
        statment.mName = possibleVaribleName->mText;

        if(expectOperator("=").has_value()) {
            optional<Statement> initialValue = expectExpression();
            if(!initialValue.has_value()) {
                throw runtime_error("Expected initial value to right of '=' in variable declaration");
            }

            statment.mStatements.push_back(initialValue.value());
        }

        return statment;
    }

    optional<Statement> Parser::expectVariableDeclaration() {
        vector<Token>::iterator startToken = mCurrentToken;
        optional<Type> possibleType = expectType();

        Statement statment;

        if(!possibleType.has_value()) {
            return expectVariableCall();
        } else {
            statment.mKind = StatementKind::VARIABLE_DECLARATION;
            statment.mType = possibleType.value();
        }

        optional<Token> possibleVaribleName = expectIdentifier();
        statment.mName = possibleVaribleName->mText;

        if(expectOperator("=").has_value()) {
            optional<Statement> initialValue = expectExpression();
            if(!initialValue.has_value()) {
                throw runtime_error("Expected initial value to right of '=' in variable declaration");
            }

            statment.mStatements.push_back(initialValue.value());
        }

        return statment;
    }

    optional<Statement> Parser::expectFunctionCall(){
        vector<Token>::iterator startToken = mCurrentToken;

        optional<Token> possibleFunctionName = expectIdentifier();
        if(!possibleFunctionName.has_value()){
            mCurrentToken = startToken;
            return nullopt;
        }
        
        if(!expectOperator("(").has_value()){
            mCurrentToken = startToken;
            return nullopt;
        }

        Statement functionCall;
        functionCall.mKind = StatementKind::FUNCTION_CALL;
        functionCall.mName = possibleFunctionName->mText;

        while (!expectOperator(")").has_value()){
            optional<Statement> parameter = expectExpressionFunc();
            if(!parameter.has_value()) {
                throw runtime_error("Expected expression as parameter.");
            }
            functionCall.mStatements.push_back(parameter.value());

            if(expectOperator(")").has_value()) break;
            if(!expectOperator(",").has_value()) throw runtime_error(string("Expected ',' to separate parameter, found '") + mCurrentToken->mText + ".");
        }

        return functionCall;
    }

    optional<Statement> Parser::expectStatement() {
        optional<Statement> result;
        //IF it's if, elif or else
        if(mCurrentToken != mEndToken && mCurrentToken->mType == IDENTIFIER && (mCurrentToken->mText == "if" || mCurrentToken->mText == "elif" || mCurrentToken->mText == "else")) {
            result = parseIfStatement();
        } else {
            result = expectExpression();

            if(!result.has_value()){
                result = expectVariableDeclaration();
            }
        }

        return result;
    }

    optional<Statement> Parser::expectExpression() {
        optional<Statement> lhs = expectOneValue();
		if(!lhs.has_value()) { return nullopt; }

        while (true){
			optional<Token> op = expectOperator();
            if(!op.has_value()) { break; }
			int rhsPrecedence = operatorPrecedence(op->mText);
			if(rhsPrecedence == 0) {
                --mCurrentToken;
                return lhs;
            }
            optional<Statement> rhs = expectOneValue();
            if(!rhs.has_value()) {
                --mCurrentToken;
                return lhs;
            }

			Statement * rightmostStatement = findRightmostStatement(&lhs.value(), rhsPrecedence);
            if(rightmostStatement) {
				Statement operationCall;
				operationCall.mKind = StatementKind::OPERATOR_CALL;
				operationCall.mName = op->mText;
				operationCall.mStatements.push_back(rightmostStatement->mStatements.at(1));
				operationCall.mStatements.push_back(rhs.value());
				rightmostStatement->mStatements[1] = operationCall;
            } else {
				Statement operationCall;
				operationCall.mKind = StatementKind::OPERATOR_CALL;
				operationCall.mName = op->mText;
				operationCall.mStatements.push_back(lhs.value());
				operationCall.mStatements.push_back(rhs.value());
				lhs = operationCall;
			}
		}

        return lhs;
    }

    optional<Statement> Parser::expectExpressionFunc() {
        optional<Statement> lhs = expectOneValueFunc();
		if(!lhs.has_value()) { return nullopt; }

        while (true){
			optional<Token> op = expectOperator();
            if(!op.has_value()) { break; }
			int rhsPrecedence = operatorPrecedence(op->mText);
			if(rhsPrecedence == 0) {
                --mCurrentToken;
                return lhs;
            }
            optional<Statement> rhs = expectOneValueFunc();
            if(!rhs.has_value()) {
                --mCurrentToken;
                return lhs;
            }

			Statement * rightmostStatement = findRightmostStatement(&lhs.value(), rhsPrecedence);
            if(rightmostStatement) {
				Statement operationCall;
				operationCall.mKind = StatementKind::OPERATOR_CALL;
				operationCall.mName = op->mText;
				operationCall.mStatements.push_back(rightmostStatement->mStatements.at(1));
				operationCall.mStatements.push_back(rhs.value());
				rightmostStatement->mStatements[1] = operationCall;
            } else {
				Statement operationCall;
				operationCall.mKind = StatementKind::OPERATOR_CALL;
				operationCall.mName = op->mText;
				operationCall.mStatements.push_back(lhs.value());
				operationCall.mStatements.push_back(rhs.value());
				lhs = operationCall;
			}
		}

        return lhs;
    }

    Statement * Parser::findRightmostStatement(Statement *lhs, size_t rhsPrecedence) {
		if(lhs->mKind != StatementKind::OPERATOR_CALL) { return nullptr; }
		if(operatorPrecedence(lhs->mName) >= rhsPrecedence) { return nullptr; }
        
        Statement * rhs = &lhs->mStatements.at(1);
        rhs = findRightmostStatement(rhs, rhsPrecedence);
        if(rhs == nullptr) {return lhs;}
        return rhs;
	}

	size_t Parser::operatorPrecedence(const string &operatorName) {
        map<string, OperatorEntry>::iterator foundOperator = sOperators.find(operatorName);
        if(foundOperator == sOperators.end()) {
            return 0;
        }
		return foundOperator->second.mPrecedence;
    }

    optional<Statement> Parser::parseIfStatement() {
        Statement ifS;
        optional<Statement> back_parameter; 

        if(mCurrentToken->mText == "if") ifS.mName = "IF";
        else if(mCurrentToken->mText == "else") ifS.mName = "ELSE";
        else if(mCurrentToken->mText == "elif") ifS.mName = "ELIF";

        ++mCurrentToken;

        if(ifS.mName == "IF" || ifS.mName == "ELIF"){
            ++mCurrentToken;

            back_parameter = expectLogicExpressionFunc();
            if(!back_parameter.has_value()) {
                throw runtime_error("Expected logic expression as parameter.");
            }

            while (!expectOperator(")").has_value()) {
                if(mCurrentToken->mText == "and"){
                    ++mCurrentToken;
                    Statement andStatement;
                    andStatement.mStatements.push_back(back_parameter.value());
                    andStatement.mName = "and";
                    andStatement.mKind = StatementKind::LOGIC_CALL;
                    andStatement.mStatements.push_back(expectLogicExpressionFunc().value());
                    
                    back_parameter = andStatement;
                } else if(mCurrentToken->mText == "or"){
                    ++mCurrentToken;
                    Statement orStatement;
                    orStatement.mStatements.push_back(back_parameter.value());
                    orStatement.mStatements.push_back(expectLogicExpressionFunc().value());
                    orStatement.mName = "or";
                    back_parameter = orStatement;
                }

                if(expectOperator(")").has_value()) break;
            }
            ifS.mStatements.push_back(back_parameter.value());
        }
        cout << "tu\n";

        optional<vector<Statement>> statements = parseFunctionBody();
        if(!statements.has_value()){
            throw runtime_error("Bad command in if statement");
        }
        ifS.mStatements.insert(ifS.mStatements.end(), statements->begin(), statements->end());
        return ifS;
    }

    optional<Statement> Parser::expectLogicExpressionFunc() {
        optional<Statement> lhs = expectOneValueFunc();
        if(!lhs.has_value()) return nullopt;
        optional<Token> log = expectLogic();
        if(!log.has_value()) return nullopt;
        optional<Statement> rhs = expectOneValueFunc();
        if(!rhs.has_value()) {mCurrentToken--; return nullopt;}

        Statement result;
        result.mKind = StatementKind::LOGIC_CALL;
        result.mName = log->mText;
        result.mStatements.push_back(lhs.value());
        result.mStatements.push_back(rhs.value());

        cout << lhs.value().mName << " " << rhs.value().mName << '\n';

        return result;
    }

    optional<Token> Parser::expectLogic(const string &name) {
        if(mCurrentToken == mEndToken) { return nullopt; }
        if(mCurrentToken->mType != LOGIC && mCurrentToken->mText != "or" && mCurrentToken->mText != "and") {return nullopt;}
        if(!name.empty() && mCurrentToken->mText != name) { return nullopt; }

        Token returnToken = *mCurrentToken;
        ++mCurrentToken;
        return returnToken;
    }
}