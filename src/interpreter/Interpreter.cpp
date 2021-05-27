#include "Interpreter.hpp"
#include "Functions.hpp"

#include "../variables/Object.hpp"
#include "../variables/Integer.hpp"
#include "../variables/String.hpp"
#include "../variables/Array.hpp"
#include "Namespace.hpp"

#include "../../transpiler/Transpiler.hpp"

#include <iostream>
#include <string>
#include <stdexcept>

#include "../libraries/MathLibrary.hpp"

namespace interpreter{
    using namespace std;
    using namespace parser;
    using namespace variable;

    map<string, builtinF> Interpreter::globalBuiltins;
    map<string, FunctionDefinition> Interpreter::definitions;
    Scope* Interpreter::includes = new Scope();

    void Interpreter::interpret(map<string, FunctionDefinition> &mFunctions) {
        definitions = mFunctions;
        // if program does not have a main function then throw an error
        if (definitions.find("main") == definitions.end()) throw runtime_error("Program must have a 'main' function");

        // include builtin libraries
        Functions::includeLibrary(definitions["*"], *includes);
        // execute main function
        vector<Object*> temp;
        evaluateFunction(definitions["main"], temp);
    }
    
    Object* Interpreter::evaluateFunction(FunctionDefinition &func, vector<Object*> &args) {
        Scope* fScope = new Scope(*includes); // create new scope for the function
        // create variables from args
        if (func.mParameters.size() != args.size())
            throw runtime_error("Function " + func.mName + " needs exactly " + to_string(func.mParameters.size()) + " parameters but got " + to_string(args.size()));
        for (int i=0; i<func.mParameters.size(); i++) {
            ParameterDefinition& d = func.mParameters[i]; // get definition
            fScope->varTab[d.mName] = args[i]; // assign parameter to the variable
        }

        // execute all statements
        for (Statement &s : func.mStatements) {
            Object* obj = evaluateStatement(s, *fScope); // evaluate statement with scope
            if (obj != nullptr && obj->getType() == "SpecialObject") {
                string t = obj->getValueString();
                if (t == "return") {
                    return ((SpecialObject*) obj)->var;
                }
            }
        }
        return nullptr;
    }
    
    Object* Interpreter::evaluateStatement(Statement &stmt, Scope &scope) {
        switch (stmt.mKind) {
            case StatementKind::VARIABLE_DECLARATION: {
                // declare variable and return null
                if (scope.varTab.find(stmt.mName) != scope.varTab.end()) throw runtime_error("variable '" + stmt.mName + "' already exists");
                transpiler::Transpiler::fixName(stmt); // fix signed integer instead of signed int
                Object* obj;
                if (stmt.mStatements.size() == 0) {obj = Object::getDefault(stmt.mType.mName);} // return default value for type
                else {obj = Object::checkAll(stmt.mType.mName, evaluateStatement(stmt.mStatements[0], scope));};
                scope.varTab[stmt.mName] = obj;
                return nullptr;
                break;
            }
            case StatementKind::LITERAL: {
                // get value from litteral :D
                return Object::fromLitteral(stmt);
                break;
            }
            case StatementKind::LOGIC_CALL: {
                return Functions::evaluateLogic(stmt, scope);
                break;
            }
            case StatementKind::OPERATOR_CALL: {
                return Functions::evaluateMath(stmt, scope);
                break;
            }
            case StatementKind::VARIABLE_CALL: {
                Object* special = Functions::specialVariable(stmt, scope);
                if (special != nullptr) return special;
                if (scope.varTab.find(stmt.mName) == scope.varTab.end()) throw runtime_error("cannot find variable '" + stmt.mName + "'");
                if (stmt.mStatements.size() >= 1) { // assign to existing variable
                    scope.varTab[stmt.mName]->assign(Object::checkAll(scope.varTab[stmt.mName]->getType(), evaluateStatement(stmt.mStatements[0], scope)));
                    return scope.varTab[stmt.mName];
                }
                return scope.varTab[stmt.mName];
                break;
            }
            case StatementKind::FUNCTION_CALL: {
                return Functions::evaluateFunctionCall(stmt, scope);
                break;
            }
            case StatementKind::ARRAY_DECLARATION: {
                return Functions::evaluateArrayDeclaration(stmt, scope);
                break;
            }
            case StatementKind::ARRAY: {
                return Functions::evaluateArrayCreation(stmt, scope);
                break;
            }
            case StatementKind::ARRAY_CALL: {
                return Array::check(*scope.varTab[stmt.mName]);
                break;
            }
            case StatementKind::ARRAY_ELEMENT: {
                return Functions::evaluateArrayElement(stmt, scope);
                break;
            }
            default:
                return nullptr;
                break;
        }
    }

    void Interpreter::addBuiltin(string name, builtinF f) {
        Interpreter::globalBuiltins[name] = f;
    }
    void Interpreter::addBuiltin(string ns, string name, builtinF f) {
        //TODO: implement namespaces
    }

    void Interpreter::addDefaultBuiltins() {
        // write function
        addBuiltin("write", [](vector<Object*> args)->vector<Object*>{
          
            for (int i=0; i<args.size(); ++i) cout << args[i]->toString();

            return {nullptr};
        });
        // read function
        addBuiltin("read", [](vector<Object*> args)->vector<Object*>{
            string r;
            cin >> r;
            return {new String(r)};
        });
        // exit function
        addBuiltin("exit", [](vector<Object*> args)->vector<Object*>{
            if (args.size() != 1 || args[0]->getType() != "Integer") throw runtime_error("invalid arguments for 'exit' function");
            exit(stoi(args[0]->getValueString()));
        });
    }
}