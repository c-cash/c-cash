#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <chrono>

#include "src/parser/Tokenizer.hpp"
#include "src/parser/Parser.hpp"
#include "src/transpiler/Transpiler.hpp"
#include "src/compiler/Compiler.hpp"
#include "src/vm/VirtualMachine.hpp"
#include "src/analysis/Analysis.hpp"
#include "src/linker/Linker.hpp"
#include "src/optimizer/Optimizer.hpp"

using namespace std;
using namespace parser;
using namespace transpiler;
using namespace compiler;
using namespace linker;
using namespace optimalizer;
using namespace analysis;

#ifdef __unix__  
    #include <sys/resource.h>

    long long getNanotime() {
        return (unsigned long long) std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    struct timetracker {
        unsigned long long temptime;
        unsigned long long tokenizationtime;
        unsigned long long parsetime;
    };
#endif

void writeHelp() {
    cout << "-H --> shows this\n";
    cout << "-D --> shows a tree of program\n";
    cout << "-VM--> execute tge porgram - you mast write name of file!";
    cout << "-T --> transpils .cash file to cpp\n";
    cout << "You can see documentation on: http://ccash.info\n";
}

int main (int argc, char **argv) {
    if(argc == 0){ throw runtime_error("arguments must be given, type -H to get help..."); return 1; }
    
    try{
        //Args
        vector<string> args(argv, argv + argc);

        //Chek (.cash or .ccc) and if someone write -H
        if(args[1].length() > 4) {
            if(!(args[1].substr(args[1].length() - 5) == ".cash") && !(args[1].substr(args[1].length() - 6) == ".ccbin")) {
                throw runtime_error("The first argument should be .cash or .ccbin file");
            }
        } else {
            if (find(cbegin(args), cend(args), "-H") != cend(args)) {
                writeHelp();
                return 0;
            }
            throw runtime_error("The first argument should be .cash or .ccbin file");
        }

        //Check if VM and if it's run code
        ifstream file;
        if (find(begin(args), end(args), "-VM") != end(args)) {
            file.open(args[1], std::fstream::binary);
            vm::VirtualMachine machine;
            map<string, vm::Class*> classes = machine.loadFromFile(file);
            machine.exectuteProgram(classes);
            return 0;
        }  
        
        //If normal cmd open file 
        file.open(args[1]);

        //Save line
        string line, allCode="";
        while (getline(file, line)){
            allCode += line + '\n';
        }

        Parser parser;
        Tokenaizer tokenaizer;
        vector<Token> tokens;

        //Debug
        bool isDebug = find(begin(args), end(args), "-D") != end(args);
        #ifdef __unix__ 
            struct timetracker* timer = isDebug ? new struct timetracker() : nullptr;
        #endif
        if (isDebug) {
            #ifdef __unix__ 
                timer->temptime = getNanotime();
            #endif

            //Get tokens
            tokens = tokenaizer.parse(allCode);

            #ifdef __unix__ 
                timer->tokenizationtime = getNanotime() - timer->temptime;
            #endif

            for (Token &t : tokens) {
                t.DebugPrint();
            }
        } else {
            //Get tokens
            tokens = tokenaizer.parse(allCode);
        }

        //Parse tokens
        #ifdef __unix__ 
            if (isDebug) timer->temptime = getNanotime();
        #endif
        parser.parse(tokens);
        #ifdef __unix__ 
            if (isDebug) timer->parsetime = getNanotime() - timer->temptime;
        #endif

        //Debug print
        

        //Link, anlalysis and optimaze
        Linker l;
        l.link(parser.mFunction, parser.mClass);
        Analysis a;
        a.analize(parser.mFunction, parser.mClass);
        Optimalizer o;
        o.optimize(parser.mFunction, parser.mClass);

        if (isDebug) { parser.DebugPrint(); }

        //Compile code
        Compiler c;
        ofstream cfile;
        cfile.open("test.ccbin", ofstream::binary);
        c.compile(parser.mFunction, cfile, parser.mClass);
        cfile.close();

        // measure memory usage
        #ifdef __unix__ 
            if(isDebug){
                int who = RUSAGE_SELF;
                struct rusage usage;
                getrusage(who, &usage);
                long memusage = usage.ru_maxrss;

                unsigned long long totaltime = timer->tokenizationtime + timer->parsetime;

                cout << "---< STATISTICS >---\n";
                cout << "TOKENIZATION TIME: " << timer->tokenizationtime/1000000.f << "ms (" << timer->tokenizationtime/1000000000.f << "s)\n";
                cout << "PARSE TIME: " << timer->parsetime/1000000.f << "ms (" << timer->parsetime/1000000000.f << "s)\n";
                cout << "TOTAL TIME: " << totaltime/1000000.f << "ms (" << totaltime/1000000000.f << "s)\n";
                cout << "\nMEMORY USAGE: " << memusage << "Kb (" << memusage/1024.f << "Mb)\n";
            }
        #endif
    } catch(exception& err) {
        cerr << "Error " << err.what() << endl; 
        return 2;
    } catch(...) {
        cerr << "Unknown Error." << endl;
        return 3;
    }

    return 0;
}
