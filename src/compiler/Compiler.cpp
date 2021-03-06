#include <iostream>
#include "Compiler.hpp"

#define getData(type, i) std::get<type>(i)

namespace compiler {
    using namespace std;

    void Compiler::popScope(Stack &s) {
        vector<string>* scope = s.scope.top(); s.scope.pop();
        for (string &str : *scope) {
            s.numerical_names.erase(str);
            s.varTypes.erase(str);
            s.loopbegins.pop();
            // TODO: remove this from heap
        }
    }

    void Compiler::compile(map<string, FunctionDefinition> &mFunctions, ofstream &out, map<string, ClassDefinition> &cDefinitions) {
        classMap = cDefinitions;
        this->out = &out;
        // check if there is a main function
        if (mFunctions.find("main") == mFunctions.end()) throw runtime_error("Code must have a main function");
        // add $Main class to the file
        out << "$Main" << NOP;
        writeInteger(mFunctions.size());
        getFunctionsMap(mFunctions, cDefinitions);
        // Complie funcions
        for (auto &p : mFunctions) {
            char flags = 0b0;
            if (std::find(p.second.mKeywords.begin(), p.second.mKeywords.end(), "static") == p.second.mKeywords.end()) {
                flags = flags | 0b0000'0001;
            }
            compileFunction(p.second, "$Main", flags);
        }

        // Compile other classes
        for (auto &p : cDefinitions) {
            out << p.first << NOP;
            writeInteger(p.second.mFunctions.size() + p.second.mConDes.size());
            // constructor
            if ((*staticFunctionsMap[p.first]).find("$C") != (*staticFunctionsMap[p.first]).end()) {
                bool isFirst = (*staticFunctionsMap[p.first])["$C"];
                FunctionDefinition &d = p.second.mConDes[isFirst ? 0 : 1];
                d.mName = "$C";
                compileFunction(d, p.first, 0b0000'0001); // TODO: add private constructors
            }
            // destructor
            if ((*staticFunctionsMap[p.first]).find("$D") != (*staticFunctionsMap[p.first]).end()) {
                bool isFirst = (*staticFunctionsMap[p.first])["$D"];
                FunctionDefinition &d = p.second.mConDes[isFirst ? 0 : 1];
                d.mName = "$D";
                compileFunction(d, p.first, 0b0000'0001); // TODO: add private constructors
            }
            // other functions
            for (auto &fp : p.second.mFunctions) {
                char flags = 0b0000'0000;
                if (std::find(fp.mKeywords.begin(), fp.mKeywords.end(), "static") != fp.mKeywords.end()) flags |= 0b0000'0001;
                compileFunction(fp, p.first, flags); // TODO: change to static if flag is set
            }
        }

        out << EOP;
    }

    Stack* Compiler::compileFunction(FunctionDefinition &d, string className, char flags) {
        *out << flags;
        ci = 0; // set current instruction to 0
        // save function name
        writeInteger(d.mName.size());
        *out << d.mName;
        // add 'this' parameter if function is not static
        if ((int)(flags & 0b0000'0001) != 1) {
            ParameterDefinition* def = new ParameterDefinition();
            def->mName = "this";
            def->mType.mName = "class";
            d.mParameters.insert(d.mParameters.begin(), *def);
        }

        // save parameters count
        writeInteger(d.mParameters.size());
        // Create new stack and compile this function
        Stack* s = new Stack();
        s->className = className;
        // create vartypes and names for parameters
        for (ParameterDefinition &def : d.mParameters) {
            s->varTypes[def.mName] = def.mType.mName;
            ++ci;
            *out << (char)BytecodeInstructions::ANYSTORE;
            writeInteger(getNumericalName(def.mName, *s));
        }
        // TODO: add simulated parameters
        compileCode(d.mStatements, *s);
        *out << (char)BytecodeInstructions::null << NOP;
        return s;
    }

    void Compiler::compileCode(vector<Statement> &statements, Stack &s) {
        for (Statement &stmt : statements) {
            compileStatement(stmt, s);
        }
    }

    void Compiler::compileStatement(Statement &stmt, Stack &s) {
        switch (stmt.mKind) {
            case StatementKind::MULTIPLE_VARIABLE_DECLARATION: {
                // compile all variable declarations
                for (Statement &v : stmt.mStatements) compileVariableDeclaration(v, s);
                break;
            }
            case StatementKind::VARIABLE_DECLARATION: {
                // compile variable declaration
                compileVariableDeclaration(stmt, s);
                break;
            }
            case StatementKind::OPERATOR_CALL: {
                // compile this math operation
                compileMathOperation(stmt, s);
                break;
            }
            case StatementKind::VARIABLE_CALL: {
                // find variable type and compile this
                compileVariableCall(stmt, s);
                break;
            }
            case StatementKind::LITERAL: {
                // compile this litteral
                compileLitteral(stmt, s);
                break;
            }
            case StatementKind::LOGIC_CALL: {
                compileLogicCall(stmt, s);
                break;
            }
            case StatementKind::FUNCTION_CALL: {
                compileFunctionCall(stmt, s);
                break;
            }
            case StatementKind::NEW: {
                compileNewClass(stmt, s);
                break;
            }
            case StatementKind::CLASS_CALL: {
                compileClassCall(stmt, s);
                break;
            }
            case StatementKind::ARRAY_ELEMENT: {
                compileArrayElement(stmt, s);
                break;
            }
            default:
                break;
        }
    }

    void Compiler::compileClassCall(Statement &stmt, Stack &s) {
        // execute everything inside
        if (stmt.mStatements[0].mKind == StatementKind::FUNCTION_CALL) {
            bool isStatic = false;
            if (classMap.find(stmt.mName) != classMap.end()) {
                isStatic = (*staticFunctionsMap[stmt.mName])[stmt.mStatements[0].mName];
            }
            if (!isStatic) {
                // load class
                ++ci;
                *out << (char)BytecodeInstructions::OLOAD;
                writeInteger(getNumericalName(stmt.mName, s));
                // compile function
                string tmp = s.className;
                s.className = getStatementType(stmt, s);
                compileFunctionCall(stmt.mStatements[0], s);
                s.className = tmp;
            } else {
                string tmp = s.className;
                s.className = stmt.mName;
                compileFunctionCall(stmt.mStatements[0], s);
                s.className = tmp;
            }
        }
    }

    void Compiler::compileNewClass(Statement &stmt, Stack &s) {
        for (Statement &st : stmt.mStatements) {
            compileStatement(st, s);
        }
        ++ci;
        *out << (char)BytecodeInstructions::OCONST;
        writeUTF8(stmt.mName);
    }

    void Compiler::compileFunctionCall(Statement &stmt, Stack &s) {
        if (stmt.mName == "IF" || stmt.mName == "COUNTING_LOOP" || stmt.mName == "CONDITION_LOOP" 
            || stmt.mName == "ITERATION_LOOP" || stmt.mName == "EACH_LOOP" || stmt.mName == "ELSE" || stmt.mName == "ELIF") 
            { compileSpecialFunctionCall(stmt, s); return; }

        if (stmt.mName == "return") {
            if (stmt.mStatements.size() == 0) { // void function
                ++ci;
                *out << (char)BytecodeInstructions::RETURN;
            }
            string rtype = getStatementType(stmt.mStatements[0], s);
            compileStatement(stmt.mStatements[0], s);
            ++ci;
            *out << (char)BytecodeInstructions::RETURN;
            return;
        }

        string cName = s.className;

        bool isStatic = (*staticFunctionsMap[cName])[stmt.mName];
        // put all arguments onto the stack
        int i = 0; //isStatic ? 0 : 1;
        for (Statement &arg : stmt.mStatements) {
            compileStatement(arg, s);
            tryConvertValue(getStatementType(arg, s), (*(*functionParamDefs[cName])[stmt.mName])[i].mType.mName);
            ++i;
        }

        if (isStatic) {
            ++ci;
            *out << (char)BytecodeInstructions::CALLSTATIC;
            writeUTF8(cName);
            writeUTF8(stmt.mName);
        } else {
            ++ci;
            *out << (char)BytecodeInstructions::CALLMETHOD;
            writeUTF8(stmt.mName);
        }
    }
    void Compiler::compileSpecialFunctionCall(Statement &stmt, Stack &s) {
        if (stmt.mName == "IF") {
            // write instruction
            compileStatement(stmt.mStatements[0], s);

            // write JMPFALSE and save current cursor position
            ++ci;
            *out << (char)BytecodeInstructions::JMPFALSE;
            streampos startPos = out->tellp();
            writeULong(0LL);

            // compile rest of the instructions
            s.scope.push(new vector<string>());
            for (int i{1}; i < stmt.mStatements.size(); ++i) {
                compileStatement(stmt.mStatements[i], s);
            }
            popScope(s);

            // go back to the jump and write correct position
            streampos tmp = out->tellp();
            out->seekp(startPos);
            writeULong(ci+1);
            out->seekp(tmp);

            ++ci;
            *out << (char)BytecodeInstructions::null;
        } else if (stmt.mName == "ELSE") {
            // write goto and save current position for later
            out->seekp(out->tellp()-(streampos)1);
            *out << (char)BytecodeInstructions::GOTO;
            unsigned long long gotoci = ci;
            streampos gotopos = out->tellp();
            writeULong(0LL);

            // compile rest of the instructions
            s.scope.push(new vector<string>());
            for (int i{0}; i < stmt.mStatements.size(); ++i) {
                compileStatement(stmt.mStatements[i], s);
            }
            popScope(s);

            // change goto position
            streampos tmp = out->tellp();
            out->seekp(gotopos);
            writeULong(ci);
            out->seekp(tmp);

        } else if (stmt.mName == "ELIF") {
            // write goto and save current position for later
            out->seekp(out->tellp()-(streampos)1);
            *out << (char)BytecodeInstructions::GOTO;
            unsigned long long gotoci = ci;
            streampos gotopos = out->tellp();
            writeULong(0LL);

            // COMPILE CHECK
            // write instruction
            compileStatement(stmt.mStatements[0], s);

            // write JMPFALSE and save current cursor position
            ++ci;
            *out << (char)BytecodeInstructions::JMPFALSE;
            streampos startPos = out->tellp();
            writeULong(0LL);

            // compile rest of the instructions
            s.scope.push(new vector<string>());
            for (int i{1}; i < stmt.mStatements.size(); ++i) {
                compileStatement(stmt.mStatements[i], s);
            }
            popScope(s);

            // go back to the jump and write correct position
            streampos tmp = out->tellp();
            out->seekp(startPos);
            writeULong(ci+1);

            // change goto position
            out->seekp(gotopos);
            writeULong(ci);
            out->seekp(tmp);

            ++ci;
            *out << (char)BytecodeInstructions::null;

        } else if (stmt.mName == "CONDITION_LOOP") {
            s.breaks.push(new vector<streampos>());
            // write goto and save current position for later
            ++ci;
            *out << (char)BytecodeInstructions::GOTO;
            unsigned long long gotoci = ci;
            streampos gotopos = out->tellp();
            s.loopbegins.push(ci-1);
            writeULong(0LL);
            
            // write all instructions
            s.scope.push(new vector<string>());
            for (int i{1}; i < stmt.mStatements.size(); ++i) {
                compileStatement(stmt.mStatements[i], s);
            }
            popScope(s);

            // change goto position
            streampos tmp = out->tellp();
            out->seekp(gotopos);
            writeULong(ci);
            out->seekp(tmp);

            // write check and jump
            compileStatement(stmt.mStatements[0].mStatements[0], s);
            ++ci;
            *out << (char)BytecodeInstructions::JMPFALSE;
            writeULong(gotoci);

            // breaks
            tmp = out->tellp();
            for (streampos p : *s.breaks.top()) {
                out->seekp(p);
                writeULong(ci);
            }
            s.breaks.pop();
            out->seekp(tmp);
        } else if (stmt.mName == "ITERATION_LOOP") {
            // normal for loop
            s.breaks.push(new vector<streampos>());
            s.scope.push(new vector<string>());
            // code before loop
            compileStatement(stmt.mStatements[0].mStatements[0], s);
            // goto check
            ++ci;
            *out << (char)BytecodeInstructions::GOTO;
            unsigned long long gotoci = ci;
            streampos gotopos = out->tellp();
            s.loopbegins.push(ci-1);
            writeULong(0LL);
            // code inside loop
            for (int i{1}; i < stmt.mStatements.size(); ++i) {
                compileStatement(stmt.mStatements[i], s);
            }
            // code after loop
            compileStatement(stmt.mStatements[0].mStatements[2], s);
            // update goto
            streampos tmp = out->tellp();
            out->seekp(gotopos);
            writeULong(ci);
            out->seekp(tmp);
            // check and jump to start
            compileStatement(stmt.mStatements[0].mStatements[1], s);
            popScope(s);
            ++ci;
            *out << (char)BytecodeInstructions::JMPFALSE;
            writeULong(gotoci);

            // breaks
            tmp = out->tellp();
            for (streampos p : *s.breaks.top()) {
                out->seekp(p);
                writeULong(ci);
            }
            s.breaks.pop();
            out->seekp(tmp);
        } else if(stmt.mName == "EACH_LOOP") {
            //To implement
        } else if(stmt.mName == "COUNTING_LOOP") {
            //To implement
        }
    }

    // TODO: rethink this and other implementations for 64 bit long values
    void Compiler::compileMathOperation(Statement &stmt, Stack &s) {
        // get left value
        string type = getStatementType(stmt.mStatements[0], s);
        compileStatement(stmt.mStatements[0], s);
        // get right value
        compileStatement(stmt.mStatements[1], s);
        tryConvertValue(getStatementType(stmt.mStatements[1], s), type);
    

        ++ci; // increase instruction count
        if (type == "signed int") {
            switch(stmt.mName[0]) {
                case '+':
                    *out << (char)BytecodeInstructions::IADD;
                    break;
                case '-':
                    *out << (char)BytecodeInstructions::ISUB;
                    break;
                case '*':
                    *out << (char)BytecodeInstructions::IMUL;
                    break;
                case '/':
                    *out << (char)BytecodeInstructions::IDIV;
                    break;
            }
        } else if (type == "long") {
            switch(stmt.mName[0]) {
                case '+':
                    *out << (char)BytecodeInstructions::LADD;
                    break;
                case '-':
                    *out << (char)BytecodeInstructions::LSUB;
                    break;
                case '*':
                    *out << (char)BytecodeInstructions::LMUL;
                    break;
                case '/':
                    *out << (char)BytecodeInstructions::LDIV;
                    break;
            }
        } else if (type == "double") {
            switch(stmt.mName[0]) {
                case '+':
                    *out << (char)BytecodeInstructions::DADD;
                    break;
                case '-':
                    *out << (char)BytecodeInstructions::DSUB;
                    break;
                case '*':
                    *out << (char)BytecodeInstructions::DMUL;
                    break;
                case '/':
                    *out << (char)BytecodeInstructions::DDIV;
                    break;
            }
        } else if (type == "char") {
            switch(stmt.mName[0]) {
                case '+':
                    *out << (char)BytecodeInstructions::CADD;
                    break;
                case '-':
                    *out << (char)BytecodeInstructions::CSUB;
                    break;
                default:
                    throw runtime_error("char mlutiplication and division are not available");
                    break;
            }
        } else {
            switch(stmt.mName[0]) {
                case '+':
                    addMethodCall("BinaryPlus"); break;
                case '-':
                    addMethodCall("BinaryMinus"); break;
                case '*':
                    addMethodCall("BinaryMultiply"); break;
                case '/':
                    addMethodCall("BinaryDivide"); break;
                case '%':
                    addMethodCall("BinaryRemainder"); break;
            }
        }
    }

    void Compiler::addMethodCall(string name) {
        ++ci;
        *out << (char)BytecodeInstructions::CALLMETHOD;
        writeUTF8(name);
    }

    void Compiler::compileLogicCall(Statement &stmt, Stack &s) {
        bool rev = (stmt.mName == "<" || stmt.mName == "<=");
        compileStatement(stmt.mStatements[rev ? 1 : 0], s);
        if (stmt.mStatements.size() == 2) { // compile second argument if there is one
            compileStatement(stmt.mStatements[rev ? 0 : 1], s);            
            tryConvertValue(getStatementType(stmt.mStatements[rev ? 0 : 1], s), getStatementType(stmt.mStatements[rev ? 1 : 0], s));
        }

        ++ci;
        if (stmt.mName == "==") {
            *out << (char)BytecodeInstructions::IFEQ;
        } else if (stmt.mName == "!=") {
            *out << (char)BytecodeInstructions::IFNE;
        } else if (stmt.mName == "<") {
            *out << (char)BytecodeInstructions::IFGT;
        } else if (stmt.mName == "<=") {
            *out << (char)BytecodeInstructions::IFGE;
        } else if (stmt.mName == ">") {
            *out << (char)BytecodeInstructions::IFGT;
        } else if (stmt.mName == ">=") {
            *out << (char)BytecodeInstructions::IFGE;
        } else if (stmt.mName == "!") {
            *out << (char)BytecodeInstructions::NOT;
        } else if (stmt.mName == "()") { /* do nothing */ }
    }

    void Compiler::compileVariableDeclaration(Statement &stmt, Stack &s) {
        // compile whatever value this have
        compileStatement(stmt.mStatements[0], s);

        // store this variable type for later
        if (stmt.mType.mName == "class") {
            s.varTypes[stmt.mName] = stmt.mStatements[0].mName;
        } else {
            s.varTypes[stmt.mName] = stmt.mType.mName;
        }
        // ensure that values have the same type
        tryConvertValue(getStatementType(stmt.mStatements[0], s), stmt.mType.mName);
        ++ci; // increase instruction count
        // add this variable to the scope
        if (!s.scope.empty()) s.scope.top()->emplace_back(stmt.mName);
        if (stmt.mType.mName == "signed int") 
            {*out << (char)BytecodeInstructions::ISTORE; writeInteger(getNumericalName(stmt.mName, s)); }
        else if (stmt.mType.mName == "long") 
            {*out << (char)BytecodeInstructions::LSTORE; writeInteger(getNumericalName(stmt.mName, s)); }
        else if (stmt.mType.mName == "double") 
            {*out << (char)BytecodeInstructions::DSTORE; writeInteger(getNumericalName(stmt.mName, s)); }
        else if (stmt.mType.mName == "char") 
            {*out << (char)BytecodeInstructions::CSTORE; writeInteger(getNumericalName(stmt.mName, s)); }
        else if (stmt.mType.mName == "bool") 
            {*out << (char)BytecodeInstructions::BSTORE; writeInteger(getNumericalName(stmt.mName, s)); }
        else if (stmt.mType.mName == "class") 
            {*out << (char)BytecodeInstructions::OSTORE; writeInteger(getNumericalName(stmt.mName, s)); }
    }
    
    // compiles variable call statement
    void Compiler::compileVariableCall(Statement &stmt, Stack &s) {
        // special variables
        // TRUE / FALSE
        if (stmt.mName == "true" || stmt.mName == "false") {
            ++ci;
            *out << (char)BytecodeInstructions::BCONST; 
            *out << (stmt.mName == "true");
            return;
        }

        // BREAK
        if (stmt.mName == "break") {
            ++ci;
            *out << (char)BytecodeInstructions::GOTO;
            if (s.breaks.empty()) throw runtime_error("'break' usage outside of the loop");
            s.breaks.top()->emplace_back(out->tellp());
            writeULong(0LL);
            return;
        }

        // CONTINUE
        if (stmt.mName == "continue") {
            ++ci;
            *out << (char)BytecodeInstructions::GOTO;
            writeULong(s.loopbegins.top());
            return;
        }


        if (s.varTypes.find(stmt.mName) == s.varTypes.end()) throw runtime_error("Variable " + stmt.mName + " should be defined before call");
        string t = s.varTypes[stmt.mName];
        // load or store based on what operation user wants to do
        BytecodeInstructions itype = stmt.mStatements.size() == 0 ? BytecodeInstructions::ILOAD : BytecodeInstructions::ISTORE;
        BytecodeInstructions ltype = stmt.mStatements.size() == 0 ? BytecodeInstructions::LLOAD : BytecodeInstructions::LSTORE;
        BytecodeInstructions dtype = stmt.mStatements.size() == 0 ? BytecodeInstructions::DLOAD : BytecodeInstructions::DSTORE;
        BytecodeInstructions ctype = stmt.mStatements.size() == 0 ? BytecodeInstructions::CLOAD : BytecodeInstructions::CSTORE;
        BytecodeInstructions btype = stmt.mStatements.size() == 0 ? BytecodeInstructions::BLOAD : BytecodeInstructions::BSTORE;
        BytecodeInstructions otype = stmt.mStatements.size() == 0 ? BytecodeInstructions::OLOAD : BytecodeInstructions::OSTORE;

        if (stmt.mStatements.size() != 0) { // push value onto the stack if it's a set instruction
            compileStatement(stmt.mStatements[0], s);
            tryConvertValue(getStatementType(stmt.mStatements[0], s), t);
        }

        ++ci; // increase instruction count
        if (t == "signed int")
            { *out << (char)itype; writeInteger(getNumericalName(stmt.mName, s)); }
        else if (t == "long")
            { *out << (char)ltype; writeInteger(getNumericalName(stmt.mName, s)); }
        else if (t == "double")
            { *out << (char)dtype; writeInteger(getNumericalName(stmt.mName, s)); }
        else if (t == "char")
            { *out << (char)ctype; writeInteger(getNumericalName(stmt.mName, s)); }
        else if (t == "bool")
            { *out << (char)btype; writeInteger(getNumericalName(stmt.mName, s)); }
        else
            { *out << (char)otype; writeInteger(getNumericalName(stmt.mName, s)); }
    }

    void Compiler::compileArrayElement(Statement &stmt, Stack &s) {
        stmt.mStatements[1].mKind = StatementKind::VARIABLE_CALL;
        // compile those two statements
        compileStatement(stmt.mStatements[1], s);
        compileStatement(stmt.mStatements[0], s);
        // add method call
        addMethodCall("ElementCall");
    }

    // compiles litteral statement
    void Compiler::compileLitteral(Statement &stmt, Stack &s) {
        ++ci; // increase instruction count
        if (stmt.mType.mName == "signed int") {
            *out << (char)BytecodeInstructions::ICONST; writeInteger(stoi(stmt.mName));
        } else if (stmt.mType.mName == "long"){
            *out << (char)BytecodeInstructions::LCONST; writeLong(stoll(stmt.mName));
        } else if (stmt.mType.mName == "double"){
            *out << (char)BytecodeInstructions::DCONST; 
            writeDouble(stod(stmt.mName));
        } else if (stmt.mType.mName == "char") {
            *out << (char)BytecodeInstructions::CCONST << (char)stmt.mName[0];
        } else if (stmt.mType.mName == "bool") {
            *out << (char)BytecodeInstructions::BCONST; 
            *out << (stmt.mName == "1");
        }
    }

    // returns statement type
    string Compiler::getStatementType(Statement &stmt, Stack &s) {
        switch(stmt.mKind) {
            case StatementKind::LITERAL:
                return stmt.mType.mName;
                break;
            case StatementKind::NEW:
                return "class";
                break;
            case StatementKind::VARIABLE_CALL:
                if (stmt.mName == "true" || stmt.mName == "false") return "bool";
                if (s.varTypes.find(stmt.mName) == s.varTypes.end()) throw runtime_error("variable " + stmt.mName + " should be declared before call");
                return s.varTypes[stmt.mName];
                break;
            case StatementKind::OPERATOR_CALL:
                return getStatementType(stmt.mStatements[0], s);
            case StatementKind::LOGIC_CALL:
                return "bool";
            case StatementKind::FUNCTION_CALL:
                return "$$$";
            case StatementKind::CLASS_CALL:
                if (classMap.find(stmt.mName) != classMap.end()) return "class";
                if (s.varTypes.find(stmt.mName) == s.varTypes.end()) throw runtime_error("class variable " + stmt.mName + " should be declared before call");
                return s.varTypes[stmt.mName];
                break;
            case StatementKind::ARRAY_ELEMENT:
                return getStatementType(stmt.mStatements[1], s);
                break;
            case StatementKind::ARRAY_CALL:
                if (s.varTypes.find(stmt.mName) == s.varTypes.end()) throw runtime_error("variable " + stmt.mName + " should be declared before call");
                return s.varTypes[stmt.mName];
                break;
        }
        stmt.DebugPrint(0);
        throw runtime_error("cannot discover type for statement above");
    }

    // tries to convert variable on the top of the stack
    void Compiler::tryConvertValue(string from, string to) {
        if (from == to) return;
        if (from == "$$$" || to == "$$$") return;

        ++ci;
        if (from == "signed int" && to == "long") {*out << (char)BytecodeInstructions::I2L; }
        else if (from == "signed int" && to == "double") {*out << (char)BytecodeInstructions::I2D; }
        else if (from == "signed int" && to == "char") {*out << (char)BytecodeInstructions::I2C; }
        else if (from == "signed int" && to == "bool") {*out << (char)BytecodeInstructions::I2B; }

        else if (from == "long" && to == "signed int") {*out << (char)BytecodeInstructions::L2I; }
        else if (from == "long" && to == "double") {*out << (char)BytecodeInstructions::L2D; }
        else if (from == "long" && to == "char") {*out << (char)BytecodeInstructions::L2C; }
        else if (from == "long" && to == "bool") {*out << (char)BytecodeInstructions::L2B; }

        else if (from == "double" && to == "signed int") {*out << (char)BytecodeInstructions::D2I; }
        else if (from == "double" && to == "long") {*out << (char)BytecodeInstructions::D2L; }
        else if (from == "double" && to == "bool") {*out << (char)BytecodeInstructions::D2B; }
       
        else if (from == "char" && to == "signed int") {*out << (char)BytecodeInstructions::C2I; }
        else if (from == "char" && to == "long") {*out << (char)BytecodeInstructions::C2L; }
        else if (from == "class" && to == "func") { return; }

        else if (from.substr(0, 2) == "c$") {
            throw runtime_error("Class '" + getClassData(from).second + "' does not have user-defined conversion to type '" + to + "'");
        }
        else if (from != "signed int" && from != "long" && from != "double" && from != "char") return;
        else if (to != "signed int" && to != "long" && to != "double" && to != "char") return;
        else throw runtime_error("Cannot find conversion from '" + from + "' to '" + to + "'");
    }


    uint32_t Compiler::getNumericalName(string name, Stack &s) {
        if (s.numerical_names.find(name) == s.numerical_names.end()) {
            s.numerical_names[name] = nextNumericalName++;
        }
        return s.numerical_names[name];
    }

    void Compiler::getFunctionsMap(map<string, FunctionDefinition> &globalDefs, map<string, ClassDefinition> &cDefinitions) {
        staticFunctionsMap["$Main"] = new map<string, bool>();
        functionParamDefs["$Main"] = new map<string, vector<ParameterDefinition>*>();
        for (auto &p : globalDefs) {
            (*staticFunctionsMap["$Main"])[p.first] = std::find(p.second.mKeywords.begin(), p.second.mKeywords.end(), "static") != p.second.mKeywords.end();

            (*functionParamDefs["$Main"])[p.first] = &p.second.mParameters;
        }

        for (auto &p : cDefinitions) {
            staticFunctionsMap[p.first] = new map<string, bool>();
            functionParamDefs[p.first] = new map<string, vector<ParameterDefinition>*>();
            for (auto &fp : p.second.mFunctions) {
                (*staticFunctionsMap[p.first])[fp.mName] = std::find(fp.mKeywords.begin(), fp.mKeywords.end(), "static") != fp.mKeywords.end();

                (*functionParamDefs[p.first])[fp.mName] = &fp.mParameters;
            }

            // contructor
            if (p.second.mConDes.size() > 0) {
                bool isFirst = p.second.mConDes[0].mName == "constructor";
                if ((!isFirst && p.second.mConDes.size() == 2) || isFirst) {
                    FunctionDefinition &cd = p.second.mConDes[isFirst ? 0 : 1];
                    ParameterDefinition pd;
                    pd.mName = "this";
                    pd.mType.mName = "c$" + p.first;
                    cd.mParameters.insert(cd.mParameters.begin(), pd);
                    (*staticFunctionsMap[p.first])["$C"] = isFirst;
                    (*functionParamDefs[p.first])["$C"] = &p.second.mConDes[isFirst ? 0 : 1].mParameters;
                }
            }
            // destructor
            if (p.second.mConDes.size() > 0) {
                bool isFirst = p.second.mConDes[0].mName == "destructor";
                if ((!isFirst && p.second.mConDes.size() == 2) || isFirst) {
                    FunctionDefinition &cd = p.second.mConDes[isFirst ? 0 : 1];
                    ParameterDefinition pd;
                    pd.mName = "this";
                    pd.mType.mName = "c$" + p.first;
                    cd.mParameters.insert(cd.mParameters.begin(), pd);
                    (*staticFunctionsMap[p.first])["$D"] = isFirst;
                    if (p.second.mConDes[isFirst ? 0 : 1].mParameters.size() > 1) throw runtime_error("Class destructor doesn't accept any parameters");
                    (*functionParamDefs[p.first])["$D"] = &p.second.mConDes[isFirst ? 0 : 1].mParameters;
                }
            }
        }
    }

    pair<string, string> Compiler::getClassData(string c) {
        string prefix = c.substr(0, 2);
        if (prefix != "c$") throw runtime_error(c + " is not a class");
        return {prefix, c.substr(2)};
    }

    void Compiler::writeInteger(int n) {
        static_assert(sizeof(n) == 4, "Field n has to have size 4.");
        out->write(reinterpret_cast<const char *>(&n), sizeof(n));
    }
    void Compiler::writeLong(long long n) {
        static_assert(sizeof(n) == 8, "Field n has to have size 8.");
        out->write(reinterpret_cast<const char *>(&n), sizeof(n));
    }
    void Compiler::writeULong(unsigned long long n) {
        static_assert(sizeof(n) == 8, "Field n has to have size 8.");
        out->write(reinterpret_cast<const char *>(&n), sizeof(n));
    }
    void Compiler::writeDouble(double n) {
        static_assert(sizeof(n) == 8, "Field n has to have size 8.");
        out->write(reinterpret_cast<const char *>(&n), sizeof(n));
    }

    void Compiler::writeUTF8(string text) {
        writeInteger(text.size());
        *out << text;
    }
}