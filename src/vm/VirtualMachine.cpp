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
        for (instruction* i : method->instructions) {
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
                    int a = getTop(long long, stack); stack.pop();
                    int b = getTop(long long, stack); stack.pop();
                    stack.push(a + b);
                    break;
                }
                // subtract two longs
                case BytecodeInstructions::LSUB: {
                    int a = getTop(long long, stack); stack.pop();
                    int b = getTop(long long, stack); stack.pop();
                    stack.push(a - b);
                    break;
                }
                // multiply two longs
                case BytecodeInstructions::LMUL: {
                    int a = getTop(long long, stack); stack.pop();
                    int b = getTop(long long, stack); stack.pop();
                    stack.push(a * b);
                    break;
                }
                // divide two longs
                case BytecodeInstructions::LDIV: {
                    int a = getTop(long long, stack); stack.pop();
                    int b = getTop(long long, stack); stack.pop();
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
                    int a = getTop(double, stack); stack.pop();
                    int b = getTop(double, stack); stack.pop();
                    stack.push(a + b);
                    break;
                }
                // subtract two doubles
                case BytecodeInstructions::DSUB: {
                    int a = getTop(double, stack); stack.pop();
                    int b = getTop(double, stack); stack.pop();
                    stack.push(a - b);
                    break;
                }
                // multiply two doubles
                case BytecodeInstructions::DMUL: {
                    int a = getTop(double, stack); stack.pop();
                    int b = getTop(double, stack); stack.pop();
                    stack.push(a * b);
                    break;
                }
                // divide two doubles
                case BytecodeInstructions::DDIV: {
                    int a = getTop(double, stack); stack.pop();
                    int b = getTop(double, stack); stack.pop();
                    stack.push(a / b);
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
                case BytecodeInstructions::L2I: {
                    int a = getTop(long long, stack);
                    stack.pop();
                    stack.push((long long) a);
                    break;
                }

                default: {
                    throw runtime_error("VM can't find instruction corresponding to the code " + (int)i->instruction);
                }
            }
        }
    }

    void VirtualMachine::exectuteProgram(map<string, Class*> &classes) {
        VMStack* s = new VMStack();
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
            }
            name += c;
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