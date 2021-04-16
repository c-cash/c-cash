#include "Interpreter.hpp"
#include "Functions.hpp"

#include <iostream>
#include <stdexcept>

namespace interpreter{
    using namespace std;
    using namespace parser;

    void Interpreter::executeCommands(FunctionDefinition &commandsFunc){
        Functions func;
        for(auto cmd : commandsFunc.mStatements){
            switch (cmd.mKind) {
                case StatementKind::VARIABLE_DECLARATION:
                    func.declareVariableFunc(cmd);
                    break;
                case StatementKind::FUNCTION_CALL:
                    if(cmd.mName == "write"){
                        func.writeFunc(cmd);
                        break;
                    }
                    break;
                default:
                    break;
            }
        }
    }

    void Interpreter::interpreter(map<string, parser::FunctionDefinition> &mFunctions){
        if(mFunctions.find("main") == mFunctions.end()){
            throw runtime_error("Main function not found.");
        }
        executeCommands(mFunctions["main"]);
    }
}