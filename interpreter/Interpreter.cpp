#include "Interpreter.hpp"

#include <iostream>
#include <string>
#include <stdexcept>

namespace interpreter{
    using namespace std;
    using namespace parser;

    void Interpreter::executeCommands(FunctionDefinition &commandsFunc, map<string, parser::FunctionDefinition> &mFunctions){
        Functions func;
        for(auto cmd : commandsFunc.mStatements){
            executeCommand(func, cmd, mFunctions);
        }
    }

    void Interpreter::executeCommand(Functions &func, Statement cmd, map<string, parser::FunctionDefinition> &mFunctions) {
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
                } else if(cmd.mName == "IF") {
                    if(func.startIf(cmd.mStatements[0])) {
                        for(int i=1; i<cmd.mStatements.size(); ++i) {
                            auto c = cmd.mStatements[i];
                            executeCommand(func, c, mFunctions);
                        }
                    }
                    break;
                } else {
                    if(mFunctions.find(cmd.mName) != mFunctions.end()){
                        executeCommands(mFunctions[cmd.mName], mFunctions);
                    } else {
                        throw runtime_error("Don't find function");
                    }
                }
                break;
            case StatementKind::VARIABLE_CALL:
                func.changeVarValue(cmd);
                break;
            default:
                throw runtime_error("Don't find function!");
        }
    }

    void Interpreter::interpreter(map<string, parser::FunctionDefinition> &mFunctions){
        if(mFunctions.find("main") == mFunctions.end()){
            throw runtime_error("Main function not found.");
        }
        executeCommands(mFunctions["main"], mFunctions);
    }
}