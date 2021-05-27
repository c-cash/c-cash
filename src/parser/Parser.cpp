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
		{"/", OperatorEntry{"/", 10}},
        {"%", OperatorEntry{"%", 10}} 
	};

    static map<string, OperatorEntry> sLogics {
		{"<", OperatorEntry{"<", 10}}, 
		{">", OperatorEntry{">", 10}}, 
		{"==", OperatorEntry{"==", 10}}, 
		{"!=", OperatorEntry{"!=", 10}},
        {"<=", OperatorEntry{"<=", 10}}, 
		{">=", OperatorEntry{">=", 10}}, 
		{"and", OperatorEntry{"and", 10}}, 
		{"or", OperatorEntry{"or", 10}} 
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
                            throw runtime_error(string("Expected a type at start of argument list in line ") + to_string(mCurrentToken->mLine));
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
                            throw runtime_error(string("Expected ',' to separate parametrs or ')' to indicate end of argument list in line ") + to_string(mCurrentToken->mLine));
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
        } else {
            mCurrentToken = parseStart;
            //TODO: Parsowanie statemetu
            optional<Statement> statement = expectStatement();
            if(statement.has_value()) {
                if(!expectOperator(";").has_value()) {throw runtime_error("Expected semicolon");}
                includes.mStatements.emplace_back(statement.value());
            }
            return true;
        }
		return false;
	}

    void Parser::parse(vector<Token> &tokens){
        mCurrentToken = tokens.begin();
        mEndToken = tokens.end();

        while (mCurrentToken != mEndToken) {
            if(expectFunctionDefinition()) {
				
			} else {
                cerr << "Unknown indentifier " << mCurrentToken->mText << endl;
                ++mCurrentToken;
            }
        }
        includes.mName = "*";
        mFunction[includes.mName] = includes;
    }

	Parser::Parser() {
		mTypes["func"] = Type("func", FUNC);
        mTypes["int"] = Type("signed int", INT32);
        mTypes["uint"] = Type("unsigned int", UINT32);
        mTypes["char"] = Type("signed char", INT8);
        mTypes["uchar"] = Type("unsigned char", UINT8);
        mTypes["double"] = Type("double", DOUBLE);
        mTypes["string"] = Type("string", STRING);
        mTypes["bool"] = Type("bool", STRING);
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
    optional<Token> Parser::expectAssignmentOperator(const string &name) {
        if(mCurrentToken == mEndToken) { return nullopt; }
        if(mCurrentToken->mType != COMPOUND_ASSIGNMENT_OPERATOR ) {return nullopt;}
        if(!name.empty() && mCurrentToken->mText != name) { return nullopt; }

        Token returnToken = *mCurrentToken;
        return returnToken;
    }

    optional<Token> Parser::expectIncDecOperator(const string &name) {
        if(mCurrentToken == mEndToken) { return nullopt; }
        if(mCurrentToken->mType !=  INC_DEC_OPERATOR) {return nullopt;}
        if(!name.empty() && mCurrentToken->mText != name) { return nullopt; }

        Token returnToken = *mCurrentToken;
        return returnToken;
    }

    optional<Token> Parser::expectFuncOperator(const string &name) {
        vector<Token>::iterator nextToken = mCurrentToken;
        ++nextToken;
        if(nextToken == mEndToken) { return nullopt; }
        if(nextToken->mType != OPERATOR ) {return nullopt;}
        if(!name.empty() && nextToken->mText != name) { return nullopt; }

        Token returnToken = *nextToken;
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
        bool ifStatement, loopStatement;
        
        while (!expectOperator("}").has_value()){
            ifStatement = false; loopStatement = false;
            if(mCurrentToken->mText == "if" || mCurrentToken->mText == "elif" || mCurrentToken->mText == "else") ifStatement = true;
            if(mCurrentToken->mText == "loop") loopStatement = true;
            optional<Statement> statement = expectStatement();
            if(statement.has_value()) { statements.push_back(statement.value()); }

            if(ifStatement == false && loopStatement == false) { if(!expectOperator(";").has_value()) { vector<Token>::iterator backToken = mCurrentToken; --backToken; throw runtime_error(string("Expected ';' at end of statement in line ") + to_string(backToken->mLine) ); }}
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
            doubleLiteralStatement.mKind = StatementKind::LITERAL;
            doubleLiteralStatement.mName = mCurrentToken->mText;
            doubleLiteralStatement.mType =  Type("double", DOUBLE);
            result = doubleLiteralStatement;
            ++mCurrentToken;
        } else if(mCurrentToken != mEndToken && mCurrentToken->mType == INTEGER_LITERAL){
            Statement integerLiteralStatement;
            integerLiteralStatement.mKind = StatementKind::LITERAL;
            integerLiteralStatement.mName = mCurrentToken->mText;
            integerLiteralStatement.mType =  Type("signed integer", INT32);
            result = integerLiteralStatement;
            ++mCurrentToken;
        } else if(mCurrentToken != mEndToken && mCurrentToken->mType == STRING_LITERAL){
            Statement stringLiteralStatement;
            stringLiteralStatement.mKind = StatementKind::LITERAL;
            stringLiteralStatement.mName = mCurrentToken->mText;
            stringLiteralStatement.mType =  Type("string", UINT8);
            result = stringLiteralStatement;
            ++mCurrentToken; 
        } else if (expectOperator("(").has_value()) {
            result = expectExpression();
            if (!expectOperator(")").has_value()) {
                throw runtime_error(string("Unbalanced '(' in parenthesized expression in line ") + to_string(mCurrentToken->mLine));
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
            doubleLiteralStatement.mKind = StatementKind::LITERAL;
            doubleLiteralStatement.mName = mCurrentToken->mText;
            doubleLiteralStatement.mType =  Type("double", DOUBLE);
            result = doubleLiteralStatement;
            ++mCurrentToken;
        } else if(mCurrentToken != mEndToken && mCurrentToken->mType == INTEGER_LITERAL){
            Statement integerLiteralStatement;
            integerLiteralStatement.mKind = StatementKind::LITERAL;
            integerLiteralStatement.mName = mCurrentToken->mText;
            integerLiteralStatement.mType =  Type("signed integer", INT32);
            result = integerLiteralStatement;
            ++mCurrentToken;
        } else if(mCurrentToken != mEndToken && mCurrentToken->mType == STRING_LITERAL){
            Statement stringLiteralStatement;
            stringLiteralStatement.mKind = StatementKind::LITERAL;
            stringLiteralStatement.mName = mCurrentToken->mText;
            stringLiteralStatement.mType =  Type("string", UINT8);
            result = stringLiteralStatement;
            ++mCurrentToken; 
        } else if(mCurrentToken != mEndToken && mCurrentToken->mType == IDENTIFIER && expectFuncOperator("(").has_value()){
            optional<Statement> functionCallStatement;
            functionCallStatement = expectFunctionCall();
            result = functionCallStatement;
        } else if(mCurrentToken != mEndToken && mCurrentToken->mType == IDENTIFIER){
            vector<Token>::iterator startToken = mCurrentToken;
            Statement variableCallStatement;

            ++mCurrentToken;
            //If its table element 
            if(expectOperator("[")){
                mCurrentToken = startToken;
                return expectVariableCall().value();
            } else {
                mCurrentToken = startToken;
                variableCallStatement.mKind = StatementKind::VARIABLE_CALL;
                variableCallStatement.mName = mCurrentToken->mText;
                variableCallStatement.mType =  Type("string", UINT8);
                result = variableCallStatement;
                ++mCurrentToken; 
            }
        } else if (expectOperator("(").has_value()) {
            result = expectExpression();
            if (!expectOperator(")").has_value()) {
                throw runtime_error(string("Unbalanced '(' in parenthesized expression in line ") + to_string(mCurrentToken->mLine));
            }
        }

        if(!result.has_value()){
            result = expectFunctionCall();
        }

        return result;
    }

    optional<Statement> Parser::expectVariableCall(){
        Statement statementVar;

        statementVar.mKind = StatementKind::VARIABLE_CALL;


        optional<Token> possibleVaribleName = expectIdentifier();
        statementVar.mName = possibleVaribleName->mText;
        if(expectOperator("(")) return nullopt;

        if(expectOperator("=").has_value()) {
            optional<Statement> initialValue = expectExpressionFunc();
            if(!initialValue.has_value()) {
                throw runtime_error(string("Expected value to right of '=' in variable call in line ") + to_string(mCurrentToken->mLine));
            }

            statementVar.mStatements.push_back(initialValue.value());
        } else if(expectAssignmentOperator("+=").has_value() || expectAssignmentOperator("-=").has_value() || expectAssignmentOperator("*=").has_value() || expectAssignmentOperator("/=").has_value()) {
            Statement operatorStatement;
            char opt = mCurrentToken->mText[0];
            operatorStatement.mName =  opt;
            operatorStatement.mKind = StatementKind::OPERATOR_CALL;
            ++mCurrentToken;
            optional<Statement> initialValue = expectExpressionFunc();
            if(!initialValue.has_value()) {
                throw runtime_error(string("Expected value to right of '=' in variable call in line ") + to_string(mCurrentToken->mLine));
            }
            operatorStatement.mStatements.push_back(statementVar);
            operatorStatement.mStatements.push_back(initialValue.value());

            statementVar.mStatements.push_back(operatorStatement);
        } else if(expectIncDecOperator("++").has_value() || expectIncDecOperator("--").has_value()) {
            Statement operatorStatement;
            char opt = mCurrentToken->mText[0];
            operatorStatement.mName =  opt;
            operatorStatement.mKind = StatementKind::OPERATOR_CALL;
            ++mCurrentToken;
            optional<Statement> initialValue = expectExpressionFunc();
            Statement one;
            one.mName = "1";
            one.mKind = StatementKind::LITERAL;
            one.mType =  Type("signed integer", INT32);
            if(!initialValue.has_value()) {
                operatorStatement.mStatements.push_back(statementVar);
                operatorStatement.mStatements.push_back(one);
                statementVar.mStatements.push_back(operatorStatement);
            } else {
                throw runtime_error(string("You can't make operations after incrementarion/decrementation in line ") + to_string(mCurrentToken->mLine));
            }
        } else if(expectOperator(".").has_value()) {
            // a.b;
            vector<Token>::iterator startToken = mCurrentToken;

            optional<Statement> varcall = expectVariableCall();
            if (varcall.has_value()) { // this is variable call :D
                statementVar.mStatements.push_back(varcall.value());
            } else {
                mCurrentToken = startToken;
                optional<Statement> funccall = expectFunctionCall();
                if (funccall.has_value()) { // this is function call
                    statementVar.mStatements.push_back(funccall.value());
                } else {
                    throw runtime_error(string("Expected variable or function call after '.' in line ") + to_string(mCurrentToken->mLine));
                }
                
            }
        }  else if(expectOperator("[").has_value()) {
            vector<Token>::iterator startToken = mCurrentToken;

            //a[1] = 2+2
            //write(a[1]);
            statementVar.mKind = StatementKind::ARRAY_ELEMENT;
            statementVar.mStatements.push_back(expectExpressionFunc().value());
            if(!expectOperator("]").has_value())  throw runtime_error(string("XD ") + to_string(mCurrentToken->mLine));
            if(expectOperator("=").has_value()){
                statementVar.mStatements.push_back(expectExpressionFunc().value());
            } else {
                string name = statementVar.mName;
                statementVar.mName = "";
                statementVar.mType.mName = "func";
                Statement s;
                s.mKind = StatementKind::ARRAY_CALL;
                s.mType.mName = "func";
                s.mName = possibleVaribleName->mText;
                statementVar.mStatements.push_back(s);
            }
        }

        return statementVar;
    }

    optional<Statement> Parser::expectVariableDeclaration() {
        vector<Token>::iterator startToken = mCurrentToken;
        optional<Type> possibleType = expectType();
 
        Statement statement;

        if(!possibleType.has_value()) {
            return expectVariableCall();
        } else {
            if(expectOperator("[").has_value() && expectOperator("]").has_value()){
                //Run table init function
                statement.mKind = StatementKind::ARRAY_DECLARATION;
                statement.mType = possibleType.value();
            } else {
                statement.mKind = StatementKind::VARIABLE_DECLARATION;
                statement.mType = possibleType.value();
            }
        }

        optional<Token> possibleVaribleName = expectIdentifier();
        statement.mName = possibleVaribleName->mText;
        if(expectOperator("=").has_value()) {
            if (statement.mKind == StatementKind::ARRAY_DECLARATION) {
                statement.mStatements.push_back(expectArrayDeclaration().value());
            } else {
                optional<Statement> initialValue = expectExpressionFunc();
                if(!initialValue.has_value()) {
                    throw runtime_error(string("Expected initial value to right of '=' in variable declaration in line ") + to_string(mCurrentToken->mLine));
                }

                statement.mStatements.push_back(initialValue.value());
            }
        }

        return statement;
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
            vector<Token>::iterator startToken  = mCurrentToken;
            optional<Statement> parameter = expectLogicExpressionFunc();
            if(parameter.has_value()) {
                startToken = mCurrentToken;
                if(expectOperator(")").has_value() || expectOperator(",").has_value()) {
                    mCurrentToken = startToken; 
                } else {
                    mCurrentToken = startToken; 
                    while (!expectOperator(")").has_value() && !expectOperator(",").has_value()) {
                        if(mCurrentToken->mText == "and"){
                            ++mCurrentToken;
                            Statement andStatement;
                            andStatement.mStatements.push_back(parameter.value());
                            andStatement.mName = "and";
                            andStatement.mKind = StatementKind::LOGIC_CALL;
                            andStatement.mStatements.push_back(expectLogicExpressionFunc().value());

                            parameter = andStatement;
                        } else if(mCurrentToken->mText == "or"){
                            ++mCurrentToken;
                            Statement orStatement;
                            orStatement.mStatements.push_back(parameter.value());
                            orStatement.mStatements.push_back(expectLogicExpressionFunc().value());
                            orStatement.mName = "or";
                            parameter = orStatement;
                        }

                        if(expectOperator(")").has_value()) {
                            functionCall.mStatements.push_back(parameter.value());
                            return functionCall;
                        }
                        if(expectOperator(",").has_value()) {
                            startToken = mCurrentToken;
                            --startToken;
                            break; 
                        }
                    }    
                    mCurrentToken = startToken;  
                }
            } else {
                mCurrentToken = startToken;
                parameter = expectExpressionFunc();
            }

            if(!parameter.has_value()) {
                throw runtime_error("Expected expression as parameter.");
            }
            functionCall.mStatements.push_back(parameter.value());

            if(expectOperator(")").has_value()) break;
            if(!expectOperator(",").has_value()) throw runtime_error(string("Expected ',' to separate parameter in line ") + to_string(mCurrentToken->mLine));
        }

        return functionCall;
    }

    optional<Statement> Parser::expectStatement() {
        optional<Statement> result;
        //IF it's if, elif or else
        if(mCurrentToken != mEndToken && mCurrentToken->mType == IDENTIFIER && (mCurrentToken->mText == "if" || mCurrentToken->mText == "elif" || mCurrentToken->mText == "else")) {
            result = parseIfStatement();
        } else if(mCurrentToken != mEndToken && mCurrentToken->mType == IDENTIFIER && (mCurrentToken->mText == "loop")) {
            result = parseLoopStatement();
        }  else {
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
		if(!lhs.has_value()) { 
            return expectArrayDeclaration();
        }

        while (true) {
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
                throw runtime_error(string("Expected logic expression as parameter in line ") + to_string(mCurrentToken->mLine));
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
            }
            ifS.mStatements.push_back(back_parameter.value());
        }

        optional<vector<Statement>> statements = parseFunctionBody();
        if(!statements.has_value()){
            throw runtime_error(string("Bad expression in if statement in line ") + to_string(mCurrentToken->mLine));
        }
        ifS.mStatements.insert(ifS.mStatements.end(), statements->begin(), statements->end());
        return ifS;
    }

    optional<Statement> Parser::expectLogicExpressionFunc() {
        optional<Statement> lhs = expectExpressionFunc();
        if(!lhs.has_value()) { return nullopt;}
        optional<Token> log = expectLogic();
        if(!log.has_value()) { return nullopt; }
        optional<Statement> rhs = expectExpressionFunc();
        if(!rhs.has_value()) {mCurrentToken--; return nullopt;}

        Statement result;
        result.mKind = StatementKind::LOGIC_CALL;
        result.mName = log->mText;
        result.mStatements.push_back(lhs.value());
        result.mStatements.push_back(rhs.value());

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

    optional<Statement> Parser::parseLoopStatement() {
        Statement loopS;
        optional<Statement> parameter;
        Statement addParametr;

        loopS.mName = "LOOP";

        ++mCurrentToken; ++mCurrentToken;
        vector<Token>::iterator startToken  = mCurrentToken;

        parameter = expectLogicExpressionFunc();
        if(!parameter.has_value()) {
            mCurrentToken = startToken;
            if(!isDeclaration()){
                parameter = expectExpressionFunc ();
            }
            if(!parameter.has_value()) {
                mCurrentToken = startToken;
                parameter = expectVariableDeclaration();
                if(!parameter.has_value()) {
                    throw runtime_error(string("Bad argument in loop in line ") + to_string(mCurrentToken->mLine));
                }
            }
        }


        if(!parameter.has_value()) { throw runtime_error(string("Expected logic expression as parameter in line ") + to_string(mCurrentToken->mLine)); }
        addParametr.mType.mType = FUNC_PARAM;
        addParametr.mType.mName = "func_param";
        addParametr.mStatements.clear();
        addParametr.mStatements.push_back(parameter.value());

        if(mCurrentToken->mText == ",") ++mCurrentToken;

        while (!expectOperator(")").has_value()) {
            if(mCurrentToken->mText == "and"){
                ++mCurrentToken;
                Statement andStatement;
                andStatement.mStatements.push_back(parameter.value());
                andStatement.mName = "and";
                andStatement.mKind = StatementKind::LOGIC_CALL;
                andStatement.mStatements.push_back(expectLogicExpressionFunc().value());
                andStatement.mType.mType = FUNC_PARAM;
                
                addParametr.mStatements.push_back(andStatement);
            } else if(mCurrentToken->mText == "or"){
                ++mCurrentToken;
                Statement orStatement;
                orStatement.mStatements.push_back(parameter.value());
                orStatement.mStatements.push_back(expectLogicExpressionFunc().value());
                orStatement.mName = "or";
                orStatement.mType.mType = FUNC_PARAM;

                addParametr.mStatements.push_back(orStatement);
            } else {
                optional<Statement> temParam;
                startToken = mCurrentToken;

                optional<Statement> logicParam = expectLogicExpressionFunc();
                if(!logicParam.has_value()) {
                    throw runtime_error(string("Expected logic expression as parameter in line ") + to_string(mCurrentToken->mLine));
                }
                while (!expectOperator(",").has_value()) {
                    if(mCurrentToken->mText == "and"){
                        ++mCurrentToken;
                        Statement andStatement;
                        andStatement.mStatements.push_back(logicParam.value());
                        andStatement.mName = "and";
                        andStatement.mKind = StatementKind::LOGIC_CALL;
                        andStatement.mStatements.push_back(expectLogicExpressionFunc().value());
                        
                        logicParam = andStatement;
                    } else if(mCurrentToken->mText == "or"){
                        ++mCurrentToken;
                        Statement orStatement;
                        orStatement.mStatements.push_back(logicParam.value());
                        orStatement.mStatements.push_back(expectLogicExpressionFunc().value());
                        orStatement.mName = "or";
                        logicParam = orStatement;
                    }

                    if(expectOperator(")").has_value()) break;
                }
                optional<Statement> mathParam = expectVariableCall();
                addParametr.mStatements.push_back(logicParam.value()); addParametr.mStatements.push_back(mathParam.value());

                ++mCurrentToken;
                break;
            }
        }

        loopS.mStatements.push_back(addParametr);

        optional<vector<Statement>> statements = parseFunctionBody();
        if(!statements.has_value()){
            throw runtime_error(string("Bad expression in loop statement body in line ") + to_string(mCurrentToken->mLine));
        }
        loopS.mStatements.insert(loopS.mStatements.end(), statements->begin(), statements->end());
        return loopS;
    }

    bool Parser::isDeclaration() {
        vector<Token>::iterator startToken = mCurrentToken;
        while (!expectOperator(",").has_value()) {
            if(expectOperator(")").has_value()) {mCurrentToken = startToken; return false;}
            ++mCurrentToken;
        }
        mCurrentToken = startToken;
        return true;
    }

    optional<Statement> Parser::expectArrayDeclaration() {
        // [] - empty array :D
        // [1] - array with one number
        // [1, 2] - array with multiple numbers

        // if no opening bracket then it's not an array declaration
        if (!expectOperator("[").has_value()) { return nullopt; }
        
        Statement statement;
        statement.mKind = StatementKind::ARRAY;
        statement.mType.mName = "arr";
        
        while (!expectOperator("]").has_value()) {
            optional<Statement> stmt = expectExpressionFunc();
            statement.mStatements.push_back(stmt.value());
            if(expectOperator("]").has_value()) { break; }
            if(!expectOperator(",").has_value()) { throw runtime_error(string("Expected ',' to separate values in array in line ") + to_string(mCurrentToken->mLine)); }
            
        }
        return statement;
    }
}