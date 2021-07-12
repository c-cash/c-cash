#include <iostream>
#include "Compiler.hpp"

namespace compiler {
    using namespace std;

    void Compiler::compile(map<string, FunctionDefinition> &mFunctions, ofstream &out) {
        this->out = &out;
        // check if there is a main function
        if (mFunctions.find("main") == mFunctions.end()) throw runtime_error("Code must have a main function");
        // add $Main class to the file
        out << "$Main" << NOP;
        writeInteger(mFunctions.size());
        //Complie funcions
        for (auto &p : mFunctions) {
            out << (char) 0b0000'0001; // static function
            compileFunction(p.second);
        }

        out << EOP;
    }

    void Compiler::compileFunction(FunctionDefinition &d) {
        // save function name
        writeInteger(d.mName.size());
        *out << d.mName;
        // save parameters count
        writeInteger(d.mParameters.size());
        // Create new stack and compile this function
        Stack s;
        // TODO: add simulated parameters
        compileCode(d.mStatements, s);
        *out << NOP;
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
            default:
                break;
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
        
        // compile operator operation
        auto t = s.pop2();
        
        if (type == "signed int") {
            switch(stmt.mName[0]) {
                case '+':
                    *out << (char)BytecodeInstructions::IADD;
                    s.elements.push(t.first + t.second); // simulate this step
                    break;
                case '-':
                    *out << (char)BytecodeInstructions::ISUB;
                    s.elements.push(t.first - t.second); // simulate this step
                    break;
                case '*':
                    *out << (char)BytecodeInstructions::IMUL;
                    s.elements.push(t.first * t.second); // simulate this step
                    break;
                case '/':
                    *out << (char)BytecodeInstructions::IDIV;
                    s.elements.push(t.first / t.second); // simulate this step
                    break;
            }
        } else if (type == "long") {
            switch(stmt.mName[0]) {
                case '+':
                    *out << (char)BytecodeInstructions::LADD;
                    s.elements.push(t.first + t.second); // simulate this step
                    break;
                case '-':
                    *out << (char)BytecodeInstructions::LSUB;
                    s.elements.push(t.first - t.second); // simulate this step
                    break;
                case '*':
                    *out << (char)BytecodeInstructions::LMUL;
                    s.elements.push(t.first * t.second); // simulate this step
                    break;
                case '/':
                    *out << (char)BytecodeInstructions::LDIV;
                    s.elements.push(t.first / t.second); // simulate this step
                    break;
            }
        } else if (type == "double") {
            switch(stmt.mName[0]) {
                case '+':
                    *out << (char)BytecodeInstructions::DADD;
                    s.elements.push(t.first + t.second); // simulate this step
                    break;
                case '-':
                    *out << (char)BytecodeInstructions::DSUB;
                    s.elements.push(t.first - t.second); // simulate this step
                    break;
                case '*':
                    *out << (char)BytecodeInstructions::DMUL;
                    s.elements.push(t.first * t.second); // simulate this step
                    break;
                case '/':
                    *out << (char)BytecodeInstructions::DDIV;
                    s.elements.push(t.first / t.second); // simulate this step
                    break;
            }
        }
    }

    void Compiler::compileVariableDeclaration(Statement &stmt, Stack &s) {
        // compile whatever value this have
        compileStatement(stmt.mStatements[0], s);

        // store this variable in the memory heap
        s.varTypes[stmt.mName] = stmt.mType.mName;
        // ensure that values have the same type
        tryConvertValue(getStatementType(stmt.mStatements[0], s), stmt.mType.mName);
        if (stmt.mType.mName == "signed int") 
            {*out << (char)BytecodeInstructions::ISTORE; writeInteger(getNumericalName(stmt.mName, s)); s.values[stmt.mName] = s.pop(); }
        else if (stmt.mType.mName == "long") 
            {*out << (char)BytecodeInstructions::LSTORE; writeInteger(getNumericalName(stmt.mName, s)); s.values[stmt.mName] = s.pop(); }
        else if (stmt.mType.mName == "double") 
            {*out << (char)BytecodeInstructions::DSTORE; writeInteger(getNumericalName(stmt.mName, s)); s.values[stmt.mName] = s.pop(); }
    }
    
    // compiles variable call statement
    void Compiler::compileVariableCall(Statement &stmt, Stack &s) {
        if (s.varTypes.find(stmt.mName) == s.varTypes.end()) throw runtime_error("Variable " + stmt.mName + " should be defined before call");
        string t = s.varTypes[stmt.mName];
        // load or store based on what operation user wants to do
        BytecodeInstructions itype = stmt.mStatements.size() == 0 ? BytecodeInstructions::ILOAD : BytecodeInstructions::ISTORE;
        BytecodeInstructions ltype = stmt.mStatements.size() == 0 ? BytecodeInstructions::LLOAD : BytecodeInstructions::LSTORE;
        BytecodeInstructions dtype = stmt.mStatements.size() == 0 ? BytecodeInstructions::DLOAD : BytecodeInstructions::DSTORE;

        if (stmt.mStatements.size() != 0) { // push value onto the stack if it's a set instruction
            compileStatement(stmt.mStatements[0], s);
            s.pop();
            tryConvertValue(getStatementType(stmt.mStatements[0], s), t);
        }
        if (t == "signed int")
            { *out << (char)itype; writeInteger(getNumericalName(stmt.mName, s)); s.elements.push(s.values[stmt.mName]); }
        else if (t == "long")
            { *out << (char)ltype; writeInteger(getNumericalName(stmt.mName, s)); s.elements.push(s.values[stmt.mName]); }
        else if (t == "double")
            { *out << (char)dtype; writeInteger(getNumericalName(stmt.mName, s)); s.elements.push(s.values[stmt.mName]); }
    }

    // compiles litteral statement
    void Compiler::compileLitteral(Statement &stmt, Stack &s) {
        if (stmt.mType.mName == "signed int") {
            *out << (char)BytecodeInstructions::ICONST; writeInteger(stoi(stmt.mName)); s.elements.push(stoi(stmt.mName));
        } else if (stmt.mType.mName == "long"){
            *out << (char)BytecodeInstructions::LCONST; writeLong(stoll(stmt.mName)); s.elements.push(stoll(stmt.mName));
        } else if (stmt.mType.mName == "double"){
            *out << (char)BytecodeInstructions::DCONST; 
            writeDouble(stod(stmt.mName));
            double d = stod(stmt.mName);
            void* tmp = (void*)&d;
            s.elements.push(*(long long*)tmp);
        }
    }

    // returns statement type
    string Compiler::getStatementType(Statement &stmt, Stack &s) {
        switch(stmt.mKind) {
            case StatementKind::LITERAL:
                return stmt.mType.mName;
                break;
            case StatementKind::VARIABLE_CALL:
                if (s.varTypes.find(stmt.mName) == s.varTypes.end()) throw runtime_error("variable " + stmt.mName + " should be declared before call");
                return s.varTypes[stmt.mName];
                break;
            case StatementKind::OPERATOR_CALL:
                return getStatementType(stmt.mStatements[0], s);
        }
        stmt.DebugPrint(0);
        throw runtime_error("cannot discover type for statement above");
    }

    // tries to convert variable on the top of the stack
    void Compiler::tryConvertValue(string from, string to) {
        if (from == to) return;
        
        if (from == "signed int" && to == "long") {*out << (char)BytecodeInstructions::I2L;}
        if (from == "long" && to == "signed int") {*out << (char)BytecodeInstructions::L2I;}
        if (from == "signed int" && to == "double") {*out << (char)BytecodeInstructions::I2D;}
        // TODO: write conversions
    }


    uint32_t Compiler::getNumericalName(string name, Stack &s) {
        if (numerical_names.find(name) == numerical_names.end()) {
            numerical_names[name] = nextNumericalName++;
        }
        return numerical_names[name];
    }

    void Compiler::writeInteger(int n) {
        static_assert(sizeof(n) == 4, "Field n has to have size 4.");
        out->write(reinterpret_cast<const char *>(&n), sizeof(n));
    }
    void Compiler::writeLong(long long n) {
        static_assert(sizeof(n) == 8, "Field n has to have size 8.");
        out->write(reinterpret_cast<const char *>(&n), sizeof(n));
    }
    void Compiler::writeDouble(double n) {
        static_assert(sizeof(n) == 8, "Field n has to have size 8.");
        out->write(reinterpret_cast<const char *>(&n), sizeof(n));
    }
}