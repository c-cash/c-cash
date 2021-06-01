#include "ExpectFunctions.hpp"

//checking if the current token is Identifier
optional<Token> ExpectFunctions::expectIdentifier(const string &name) {
    if(mCurrentToken == mEndToken) { return nullopt; }
    if(mCurrentToken->mType != IDENTIFIER) {return nullopt;}
    if(!name.empty() && mCurrentToken->mText != name) { return nullopt; }

    Token returnToken = *mCurrentToken;
    ++mCurrentToken;
    return returnToken;
}

//checking if the current token is If Identifier
optional<Token> ExpectFunctions::expectIdentifierIf(const string &name) {
    if(mCurrentToken == mEndToken) { return nullopt; }
    if(mCurrentToken->mType == OPERATOR) {return nullopt;}

    Token returnToken = *mCurrentToken;
    ++mCurrentToken;
    return returnToken;
}

//checking if the current token is Operator
optional<Token> ExpectFunctions::expectOperator(const string &name) {
    if(mCurrentToken == mEndToken) { return nullopt; }
    if(mCurrentToken->mType != OPERATOR ) {return nullopt;}
    if(!name.empty() && mCurrentToken->mText != name) { return nullopt; }

    Token returnToken = *mCurrentToken;
    ++mCurrentToken;
    return returnToken;
}

//checking if the current token is Assigment operator
optional<Token> ExpectFunctions::expectAssignmentOperator(const string &name) {
    if(mCurrentToken == mEndToken) { return nullopt; }
    if(mCurrentToken->mType != COMPOUND_ASSIGNMENT_OPERATOR ) {return nullopt;}
    if(!name.empty() && mCurrentToken->mText != name) { return nullopt; }

    Token returnToken = *mCurrentToken;
    return returnToken;
}

//checking if the current token is incrementation or decrementation operator
optional<Token> ExpectFunctions::expectIncDecOperator(const string &name) {
    if(mCurrentToken == mEndToken) { return nullopt; }
    if(mCurrentToken->mType !=  INC_DEC_OPERATOR) {return nullopt;}
    if(!name.empty() && mCurrentToken->mText != name) { return nullopt; }

    Token returnToken = *mCurrentToken;
    return returnToken;
}

//checking if the current token is functiom operator
optional<Token> ExpectFunctions::expectFuncOperator(const string &name) {
    vector<Token>::iterator nextToken = mCurrentToken;
    ++nextToken;
    if(nextToken == mEndToken) { return nullopt; }
    if(nextToken->mType != OPERATOR ) {return nullopt;}
    if(!name.empty() && nextToken->mText != name) { return nullopt; }

    Token returnToken = *nextToken;
    return returnToken;
}

//Find possibe type for varible
optional<Type> ExpectFunctions::expectType() {
    optional<Token> possibleType = expectIdentifier();
    if(!possibleType) { return nullopt; }

    map<string, Type>::iterator foundType = mTypes.find(possibleType->mText);
    
    if(foundType == mTypes.end()) {
        --mCurrentToken;
        return nullopt; 
    }
    return foundType->second; 
}

//Check logic
optional<Token> ExpectFunctions::expectLogic(const string &name) {
    if(mCurrentToken == mEndToken) { return nullopt; }
    if(mCurrentToken->mType != LOGIC && mCurrentToken->mText != "or" && mCurrentToken->mText != "and") {return nullopt;}
    if(!name.empty() && mCurrentToken->mText != name) { return nullopt; }

    Token returnToken = *mCurrentToken;
    ++mCurrentToken;
    return returnToken;
}

//Check if it's declaration
bool ExpectFunctions::isDeclaration() {
    vector<Token>::iterator startToken;
    startToken = mCurrentToken;
    while (!expectOperator(",").has_value()) {
        if(expectOperator(")").has_value()) {mCurrentToken = startToken; return false;}
        ++mCurrentToken;
    }
    mCurrentToken = startToken;
    return true;
}

//Check arrys declaration
optional<Statement> ExpectFunctions::expectArrayDeclaration() {
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

optional<Statement> ExpectFunctions::expectLogicExpression() {
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

//Check expression    
optional<Statement> ExpectFunctions::expectExpression() {
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

//Check expresion on func functions 
optional<Statement> ExpectFunctions::expectExpressionFunc() {
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

    //checks type of Statement
optional<Statement> ExpectFunctions::expectOneValue() {
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
    if(!result.has_value()) { 
        if(mCurrentToken != mEndToken && mCurrentToken->mType == IDENTIFIER && nextTokenIsAlias()) {
            result = parseNamespaceAlias();
        } else {
            result = expectFunctionCall(); 
        }
    }

    return result;
}

optional<Statement> ExpectFunctions::expectOneValueFunc() {
    optional<Statement> result;
    if(mCurrentToken != mEndToken && ( mCurrentToken->mType == DOUBLE_LITERAL || mCurrentToken->mType == INTEGER_LITERAL || mCurrentToken->mType == STRING_LITERAL)){
        return  expectOneValue();
    } else if(mCurrentToken != mEndToken && mCurrentToken->mType == IDENTIFIER && nextTokenIsAlias()){
        optional<Statement> namespaceAlias;
        namespaceAlias = parseNamespaceAlias();
        result = namespaceAlias;
    } else if(mCurrentToken != mEndToken && mCurrentToken->mType == IDENTIFIER && expectFuncOperator("(").has_value()){
        optional<Statement> functionCallStatement;
        functionCallStatement = expectFunctionCall();
        result = functionCallStatement;
    } else if(mCurrentToken != mEndToken && mCurrentToken->mType == IDENTIFIER && expectFuncOperator(".").has_value()){
        vector<Token>::iterator startToken = mCurrentToken;

        optional<Statement> varcall = expectVariableCall();
        if (varcall.has_value()) { // this is variable call :D
            result = varcall.value();
        } else {
            mCurrentToken = startToken;
            optional<Statement> funccall = expectFunctionCall();
            if (funccall.has_value()) { // this is function call
                result = funccall.value();
            } else {
                throw runtime_error(string("Expected variable or function call after '.' in line ") + to_string(mCurrentToken->mLine));
            }
        }
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

    if(!result.has_value()) { 
        if(mCurrentToken != mEndToken && mCurrentToken->mType == IDENTIFIER && nextTokenIsAlias()) {
            result = parseNamespaceAlias();
        } else {
            result = expectFunctionCall(); 
        } 
    }

    return result;
}

//Parse varible call
optional<Statement> ExpectFunctions::expectVariableCall(){
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
    } else if(expectAssignmentOperator("+=").has_value() || expectAssignmentOperator("-=").has_value() || expectAssignmentOperator("*=").has_value() || expectAssignmentOperator("/=").has_value() || expectAssignmentOperator("%=").has_value()) {
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

        statementVar.mKind = StatementKind::ARRAY_ELEMENT;
        statementVar.mStatements.push_back(expectExpressionFunc().value());
        if(!expectOperator("]").has_value())  throw runtime_error(string("You have to close square bracket in line ") + to_string(mCurrentToken->mLine));
        if(expectOperator("=").has_value()){
            statementVar.mStatements.push_back(expectExpressionFunc().value());
        }  else if(expectAssignmentOperator("+=").has_value() || expectAssignmentOperator("-=").has_value() || expectAssignmentOperator("*=").has_value() || expectAssignmentOperator("/=").has_value() || expectAssignmentOperator("%=").has_value()) {
            Statement operatorStatement;
            char opt = mCurrentToken->mText[0];
            operatorStatement.mName =  opt;
            operatorStatement.mKind = StatementKind::OPERATOR_CALL;
            ++mCurrentToken;
            optional<Statement> initialValue = expectExpressionFunc();
            if(!initialValue.has_value()) {
                throw runtime_error(string("Expected value to right of '=' in variable call in line ") + to_string(mCurrentToken->mLine));
            }
            Statement array = statementVar;
            Statement arrName;
            arrName.mName = array.mName;
            arrName.mKind = StatementKind::ARRAY_CALL;
            arrName.mType.mName = "func";
            array.mName = "";
            array.mStatements.push_back(arrName);
            operatorStatement.mStatements.push_back(array);
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
            Statement array = statementVar;
            Statement arrName;
            arrName.mName = array.mName;
            arrName.mKind = StatementKind::ARRAY_CALL;
            arrName.mType.mName = "func";
            array.mName = "";
            array.mStatements.push_back(arrName);
            operatorStatement.mStatements.push_back(array);
            operatorStatement.mStatements.push_back(one);
            statementVar.mStatements.push_back(operatorStatement);
        } else {
            throw runtime_error(string("You can't make operations after incrementarion/decrementation in line ") + to_string(mCurrentToken->mLine));
        }
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

//Try to declare a varible
optional<Statement> ExpectFunctions::expectVariableDeclaration() {
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
            optional<Statement> check = expectArrayDeclaration();
            if(!check.has_value()) {
                optional<Statement> initialValue = expectExpressionFunc();
                if(!initialValue.has_value()) {
                    throw runtime_error(string("Expected initial value to right of '=' in variable declaration in line ") + to_string(mCurrentToken->mLine));
                }
                statement.mStatements.push_back(initialValue.value());
            } else {
                statement.mStatements.push_back(check.value());
            }
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

//Parse funcion
optional<Statement> ExpectFunctions::expectFunctionCall(){
    vector<Token>::iterator startToken;
    startToken = mCurrentToken;

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
        startToken  = mCurrentToken;
        optional<Statement> parameter = expectLogicExpression();
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
                        andStatement.mStatements.push_back(expectLogicExpression().value());

                        parameter = andStatement;
                    } else if(mCurrentToken->mText == "or"){
                        ++mCurrentToken;
                        Statement orStatement;
                        orStatement.mStatements.push_back(parameter.value());
                        orStatement.mStatements.push_back(expectLogicExpression().value());
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

//Parse statement 
optional<Statement> ExpectFunctions::expectStatement() {
    optional<Statement> result;
    //IF it's if, elif or else
    if(mCurrentToken != mEndToken && mCurrentToken->mType == IDENTIFIER && (mCurrentToken->mText == "if" || mCurrentToken->mText == "elif" || mCurrentToken->mText == "else")) {
        result = parseIfStatement();
    } else if(mCurrentToken != mEndToken && mCurrentToken->mType == IDENTIFIER && (mCurrentToken->mText == "loop")) {
        result = parseLoopStatement();
    } else {
        result = expectExpression();

        if(!result.has_value()){
            result = expectVariableDeclaration();
        }
    }

    return result;
}