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

    void Functions::declareVariableFunc(Statement &variable, Functions &func) {
        switch(variable.mType.mType) {
            case DOUBLE:
                if(variable.mStatements.size() > 0){
                    func.doubleVarTab[variable.mName] = startCalculations(variable.mStatements[0], func);
                } else {
                    func.doubleVarTab[variable.mName] = 0;
                }
                //cout<< variable.mName << " " << doubleVarTab[variable.mName] << '\n';
                break;
            case INT8:
                if(variable.mStatements.size() > 0){
                    func.charVarTab[variable.mName] = startCalculations(variable.mStatements[0], func);
                } else {
                    func.charVarTab[variable.mName] = 0;
                }
                //cout<< variable.mName << " " << charVarTab[variable.mName] << '\n';
                break;
            case UINT8:
                if(variable.mStatements.size() > 0){
                    func.ucharVarTab[variable.mName] = startCalculations(variable.mStatements[0], func);
                } else {
                    func.ucharVarTab[variable.mName] = 0;
                }
                //cout<< variable.mName << " " << ucharVarTab[variable.mName] << '\n';
                break;
            case INT32:
                if(variable.mStatements.size() > 0){
                    func.intVarTab[variable.mName] = startCalculations(variable.mStatements[0], func);
                } else {
                    func.intVarTab[variable.mName] = 0;
                }
                //cout<< variable.mName << " " << intVarTab[variable.mName] << '\n';
                break;
            case UINT32:
                if(variable.mStatements.size() > 0){
                    func.uintVarTab[variable.mName] = startCalculations(variable.mStatements[0], func);
                } else {
                   func. uintVarTab[variable.mName] = 0;
                }
                //cout<< variable.mName << " " << uintVarTab[variable.mName] << '\n';
                break;
            default:
                return;
        }
    }

    void Functions::declareParameter(ParameterDefinition &var, Statement &value,  Functions &func) {
        if(var.mType.mName == "double") {
            func.doubleVarTab[var.mName] = startCalculations(value, func);
        } else if(var.mType.mName == "signed char") {  
            func.charVarTab[var.mName] = startCalculations(value, func);
        } else if(var.mType.mName == "unsigned char")  {
            func.ucharVarTab[var.mName] = startCalculations(value, func);
        } else if(var.mType.mName == "signed int")  {
            func.intVarTab[var.mName] = startCalculations(value, func);
        } else if(var.mType.mName == "unsigned int") {
            func.uintVarTab[var.mName] = startCalculations(value, func);
        } else {
            return;
        }
    }

    double Functions::startCalculations(Statement &operations, Functions &func) {
        switch (operations.mName[0]) {
            case '+':
                return calculating(operations.mStatements[0], func) + calculating(operations.mStatements[1], func);
                break;
            case '-':
                return calculating(operations.mStatements[0], func) - calculating(operations.mStatements[1], func);
                break;
            case '*':
                return calculating(operations.mStatements[0], func) * calculating(operations.mStatements[1], func);
                break;
            case '/':
                if(calculating(operations.mStatements[1], func) == 0) {
                    throw runtime_error("Division by zero is impossible");
                }
                return calculating(operations.mStatements[0], func) / calculating(operations.mStatements[1], func);
                break;
            default:
                if(operations.mKind == StatementKind::LITTERAL){
                    std::string::size_type st;
                    return stod(operations.mName, &st);
                } else if(operations.mKind == StatementKind::VARIBLE_CALL_FUNC){
                    return findVar(operations, func);
                } else if(operations.mKind == StatementKind::FUNCTION_CALL){
                    if(mFunctions.find(operations.mName) != mFunctions.end()){
                        vector<Statement> args;
                        for(auto i : operations.mStatements){
                            Statement arg;
                            arg.mName = to_string(startCalculations(i, func));
                            arg.mKind = StatementKind::LITTERAL;
                            arg.mType.mType = BUILTIN_TYPE::INT32;
                            arg.mType.mName = "double";
                            args.push_back(arg);
                        }
                        cout << "==========\n";
                        for(auto i : args){
                            cout << i.mName  << " ";
                        }
                        cout << "\n==========\n";

                        inter.executeCommands(mFunctions[operations.mName], args);
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

    double Functions::calculating(Statement &operations,  Functions &func) {
        if(operations.mKind == StatementKind::LITTERAL){
            std::string::size_type st;
            return stod(operations.mName, &st);
        } else if(operations.mKind == StatementKind::VARIBLE_CALL_FUNC){
            return findVar(operations, func);
        } else if(operations.mKind == StatementKind::FUNCTION_CALL){
            if(mFunctions.find(operations.mName) != mFunctions.end()){
                inter.executeCommands(mFunctions[operations.mName], operations.mStatements);
            } else {
                throw runtime_error("No function find!");
            }
            return ret;
        } else {
            return startCalculations(operations, func);
        }
        return 0;
    }

    double Functions::findVar(Statement &operations, Functions &func){
        //cout << operations.mName << '\n';
        if(func.doubleVarTab.find(operations.mName) != func.doubleVarTab.end()){
            return func.doubleVarTab[operations.mName];
        } else if(func.intVarTab.find(operations.mName) != func.intVarTab.end()){
            return func.intVarTab[operations.mName];
        } else if(func.uintVarTab.find(operations.mName) != func.uintVarTab.end()){
            return func.uintVarTab[operations.mName];
        } else if(func.charVarTab.find(operations.mName) != func.charVarTab.end()){
            return func.charVarTab[operations.mName];
        } else if(func.ucharVarTab.find(operations.mName) != func.ucharVarTab.end()){
            return func.ucharVarTab[operations.mName];
        } else {
            throw runtime_error("Don't find varible!");
        }
    }
   
    void Functions::returnFunc(Statement &operations, Functions &func) {
        Statement o = operations.mStatements[0];
        if(o.mKind == StatementKind::VARIBLE_CALL_FUNC){
            ret = findVar(o, func);
        } else if(o.mKind == StatementKind::OPERATOR_CALL) {
            ret = startCalculations(o, func);
        } else if(o.mKind == StatementKind::LITTERAL) {
            std::string::size_type st;
            ret = stod(o.mName, &st);
            return;
        } else if(operations.mKind == StatementKind::FUNCTION_CALL){
            //cout << "=>> " << operations.mStatements[0].mName << '\n';
            if(mFunctions.find(operations.mStatements[0].mName) != mFunctions.end()){
                vector<Statement> args;
                for(auto i : operations.mStatements){
                    Statement arg;
                    arg.mName = to_string(startCalculations(i, func));
                    arg.mKind = StatementKind::LITTERAL;
                    arg.mType.mType = BUILTIN_TYPE::INT32;
                    arg.mType.mName = "double";
                    args.push_back(arg);
                }

                inter.executeCommands(mFunctions[operations.mStatements[0].mName], args);
            } else {
                throw runtime_error("No function find!");
            }
        } else {
            ret = 0;
        }
        //cout << "I'm going out this function" << endl;
    }

    void Functions::writeFunc(Statement &operations, Functions &func) {
        for(auto i : operations.mStatements){
            if(i.mKind == StatementKind::VARIBLE_CALL_FUNC){
                //SEARCH VALUE
                if(doubleVarTab.find(i.mName) != doubleVarTab.end()){
                    cout << doubleVarTab[i.mName];
                } else if(intVarTab.find(i.mName) != intVarTab.end()){
                    cout << intVarTab[i.mName];
                } else if(uintVarTab.find(i.mName) != uintVarTab.end()){
                    cout << uintVarTab[i.mName];
                } else if(charVarTab.find(i.mName) != charVarTab.end()){
                    cout << charVarTab[i.mName];
                } else if(ucharVarTab.find(i.mName) != ucharVarTab.end()){
                    cout << ucharVarTab[i.mName];
                } else {
                    throw runtime_error("Don't find varible!");
                }
            } else if(i.mKind == StatementKind::OPERATOR_CALL) {
                cout << startCalculations(i, func);
            } else if(i.mKind == StatementKind::LITTERAL) {
                cout << i.mName;
            } else if(i.mKind == StatementKind::FUNCTION_CALL){
                if(mFunctions.find(i.mName) != mFunctions.end()){
                    inter.executeCommands(mFunctions[i.mName], i.mStatements);
                    cout << ret;
                } else {
                    throw runtime_error("No function find!");
                }  
            } else {
                cout << i.mName;
            }
        }
    }

    void Functions::changeVarValue(Statement &operations, Functions &func) {
        if(func.doubleVarTab.find(operations.mName) != func.doubleVarTab.end()){
            func.doubleVarTab[operations.mName] = startCalculations(operations.mStatements[0], func);
        } else if(func.intVarTab.find(operations.mName) != func.intVarTab.end()){
            func.intVarTab[operations.mName] = startCalculations(operations.mStatements[0], func);
        } else if(func.uintVarTab.find(operations.mName) != func.uintVarTab.end()){
            func.uintVarTab[operations.mName] = startCalculations(operations.mStatements[0], func);
        } else if(func.charVarTab.find(operations.mName) != func.charVarTab.end()){
            func.charVarTab[operations.mName] = startCalculations(operations.mStatements[0], func);
        } else if(func.ucharVarTab.find(operations.mName) != func.ucharVarTab.end()){
            func.ucharVarTab[operations.mName] = startCalculations(operations.mStatements[0], func);
        } else {
            throw runtime_error("Don't find varible!");
        }
    }

    void Functions::readFunc(Statement &operations, Functions &func) {
        for(auto i=0; i<operations.mStatements.size(); ++i){
            if(func.doubleVarTab.find(operations.mStatements[i].mName) != func.doubleVarTab.end()){
                cin >> func.doubleVarTab[operations.mStatements[i].mName];
            } else if(func.intVarTab.find(operations.mStatements[i].mName) != func.intVarTab.end()){
                cin >> func.intVarTab[operations.mStatements[i].mName];
            } else if(func.uintVarTab.find(operations.mStatements[i].mName) != func.uintVarTab.end()){
                cin >> func.uintVarTab[operations.mStatements[i].mName];
            } else if(func.charVarTab.find(operations.mStatements[i].mName) != func.charVarTab.end()){
                cin >> func.charVarTab[operations.mStatements[i].mName];
            } else if(func.ucharVarTab.find(operations.mStatements[i].mName) != func.ucharVarTab.end()){
                cin >> func.ucharVarTab[operations.mStatements[i].mName];
            } else {
                throw runtime_error("Don't find varible!");
            }
        }
    }

    bool Functions::startIf(Statement &operations, Functions &func) {
        if ("and" == operations.mName) {
            return calculateIf(operations.mStatements[0], func) && calculateIf(operations.mStatements[1], func);
        } else if("or" == operations.mName) {
            return calculateIf(operations.mStatements[0], func) || calculateIf(operations.mStatements[1], func);
        } else {
            return calculateIf(operations, func);
        }
    }

    bool Functions::calculateIf(Statement &operations, Functions &func) {
        if(">" == operations.mName){
            return startCalculations(operations.mStatements[0], func) >  startCalculations(operations.mStatements[1], func);
        } else if("<" == operations.mName) {
            return startCalculations(operations.mStatements[0], func) < startCalculations(operations.mStatements[1], func);
        } else if("==" == operations.mName) {
            return startCalculations(operations.mStatements[0], func) == startCalculations(operations.mStatements[1], func);
        } else if ("!=" == operations.mName) {
            return startCalculations(operations.mStatements[0], func) != startCalculations(operations.mStatements[1], func);
        } else if("<=" == operations.mName) {
            return startCalculations(operations.mStatements[0], func) <= startCalculations(operations.mStatements[1], func);
        } else if (">=" == operations.mName) {
            return startCalculations(operations.mStatements[0], func) >= startCalculations(operations.mStatements[1], func);
        } else if("or" == operations.mName){
            startIf(operations, func);
        } else if("and" == operations.mName){
            startIf(operations, func);
        } else {
            throw runtime_error("Error in if operator");
        }
        return false;
    }
}