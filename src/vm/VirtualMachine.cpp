#include "VirtualMachine.hpp"

#include <iostream>

#define getData(type, i, index) std::get<type>(i->data[index])
#define getTop(type, stack) std::get<type>(stack.top());

#include <iomanip>
#define HEX( x ) setw(2) << setfill('0') << hex << (int)( x )

namespace vm {
    using namespace std;

    void VirtualMachine::executeMethod(Method* method, VMStack &stack) {
        if (stack.size() != method->pcount) 
            throw runtime_error("bytecode is corrupted or has been modified in a wrong way, please don't touch it again!");
        size_t i_count = method->instructions.size();
        for (unsigned long long n{0}; n<i_count; ++n/*instruction* i : method->instructions*/) {
            instruction* i = method->instructions[n];
            switch(i->instruction) {
                // ===< INTEGER >===
                // create new int on a stack
                case BytecodeInstructions::ICONST: {
                    stack.push(getData(int, i, 0));
                    break;
                }
                // store an int in a memory heap
                case BytecodeInstructions::ISTORE: {
                    heap[getData(int, i, 0)] = stack.top();
                    stack.pop();
                    break;
                }
                // store an int in an array
                case BytecodeInstructions::IASTORE: {
                    // TODO: implement
                    break;
                }
                // load value from the memory heap
                case BytecodeInstructions::ILOAD: {
                    stack.push(heap[getData(int, i, 0)]);
                    break;
                }
                // load an int from an array
                case BytecodeInstructions::IALOAD: {
                    // TODO: implement
                    break;
                }
                // add two integers
                case BytecodeInstructions::IADD: {
                    int a = getTop(int, stack); stack.pop();
                    int b = getTop(int, stack); stack.pop();
                    stack.push(a + b);
                    break;
                }
                // subtract two integers
                case BytecodeInstructions::ISUB: {
                    int a = getTop(int, stack); stack.pop();
                    int b = getTop(int, stack); stack.pop();
                    stack.push(a - b);
                    break;
                }
                // multiply two integers
                case BytecodeInstructions::IMUL: {
                    int a = getTop(int, stack); stack.pop();
                    int b = getTop(int, stack); stack.pop();
                    stack.push(a * b);
                    break;
                }
                // divide two integers
                case BytecodeInstructions::IDIV: {
                    int a = getTop(int, stack); stack.pop();
                    int b = getTop(int, stack); stack.pop();
                    stack.push(a / b);
                    break;
                }


                // ===< LONG >===
                // create new long on a stack
                case BytecodeInstructions::LCONST: {
                    stack.push(getData(long long, i, 0));
                    break;
                }
                // store a long in a memory heap
                case BytecodeInstructions::LSTORE: {
                    heap[getData(int, i, 0)] = stack.top();
                    stack.pop();
                    break;
                }
                // store an long in an array
                case BytecodeInstructions::LASTORE: {
                    // TODO: implement
                    break;
                }
                // load value from the memory heap
                case BytecodeInstructions::LLOAD: {
                    stack.push(heap[getData(int, i, 0)]);
                    break;
                }
                // load a long from an array
                case BytecodeInstructions::LALOAD: {
                    // TODO: implement
                    break;
                }
                // add two longs
                case BytecodeInstructions::LADD: {
                    long long a = getTop(long long, stack); stack.pop();
                    long long b = getTop(long long, stack); stack.pop();
                    stack.push(a + b);
                    break;
                }
                // subtract two longs
                case BytecodeInstructions::LSUB: {
                    long long a = getTop(long long, stack); stack.pop();
                    long long b = getTop(long long, stack); stack.pop();
                    stack.push(a - b);
                    break;
                }
                // multiply two longs
                case BytecodeInstructions::LMUL: {
                    long long a = getTop(long long, stack); stack.pop();
                    long long b = getTop(long long, stack); stack.pop();
                    stack.push(a * b);
                    break;
                }
                // divide two longs
                case BytecodeInstructions::LDIV: {
                    long long a = getTop(long long, stack); stack.pop();
                    long long b = getTop(long long, stack); stack.pop();
                    stack.push(a / b);
                    break;
                }


                // ===< DOUBLE >===
                // create new double on a stack
                case BytecodeInstructions::DCONST: {
                    stack.push(getData(double, i, 0));
                    break;
                }
                // store a double in a memory heap
                case BytecodeInstructions::DSTORE: {
                    heap[getData(int, i, 0)] = stack.top();
                    stack.pop();
                    break;
                }
                // store a double in an array
                case BytecodeInstructions::DASTORE: {
                    // TODO: implement
                    break;
                }
                // load value from the memory heap
                case BytecodeInstructions::DLOAD: {
                    stack.push(heap[getData(int, i, 0)]);
                    break;
                }
                // load an int from an array
                case BytecodeInstructions::DALOAD: {
                    // TODO: implement
                    break;
                }
                // add two doubles
                case BytecodeInstructions::DADD: {
                    double a = getTop(double, stack); stack.pop();
                    double b = getTop(double, stack); stack.pop();
                    stack.push(a + b);
                    break;
                }
                // subtract two doubles
                case BytecodeInstructions::DSUB: {
                    double a = getTop(double, stack); stack.pop();
                    double b = getTop(double, stack); stack.pop();
                    stack.push(a - b);
                    break;
                }
                // multiply two doubles
                case BytecodeInstructions::DMUL: {
                    double a = getTop(double, stack); stack.pop();
                    double b = getTop(double, stack); stack.pop();
                    stack.push(a * b);
                    break;
                }
                // divide two doubles
                case BytecodeInstructions::DDIV: {
                    double a = getTop(double, stack); stack.pop();
                    double b = getTop(double, stack); stack.pop();
                    stack.push(a / b);
                    break;
                }


                // ===< CHAR >===
                // create new double on a stack
                case BytecodeInstructions::CCONST: {
                    stack.push(getData(char, i, 0));
                    break;
                }
                // store a double in a memory heap
                case BytecodeInstructions::CSTORE: {
                    heap[getData(int, i, 0)] = stack.top();
                    stack.pop();
                    break;
                }
                // store a double in an array
                case BytecodeInstructions::CASTORE: {
                    // TODO: implement
                    break;
                }
                // load value from the memory heap
                case BytecodeInstructions::CLOAD: {
                    stack.push(heap[getData(char, i, 0)]);
                    break;
                }
                // load an int from an array
                case BytecodeInstructions::CALOAD: {
                    // TODO: implement
                    break;
                }
                // add two doubles
                case BytecodeInstructions::CADD: {
                    char a = getTop(char, stack); stack.pop();
                    char b = getTop(char, stack); stack.pop();
                    stack.push(a + b);
                    break;
                }
                // subtract two doubles
                case BytecodeInstructions::CSUB: {
                    char a = getTop(char, stack); stack.pop();
                    int b = getTop(char, stack); stack.pop();
                    stack.push(a - b);
                    break;
                }


                // 0011 0000 - false
                // 0011 0001 - true
                // ===< BOOL >===
                // create new double on a stack
                case BytecodeInstructions::BCONST: {
                    stack.push(getData(bool, i, 0));
                    break;
                }
                // store a double in a memory heap
                case BytecodeInstructions::BSTORE: {
                    heap[getData(int, i, 0)] = stack.top();
                    stack.pop();
                    break;
                }
                // store a double in an array
                case BytecodeInstructions::BASTORE: {
                    // TODO: implement
                    break;
                }
                // load value from the memory heap
                case BytecodeInstructions::BLOAD: {
                    stack.push(heap[getData(bool, i, 0)]);
                    break;
                }
                // load an int from an array
                case BytecodeInstructions::BALOAD: {
                    // TODO: implement
                    break;
                }

                // ===< IFS >===
                case BytecodeInstructions::IFEQ: {
                    dataType a = stack.top(); stack.pop();
                    dataType b = stack.top(); stack.pop();
                    stack.push(a == b);
                    break;
                }
                case BytecodeInstructions::IFGE: {
                    dataType a = stack.top(); stack.pop();
                    dataType b = stack.top(); stack.pop();
                    stack.push(a >= b);
                    break;
                }
                case BytecodeInstructions::IFGT: {
                    dataType a = stack.top(); stack.pop();
                    dataType b = stack.top(); stack.pop();
                    stack.push(a > b);
                    break;
                }
                case BytecodeInstructions::IFNE: {
                    dataType a = stack.top(); stack.pop();
                    dataType b = stack.top(); stack.pop();
                    stack.push(a != b);
                    break;
                }

                // ===< JUMPS >===
                case BytecodeInstructions::GOTO: {
                    n = getData(unsigned long long, i, 0) - 1;
                    break;
                }
                case BytecodeInstructions::JMPFALSE: {
                    bool top = getTop(bool, stack);
                    if (!top) {
                        n = getData(unsigned long long, i, 0) - 1;
                    }
                    stack.pop();
                    break;
                }
                case BytecodeInstructions::JMPTRUE: {
                    bool top = getTop(bool, stack);
                    if (top) {
                        n = getData(unsigned long long, i, 0) - 1;
                    }
                    stack.pop();
                    break;
                }
                // ===< CONVERSIONS >===
                case BytecodeInstructions::I2L: {
                    int a = getTop(int, stack);
                    stack.pop();
                    stack.push((long long) a);
                    break;
                }
                case BytecodeInstructions::I2D: {
                    int a = getTop(int, stack);
                    stack.pop();
                    stack.push((double) a);
                    break;
                }
                case BytecodeInstructions::I2C: {
                    int a = getTop(int, stack);
                    stack.pop();
                    stack.push((char) a);
                    break;
                }
                case BytecodeInstructions::I2B: {
                    int a = getTop(int, stack);
                    stack.pop();
                    stack.push(a > 0);
                    break;
                }

                case BytecodeInstructions::D2I: {
                    double a = getTop(double, stack);
                    stack.pop();
                    stack.push((int) a);
                    break;
                }
                case BytecodeInstructions::D2L: {
                    double a = getTop(double, stack);
                    stack.pop();
                    stack.push((long long) a);
                    break;
                }
                case BytecodeInstructions::D2B: {
                    double a = getTop(int, stack);
                    stack.pop();
                    stack.push(a > 0.0);
                    break;
                }

                case BytecodeInstructions::L2I: {
                    long long a = getTop(long long, stack);
                    stack.pop();
                    stack.push((int) a);
                    break;
                }
                case BytecodeInstructions::L2D: {
                    long long a = getTop(long long, stack);
                    stack.pop();
                    stack.push((double) a);
                    break;
                }
                case BytecodeInstructions::L2C: {
                    long long a = getTop(long long, stack);
                    stack.pop();
                    stack.push((char) a);
                    break;
                }
                case BytecodeInstructions::L2B: {
                    long long a = getTop(long long, stack);
                    stack.pop();
                    stack.push(a > 0LL);
                    break;
                }

                case BytecodeInstructions::C2I: {
                    char a = getTop(char, stack);
                    stack.pop();
                    stack.push((int) a);
                    break;
                }
                case BytecodeInstructions::C2L: {
                    char a = getTop(char, stack);
                    stack.pop();
                    stack.push((long long) a);
                    break;
                }

                case BytecodeInstructions::DELETE: {
                    heap.erase(getData(int, i, 0));
                    break;
                }

                case BytecodeInstructions::ANYSTORE: {
                    heap[getData(int, i, 0)] = stack.top();
                    stack.pop();
                    break;
                }

                // ===< OBJECTS >===
                case BytecodeInstructions::OCONST: {
                    Class* cls = classMap[getData(string, i, 0)];
                    Method* mt = cls->methods["$C"];
                    VMStack* ns = new VMStack();
                    void* t = executeConstructor(mt, *ns, cls);
                    for (int i{0}; i<mt->pcount-1; ++i) // add all arguments to the beggining stack
                        { ns->push(stack.top()); stack.pop(); }
                    executeMethod(mt, *ns);
                    stack.push(t);
                    break;
                }
                case BytecodeInstructions::OSTORE: {
                    heap[getData(int, i, 0)] = stack.top();
                    stack.pop();
                    break;
                }

                // ===< CALLS >===
                case BytecodeInstructions::CALLSTATIC: {
                    Class* cls = classMap[getData(string, i, 0)];
                    Method* mt = cls->methods[getData(string, i, 1)];
                    VMStack* ns = new VMStack();
                    for (int i{0}; i<mt->pcount; ++i) // add all arguments to the beggining stack
                        { ns->push(stack.top()); stack.pop(); }
                    executeMethod(mt, *ns);
                    stack.push(ns->top()); // put return value onto the stack
                    delete ns;
                    break;
                }

                // ===< RETURNS >===
                case BytecodeInstructions::RETURN: {
                    return; // return :D
                    break;
                }
                
                case BytecodeInstructions::null: {
                    printStackAndHeap(stack);
                    break;
                }

                default: {
                    throw runtime_error("VM can't find instruction corresponding to the code " + (int)i->instruction);
                    break;
                }
            }
        }
    }

    void* VirtualMachine::executeConstructor(Method* method, VMStack &stack, Class* cls) {
        void* c = (void*)new VMClass(cls->name);
        stack.push(c);
        return c;
    }

    void VirtualMachine::exectuteProgram(map<string, Class*> &classes) {
        VMStack* s = new VMStack();
        classMap = classes;
        executeMethod(classes["$Main"]->methods["main"], *s);
    }

    template <typename T>
    T readNext(istream &in) {
        T t;
        in.read((char*)&t, sizeof(t));
        return t;
    }

    map<string, Class*> VirtualMachine::loadFromFile(ifstream &in) {
        map<string, Class*> classes;
        char c; string name;
        c = readByte(in);
        while(c != EOP) {
            if (c == NOP) {
                Class* cls = new Class();
                classes[name] = cls;
                cls->name = name;
                name = ""; // reset class name for the future class

                // get methods count and iterate over all of them 
                //int32_t mcount = readInt(in);
                int mcount = readNext<int>(in);
                string mname;
                for (int i{0}; i<mcount; ++i) {
                    // get method flags
                    char flags = readByte(in);
                    // get function name
                    mname = readUTF8(in);
                    Method* m = new Method();
                    m->flags = flags;
                    m->pcount = readNext<int>(in);
                    //Save func
                    cls->methods[mname] = m;

                    // load instructions
                    char instruction = readByte(in);
                    while (instruction != NOP) {
                        m->instructions.emplace_back(loadInstruction(instruction, in));
                        instruction = readByte(in);
                    }
                }
            } else {
                name += c;
            }
            c = readByte(in);
        }
        return classes;
    }

    instruction* VirtualMachine::loadInstruction(char &it, ifstream &in) {
        instruction* i = new instruction();
        switch(it) {
            case BytecodeInstructions::ICONST:
                i->instruction = BytecodeInstructions::ICONST;
                i->data.emplace_back(readNext<int>(in));
                break;
            case BytecodeInstructions::ISTORE:
                i->instruction = BytecodeInstructions::ISTORE;
                i->data.emplace_back(readNext<int>(in));
                break;
            case BytecodeInstructions::ILOAD:
                i->instruction = BytecodeInstructions::ILOAD;
                i->data.emplace_back(readNext<int>(in));
                break;

            case BytecodeInstructions::LCONST:
                i->instruction = BytecodeInstructions::LCONST;
                i->data.emplace_back(readNext<long long>(in));
                break;
            case BytecodeInstructions::LSTORE:
                i->instruction = BytecodeInstructions::LSTORE;
                i->data.emplace_back(readNext<int>(in));
                break;
            case BytecodeInstructions::LLOAD:
                i->instruction = BytecodeInstructions::LLOAD;
                i->data.emplace_back(readNext<int>(in));
                break;

            case BytecodeInstructions::DCONST:
                i->instruction = BytecodeInstructions::DCONST;
                i->data.emplace_back(readNext<double>(in));
                break;
            case BytecodeInstructions::DSTORE:
                i->instruction = BytecodeInstructions::DSTORE;
                i->data.emplace_back(readNext<int>(in));
                break;
            case BytecodeInstructions::DLOAD:
                i->instruction = BytecodeInstructions::DLOAD;
                i->data.emplace_back(readNext<int>(in));
                break;

            case BytecodeInstructions::CCONST:
                i->instruction = BytecodeInstructions::CCONST;
                i->data.emplace_back(readNext<char>(in));
                break;
            case BytecodeInstructions::CSTORE:
                i->instruction = BytecodeInstructions::CSTORE;
                i->data.emplace_back(readNext<int>(in));
                break;
            case BytecodeInstructions::CLOAD:
                i->instruction = BytecodeInstructions::CLOAD;
                i->data.emplace_back(readNext<int>(in));
                break;
                
            case BytecodeInstructions::BCONST:
                i->instruction = BytecodeInstructions::BCONST;
                i->data.emplace_back(readNext<bool>(in));
                break;
            case BytecodeInstructions::BSTORE:
                i->instruction = BytecodeInstructions::BSTORE;
                i->data.emplace_back(readNext<int>(in));
                break;
            case BytecodeInstructions::BLOAD:
                i->instruction = BytecodeInstructions::BLOAD;
                i->data.emplace_back(readNext<int>(in));
                break;


            case BytecodeInstructions::GOTO:
                i->instruction = BytecodeInstructions::GOTO;
                i->data.emplace_back(readNext<unsigned long long>(in));
                break;
            case BytecodeInstructions::JMPTRUE:
                i->instruction = BytecodeInstructions::JMPTRUE;
                i->data.emplace_back(readNext<unsigned long long>(in));
                break;
            case BytecodeInstructions::JMPFALSE:
                i->instruction = BytecodeInstructions::JMPFALSE;
                i->data.emplace_back(readNext<unsigned long long>(in));
                break;

            case BytecodeInstructions::ANYSTORE:
                i->instruction = BytecodeInstructions::BSTORE;
                i->data.emplace_back(readNext<int>(in));
                break;
            case BytecodeInstructions::CALLSTATIC:
                i->instruction = BytecodeInstructions::CALLSTATIC;
                i->data.emplace_back(readUTF8(in));
                i->data.emplace_back(readUTF8(in));
                break;

            case BytecodeInstructions::OCONST:
                i->instruction = BytecodeInstructions::OCONST;
                i->data.emplace_back(readUTF8(in));
                break;
            case BytecodeInstructions::OSTORE:
                i->instruction = BytecodeInstructions::OSTORE;
                i->data.emplace_back(readNext<int>(in));
                break;
            case BytecodeInstructions::OLOAD:
                i->instruction = BytecodeInstructions::OLOAD;
                i->data.emplace_back(readNext<int>(in));
                break;

            default:
                i->instruction = (BytecodeInstructions)it;
                break;
        }
        return i;
    }

    /*
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
    */

    void VirtualMachine::printStackAndHeap(VMStack &stack) {
        cout << "STACK " << stack.size() << ":\n";
        while (!stack.empty()) {
            dataType &d = stack.top(); stack.pop();
            // int, long long, double, char, bool, void*, unsigned long long
            cout << "| ";
            printData(d);
        }
        cout << "HEAP " << heap.size() << ":\n";
        for (auto &p : heap) {
            cout << to_string(p.first) << " | ";
            printData(p.second);
        }
    }

    void VirtualMachine::printData(dataType &d) {
        switch(d.index()) {
                case 0:
                    cout << std::get<int>(d) << "\n";
                    break;
                case 1:
                    cout << std::get<long long>(d) << "\n";
                    break;
                case 2:
                    cout << std::get<double>(d) << "\n";
                    break;
                case 3:
                    cout << std::get<char>(d) << "\n";
                    break;
                case 4:
                    cout << std::get<bool>(d) << "\n";
                    break;
                case 5:
                    printf("%d", *(int*)std::get<void*>(d)); cout << "\n";
                    break;
                case 6:
                    cout << std::get<unsigned long long>(d) << "\n";
                    break;
        }
    }

    string VirtualMachine::readUTF8(ifstream &in) {
        int l = readNext<int>(in);
        string r; char c;
        for (int i{0}; i<l; ++i) {
            in.get(c);
            r += c;
        }
        return r;
    }

    char VirtualMachine::readByte(ifstream &in) {
        char c;
        in.read((&c), 1);
        return c;
    }
}