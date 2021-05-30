#include "Functions.hpp"
#include "Interpreter.hpp"
#include "../variables/Object.hpp"

#include <stdexcept>
#include <algorithm>
#include <string>

#include "../variables/Double.hpp"
#include "../variables/Integer.hpp"
#include "../variables/Boolean.hpp"
#include "../variables/SpecialObject.hpp"
#include "../variables/Array.hpp"

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
            return Interpreter::globalBuiltins[stmt.mName](args);
        }

        // library functions
        if (scope.functions.find(stmt.mName) != scope.functions.end()) {
            // this is a global function from library :D
            return scope.functions[stmt.mName](args);
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
            } else if (s.mName == "nsinclude") {
                if (s.mStatements[0].mKind != StatementKind::LITERAL) throw runtime_error("you need to provide string into include function");
                string nsname;
                string libname;
                if (s.mStatements.size() == 1) { // library name
                    nsname = s.mStatements[0].mName;
                    libname = s.mStatements[0].mName;
                } else if (s.mStatements.size() == 2) { // custom name
                    if (s.mStatements[1].mKind != StatementKind::LITERAL) throw runtime_error("you need to provide string into include function");
                    nsname = s.mStatements[0].mName;
                    libname = s.mStatements[1].mName;
                } else {throw runtime_error("invalid argument count for 'include' function");}
                // include this library into namespace
                if(library::libraries.find(libname) == library::libraries.end()) throw runtime_error("cannot find library '" + libname + "'");
                Scope* ns = new Scope(*Interpreter::includes);
                library::libraries[libname]()->linkNamespaced(nsname, *ns);
                scope.namespaces[nsname] = ns;
            }
        }
    }

    Object* Functions::evaluateArrayDeclaration(Statement &stmt, Scope &scope) {
        // declare array and return null
        if (stmt.mStatements.size() == 0 || stmt.mStatements[0].mStatements.size() == 0) scope.varTab[stmt.mName] = Array::getDefault(stmt.mType.mName);
        else if (stmt.mStatements.size() > 1) throw runtime_error("unexpected error (1)");
        else {
            scope.varTab[stmt.mName] = Array::checkAll(stmt.mType.mName, Interpreter::evaluateStatement(stmt.mStatements[0], scope));
        }
        return nullptr;
    }

    Object* Functions::evaluateArrayCreation(Statement &stmt, Scope &scope) {
        // create new array from all of the objects
        vector<Object*> objects;
        for (Statement &s : stmt.mStatements) {
            objects.emplace_back(Interpreter::evaluateStatement(s, scope));
        }
        return new Array(objects);
    }

    Object* Functions::evaluateArrayElement(Statement &stmt, Scope &scope) {
        if (stmt.mStatements.size() != 2) throw runtime_error("unexpected error (2)");
        // array assignment
        if (stmt.mStatements[1].mKind != StatementKind::ARRAY_CALL) {
            Array::assignIndex(stoi(Interpreter::evaluateStatement(stmt.mStatements[0], scope)->getValueString()), scope.varTab[stmt.mName], 
            Interpreter::evaluateStatement(stmt.mStatements[1], scope));
        }
        // array call :D
        else {
            return Array::getIndex(stoi(Interpreter::evaluateStatement(stmt.mStatements[0], scope)->getValueString()), 
                Interpreter::evaluateStatement(stmt.mStatements[1], scope));
        }
        return nullptr;
    }

    Object* Functions::evaluateVariableCall(Statement &stmt, Scope &scope) {
        Object* special = Functions::specialVariable(stmt, scope);
            if (special != nullptr) return special;
            if (scope.varTab.find(stmt.mName) == scope.varTab.end()) throw runtime_error("cannot find variable '" + stmt.mName + "'");
            if (stmt.mStatements.size() >= 1) { // assign to existing variable
                 if (stmt.mStatements[0].mKind == StatementKind::FUNCTION_CALL) {
                    // this is a function inside of object
                    map<string, objectF> funcs = scope.varTab[stmt.mName]->getFunctions();
                    if (funcs.find(stmt.mStatements[0].mName) == funcs.end()) throw runtime_error("class " + stmt.mName + " does not have a member " + stmt.mStatements[0].mName);
                    // collect arguments
                    vector<Object*> args;
                    for (Statement &s : stmt.mStatements[0].mStatements) {
                        args.emplace_back(Interpreter::evaluateStatement(s, scope));
                    }
                    return funcs[stmt.mStatements[0].mName](scope.varTab[stmt.mName], args);
                }
                else scope.varTab[stmt.mName]->assign(Object::checkAll(scope.varTab[stmt.mName]->getType(), Interpreter::evaluateStatement(stmt.mStatements[0], scope)));
                return scope.varTab[stmt.mName];
            }
            return scope.varTab[stmt.mName];
    }

    Scope::Scope(Scope &b) {
        for (pair<string, variable::Object*> p : b.varTab) {
            varTab[p.first] = p.second; // create clone of current scope
        }
        for (pair<string, builtinF> f : b.functions) {
            functions[f.first] = f.second;
        }
        for (pair<string, Scope*> n : b.namespaces) {
            namespaces[n.first] = n.second; // create clone of current namespaces
        }
    }

    Scope::Scope(Scope &i, Scope &b) {
        for (pair<string, variable::Object*> p : i.varTab) {
            varTab[p.first] = p.second; // create clone of i scope
        }
        for (pair<string, builtinF> f : i.functions) {
            functions[f.first] = f.second;
        }
        for (pair<string, Scope*> n : i.namespaces) {
            namespaces[n.first] = n.second; // create clone of i namespaces
        }
        for (pair<string, variable::Object*> p : b.varTab) {
            varTab[p.first] = p.second; // create clone of b scope
        }
        for (pair<string, builtinF> f : b.functions) {
            functions[f.first] = f.second;
        }
        for (pair<string, Scope*> n : b.namespaces) {
            namespaces[n.first] = n.second; // create clone of b namespaces
        }
    }
}