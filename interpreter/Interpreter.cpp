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

    bool Interpreter::executeCommands(FunctionDefinition &commandsFunc, vector<Statement> &args, Scope &scope) {
        size_t i=0;
        if(commandsFunc.mName != "IF" && commandsFunc.mName != "ELIF" && commandsFunc.mName != "ELSE") {
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
                        bool checkReturn = executeCommands(ifDef, args, scope);
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
                            bool checkReturn = executeCommands(ifDef, args, scope);
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
                        bool checkReturn = executeCommands(ifDef, args, scope);
                        ifIf = false;
                        if(checkReturn == false) return  false;
                    }
                    break;
                } else if(cmd.mName == "return") {
                    func.returnFunc(cmd, scope);
                    return false;
                    break;
                } else {
                    if(mFunctions.find(cmd.mName) != mFunctions.end()){
                        bool checkReturn = executeCommands(mFunctions[cmd.mName], cmd.mStatements, scope);
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
        executeCommands(mFunctions["main"], args, scope);
    }
}