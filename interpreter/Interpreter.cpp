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

    bool Interpreter::executeCommands(FunctionDefinition &commandsFunc, vector<Statement> &args) {
        if(commandsFunc.mName != "IF" && commandsFunc.mName != "ELIF" && commandsFunc.mName != "ELSE") func = Functions();
        size_t i=0;

        //cout <<  commandsFunc.mName << " " << commandsFunc.mParameters.size() << " " << args.size() << endl;

        if(commandsFunc.mParameters.size() == args.size()) {
            for(auto var : commandsFunc.mParameters) {
                func.declareParameter(var, args[i], func);
                ++i;
            }
            for(auto cmd : commandsFunc.mStatements){
                //cout << cmd.mName << endl;
                bool ex = executeCommand(func, cmd);
                //cout << ex << " ";
                if(ex == false){cout << "return "; return false;}
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
                        executeCommands(ifDef, args);
                        ifIf = false;
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
                            executeCommands(ifDef, args);
                            ifIf = false;
                        }
                    }
                    break;
                } else if(cmd.mName == "ELSE") {
                    if(ifIf){
                        FunctionDefinition ifDef;
                        ifDef.mName = cmd.mName;
                        ifDef.mStatements = cmd.mStatements;
                        vector<Statement> args;
                        executeCommands(ifDef, args);
                        ifIf = false;
                    }
                    break;
                } else if(cmd.mName == "return") {
                    cout << "w return\n";
                    func.returnFunc(cmd, func);
                    return false;
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
        executeCommands(mFunctions["main"], args);
    }
}