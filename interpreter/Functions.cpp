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

    void Functions::declareVariableFunc(Statement &variable) {
        switch(variable.mType.mType) {
            case DOUBLE:
                if(variable.mStatements.size() > 0){
                    doubleVarTab[variable.mName] = startCalculations(variable.mStatements[0]);
                } else {
                    doubleVarTab[variable.mName] = 0;
                }
                //cout<< variable.mName << " " << doubleVarTab[variable.mName] << '\n';
                break;
            case INT8:
                if(variable.mStatements.size() > 0){
                    charVarTab[variable.mName] = startCalculations(variable.mStatements[0]);
                } else {
                    charVarTab[variable.mName] = 0;
                }
                //cout<< variable.mName << " " << charVarTab[variable.mName] << '\n';
                break;
            case UINT8:
                if(variable.mStatements.size() > 0){
                    ucharVarTab[variable.mName] = startCalculations(variable.mStatements[0]);
                } else {
                    ucharVarTab[variable.mName] = 0;
                }
                //cout<< variable.mName << " " << ucharVarTab[variable.mName] << '\n';
                break;
            case INT32:
                if(variable.mStatements.size() > 0){
                    intVarTab[variable.mName] = startCalculations(variable.mStatements[0]);
                } else {
                    intVarTab[variable.mName] = 0;
                }
                //cout<< variable.mName << " " << intVarTab[variable.mName] << '\n';
                break;
            case UINT32:
                if(variable.mStatements.size() > 0){
                    uintVarTab[variable.mName] = startCalculations(variable.mStatements[0]);
                } else {
                    uintVarTab[variable.mName] = 0;
                }
                //cout<< variable.mName << " " << uintVarTab[variable.mName] << '\n';
                break;
            default:
                return;
        }
    }

    void Functions::declareParameter(ParameterDefinition &var, Statement &value) {
        if(var.mType.mName == "double")
            doubleVarTab[var.mName] = startCalculations(value);
        else if(var.mType.mName == "signed char")  
            charVarTab[var.mName] = startCalculations(value);
        else if(var.mType.mName == "unsigned char")  
            ucharVarTab[var.mName] = startCalculations(value);
        else if(var.mType.mName == "signed int")  
            intVarTab[var.mName] = startCalculations(value);
        else if(var.mType.mName == "unsigned int")  
            uintVarTab[var.mName] = startCalculations(value);
        else
            return;
    }

    double Functions::startCalculations(Statement &operations) {
        switch (operations.mName[0]) {
            case '+':
                return calculating(operations.mStatements[0]) + calculating(operations.mStatements[1]);
                break;
            case '-':
                return calculating(operations.mStatements[0]) - calculating(operations.mStatements[1]);
                break;
            case '*':
                return calculating(operations.mStatements[0]) * calculating(operations.mStatements[1]);
                break;
            case '/':
                if(calculating(operations.mStatements[1]) == 0) {
                    throw runtime_error("Division by zero is impossible");
                }
                return calculating(operations.mStatements[0]) / calculating(operations.mStatements[1]);
                break;
            default:
                if(operations.mKind == StatementKind::LITTERAL){
                    std::string::size_type st;
                    return stod(operations.mName, &st);
                } else if(operations.mKind == StatementKind::VARIBLE_CALL_FUNC){
                    return findVar(operations);
                } else if(operations.mKind == StatementKind::FUNCTION_CALL){
                    if(mFunctions.find(operations.mName) != mFunctions.end()){
                        inter.executeCommands(mFunctions[operations.mName], operations.mStatements);
                    }
                    return ret;
                }
                break;
        } 
        return 0;   
    }

    double Functions::calculating(Statement &operations) {
        if(operations.mKind == StatementKind::LITTERAL){
            std::string::size_type st;
            return stod(operations.mName, &st);
        } else if(operations.mKind == StatementKind::VARIBLE_CALL_FUNC){
            return findVar(operations);
        } else if(operations.mKind == StatementKind::FUNCTION_CALL){
            if(mFunctions.find(operations.mName) != mFunctions.end()){
                inter.executeCommands(mFunctions[operations.mName], operations.mStatements);
            }
            return ret;
        } else {
            return startCalculations(operations);
        }
        return 0;
    }

    double Functions::findVar(Statement &operations){
        if(doubleVarTab.find(operations.mName) != doubleVarTab.end()){
            return doubleVarTab[operations.mName];
        } else if(intVarTab.find(operations.mName) != intVarTab.end()){
            return intVarTab[operations.mName];
        } else if(uintVarTab.find(operations.mName) != uintVarTab.end()){
            return uintVarTab[operations.mName];
        } else if(charVarTab.find(operations.mName) != charVarTab.end()){
            return charVarTab[operations.mName];
        } else if(ucharVarTab.find(operations.mName) != ucharVarTab.end()){
            return ucharVarTab[operations.mName];
        } else {
            throw runtime_error("Don't find varible!");
        }
    }
   
    void Functions::returnFunc(Statement &operations) {
            Statement o = operations.mStatements[0];
            if(o.mKind == StatementKind::VARIBLE_CALL_FUNC){
                ret = findVar(o);
            } else if(o.mKind == StatementKind::OPERATOR_CALL) {
                ret = startCalculations(o);
            } else if(o.mKind == StatementKind::LITTERAL) {
                std::string::size_type st;
                ret = stod(o.mName, &st);
            } else if(operations.mKind == StatementKind::FUNCTION_CALL){
                if(mFunctions.find(operations.mName) != mFunctions.end()){
                    ret = 0;
                    inter.executeCommands(mFunctions[operations.mName], operations.mStatements);
                }
            } else {
                std::string::size_type st;
                ret = stod(o.mName, &st);
            }
    }

    void Functions::writeFunc(Statement &operations) {
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
                cout << startCalculations(i);
            } else if(i.mKind == StatementKind::LITTERAL) {
                cout << i.mName;
            } else if(operations.mKind == StatementKind::FUNCTION_CALL){
                if(mFunctions.find(operations.mName) != mFunctions.end()){
                    inter.executeCommands(mFunctions[operations.mName],  operations.mStatements);
                    cout << ret;
                }
            } else {
                cout << i.mName;
            }
        }
    }

    void Functions::changeVarValue(Statement &operations) {
        if(doubleVarTab.find(operations.mName) != doubleVarTab.end()){
            doubleVarTab[operations.mName] = startCalculations(operations.mStatements[0]);
        } else if(intVarTab.find(operations.mName) != intVarTab.end()){
            intVarTab[operations.mName] = startCalculations(operations.mStatements[0]);
        } else if(uintVarTab.find(operations.mName) != uintVarTab.end()){
            uintVarTab[operations.mName] = startCalculations(operations.mStatements[0]);
        } else if(charVarTab.find(operations.mName) != charVarTab.end()){
            charVarTab[operations.mName] = startCalculations(operations.mStatements[0]);
        } else if(ucharVarTab.find(operations.mName) != ucharVarTab.end()){
            ucharVarTab[operations.mName] = startCalculations(operations.mStatements[0]);
        } else {
            throw runtime_error("Don't find varible!");
        }
    }

    void Functions::readFunc(Statement &operations) {
        for(auto i=0; i<operations.mStatements.size(); ++i){
            if(doubleVarTab.find(operations.mStatements[i].mName) != doubleVarTab.end()){
                cin >> doubleVarTab[operations.mStatements[i].mName];
            } else if(intVarTab.find(operations.mStatements[i].mName) != intVarTab.end()){
                cin >> intVarTab[operations.mStatements[i].mName];
            } else if(uintVarTab.find(operations.mStatements[i].mName) != uintVarTab.end()){
                cin >> uintVarTab[operations.mStatements[i].mName];
            } else if(charVarTab.find(operations.mStatements[i].mName) != charVarTab.end()){
                cin >> charVarTab[operations.mStatements[i].mName];
            } else if(ucharVarTab.find(operations.mStatements[i].mName) != ucharVarTab.end()){
                cin >> ucharVarTab[operations.mStatements[i].mName];
            } else {
                throw runtime_error("Don't find varible!");
            }
        }
    }

    bool Functions::startIf(Statement &operations) {
        if ("and" == operations.mName) {
            return calculateIf(operations.mStatements[0]) && calculateIf(operations.mStatements[1]);
        } else if("or" == operations.mName) {
            return calculateIf(operations.mStatements[0]) || calculateIf(operations.mStatements[1]);
        } else {
            return calculateIf(operations);
        }
    }

    bool Functions::calculateIf(Statement &operations) {
        if(">" == operations.mName){
            return startCalculations(operations.mStatements[0]) >  startCalculations(operations.mStatements[1]);
        } else if("<" == operations.mName) {
            return startCalculations(operations.mStatements[0]) < startCalculations(operations.mStatements[1]);
        } else if("==" == operations.mName) {
            return startCalculations(operations.mStatements[0]) == startCalculations(operations.mStatements[1]);
        } else if ("!=" == operations.mName) {
            return startCalculations(operations.mStatements[0]) != startCalculations(operations.mStatements[1]);
        } else if("<=" == operations.mName) {
            return startCalculations(operations.mStatements[0]) <= startCalculations(operations.mStatements[1]);
        } else if (">=" == operations.mName) {
            return startCalculations(operations.mStatements[0]) >= startCalculations(operations.mStatements[1]);
        } else if("or" == operations.mName){
            startIf(operations);
        } else if("and" == operations.mName){
            startIf(operations);
        } else {
            throw runtime_error("Error in if operator");
        }
        return false;
    }
}