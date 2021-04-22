#include "Interpreter.hpp"

#include <iostream>
#include <string>
#include <stdexcept>

namespace interpreter{
    using namespace std;
    using namespace parser;

    bool ifIf;
    map<string, parser::FunctionDefinition> mFunctions;

    void Interpreter::executeCommands(FunctionDefinition &commandsFunc, vector<Statement> args) {
        Functions func;
        size_t i=0;
        if(commandsFunc.mParameters.size() == args.size()) {
            for(auto var : commandsFunc.mParameters) {
                func.declareParameter(var, args[i]);
                ++i;
            }
            for(auto cmd : commandsFunc.mStatements){
                executeCommand(func, cmd);
            }
        } else {
            throw runtime_error(": Invalid number of arguments was supplied!");
        }
    }

    void Interpreter::executeCommand(Functions &func, Statement cmd) {
        switch (cmd.mKind) {
            case StatementKind::VARIABLE_DECLARATION:
                func.declareVariableFunc(cmd);
                break;
            case StatementKind::FUNCTION_CALL:
                if(cmd.mName == "write"){
                    func.writeFunc(cmd);
                    break;
                } else if(cmd.mName == "read"){
                    func.readFunc(cmd);
                    break;
                } else if(cmd.mName == "exit"){
                    exit(0);
                    break;
                }else if(cmd.mName == "IF") {
                    if(func.startIf(cmd.mStatements[0])) {
                        for(int i=1; i<cmd.mStatements.size(); ++i) {
                            auto c = cmd.mStatements[i];
                            executeCommand(func, c);
                        }
                        ifIf = false;
                    } else {
                        ifIf = true;
                    }
                    break;
                } else if(cmd.mName == "ELIF") {
                    if(ifIf){
                        if(func.startIf(cmd.mStatements[0])) {
                            for(int i=1; i<cmd.mStatements.size(); ++i) {
                                auto c = cmd.mStatements[i];
                                executeCommand(func, c);
                            }
                            ifIf = false;
                        }
                    }
                    break;
                } else if(cmd.mName == "ELSE") {
                    if(ifIf){
                        for(int i=0; i<cmd.mStatements.size(); ++i) {
                            auto c = cmd.mStatements[i];
                            executeCommand(func, c);
                        }
                        ifIf = false;
                    }
                    break;
                } else if(cmd.mName == "return") {
                    func.returnFunc(cmd);
                    break;
                } else {
                    if(mFunctions.find(cmd.mName) != mFunctions.end()){
                        executeCommands(mFunctions[cmd.mName], cmd.mStatements);
                    } else {
                        throw runtime_error("Don't find function");
                    }
                    break;
                }
                break;
            case StatementKind::VARIABLE_CALL:
                func.changeVarValue(cmd);
                break;
            default:
                throw runtime_error("Don't find function!");
        }
    }

    void Interpreter::interpreter(map<string, parser::FunctionDefinition> &mFunctionss){
        mFunctions = mFunctionss;
        if(mFunctions.find("main") == mFunctions.end()){
            throw runtime_error("Main function not found.");
        }
        vector<Statement> args;
        executeCommands(mFunctions["main"], args);
    }
}