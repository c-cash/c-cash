#include "Interpreter.hpp"

#include <iostream>
#include <string>
#include <stdexcept>

namespace interpreter{
    using namespace std;
    using namespace parser;

    bool ifIf;
    map<string, parser::FunctionDefinition> mFunctions;
    Functions func;

    bool Interpreter::executeFunction(FunctionDefinition &commandsFunc, vector<Statement> &args, Scope &scope) {
        size_t i=0;
        if(commandsFunc.mName != "IF" && commandsFunc.mName != "ELIF" && commandsFunc.mName != "ELSE" && commandsFunc.mName != "LOOP") {
            Scope secondScope = Scope();
            if(commandsFunc.mParameters.size() == args.size()) {
                for(auto var : commandsFunc.mParameters) {
                    func.declareParameter(var, args[i], secondScope);
                    ++i;
                }
                for(auto cmd : commandsFunc.mStatements){
                    bool ex = executeCommand(secondScope, cmd);
                    if(ex == false){ return false;}
                }
            } else {
                throw runtime_error(": Invalid number of arguments was supplied!");
            }

            return true;
        }

        if(commandsFunc.mParameters.size() == args.size()) {
            for(auto var : commandsFunc.mParameters) {
                func.declareParameter(var, args[i], scope);
                ++i;
            }
            for(auto cmd : commandsFunc.mStatements){
                bool ex = executeCommand(scope, cmd);
                if(ex == false){ return false;}
            }
        } else {
            throw runtime_error(": Invalid number of arguments was supplied!");
        }

        return true;
    }

    bool Interpreter::executeCommand(Scope &scope, Statement &cmd) {
        switch (cmd.mKind) {
            case StatementKind::VARIABLE_DECLARATION:
                func.declareVariableFunc(cmd, scope);
                break;
            case StatementKind::FUNCTION_CALL:
                if(cmd.mName == "write"){
                    func.writeFunc(cmd, scope);
                    break;
                } else if(cmd.mName == "read"){
                    func.readFunc(cmd, scope);
                    break;
                } else if(cmd.mName == "exit"){
                    exit(0);
                    break;
                } else if(cmd.mName == "IF") {
                    if(func.startIf(cmd.mStatements[0], scope)) {
                        cmd.mStatements.erase(cmd.mStatements.begin());
                        FunctionDefinition ifDef;
                        ifDef.mName = cmd.mName;
                        ifDef.mStatements = cmd.mStatements;
                        vector<Statement> args;
                        bool checkReturn = executeFunction(ifDef, args, scope);
                        ifIf = false;
                        if(checkReturn == false) return  false;
                    } else {
                        ifIf = true;
                    }
                    break;
                } else if(cmd.mName == "ELIF") {
                    if(ifIf){
                        if(func.startIf(cmd.mStatements[0], scope)) {
                            cmd.mStatements.erase(cmd.mStatements.begin());
                            FunctionDefinition ifDef;
                            ifDef.mName = cmd.mName;
                            ifDef.mStatements = cmd.mStatements;
                            vector<Statement> args;
                            bool checkReturn = executeFunction(ifDef, args, scope);
                            ifIf = false;
                            if(checkReturn == false) return  false;
                        }
                    }
                    break;
                } else if(cmd.mName == "ELSE") {
                    if(ifIf){
                        FunctionDefinition ifDef;
                        ifDef.mName = cmd.mName;
                        ifDef.mStatements = cmd.mStatements;
                        vector<Statement> args;
                        bool checkReturn = executeFunction(ifDef, args, scope);
                        ifIf = false;
                        if(checkReturn == false) return  false;
                    }
                    break;
                } else if(cmd.mName == "return") {
                    func.returnFunc(cmd, scope);
                    return false;
                    break;
                } else if(cmd.mName == "LOOP") {

                    if(cmd.mStatements[0].mStatements.size() == 3) {
                        if(cmd.mStatements[0].mStatements[0].mKind == StatementKind::VARIABLE_DECLARATION){
                            func.declareVariableFunc(cmd.mStatements[0].mStatements[0], scope);
                        }
                        if(cmd.mStatements[0].mStatements[1].mKind == StatementKind::LOGIC_CALL){
                            Statement condition = cmd.mStatements[0].mStatements[1];
                            Statement varCall = cmd.mStatements[0].mStatements[2];
                            cmd.mStatements.erase(cmd.mStatements.begin());

                            FunctionDefinition loopDef;
                            loopDef.mName = cmd.mName;
                            loopDef.mStatements = cmd.mStatements;
                            vector<Statement> args;
                            while(func.startIf(condition, scope)) {
                                bool chceckReturn = executeFunction(loopDef, args, scope);
                                func.changeVarValue(varCall, scope);
                            }
                        }
                    } else if(cmd.mStatements[0].mStatements[0].mKind == StatementKind::LOGIC_CALL) {

                        Statement condition = cmd.mStatements[0].mStatements[0];
                        cmd.mStatements.erase(cmd.mStatements.begin());

                        FunctionDefinition loopDef;
                        loopDef.mName = cmd.mName;
                        loopDef.mStatements = cmd.mStatements;

                        vector<Statement> args;

                        while(func.startIf(condition, scope)) {
                            bool chceckReturn = executeFunction(loopDef, args, scope);
                        }
                    } else if(cmd.mStatements[0].mStatements[0].mKind == StatementKind::LITERAL || cmd.mStatements[0].mStatements[0].mKind == StatementKind::OPERATOR_CALL || cmd.mStatements[0].mStatements[0].mKind == StatementKind::VARIABLE_CALL || cmd.mStatements[0].mStatements[0].mKind == StatementKind::FUNCTION_CALL) {
                        long long forCounting = func.startCalculations(cmd.mStatements[0].mStatements[0], scope);
                        cmd.mStatements.erase(cmd.mStatements.begin());

                        FunctionDefinition loopDef;
                        loopDef.mName = cmd.mName;
                        loopDef.mStatements = cmd.mStatements;
                        vector<Statement> args;
                        while(forCounting > 0) {
                            bool chceckReturn = executeFunction(loopDef, args, scope);                
                            --forCounting;
                        }                        
                    } else {
                        throw runtime_error("Wrong arguments in loop statement.");
                    }
                } else {
                    if(mFunctions.find(cmd.mName) != mFunctions.end()){
                        bool checkReturn = executeFunction(mFunctions[cmd.mName], cmd.mStatements, scope);
                        if(checkReturn == false) return  false;
                    } else {
                        throw runtime_error("Don't find function");
                    }
                    break;
                }
                break;
            case StatementKind::VARIABLE_CALL:
                func.changeVarValue(cmd, scope);
                break;
            default:
                throw runtime_error("Don't find function!");
        }
        //cout << "po switch\n";
        return true;
    }

    void Interpreter::interpreter(map<string, parser::FunctionDefinition> &mFunctionss){
        mFunctions = mFunctionss;
        if(mFunctions.find("main") == mFunctions.end()){
            throw runtime_error("Main function not found.");
        }
        vector<Statement> args;
        Scope scope;
        executeFunction(mFunctions["main"], args, scope);
    }
}