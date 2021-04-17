#include "Functions.hpp"

#include <stdexcept>
#include <algorithm>
#include <string>

namespace interpreter {
    using namespace std;
    using namespace parser;

    void Functions::declareVariableFunc(Statement &variable){
        switch(variable.mType.mType) {
            case DOUBLE:
                doubleVarTab[variable.mName] = startCalculations(variable.mParameters[0]);
                //cout<< variable.mName << " " << doubleVarTab[variable.mName] << '\n';
                break;
            case INT8:
                charVarTab[variable.mName] = startCalculations(variable.mParameters[0]);
                //cout<< variable.mName << " " << charVarTab[variable.mName] << '\n';
                break;
            case UINT8:
                ucharVarTab[variable.mName] = startCalculations(variable.mParameters[0]);
                //cout<< variable.mName << " " << ucharVarTab[variable.mName] << '\n';
                break;
            case INT32:
                intVarTab[variable.mName] = startCalculations(variable.mParameters[0]);
                //cout<< variable.mName << " " << intVarTab[variable.mName] << '\n';
                break;
            case UINT32:
                uintVarTab[variable.mName] = startCalculations(variable.mParameters[0]);
                //cout<< variable.mName << " " << uintVarTab[variable.mName] << '\n';
                break;
            default:
                return;
        }
    }

    double Functions::startCalculations(Statement &operations) {
        switch (operations.mName[0]) {
            case '+':
                return calculating(operations.mParameters[0]) + calculating(operations.mParameters[1]);
                break;
            case '-':
                return calculating(operations.mParameters[0]) - calculating(operations.mParameters[1]);
                break;
            case '*':
                return calculating(operations.mParameters[0]) * calculating(operations.mParameters[1]);
                break;
            case '/':
                return calculating(operations.mParameters[0]) / calculating(operations.mParameters[1]);
                break;
            default:
                std::string::size_type st;
                return stod(operations.mName, &st);
                break;
        }     
    }

    double Functions::calculating(Statement &operations){
        if(operations.mKind == StatementKind::LITTERAL){
            std::string::size_type st;
            return stod(operations.mName, &st);
        } else {
            return startCalculations(operations);
        }
    }

    void Functions::writeFunc(Statement &operations){
        for(auto i : operations.mParameters){
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

    void Functions::changeVarValue(Statement &cmd){
        string::size_type st;
        stod(cmd.mParameters[0].mName, &st);
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

    void Functions::readFunc(Statement &cmd){;
        if(doubleVarTab.find(cmd.mParameters[0].mName) != doubleVarTab.end()){
            cin >> doubleVarTab[cmd.mParameters[0].mName];
        } else if(intVarTab.find(cmd.mParameters[0].mName) != intVarTab.end()){
            cin >> intVarTab[cmd.mParameters[0].mName];
        } else if(uintVarTab.find(cmd.mParameters[0].mName) != uintVarTab.end()){
            cin >> uintVarTab[cmd.mParameters[0].mName];
        } else if(charVarTab.find(cmd.mParameters[0].mName) != charVarTab.end()){
            cin >> charVarTab[cmd.mParameters[0].mName];
        } else if(ucharVarTab.find(cmd.mParameters[0].mName) != ucharVarTab.end()){
            cin >> ucharVarTab[cmd.mParameters[0].mName];
        } else {
            throw runtime_error("Don't find varible!");
        }
    }
}