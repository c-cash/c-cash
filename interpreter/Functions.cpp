#include "Functions.hpp"
#include "Interpreter.hpp"

#include <stdexcept>
#include <algorithm>
#include <string>

namespace interpreter {
    using namespace std;
    using namespace parser;

    double ret=0;
    extern map<string, parser::FunctionDefinition> mFunctions;

    Interpreter inter;

    void Functions::declareVariableFunc(Statement &variable, Scope &scope) {
        switch(variable.mType.mType) {
            case DOUBLE:
                if(variable.mStatements.size() > 0){
                    scope.doubleVarTab[variable.mName] =  startCalculations(variable.mStatements[0], scope);
                } else {
                    scope.doubleVarTab[variable.mName] = 0;
                }
                break;
            case INT8:
                if(variable.mStatements.size() > 0){
                    scope.charVarTab[variable.mName] =  startCalculations(variable.mStatements[0], scope);
                } else {
                    scope.charVarTab[variable.mName] = 0;
                }
                break;
            case UINT8:
                if(variable.mStatements.size() > 0){
                    scope.ucharVarTab[variable.mName] = startCalculations(variable.mStatements[0], scope);
                } else {
                    scope.ucharVarTab[variable.mName] = 0;
                }
                break;
            case INT32:
                if(variable.mStatements.size() > 0){
                    scope.intVarTab[variable.mName] = startCalculations(variable.mStatements[0], scope);
                } else {
                    scope.intVarTab[variable.mName] = 0;
                }
                break;
            case UINT32:
                if(variable.mStatements.size() > 0){
                    scope.uintVarTab[variable.mName] = startCalculations(variable.mStatements[0], scope);
                } else {
                   scope. uintVarTab[variable.mName] = 0;
                }
                break;
            case STRING:
                if(variable.mStatements.size() > 0){
                    scope.stringVarTab[variable.mName] = variable.mStatements[0].mName;
                } else {
                   scope. stringVarTab[variable.mName] = "";
                }
            
            default:
                return;
        }
    }

    void Functions::declareParameter(ParameterDefinition &var, Statement &value, Scope &scope) {
        if(var.mType.mName == "double") {
            scope.doubleVarTab[var.mName] = startCalculations(value, scope);
        } else if(var.mType.mName == "signed char") {  
            scope.charVarTab[var.mName] = startCalculations(value, scope);
        } else if(var.mType.mName == "unsigned char")  {
            scope.ucharVarTab[var.mName] = startCalculations(value, scope);
        } else if(var.mType.mName == "signed int")  {
            scope.intVarTab[var.mName] = startCalculations(value, scope);
        } else if(var.mType.mName == "unsigned int") {
            scope.uintVarTab[var.mName] = startCalculations(value, scope);
        } else if(var.mType.mName == "string") {
            scope.stringVarTab[var.mName] = startCalculations(value, scope);
        }  else {
            return;
        }
    }

    double Functions::startCalculations(Statement &operations, Scope &scope) {
        switch (operations.mName[0]) {
            case '+':
                return calculating(operations.mStatements[0], scope) + calculating(operations.mStatements[1], scope);
                break;
            case '-':
                return calculating(operations.mStatements[0], scope) - calculating(operations.mStatements[1], scope);
                break;
            case '*':
                return calculating(operations.mStatements[0], scope) * calculating(operations.mStatements[1], scope);
                break;
            case '/':
                if(calculating(operations.mStatements[1], scope) == 0) {
                    throw runtime_error("Division by zero is impossible");
                }
                return calculating(operations.mStatements[0], scope) / calculating(operations.mStatements[1], scope);
                break;
            default:
                if(operations.mKind == StatementKind::LITERAL){
                    std::string::size_type st;
                    return stod(operations.mName, &st);
                }/* else if(operations.mKind == StatementKind::VARIBLE_CALL){
                    return findVar(operations, scope);
                } */ else if(operations.mKind == StatementKind::VARIABLE_CALL){
                    return findVar(operations, scope);
                } else if(operations.mKind == StatementKind::FUNCTION_CALL){
                    if(mFunctions.find(operations.mName) != mFunctions.end()){
                        vector<Statement> args;
                        for(auto i : operations.mStatements){
                            Statement arg;
                            arg.mName = to_string(startCalculations(i, scope));
                            arg.mKind = StatementKind::LITERAL;
                            args.push_back(arg);
                        }

                        inter.executeFunction(mFunctions[operations.mName], args, scope);
                        return ret;
                    } else {
                        throw runtime_error("No function find!");
                    }   
                    return ret;
                }
                break;
        } 
        return 0;   
    }

    double Functions::calculating(Statement &operations, Scope &scope) {
        if(operations.mKind == StatementKind::LITERAL){
            std::string::size_type st;
            return stod(operations.mName, &st);
        } else if(operations.mKind == StatementKind::VARIABLE_CALL){
            return findVar(operations, scope);
        } else if(operations.mKind == StatementKind::FUNCTION_CALL){
            if(mFunctions.find(operations.mName) != mFunctions.end()){
                vector<Statement> args;
                for(auto i : operations.mStatements){
                    Statement arg;
                    arg.mName = to_string(startCalculations(i, scope));
                    arg.mKind = StatementKind::LITERAL;
                    args.push_back(arg);
                }

                inter.executeFunction(mFunctions[operations.mName], args, scope);
                return ret;
            } else {
                throw runtime_error("No function find!");
            }
        } else {
            return startCalculations(operations, scope);
        }
        return 0;
    }

    double Functions::findVar(Statement &operations, Scope &scope){
        //cout << operations.mName << '\n';
        if(scope.doubleVarTab.find(operations.mName) != scope.doubleVarTab.end()){
           return scope.doubleVarTab[operations.mName];
        } else if(scope.intVarTab.find(operations.mName) != scope.intVarTab.end()){
            return scope.intVarTab[operations.mName];
        } else if(scope.uintVarTab.find(operations.mName) != scope.uintVarTab.end()){
            return scope.uintVarTab[operations.mName];
        } else if(scope.charVarTab.find(operations.mName) != scope.charVarTab.end()){
            return scope.charVarTab[operations.mName];
        } else if(scope.ucharVarTab.find(operations.mName) != scope.ucharVarTab.end()){
            return  scope.ucharVarTab[operations.mName];
        } /* else if(scope.stringVarTab.find(operations.mName) != scope.stringVarTab.end()){
            return scope.stringVarTab[operations.mName];
        } */ else {
            throw runtime_error("Don't find varible!");
        }
        return 0;
    }
   
    void Functions::returnFunc(Statement &operations, Scope &scope) {
        Statement o = operations.mStatements[0];
        if(o.mKind == StatementKind::VARIABLE_CALL){
            ret = findVar(o, scope);
        } else if(o.mKind == StatementKind::OPERATOR_CALL) {
            ret = startCalculations(o, scope);
        } else if(o.mKind == StatementKind::LITERAL) {
            std::string::size_type st;
            ret = stod(o.mName, &st);
        } else if(operations.mKind == StatementKind::FUNCTION_CALL){
            if(mFunctions.find(operations.mStatements[0].mName) != mFunctions.end()){
                vector<Statement> args;
                for(auto i : operations.mStatements[0].mStatements){
                    Statement arg;
                    arg.mName = to_string(startCalculations(i, scope));
                    arg.mKind = StatementKind::LITERAL;
                    args.push_back(arg);
                }

                inter.executeFunction(mFunctions[operations.mStatements[0].mName], args, scope);
            } else {
                throw runtime_error("No function find!");
            }
        } else {
            ret = 0;
        } 
    }

    void Functions::writeFunc(Statement &operations, Scope &scope) {
        for(auto i : operations.mStatements){
            if(i.mKind == StatementKind::VARIABLE_CALL){
                cout << findVar(i, scope);
            } else if(i.mKind == StatementKind::OPERATOR_CALL) {
                cout << startCalculations(i, scope);
            } else if(i.mKind == StatementKind::LITERAL) {
                cout << i.mName;
            } else if(i.mKind == StatementKind::LOGIC_CALL) {
                if(startIf(i, scope)) { cout << true; }
                else { cout << false; }
            } else if(i.mKind == StatementKind::FUNCTION_CALL){
                if(mFunctions.find(i.mName) != mFunctions.end()){
                    startCalculations(i, scope);
                    cout << ret;
                } else {
                    throw runtime_error("No function find!");
                }  
            } else {
                throw runtime_error("Don't find cmd in write!");
            }
        }
    }

    void Functions::changeVarValue(Statement &operations, Scope &scope) {
        if(scope.doubleVarTab.find(operations.mName) != scope.doubleVarTab.end()){
            scope.doubleVarTab[operations.mName] = startCalculations(operations.mStatements[0], scope);
        } else if(scope.intVarTab.find(operations.mName) != scope.intVarTab.end()){
            scope.intVarTab[operations.mName] = startCalculations(operations.mStatements[0], scope);
        } else if(scope.uintVarTab.find(operations.mName) != scope.uintVarTab.end()){
            scope.uintVarTab[operations.mName] = startCalculations(operations.mStatements[0], scope);
        } else if(scope.charVarTab.find(operations.mName) != scope.charVarTab.end()){
            scope.charVarTab[operations.mName] = startCalculations(operations.mStatements[0], scope);
        } else if(scope.ucharVarTab.find(operations.mName) != scope.ucharVarTab.end()){
            scope.ucharVarTab[operations.mName] = startCalculations(operations.mStatements[0], scope);
        } else if(scope.stringVarTab.find(operations.mName) != scope.stringVarTab.end()){
            scope.stringVarTab[operations.mName] = startCalculations(operations.mStatements[0], scope);
        } else {
            throw runtime_error("Don't find varible!");
        }
    }

    void Functions::readFunc(Statement &operations, Scope &scope) {
        for(auto i=0; i<operations.mStatements.size(); ++i){
            if(scope.doubleVarTab.find(operations.mStatements[i].mName) != scope.doubleVarTab.end()){
                cin >> scope.doubleVarTab[operations.mStatements[i].mName];
            } else if(scope.intVarTab.find(operations.mStatements[i].mName) != scope.intVarTab.end()){
                cin >> scope.intVarTab[operations.mStatements[i].mName];
            } else if(scope.uintVarTab.find(operations.mStatements[i].mName) != scope.uintVarTab.end()){
                cin >> scope.uintVarTab[operations.mStatements[i].mName];
            } else if(scope.charVarTab.find(operations.mStatements[i].mName) != scope.charVarTab.end()){
                cin >> scope.charVarTab[operations.mStatements[i].mName];
            } else if(scope.ucharVarTab.find(operations.mStatements[i].mName) != scope.ucharVarTab.end()){
                cin >> scope.ucharVarTab[operations.mStatements[i].mName];
            } else if(scope.stringVarTab.find(operations.mName) != scope.stringVarTab.end()){
                scope.stringVarTab[operations.mName] = startCalculations(operations.mStatements[0], scope);
            }  else {
                throw runtime_error("Don't find varible!");
            }
        }
    }

    bool Functions::startIf(Statement &operations, Scope &scope) {
        if ("and" == operations.mName) {
            return calculateIf(operations.mStatements[0], scope) && calculateIf(operations.mStatements[1], scope);
        } else if("or" == operations.mName) {
            return calculateIf(operations.mStatements[0], scope) || calculateIf(operations.mStatements[1], scope);
        } else {
            return calculateIf(operations, scope);
        }
    }

    bool Functions::calculateIf(Statement &operations, Scope &scope) {
        if(">" == operations.mName){
            return startCalculations(operations.mStatements[0], scope) >  startCalculations(operations.mStatements[1], scope);
        } else if("<" == operations.mName) {
            return startCalculations(operations.mStatements[0], scope) < startCalculations(operations.mStatements[1], scope);
        } else if("==" == operations.mName) {
            return startCalculations(operations.mStatements[0], scope) == startCalculations(operations.mStatements[1], scope);
        } else if ("!=" == operations.mName) {
            return startCalculations(operations.mStatements[0], scope) != startCalculations(operations.mStatements[1], scope);
        } else if("<=" == operations.mName) {
            return startCalculations(operations.mStatements[0], scope) <= startCalculations(operations.mStatements[1], scope);
        } else if (">=" == operations.mName) {
            return startCalculations(operations.mStatements[0], scope) >= startCalculations(operations.mStatements[1], scope);
        } else if("or" == operations.mName){
            startIf(operations, scope);
        } else if("and" == operations.mName){
            startIf(operations, scope);
        } else {
            throw runtime_error("Error in if operator");
        }
        return false;
    }
}