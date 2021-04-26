#include "Interpreter.hpp"

#include <iostream>
#include <string>
#include <stdexcept>

namespace interpreter{
    using namespace std;
    using namespace parser;

    bool ifIf;
    map<string, parser::FunctionDefinition> mFunctions;

    bool Interpreter::executeCommands(FunctionDefinition &commandsFunc, vector<Statement> &args, Functions &func) {
        size_t i=0;
        if(commandsFunc.mName != "IF" && commandsFunc.mName != "ELIF" && commandsFunc.mName != "ELSE") {
            Functions secFunc = Functions();
            if(commandsFunc.mParameters.size() == args.size()) {
                for(auto var : commandsFunc.mParameters) {
                    secFunc.declareParameter(var, args[i], secFunc);
                    ++i;
                }
                for(auto cmd : commandsFunc.mStatements){
                    bool ex = executeCommand(secFunc, cmd);
                    if(ex == false){ return false;}
                }
            } else {
                throw runtime_error(": Invalid number of arguments was supplied!");
            }

            return true;
        }

        if(commandsFunc.mParameters.size() == args.size()) {
            for(auto var : commandsFunc.mParameters) {
                func.declareParameter(var, args[i], func);
                ++i;
            }
            for(auto cmd : commandsFunc.mStatements){
                bool ex = executeCommand(func, cmd);
                if(ex == false){ return false;}
            }
        } else {
            throw runtime_error(": Invalid number of arguments was supplied!");
        }

        return true;
    }

    bool Interpreter::executeCommand(Functions &func, Statement &cmd) {
        switch (cmd.mKind) {
            case StatementKind::VARIABLE_DECLARATION:
                func.declareVariableFunc(cmd, func);
                break;
            case StatementKind::FUNCTION_CALL:
                if(cmd.mName == "write"){
                    func.writeFunc(cmd, func);
                    break;
                } else if(cmd.mName == "read"){
                    func.readFunc(cmd, func);
                    break;
                } else if(cmd.mName == "exit"){
                    exit(0);
                    break;
                } else if(cmd.mName == "IF") {
                    if(func.startIf(cmd.mStatements[0], func)) {
                        cmd.mStatements.erase(cmd.mStatements.begin());
                        FunctionDefinition ifDef;
                        ifDef.mName = cmd.mName;
                        ifDef.mStatements = cmd.mStatements;
                        vector<Statement> args;
                        bool checkReturn = executeCommands(ifDef, args,func);
                        ifIf = false;
                        if(checkReturn == false) return  false;
                    } else {
                        ifIf = true;
                    }
                    break;
                } else if(cmd.mName == "ELIF") {
                    if(ifIf){
                        if(func.startIf(cmd.mStatements[0], func)) {
                            cmd.mStatements.erase(cmd.mStatements.begin());
                            FunctionDefinition ifDef;
                            ifDef.mName = cmd.mName;
                            ifDef.mStatements = cmd.mStatements;
                            vector<Statement> args;
                            bool checkReturn = executeCommands(ifDef, args, func);
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
                        bool checkReturn = executeCommands(ifDef, args, func);
                        ifIf = false;
                        if(checkReturn == false) return  false;
                    }
                    break;
                } else if(cmd.mName == "return") {
                    func.returnFunc(cmd, func);
                    return false;
                    break;
                } else {
                    if(mFunctions.find(cmd.mName) != mFunctions.end()){
                        bool checkReturn = executeCommands(mFunctions[cmd.mName], cmd.mStatements, func);
                        if(checkReturn == false) return  false;
                    } else {
                        throw runtime_error("Don't find function");
                    }
                    break;
                }
                break;
            case StatementKind::VARIABLE_CALL:
                func.changeVarValue(cmd, func);
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
        Functions func;
        executeCommands(mFunctions["main"], args, func);
    }
}