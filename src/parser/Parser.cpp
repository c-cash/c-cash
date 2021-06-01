#include "Parser.hpp"

namespace parser {
    using namespace std;

	bool Parser::expectFunctionDefinition() {
		vector<Token>::iterator parseStart = mCurrentToken;
		optional<Type> possibleType = expectType();

        if(possibleType.has_value()){ //Value
            optional<Token> possibleName = expectIdentifier();
            if(possibleName.has_value()){ //Name
                optional<Token> possibleOperator = expectOperator("("); 
                if(possibleOperator.has_value()){ //Function or varible
				    FunctionDefinition func;
                    optional<Type> possibleParamType;
                    bool ifArray;

					func.mName = possibleName->mText;

                    while (!expectOperator(")").has_value()) {
                        //Set value
                        possibleParamType = expectType();
                        ifArray = false; //Check bool for arrays

                        if(!possibleParamType.has_value()){
                            throw runtime_error(string("Expected a type at start of argument list in line ") + to_string(mCurrentToken->mLine));
                        } else {
                            if(expectOperator("[").has_value() && expectOperator("]").has_value()) ifArray = true;
                        }

                        optional<Token> possibleVaribleName = expectIdentifier();

                        ParameterDefinition param;
                        param.mType = possibleParamType->mName;
                        if(ifArray) param.isArray = true;

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
            optional<Statement> statement = expectStatement();
            if(statement.has_value()) {
                if(!expectOperator(";").has_value()) {throw runtime_error("Expected semicolon");}
                includes.mStatements.emplace_back(statement.value());
            }
            return true;
        }
		return false;
	}

    //Start parsing
    void Parser::parse(vector<Token> &tokens){
        mCurrentToken = tokens.begin();
        mEndToken = tokens.end();

        while (mCurrentToken != mEndToken) {
            if(!expectFunctionDefinition()) {
				cerr << "Unknown indentifier " << mCurrentToken->mText << endl;
                ++mCurrentToken;
			}
        }
        includes.mName = "*";
        mFunction[includes.mName] = includes;
    }

    //Set typess
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

    //prints parser logs
	void Parser::DebugPrint() const {
		for(auto funcPair : mFunction) {
			funcPair.second.DebugPrint();
		}
    }



    //Parse function
	optional<vector<Statement>> Parser::parseFunctionBody() {
		if(!expectOperator("{").has_value()) return nullopt;

        vector<Statement> statements;
        bool specialStatement;
        
        while (!expectOperator("}").has_value()){
            specialStatement = false;
            if(mCurrentToken->mText == "if" || mCurrentToken->mText == "elif" || mCurrentToken->mText == "else" || mCurrentToken->mText == "loop") { specialStatement = true; }
            optional<Statement> statement = expectStatement();
            if(statement.has_value()) { statements.push_back(statement.value()); }

            if(specialStatement == false) {
                //Check ';' char at the end of line 
                if(!expectOperator(";").has_value()) { 
                    vector<Token>::iterator backToken = mCurrentToken; 
                    --backToken; 
                    throw runtime_error(string("Expected ';' at end of statement in line ") + to_string(backToken->mLine)); 
                }
            }
        }

        return statements;
	}

    bool Parser::nextTokenIsAlias() {
        vector<Token>::iterator actual = mCurrentToken;
        mCurrentToken++;
        if(mCurrentToken->mType == NAMESPACE_ALIAS) {
            mCurrentToken = actual;
            return true;
        } else {
            mCurrentToken = actual;
            return false;
        }
    }

    optional<Statement> Parser::parseNamespaceAlias() {
        Statement namespaceAlias;
		namespaceAlias.mKind = StatementKind::NAMESPACE;
		namespaceAlias.mName = mCurrentToken->mText;
        mCurrentToken++;
        mCurrentToken++;
		namespaceAlias.mStatements.push_back(expectFunctionCall().value());
        return namespaceAlias;
    }

    //Find right statement
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

    //Parse if statements
    optional<Statement> Parser::parseIfStatement() {
        Statement ifS;
        optional<Statement> back_parameter; 

        if(mCurrentToken->mText == "if") ifS.mName = "IF";
        else if(mCurrentToken->mText == "else") ifS.mName = "ELSE";
        else if(mCurrentToken->mText == "elif") ifS.mName = "ELIF";

        ++mCurrentToken;

        if(ifS.mName == "IF" || ifS.mName == "ELIF"){
            ++mCurrentToken;

            back_parameter = expectLogicExpression();
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
                    andStatement.mStatements.push_back(expectLogicExpression().value());
                    
                    back_parameter = andStatement;
                } else if(mCurrentToken->mText == "or"){
                    ++mCurrentToken;
                    Statement orStatement;
                    orStatement.mStatements.push_back(back_parameter.value());
                    orStatement.mStatements.push_back(expectLogicExpression().value());
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


    //Parse loops
    optional<Statement> Parser::parseLoopStatement() {
        Statement loopS;
        optional<Statement> parameter;
        Statement addParametr;

        loopS.mName = "LOOP";

        ++mCurrentToken; ++mCurrentToken;
        vector<Token>::iterator startToken  = mCurrentToken;

        parameter = expectLogicExpression();
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
                andStatement.mStatements.push_back(expectLogicExpression().value());
                andStatement.mType.mType = FUNC_PARAM;
                
                addParametr.mStatements.push_back(andStatement);
            } else if(mCurrentToken->mText == "or"){
                ++mCurrentToken;
                Statement orStatement;
                orStatement.mStatements.push_back(parameter.value());
                orStatement.mStatements.push_back(expectLogicExpression().value());
                orStatement.mName = "or";
                orStatement.mType.mType = FUNC_PARAM;

                addParametr.mStatements.push_back(orStatement);
            } else {
                optional<Statement> temParam;
                startToken = mCurrentToken;

                optional<Statement> logicParam = expectLogicExpression();
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
                        andStatement.mStatements.push_back(expectLogicExpression().value());
                        
                        logicParam = andStatement;
                    } else if(mCurrentToken->mText == "or"){
                        ++mCurrentToken;
                        Statement orStatement;
                        orStatement.mStatements.push_back(logicParam.value());
                        orStatement.mStatements.push_back(expectLogicExpression().value());
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
}