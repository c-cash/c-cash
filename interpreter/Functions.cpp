#include "Functions.hpp"
#include "Types.hpp"

#include "varibles/variblesFunc.hpp"

#include <stdexcept>
#include <algorithm>
#include <string>

namespace interpreter {
    using namespace std;
    using namespace parser;

    void Functions::declareVariableFunc(Statement &variable){
        Variables var;
        VariblesFunc varFunc;
        
        double w=0;

        switch(variable.mType.mType) {
            case DOUBLE:
                var.doubleVarTab[variable.mName] = startCalculations(variable.mParameters[0]);
                cout<< variable.mName << " " << var.doubleVarTab[variable.mName] << '\n';
                break;
            case INT8:
                var.charVarTab[variable.mName] = startCalculations(variable.mParameters[0]);
                cout<< variable.mName << " " << var.charVarTab[variable.mName] << '\n';
                break;
            case UINT8:
                var.ucharVarTab[variable.mName] = startCalculations(variable.mParameters[0]);
                cout<< variable.mName << " " << var.ucharVarTab[variable.mName] << '\n';
                break;
            case INT32:
                var.intVarTab[variable.mName] = startCalculations(variable.mParameters[0]);
                cout<< variable.mName << " " << var.intVarTab[variable.mName] << '\n';
                break;
            case UINT32:
                var.uintVarTab[variable.mName] = startCalculations(variable.mParameters[0]);
                cout<< variable.mName << " " << var.uintVarTab[variable.mName] << '\n';
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
            cout << i.mName;
        }
    }
}