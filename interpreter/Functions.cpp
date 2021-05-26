#include "Functions.hpp"
#include "Interpreter.hpp"
#include "../variables/Object.hpp"

#include <stdexcept>
#include <algorithm>
#include <string>

#include "Namespace.hpp"

#include "../variables/Double.hpp"
#include "../variables/Integer.hpp"
#include "../variables/Boolean.hpp"
#include "../variables/SpecialObject.hpp"

#include "../libraries/LibraryRegistry.hpp"

namespace interpreter {
    using namespace std;
    using namespace parser;
    using namespace variable;

    Object* Functions::evaluateMath(Statement &stmt, Scope &scope) {
        switch (stmt.mName[0]) {
            case '+':
                // evaluate and add next two statements
                return Interpreter::evaluateStatement(stmt.mStatements[0], scope)->add(Interpreter::evaluateStatement(stmt.mStatements[1], scope));
                break;
            case '-':
                // evaluate and subtract next two statements
                return Interpreter::evaluateStatement(stmt.mStatements[0], scope)->subtract(Interpreter::evaluateStatement(stmt.mStatements[1], scope));
                break;
            case '*':
                // evaluate and multiply next two statements
                return Interpreter::evaluateStatement(stmt.mStatements[0], scope)->multiply(Interpreter::evaluateStatement(stmt.mStatements[1], scope));
                break;
            case '/':
                // evaluate and divide next two statements
                return Interpreter::evaluateStatement(stmt.mStatements[0], scope)->divide(Interpreter::evaluateStatement(stmt.mStatements[1], scope));
                break;
            case '%':
                // evaluate and use modulo on next two statements
                return Interpreter::evaluateStatement(stmt.mStatements[0], scope)->modulo(Interpreter::evaluateStatement(stmt.mStatements[1], scope));
                break;
            default:
                return nullptr;
        }
    }

    Object* Functions::evaluateLogic(Statement &stmt, Scope &scope) {
        if (stmt.mName == "==")
            return new Boolean(Interpreter::evaluateStatement(stmt.mStatements[0], scope)->equal(Interpreter::evaluateStatement(stmt.mStatements[1], scope)));
        else if (stmt.mName == "<") 
            return new Boolean(Interpreter::evaluateStatement(stmt.mStatements[0], scope)->less(Interpreter::evaluateStatement(stmt.mStatements[1], scope)));
        else if (stmt.mName == "<=")
            return new Boolean(Interpreter::evaluateStatement(stmt.mStatements[0], scope)->lesseq(Interpreter::evaluateStatement(stmt.mStatements[1], scope)));
        else if (stmt.mName == ">")
            return new Boolean(Interpreter::evaluateStatement(stmt.mStatements[0], scope)->greater(Interpreter::evaluateStatement(stmt.mStatements[1], scope)));
        else if (stmt.mName == ">=")
            return new Boolean(Interpreter::evaluateStatement(stmt.mStatements[0], scope)->greatereq(Interpreter::evaluateStatement(stmt.mStatements[1], scope)));
        else if (stmt.mName == "!=")
            return new Boolean(Interpreter::evaluateStatement(stmt.mStatements[0], scope)->noteq(Interpreter::evaluateStatement(stmt.mStatements[1], scope)));
        
        else if (stmt.mName == "and")
            return new Boolean((stoi(Interpreter::evaluateStatement(stmt.mStatements[0], scope)->getValueString()) == 1) &&  (stoi(Interpreter::evaluateStatement(stmt.mStatements[1], scope)->getValueString()) == 1));
        else if (stmt.mName == "or")
            return new Boolean((stoi(Interpreter::evaluateStatement(stmt.mStatements[0], scope)->getValueString()) == 1) ||  (stoi(Interpreter::evaluateStatement(stmt.mStatements[1], scope)->getValueString()) == 1));
        
        return nullptr;
    }

    Object* Functions::specialVariable(Statement &stmt, Scope &scope) {
        if (stmt.mName == "break") {
            return new SpecialObject(SpecialType::BREAK, nullptr);
        } else if (stmt.mName == "continue") {
            return new SpecialObject(SpecialType::CONTINUE, nullptr);
        } 
        else if (stmt.mName == "true") return new variable::Boolean(true);
        else if (stmt.mName == "false") return new variable::Boolean(false);
        return nullptr;
    }

    Object* Functions::evaluateFunctionCall(Statement &stmt, Scope &scope) {

        // RETURN
        if (stmt.mName == "return") {
            return new SpecialObject(SpecialType::RETURN, Interpreter::evaluateStatement(stmt.mStatements[0], scope));
        }

        // LOOP
        if (stmt.mName == "LOOP") {
            return evaluateLoop(stmt, scope);
        }

        // IF / ELSE / ELIF
        if (stmt.mName == "IF") {
            scope.isPreviousIf = true;
            // check if correct
            if (stmt.mStatements[0].mKind != StatementKind::LOGIC_CALL) throw runtime_error("'if' must have an expression");
            // check logic
            bool e = (bool) stoi(evaluateLogic(stmt.mStatements[0], scope)->getValueString());
            scope.previousResult = e;
            if (!e) return nullptr;
            // create scope for variables
            Scope* s = new Scope(scope);
            // execute everything inside :D
            for (int i=1; i<stmt.mStatements.size(); i++){
                Object* obj = Interpreter::evaluateStatement(stmt.mStatements[i], *s);
                if (obj != nullptr && obj->getType() == "SpecialObject") {
                    string t=obj->getValueString(); 
                    if (t == "break" || t == "return" || t == "continue") {return Functions::useSpecial((SpecialObject*) obj, "if");}
                }
            }
            return nullptr;
        } else if (stmt.mName == "ELSE") {
            if (!scope.isPreviousIf || scope.previousResult) return nullptr;
            scope.isPreviousIf = false;
            // create scope for variables
            Scope* s = new Scope(scope);
            // execute everything inside :D
            for (int i=0; i<stmt.mStatements.size(); i++){
                Object* obj = Interpreter::evaluateStatement(stmt.mStatements[i], *s);
                if (obj != nullptr && obj->getType() == "SpecialObject") {
                    string t=obj->getValueString(); 
                    if (t == "break" || t == "return" || t == "continue") {return Functions::useSpecial((SpecialObject*) obj, "else");}
                }
            }
            return nullptr;
        } else if (stmt.mName == "ELIF") {
            if (!scope.isPreviousIf || scope.previousResult) return nullptr;
            scope.isPreviousIf = true;
            // check if correct
            if (stmt.mStatements[0].mKind != StatementKind::LOGIC_CALL) throw runtime_error("'elif' must have an expression");
            // check logic
            bool e = (bool) stoi(evaluateLogic(stmt.mStatements[0], scope)->getValueString());
            scope.previousResult = e;
            if (!e) return nullptr;
            // create scope for variables
            Scope* s = new Scope(scope);
            // execute everything inside :D
            for (int i=1; i<stmt.mStatements.size(); i++){
                Object* obj = Interpreter::evaluateStatement(stmt.mStatements[i], *s);
                if (obj != nullptr && obj->getType() == "SpecialObject") {
                    string t=obj->getValueString(); 
                    if (t == "break" || t == "return" || t == "continue") {return Functions::useSpecial((SpecialObject*) obj, "elif");}
                }
            }
            return nullptr;
        }
        scope.isPreviousIf = false;

        // generate function arguments
        vector<Object*> args;
        for (Statement &s : stmt.mStatements) {
            args.emplace_back(Interpreter::evaluateStatement(s, scope));
        }

        // check if function is in builtins
        if (Interpreter::globalBuiltins.find(stmt.mName) != Interpreter::globalBuiltins.end()) {
            // if it is, then run it :D
            return Interpreter::globalBuiltins[stmt.mName](args)[0];
        }

        // library functions
        if (Interpreter::includes->functions.find(stmt.mName) != Interpreter::includes->functions.end()) {
            // this is a global function from library :D
            return Interpreter::includes->functions[stmt.mName](args)[0];
        }
        // TODO: implement namespaced builtins

        // check if function exists and if not then throw error
        if (Interpreter::definitions.find(stmt.mName) == Interpreter::definitions.end()) throw runtime_error("Cannot find function '" + stmt.mName + "'");

        // run function
        return Interpreter::evaluateFunction(Interpreter::definitions[stmt.mName], args);
    }

    Object* Functions::useSpecial(SpecialObject* s, string place) {
        if ((s->getValueString() == "break" || s->getValueString() == "continue") && place == "loop") {s->assign(SpecialType::NONE); return s;}
        return s;
    }

    Object* Functions::evaluateLoop(Statement &stmt, Scope &scope) {
        if (stmt.mStatements[0].mKind != StatementKind::FUNCTION_CALL) throw runtime_error("Loop must have an expression");
        Statement &params = stmt.mStatements[0];
        // LOOP WITH INTEGER
        if (params.mStatements.size() == 1 && params.mStatements[0].mKind != StatementKind::LOGIC_CALL) {
            Scope* t = new Scope(scope);
            // execute loop
            int count = stoi(Interpreter::evaluateStatement(params.mStatements[0], *t)->getValueString());
            for (int i=0; i<count; i++) {
                Scope* s = new Scope(*t);
                Object* obj;
                for (int j=1; j<stmt.mStatements.size(); j++){ // execute each statement
                    obj = Interpreter::evaluateStatement(stmt.mStatements[j], *s);
                    if (obj == nullptr) continue;
                    if (obj->getType() == "SpecialObject") {
                        string t=obj->getValueString();if (t == "break" || t == "return" || t == "continue") break;
                    }
                }
                if (obj != nullptr){
                    if (obj->getValueString() == "break") {Functions::useSpecial((SpecialObject*) obj, "loop"); break;}
                    else if (obj->getValueString() == "return") {return Functions::useSpecial((SpecialObject*) obj, "loop");}
                    else if (obj->getValueString() == "continue") {Functions::useSpecial((SpecialObject*) obj, "loop");}
                }
            }
        }
        // LOOP WITH LOGIC STATEMENT
        else if (params.mStatements.size() == 1 && params.mStatements[0].mKind == StatementKind::LOGIC_CALL) {
            Scope* t = new Scope(scope);
            // check logic statement
            bool e = (bool) stoi(evaluateLogic(params.mStatements[0], *t)->getValueString());
            while (e) {
                Scope* s = new Scope(*t);

                Object* obj;
                for (int j=1; j<stmt.mStatements.size(); j++){ // execute each statement
                    obj = Interpreter::evaluateStatement(stmt.mStatements[j], *s);
                    if (obj == nullptr) continue;
                    if (obj->getType() == "SpecialObject") {
                        string t=obj->getValueString();if (t == "break" || t == "return" || t == "continue") break;
                    }
                }
                if (obj != nullptr){
                    if (obj->getValueString() == "break") {Functions::useSpecial((SpecialObject*) obj, "loop"); break;}
                        else if (obj->getValueString() == "return") {return Functions::useSpecial((SpecialObject*) obj, "loop");}
                        else if (obj->getValueString() == "continue") {Functions::useSpecial((SpecialObject*) obj, "loop");}
                }

                e = (bool) stoi(evaluateLogic(params.mStatements[0], *t)->getValueString());
            }
        }
        // NORMAL FOR LOOP
        else if (params.mStatements.size() == 3 && params.mStatements[1].mKind == StatementKind::LOGIC_CALL) {
            Scope* t = new Scope(scope);
            // execute before statement
            Interpreter::evaluateStatement(params.mStatements[0], *t);
            // check logic statement
            bool e = (bool) stoi(evaluateLogic(params.mStatements[1], *t)->getValueString());
            while (e) {
                Scope* s = new Scope(*t);
                Object* obj;
                for (int j=1; j<stmt.mStatements.size(); j++){ // execute each statement
                    obj = Interpreter::evaluateStatement(stmt.mStatements[j], *s);
                    if (obj == nullptr) continue;
                    if (obj->getType() == "SpecialObject") {
                        string t=obj->getValueString();
                        if (t == "break" || t == "return" || t == "continue") break;
                    }
                }
                if (obj != nullptr){
                    if (obj->getValueString() == "break") {Functions::useSpecial((SpecialObject*) obj, "loop"); break;}
                        else if (obj->getValueString() == "return") {return Functions::useSpecial((SpecialObject*) obj, "loop");}
                        else if (obj->getValueString() == "continue") {Functions::useSpecial((SpecialObject*) obj, "loop");}
                }
                // execute after and check again
                Interpreter::evaluateStatement(params.mStatements[2], *t);
                e = (bool) stoi(evaluateLogic(params.mStatements[1], *t)->getValueString());
            }
        }
        return nullptr;
    }

    void Functions::includeLibrary(FunctionDefinition &func, Scope &scope) {
        for (Statement &s : func.mStatements) {
            // check if it is include
            if (s.mKind != StatementKind::FUNCTION_CALL || (s.mName != "include" && s.mName != "nsinclude"))
                throw runtime_error("you can only use 'include' outside of scope");
            
            if (s.mName == "include") {
                if (s.mStatements.size() != 1 || s.mStatements[0].mKind != StatementKind::LITERAL || s.mStatements[0].mType.mName != "string")
                    throw runtime_error("invalid arguments for include");
                    // include globally
                    string name = s.mStatements[0].mName;
                    if (library::libraries.find(name) == library::libraries.end()) throw runtime_error("cannot find library '" + name + "'");
                    library::libraries[name]()->linkGlobally(*Interpreter::includes);
            } else {
                if ((s.mStatements.size() != 1 && s.mStatements.size() != 2) ||
                    s.mStatements[0].mKind != StatementKind::LITERAL || 
                    (s.mStatements.size() == 2 && s.mStatements[1].mKind != StatementKind::LITERAL) ||
                    s.mStatements[0].mType.mName != "string" ||
                    (s.mStatements.size() == 2 && s.mStatements[1].mType.mName != "string")) {
                        // include namespaced
                    }
            }
        }
    }
}