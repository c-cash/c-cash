#include "Parser.hpp"
#include <iostream>

namespace parser {
    using namespace std;

	bool Parser::expectFunctionDefinition() {
		vector<Token>::iterator parseStart = mCurrentToken;
		optional<Type> possibleType = expectType();
        if(possibleType.has_value()){ //Value
            optional<Token> possibleName = exceptIdentifier();
            if(possibleName.has_value()){ //Name
                optional<Token> possibleOperator = exceptOperator("("); 
                if(possibleOperator.has_value()){ //Function
					FunctionDefinition func;
					func.mName = possibleName->mText;

                    while (!exceptOperator(")").has_value()) {
                        optional<Type> possibleParamType = expectType();
                        if(!possibleParamType.has_value()){
                            throw runtime_error("Expected a type at start of argument list.");
                        }

                        optional<Token> possibleVaribleName = exceptIdentifier();

                        ParameterDefinition param;
                        param.mType = possibleParamType->mName;
                        if(possibleVaribleName.has_value()){
                            param.mName = possibleVaribleName->mText;
                        }
                        func.mParameters.push_back(param);

                        if(exceptOperator(")").has_value()) break;
						if(!exceptOperator(",").has_value()) {
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
        mTypes["uchar"] = Type("uin8_t", UINT8);
        mTypes["double"] = Type("double", DOUBLE);
    }

    optional<Token> Parser::exceptIdentifier(const string &name) {
        if(mCurrentToken == mEndToken) { return nullopt; }
        if(mCurrentToken->mType != IDENTIFIER) {return nullopt;}
        if(!name.empty() && mCurrentToken->mText != name) { return nullopt; }

        Token returnToken = *mCurrentToken;
        ++mCurrentToken;
        return returnToken;
    }

    optional<Token> Parser::exceptOperator(const string &name) {
        if(mCurrentToken == mEndToken) { return nullopt; }
        if(mCurrentToken->mType != OPERATOR) {return nullopt;}
        if(!name.empty() && mCurrentToken->mText != name) { return nullopt; }

        Token returnToken = *mCurrentToken;
        ++mCurrentToken;
        return returnToken;
    }
    
    optional<Type> Parser::expectType() {
		optional<Token> possibleType = exceptIdentifier();
		if(!possibleType) { return nullopt; }

		map<string, Type>::iterator foundType = mTypes.find(possibleType->mText);
		
        if(foundType == mTypes.end()) {
			--mCurrentToken;
			return nullopt; 
		}

		return foundType->second; 
	}

	optional<vector<Statement>> Parser::parseFunctionBody() {
		if(!exceptOperator("{").has_value()) return nullopt;

        vector<Statement> statements;
        
        optional<Statement> statement = exceptOneStatement();
        if(statement.has_value()) {
            statements.push_back(statement.value());
        }

        //if(!exceptOperator("}").has_value()) throw runtime_error("Unbalanced '{'.");
        return statements;
	}

	void Parser::DebugPrint() const {
		for(auto funcPair : mFunction) {
			funcPair.second.DebugPrint();
		}
    }

    optional<Statement> Parser::exceptOneStatement() {
        optional<Statement> result;
        if(mCurrentToken != mEndToken && mCurrentToken->mType == DOUBLE_LITERAL){
            Statement doubleLiteralStatement;
            doubleLiteralStatement.mKind = StatementKind::LITTERAL;
            doubleLiteralStatement.mName = mCurrentToken->mText;
            doubleLiteralStatement.mType =  Type("double", DOUBLE);
            result = doubleLiteralStatement;
            ++mCurrentToken;
        } else {
            result = expectVariableDeclaration();
        }

        if(!result.has_value()){
            result = expectFunctionCall();
        }

        return result;
    }

    optional<Statement> Parser::expectVariableDeclaration() {
        vector<Token>::iterator startToken = mCurrentToken;
        optional<Type> possibleType = expectType();
        if(!possibleType.has_value()) {
            mCurrentToken = startToken;
            return nullopt;
        }

        optional<Token> possibleVaribleName = exceptIdentifier();
        if(!possibleType.has_value()) {
            mCurrentToken = startToken;
            return nullopt;
        }

        Statement statment;

        statment.mKind = StatementKind::VARIBLE_DECLARATION;
        statment.mName = possibleVaribleName->mText;
        statment.mType = possibleType.value();

        if(exceptOperator("=").has_value()) {
            optional<Statement> initialValue = exceptOneStatement();
            if(!initialValue.has_value()) {
                throw runtime_error("Expected initial value to right of '=' in variable declaration");
            }

            statment.mParameters.push_back(initialValue.value());
        }
    
        if(!exceptOperator(";").has_value()) {
            throw runtime_error("Expected ';' at end of variable declaration.");
        }

        return statment;
    }

    optional<Statement> Parser::expectFunctionCall(){
        return nullopt;
    }
}
