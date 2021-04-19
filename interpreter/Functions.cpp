#include "Functions.hpp"

#include <stdexcept>
#include <algorithm>
#include <string>

namespace interpreter {
    using namespace std;
    using namespace parser;

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
                return calculating(operations.mStatements[0]) / calculating(operations.mStatements[1]);
                break;
            default:
                std::string::size_type st;
                return stod(operations.mName, &st);
                break;
        }     
    }

    double Functions::calculating(Statement &operations) {
        if(operations.mKind == StatementKind::LITTERAL){
            std::string::size_type st;
            return stod(operations.mName, &st);
        } else {
            return startCalculations(operations);
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
            } else {
                cout << i.mName;
            }
        }
    }

    void Functions::changeVarValue(Statement &cmd) {
        string::size_type st;
        stod(cmd.mStatements[0].mName, &st);
        if(doubleVarTab.find(cmd.mName) != doubleVarTab.end()){
            doubleVarTab[cmd.mName] = st;
        } else if(intVarTab.find(cmd.mName) != intVarTab.end()){
            intVarTab[cmd.mName] = st;
        } else if(uintVarTab.find(cmd.mName) != uintVarTab.end()){
            uintVarTab[cmd.mName] = st;
        } else if(charVarTab.find(cmd.mName) != charVarTab.end()){
            charVarTab[cmd.mName] = st;
        } else if(ucharVarTab.find(cmd.mName) != ucharVarTab.end()){
            ucharVarTab[cmd.mName] = st;
        } else {
            throw runtime_error("Don't find varible!");
        }
    }

    void Functions::readFunc(Statement &cmd) {
        for(auto i=0; i<cmd.mStatements.size(); ++i){
            if(doubleVarTab.find(cmd.mStatements[i].mName) != doubleVarTab.end()){
                cin >> doubleVarTab[cmd.mStatements[i].mName];
            } else if(intVarTab.find(cmd.mStatements[i].mName) != intVarTab.end()){
                cin >> intVarTab[cmd.mStatements[i].mName];
            } else if(uintVarTab.find(cmd.mStatements[i].mName) != uintVarTab.end()){
                cin >> uintVarTab[cmd.mStatements[i].mName];
            } else if(charVarTab.find(cmd.mStatements[i].mName) != charVarTab.end()){
                cin >> charVarTab[cmd.mStatements[i].mName];
            } else if(ucharVarTab.find(cmd.mStatements[i].mName) != ucharVarTab.end()){
                cin >> ucharVarTab[cmd.mStatements[i].mName];
            } else {
                throw runtime_error("Don't find varible!");
            }
        }
    }

    bool Functions::startIf(Statement &cmd) {
        if(">" == cmd.mName){
            return calculateIf(cmd.mStatements[0]) > calculateIf(cmd.mStatements[1]);
        } else if("<" == cmd.mName) {
            return calculateIf(cmd.mStatements[0]) < calculateIf(cmd.mStatements[1]);
        } else if("==" == cmd.mName) {
            return calculateIf(cmd.mStatements[0]) == calculateIf(cmd.mStatements[1]);
        } else if ("!=" == cmd.mName) {
            return calculateIf(cmd.mStatements[0]) != calculateIf(cmd.mStatements[1]);
        } else if("<=" == cmd.mName) {
            return calculateIf(cmd.mStatements[0]) <= calculateIf(cmd.mStatements[1]);
        } else if (">=" == cmd.mName) {
            return calculateIf(cmd.mStatements[0]) >= calculateIf(cmd.mStatements[1]);
        } /* HERE WE HAVE A BUG
        else if ("and" == cmd.mName) {
            return calculateIf(cmd.mStatements[0]) && calculateIf(cmd.mStatements[1]);
        } else if("or" == cmd.mName) {
            return calculateIf(cmd.mStatements[0]) || calculateIf(cmd.mStatements[1]);
        } */else {
            return false;
        }
    }

    bool Functions::calculateIf(Statement &cmd) {
        //To write
        return true;
    }
}