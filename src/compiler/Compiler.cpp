#include <iostream>
#include "Compiler.hpp"

namespace compiler {
    using namespace std;

    void Compiler::compile(map<string, FunctionDefinition> &mFunctions, ofstream &out) {
        this->out = &out;
        // TEMPORARY CODE
        Stack s;
        compileCode(mFunctions["main"].mStatements, s);
    }

    void Compiler::compileCode(vector<Statement> &statements, Stack &s) {
        for (Statement &stmt : statements) {
            compileStatement(stmt, s);
        }
    }
    void Compiler::compileStatement(Statement &stmt, Stack &s) {
        switch (stmt.mKind)
            {
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
        compileStatement(stmt.mStatements[0], s);
        // get right value
        compileStatement(stmt.mStatements[1], s);
        // compile operator operation
        auto t = s.pop2();
        
        if (/*[get type of the values] == "signed int"*/true) {
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
        }
    }

    void Compiler::compileVariableDeclaration(Statement &stmt, Stack &s) {
        // compile whatever value this have
        compileStatement(stmt.mStatements[0], s);

        // store this variable in the memory heap
        s.varTypes[stmt.mName] = stmt.mType.mName;
        if (stmt.mType.mName == "signed int") 
            {*out << (char)BytecodeInstructions::ISTORE; writeInteger(getNumericalName(stmt.mName)); s.values[stmt.mName] = s.pop(); }
    }
    
    // compiles variable call statement
    void Compiler::compileVariableCall(Statement &stmt, Stack &s) {
        if (s.varTypes.find(stmt.mName) == s.varTypes.end()) throw runtime_error("Variable " + stmt.mName + " should be defined before call");
        string t = s.varTypes[stmt.mName];
        // iload or istore based on what operation user wants to do
        BytecodeInstructions ctype = stmt.mStatements.size() == 0 ? BytecodeInstructions::ILOAD : BytecodeInstructions::ISTORE;

        if (stmt.mStatements.size() != 0) { // push value onto the stack if it's a set instruction
            compileStatement(stmt.mStatements[0], s);
            s.pop();
        }
        if (t == "signed int")
            { *out << (char)ctype; writeInteger(getNumericalName(stmt.mName)); s.elements.push(s.values[stmt.mName]); }
    }

    // compiles litteral statement
    void Compiler::compileLitteral(Statement &stmt, Stack &s) {
        if (stmt.mType.mName == "signed int")
            {*out << (char)BytecodeInstructions::ICONST; writeInteger(stoi(stmt.mName)); s.elements.push(stoi(stmt.mName));}
    }

    uint Compiler::getNumericalName(string name) {
        if (numerical_names.find(name) == numerical_names.end()) {
            numerical_names[name] = nextNumericalName++;
        }
        return numerical_names[name];
    }

    void Compiler::writeInteger(int n) {
        static_assert(sizeof(n) == 4, "Field i has to have size 4.");
        out->write(reinterpret_cast<const char *>(&n), sizeof(n));
    }
}