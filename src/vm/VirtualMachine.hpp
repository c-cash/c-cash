#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <variant>
#include <stack>
#include "../compiler/Compiler.hpp"

typedef std::variant<int, long long, double, char, bool, void*, unsigned long long> dataType;
typedef std::variant<int, long long, double, char, bool, void*, unsigned long long, std::string> iDataType;
typedef std::stack<dataType> VMStack;

namespace vm {
    using namespace std;
    using namespace compiler;

    struct instruction {
        BytecodeInstructions instruction;
        vector<iDataType> data;
    };
    struct Method {
        char flags;
        int pcount;
        vector<instruction*> instructions;
    };
    struct Class {
        map<string, Method*> methods;
    };

    class VirtualMachine {
        public:

            void exectuteProgram(map<string, Class*>& classes);
            void executeMethod(Method* method, VMStack &stack);
            
            void printStackAndHeap(VMStack &stack);
            void printData(dataType &d);

            map<string, Class*> loadFromFile(ifstream &in);

            string readUTF8(ifstream &in);
            char readByte(ifstream &in);

        private:
            static const char NOP {(char)0xff};
            static const char EOP {(char)0xfe};

            map<int32_t, dataType> heap;
            map<string, Class*> classMap;

            instruction* loadInstruction(char &it, ifstream &in);
    };
}