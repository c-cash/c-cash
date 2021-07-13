#pragma once

#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <stack>
#include <string>
#include <variant>

#include "../parser/Parser.hpp"
#include "../parser/FunctionDefinition.hpp"
#include "../parser/Statement.hpp"

typedef std::variant<int, long long, double, char, bool, void*, unsigned long long> dataType;

namespace compiler {
    using namespace std;
    using namespace parser;

    struct Stack {
        std::map<string, string> varTypes;
        map<string, uint32_t> numerical_names;
        stack<vector<string>*> scope;

        // for later
        stack<vector<streampos>*> breaks;

        //std::map<string, dataType> values;
    };

    class Compiler {
        public:
            void compile(map<string, FunctionDefinition> &mFunctions, ofstream &out);
            void compileFunction(FunctionDefinition &d);
            void compileCode(vector<Statement> &statements, Stack &s);
            
            void compileFunctionCall(Statement &stmt, Stack &s);
            void compileSpecialFunctionCall(Statement &stmt, Stack &s);
            void compileStatement(Statement &stmt, Stack &s);
            void compileVariableDeclaration(Statement &stmt, Stack &s);
            void compileLitteral(Statement &stmt, Stack &s);
            void compileMathOperation(Statement &stmt, Stack &s);
            void compileLogicCall(Statement &stmt, Stack &s);
            void compileVariableCall(Statement &stmt, Stack &s);
            void tryConvertValue(string from, string to);
            
            void popScope(Stack &s);

            string getStatementType(Statement &stmt, Stack &s);

            uint32_t getNumericalName(string name, Stack &s);
            void writeInteger(int n);
            void writeLong(long long n);
            void writeULong(unsigned long long n);
            void writeDouble(double n);

        private:
            static const char NOP {(char)0xff};
            static const char EOP {(char)0xfe};
            unsigned long long ci; // current instruction

            uint32_t nextNumericalName = 0;
            ofstream* out;
    };

    enum BytecodeInstructions {
        ICONST = (char) 0x0,
        ISTORE = (char) 0x1,
        IASTORE = (char) 0x2,
        ILOAD = (char) 0x3,
        IALOAD = (char) 0x4,
        IADD = (char) 0x5,
        ISUB = (char) 0x6,
        IMUL = (char) 0x7,
        IDIV = (char) 0x8,

        LCONST = (char) 0x9,
        LSTORE = (char) 0xA,
        LASTORE = (char) 0xB,
        LLOAD = (char) 0xC,
        LALOAD = (char) 0xD,
        LADD = (char) 0xE,
        LSUB = (char) 0xF,
        LMUL = (char) 0x10,
        LDIV = (char) 0x11,

        DCONST = (char) 0x12,
        DSTORE = (char) 0x13,
        DASTORE = (char) 0x14,
        DLOAD = (char) 0x15,
        DALOAD = (char) 0x16,
        DADD = (char) 0x17,
        DSUB = (char) 0x18,
        DMUL = (char) 0x19,
        DDIV = (char) 0x1A,

        CCONST = (char) 0x1B,
        CSTORE = (char) 0x1C,
        CASTORE = (char) 0x1D,
        CLOAD = (char) 0x1E,
        CALOAD = (char) 0x1F,
        CADD = (char) 0x20,
        CSUB = (char) 0x21,

        BCONST = (char) 0x22,
        BSTORE = (char) 0x23,
        BASTORE = (char) 0x24,
        BLOAD = (char) 0x25,
        BALOAD = (char) 0x26,

        ONULL = (char) 0x27,
        OCONST = (char) 0x28,
        CALLSTATIC = (char) 0x29,
        CALLMETHOD = (char) 0x2A,
        OVLOAD = (char) 0x2B,
        OVSTORE = (char) 0x2C,
        OSTORE = (char) 0x2D,
        OASTORE = (char) 0x2E,
        OLOAD = (char) 0x2F,
        OALOAD = (char) 0x30,

        GOTO = (char) 0x31,
        IFEQ = (char) 0x32,
        IFGE = (char) 0x33,
        IFGT = (char) 0x34,
        IFNE = (char) 0x35,
        IFNULL = (char) 0x36,
        IFNOTNULL = (char) 0x37,
        INSTANCEOF = (char) 0x38,
        THROW = (char) 0x39,

        RETURN = (char) 0x3A,
        IRETURN = (char) 0x3B,
        LRETURN = (char) 0x3C,
        DRETURN = (char) 0x3D,
        CRETURN = (char) 0x3E,
        BRETURN = (char) 0x3F,
        ORETURN = (char) 0x40,

        I2L = (char) 0x41,
        I2D = (char) 0x42,
        I2C = (char) 0x43,
        I2B = (char) 0x44,
        
        D2I = (char) 0x45,
        D2L = (char) 0x46,
        D2B = (char) 0x47,

        L2I = (char) 0x48,
        L2D = (char) 0x49,
        L2C = (char) 0x4A,
        L2B = (char) 0x4B,

        C2I = (char) 0x4C,
        C2L = (char) 0x4D,

        ARRCONST = (char) 0x4E,
        ARRSTORE = (char) 0x4F,
        ARRASTORE = (char) 0x50,
        ARRLOAD = (char) 0x51,
        ARRALOAD = (char) 0x52,

        JMPTRUE = (char) 0x53,
        JMPFALSE = (char) 0x54,
        NOT = (char) 0x55,

        DELETE = (char) 0x56,

        SWITCH = (char) 0x57,
        ISFLAG = (char) 0x58,

        null = (char) 0xFD,
    };
}