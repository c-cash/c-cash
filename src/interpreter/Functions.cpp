#include "Functions.hpp"
#include "Interpreter.hpp"
#include "../parser/Parser.hpp"
#include "../parser/Tokenizer.hpp"
#include "../variables/Object.hpp"

#include <stdexcept>
#include <algorithm>
#include <string>
#include <fstream>

#include "../variables/Double.hpp"
#include "../variables/Integer.hpp"
#include "../variables/Boolean.hpp"
#include "../variables/SpecialObject.hpp"
#include "../variables/Array.hpp"

#include "../libraries/LibraryRegistry.hpp"
#include "../errors/interpreterError.hpp"

namespace interpreter {
    using namespace std;
    using namespace parser;
    using namespace variable;

    Object* Functions::evaluateMath(Statement &stmt, Scope &scope) {
        switch (stmt.mName[0]) {
            case '+':
                // evaluate and add next two statements
                return Interpreter::evaluateStatement(stmt.mStatements[0], scope)->add(Interpreter::evaluateStatement(stmt.mStatements[1], scope));
            case '-':
                // evaluate and subtract next two statements
                return Interpreter::evaluateStatement(stmt.mStatements[0], scope)->subtract(Interpreter::evaluateStatement(stmt.mStatements[1], scope));
            case '*':
                // evaluate and multiply next two statements
                return Interpreter::evaluateStatement(stmt.mStatements[0], scope)->multiply(Interpreter::evaluateStatement(stmt.mStatements[1], scope));
            case '/':
                // evaluate and divide next two statements
                return Interpreter::evaluateStatement(stmt.mStatements[0], scope)->divide(Interpreter::evaluateStatement(stmt.mStatements[1], scope));
            case '%':
                // evaluate and use modulo on next two statements
                return Interpreter::evaluateStatement(stmt.mStatements[0], scope)->modulo(Interpreter::evaluateStatement(stmt.mStatements[1], scope));
            case '(': 
                //If () brackets
                return evaluateMath(stmt.mStatements[0], scope);
            default:
                return nullptr;
        }
    }

    Object* Functions::evaluateLogic(Statement &stmt, Scope &scope) {
        if (stmt.mKind == StatementKind::VARIABLE_CALL) {
            if (stmt.mName == "true") return new Boolean(true);
            else if (stmt.mName == "false") return new Boolean(false);
            throw runtime_error("logic expression needed in line " + to_string(stmt.mLine));
        }
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
        else if(stmt.mName == "()")
            return evaluateLogic(stmt.mStatements[0], scope);
        else if (stmt.mName == "!")
            return new Boolean(!static_cast<Boolean*>(Interpreter::evaluateStatement(stmt.mStatements[0], scope))->value);
        return nullptr;
    }

    Object* Functions::specialVariable(Statement &stmt, Scope &scope) {
        if (stmt.mName == "break") { return new SpecialObject(SpecialType::BREAK, nullptr);
        } else if (stmt.mName == "continue") { return new SpecialObject(SpecialType::CONTINUE, nullptr);
        } else if (stmt.mName == "true") { return new variable::Boolean(true); 
        } else if (stmt.mName == "false") { return new variable::Boolean(false); }
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

        // try / catch
        if (stmt.mName == "TRY") {
            if (stmt.mStatements[0].mName != "TRYCMD") throw runtime_error("unexpected error(5)");
            try {
                for (Statement &s : stmt.mStatements[0].mStatements) Interpreter::evaluateStatement(s, scope);
            } catch (runtime_error &err) {
                if (stmt.mStatements.size() <= 1) throw err;
                else if (stmt.mStatements[1].mName != "CATCHCMD") {
                    if (stmt.mStatements[1].mName == "FINALLYCMD") {
                        for (Statement &s : stmt.mStatements[1].mStatements) Interpreter::evaluateStatement(s, scope);
                        throw err;
                    }
                    throw runtime_error("expected catch instead of " + stmt.mStatements[1].mName + " in line " + to_string(stmt.mStatements[1].mLine));
                };
                for (Statement &s : stmt.mStatements[1].mStatements) Interpreter::evaluateStatement(s, scope);
            }
            if (stmt.mStatements.size() == 2 && stmt.mStatements[1].mName == "FINALLYCMD") {
                for (Statement &s : stmt.mStatements[1].mStatements) Interpreter::evaluateStatement(s, scope);
            }
            if (stmt.mStatements.size() <= 2) return nullptr;
            else if (stmt.mStatements[2].mName != "FINALLYCMD") throw runtime_error("expected try-catch-finally or try-finally in line " + to_string(stmt.mLine));
            else for (Statement &s : stmt.mStatements[2].mStatements) Interpreter::evaluateStatement(s, scope);
            return nullptr;
        }

        // IF / ELSE / ELIF
        if (stmt.mName == "IF") {
            scope.isPreviousIf = true;
            // check if correct
            if (stmt.mStatements[0].mKind != StatementKind::VARIABLE_CALL && stmt.mStatements[0].mKind != StatementKind::LOGIC_CALL) throw runtime_error(error::getInterpreterError(error::InterpreterErrorType::EXPECTED_EXPRESSION, {"if"}));
            // check logic
            bool e = static_cast<Boolean*>(evaluateLogic(stmt.mStatements[0], scope))->value;
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
            if (stmt.mStatements[0].mKind != StatementKind::VARIABLE_CALL && stmt.mStatements[0].mKind != StatementKind::LOGIC_CALL) throw runtime_error(error::getInterpreterError(error::InterpreterErrorType::EXPECTED_EXPRESSION, {"elif"}));
            // check logic
            bool e = static_cast<Boolean*>(evaluateLogic(stmt.mStatements[0], scope))->value;
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
        builtinF f = findFunction(stmt.mName, scope);
        if (f != nullptr) {
            // this is a library function :D
            return f(args); 
        }
        // TODO: implement namespaced builtins

        // check if function exists and if not then throw error
        if (Interpreter::definitions.find(stmt.mName) == Interpreter::definitions.end()) throw runtime_error(error::getInterpreterError(error::InterpreterErrorType::NO_FUNCTION, {stmt.mName, to_string(stmt.mLine)})/*"Cannot find function '" + stmt.mName + "'"*/);

        // run function
        return Interpreter::evaluateFunction(Interpreter::definitions[stmt.mName], args);
    }

    Object* Functions::useSpecial(SpecialObject* s, string place) {
        if ((s->getValueString() == "break" || s->getValueString() == "continue") && place == "loop") {s->assign(SpecialType::NONE); return s;}
        return s;
    }

    Object* Functions::evaluateLoop(Statement &stmt, Scope &scope) {
        if (stmt.mStatements[0].mKind != StatementKind::FUNCTION_CALL) throw runtime_error(error::getInterpreterError(error::InterpreterErrorType::EXPECTED_EXPRESSION, {"loop"}));
        Statement &params = stmt.mStatements[0];
        // LOOP WITH INTEGER
        if (params.mStatements.size() == 1 && params.mStatements[0].mKind != StatementKind::LOGIC_CALL) {
            // execute loop
            uint count = static_cast<Integer*>(Interpreter::evaluateStatement(params.mStatements[0], scope))->getSignedValue();
            Scope* s = new Scope(scope);
            s->varCache = new vector<const char*>();
            for (int i=0; i<count; i++) {
                s->reset();
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
            // check logic statement
            bool e = static_cast<Boolean*>(evaluateLogic(params.mStatements[0],scope))->value;
            Scope* s = new Scope(scope);
            s->varCache = new vector<const char*>();
            while (e) {
                s->reset();
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

                e = static_cast<Boolean*>(evaluateLogic(params.mStatements[0],scope))->value;
            }
        }
        // NORMAL FOR LOOP
        else if (params.mStatements.size() == 3 && params.mStatements[1].mKind == StatementKind::LOGIC_CALL) {
            Scope* t = new Scope(scope);
            // execute before statement
            Interpreter::evaluateStatement(params.mStatements[0], *t);
            // check logic statement
            bool e = static_cast<Boolean*>(evaluateLogic(params.mStatements[1], *t))->value;
            Scope* s = new Scope(*t);
            s->varCache = new vector<const char*>();
            while (e) {
                s->reset();
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
                e = static_cast<Boolean*>(evaluateLogic(params.mStatements[1],*t))->value;
            }
        }
        return nullptr;
    }

    void Functions::includeLibrary(FunctionDefinition &func, Scope &scope) {
        for (Statement &s : func.mStatements) {
            // check if it is include
            if (s.mKind != StatementKind::FUNCTION_CALL || (s.mName != "include" && s.mName != "nsinclude"))
                throw runtime_error(error::getInterpreterError(error::InterpreterErrorType::OUTSIDE_SCOPE, {"include", to_string(s.mLine)}));
            
            if (s.mName == "include") {
                if (s.mStatements.size() != 1 || s.mStatements[0].mKind != StatementKind::LITERAL || s.mStatements[0].mType.mName != "string")
                    throw runtime_error(error::getInterpreterError(error::InterpreterErrorType::INVALID_ARGUMENTS, {"include", to_string(s.mLine)}));
                    // include globally
                    string name = s.mStatements[0].mName;

                    // if this is a .cash file
                    if(name.size() >= 5) {
                        if (name.substr(name.size()-5, name.size()) == ".cash") {
                            //Read file
                            ifstream file(name);
                            string line, allCode="";
                            bool isParsed = false, isFirst = true;
                            while (getline(file, line)){
                                if (isFirst && line == "#!/parsed") {
                                    isParsed = true;
                                    break;
                                }
                                allCode += line + '\n';
                                isFirst = false;
                            }
                            //Tokenize
                            parser::Tokenaizer tokenizer;
                            parser::Parser parser;
                            vector<Token> tokens = tokenizer.parse(allCode);
                            //Parse
                            parser.parse(tokens);
                            for (pair<string, parser::FunctionDefinition> d : parser.mFunction) {
                                if (d.first != "*" && Interpreter::definitions.find(d.first) != Interpreter::definitions.end()) throw runtime_error("refefinition of function '" + d.first + "' in file '" + name + "'");
                                else if (d.first != "*") Interpreter::definitions[d.first] = d.second;
                                else Functions::includeLibrary(d.second, *Interpreter::includes);
                            }
                            return;
                        }
                    }

                    if (library::libraries.find(name) == library::libraries.end()) throw runtime_error(error::getInterpreterError(error::InterpreterErrorType::NO_LIBRARY, {name, to_string(s.mLine)}));
                    library::libraries[name]()->linkGlobally(*Interpreter::includes);
            } else if (s.mName == "nsinclude") {
                if (s.mStatements[0].mKind != StatementKind::LITERAL) throw runtime_error("you need to provide string into include function in line " + to_string(s.mLine));
                string nsname;
                string libname;
                if (libname.substr(libname.size()-5, libname.size()) == ".cash") { // include from ccash file
                    throw runtime_error("including other .cash file as namespaced library is not yet available");
                }

                if (s.mStatements.size() == 1) { // library name
                    nsname = s.mStatements[0].mName;
                    libname = s.mStatements[0].mName;
                } else if (s.mStatements.size() == 2) { // custom name
                    if (s.mStatements[1].mKind != StatementKind::LITERAL) throw runtime_error("you need to provide string into include function in line " + to_string(s.mLine));
                    nsname = s.mStatements[0].mName;
                    libname = s.mStatements[1].mName;
                } else {throw runtime_error(error::getInterpreterError(error::InterpreterErrorType::INVALID_ARGUMENT_COUNT_LINE, {"include", "2", to_string(s.mStatements.size()), to_string(s.mLine)}));}
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
        if (stmt.mKind == StatementKind::ARRAY && (stmt.mStatements.size() == 0 || stmt.mStatements[0].mStatements.size() == 0)) scope.varTab[stmt.mName] = Array::getDefault(stmt.mType.mName);
        else if (stmt.mStatements.size() > 1) throw runtime_error("unexpected error (1)");
        else {
            Object* temp = Array::checkAll(stmt.mType.mName, Interpreter::evaluateStatement(stmt.mStatements[0], scope));
            scope.memoryClean.emplace_back(temp);
            scope.varTab[stmt.mName] = temp;
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
        if (stmt.mStatements.size() >= 1) { // assign to existing variable
            if (stmt.mStatements[0].mKind == StatementKind::FUNCTION_CALL) {
                // this is a function inside of object
                Object* v = findVariable(stmt.mName, scope);
                if (v == nullptr) throw runtime_error(error::getInterpreterError(error::InterpreterErrorType::NO_VARIABLE, {stmt.mName, to_string(stmt.mLine)}));
                map<string, objectF> funcs = v->getFunctions();
                if (funcs.find(stmt.mStatements[0].mName) == funcs.end()) throw runtime_error("class " + stmt.mName + " does not have a member " + stmt.mStatements[0].mName);
                // collect arguments
                vector<Object*> args;
                for (Statement &s : stmt.mStatements[0].mStatements) {
                    args.emplace_back(Interpreter::evaluateStatement(s, scope));
                }
                return funcs[stmt.mStatements[0].mName](v, args);
            } else {
                Object* v = findVariable(stmt.mName, scope);
                Object* temp = Object::checkAll(v->getType(), Interpreter::evaluateStatement(stmt.mStatements[0], scope));
                v->assign(temp);
                scope.memoryClean.emplace_back(temp);
            }
            Object* v = findVariable(stmt.mName, scope);
            if (v == nullptr) throw runtime_error(error::getInterpreterError(error::InterpreterErrorType::NO_VARIABLE, {stmt.mName, to_string(stmt.mLine)}));
            return v;
        }
        Object* v = findVariable(stmt.mName, scope);
        if (v == nullptr) throw runtime_error(error::getInterpreterError(error::InterpreterErrorType::NO_VARIABLE, {stmt.mName, to_string(stmt.mLine)}));
        return v;
    }
  
    Object* Functions::findVariable(string name, Scope &scope) {
        if (scope.varTab.unordered_map::find(name) == scope.varTab.end()) {
            if (scope.parent != nullptr) return findVariable(name, *scope.parent);
            return nullptr;
        }
        return scope.varTab[name];
    }

    vector<const char*>* Functions::getNearestCache(Scope &scope) {
        if (scope.varCache != nullptr) return scope.varCache;
        else if (scope.parent != nullptr) return getNearestCache(*scope.parent);
        return nullptr;
    }

    builtinF Functions::findFunction(string name, Scope &scope) {
        if (scope.nsparent != nullptr) {
            builtinF f = findFunction(name, *scope.nsparent); 
            if (f==nullptr) {throw runtime_error(error::getInterpreterError(error::InterpreterErrorType::NO_FUNCTION, {name}));}
            return f;
        }
        if (scope.functions.find(name) == scope.functions.end()) {
            if (scope.parent != nullptr) return findFunction(name, *scope.parent);
            return nullptr;
        }
        return scope.functions[name];
    }

    Scope* Functions::findNamespace(string name, Scope &scope) {
        if (scope.namespaces.find(name) == scope.namespaces.end()) {
            if (scope.parent != nullptr) return findNamespace(name, *scope.parent);
            throw runtime_error("cannot find namespace '" + name + "'");
        }
        return scope.namespaces[name];
    }

    Scope::Scope(Scope &b) {
        this->parent = &b;
    }

    Scope::Scope(Scope &b, Scope* n) {
        this->parent = &b;
        this->nsparent = n;
    }

    void Scope::reset() {
        for (auto &o : this->varTab) delete o.second;
        for (auto &o : this->namespaces) delete o.second;
        for(auto &o : this->memoryClean) delete o;
        this->memoryClean.clear();
        this->varTab.clear();
        this->functions.clear();
        this->namespaces.clear();
    }
}