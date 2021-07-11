#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <chrono>

#include <sys/resource.h>

#include "src/parser/Tokenizer.hpp"
#include "src/parser/Parser.hpp"
#include "src/interpreter/Interpreter.hpp"
#include "src/other/ParseSaver.hpp"
#include "src/transpiler/Transpiler.hpp"
#include "src/compiler/Compiler.hpp"

using namespace std;
using namespace parser;
using namespace interpreter;
using namespace parsesaver;
using namespace transpiler;

long long getNanotime() {
    return (unsigned long long) std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

struct timetracker {
    unsigned long long temptime;
    unsigned long long tokenizationtime;
    unsigned long long parsetime;
    unsigned long long executiontime;
};

int main (int argc, char **argv) {
    if(argc == 0){throw runtime_error("arguments must be given, type -H to get help..."); return 1;}
    
    try{

        vector<string> args(argv, argv + argc);
        // run from code

        if(args[1].length() > 4) {
            if(!(args[1].substr(args[1].length() - 5) == ".cash") && !(args[1].substr(args[1].length() - 4) == ".ccc")) {
                throw runtime_error("The first argument should be .cash or .ccc file");
            }
        } else {
            vector<string>::iterator helpIterator = find(begin(args), end(args), "-H");
            if (helpIterator != end(args)) {
                ParseSaver saver;
                saver.writeHelp();
                return 0;
            }
            throw runtime_error("The first argument should be .cash or .ccc file");
        }

        ifstream file(args[1]);
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

        Parser parser;
        Tokenaizer tokenaizer;
        vector<Token> tokens;
        bool isDebug = find(begin(args), end(args), "-D") != end(args);

        struct timetracker* timer = isDebug ? new struct timetracker() : nullptr;
        
        if (isParsed) {
            ParseSaver saver;
            parser.mFunction = saver.load(args[1]);
        } else {
            if (isDebug) timer->temptime = getNanotime();
            tokens = tokenaizer.parse(allCode);
            if (isDebug) timer->tokenizationtime = getNanotime() - timer->temptime;
            if (isDebug) {
                // there is -D in argv
                for (Token &t : tokens) {
                    t.DebugPrint();
                }
            }

            if (isDebug) timer->temptime = getNanotime();
            parser.parse(tokens);
            if (isDebug) timer->parsetime = getNanotime() - timer->temptime;
        }

        // debug print
        vector<string>::iterator debugIterator = find(begin(args), end(args), "-D");
        if (debugIterator != end(args)) {
            // there is -D in argv
            parser.DebugPrint();
        }

        /*
        ================< TODO: REMOVE THIS CODE BELOW >================
        */
    //    compiler::Compiler c;
    //    ofstream cfile;
    //    cfile.open("test.ccbin", ofstream::binary);
    //    c.compile(parser.mFunction, cfile);
    //    cfile.close();
    //    return 0;


        vector<string>::iterator helpIterator = find(begin(args), end(args), "-H");
        if (helpIterator != end(args)) {
            ParseSaver saver;
            saver.writeHelp();
        }

        map<string, FunctionDefinition> functions = parser.mFunction;

        // saving
        vector<string>::iterator argIterator = find(begin(args), end(args), "-E");
        vector<string>::iterator transpileIterator = find(begin(args), end(args), "-T");
        if (argIterator != end(args)) {
            // there is -E in argv
            int index = distance(begin(args), argIterator);
            ParseSaver saver;
            if (args.size() < index+2) {
                saver.save("c.ccc", functions);
            } else {
                if(args[index+1][0] != '-') {
                    saver.save(args[index+1]+".ccc", functions);
                } else {
                    saver.save("c.ccc", functions);
                }
            }   
        } else { // interpret
            if (transpileIterator != end(args)) {
                // transpile
                int index = distance(begin(args), transpileIterator);
                Transpiler t;
                t.transpile(args[index+1]+".cpp", functions);
            } else {
                Interpreter::addDefaultBuiltins();
                if (isDebug) timer->temptime = getNanotime();
                Interpreter::interpret(functions);
                if (isDebug) timer->executiontime = getNanotime() - timer->temptime;
            }
        }

        // measure memory usage
        if(isDebug){
            int who = RUSAGE_SELF;
            struct rusage usage;
            getrusage(who, &usage);
            long memusage = usage.ru_maxrss;

            unsigned long long totaltime = timer->tokenizationtime + timer->parsetime + timer->executiontime;

            cout << "---< STATISTICS >---\n";
            cout << "TOKENIZATION TIME: " << timer->tokenizationtime/1000000.f << "ms (" << timer->tokenizationtime/1000000000.f << "s)\n";
            cout << "PARSE TIME: " << timer->parsetime/1000000.f << "ms (" << timer->parsetime/1000000000.f << "s)\n";
            cout << "EXECUTION TIME: " << timer->executiontime/1000000.f << "ms (" << timer->executiontime/1000000000.f << "s)\n";
            cout << "TOTAL TIME: " << totaltime/1000000.f << "ms (" << totaltime/1000000000.f << "s)\n";
            cout << "\nMEMORY USAGE: " << memusage << "Kb (" << memusage/1024.f << "Mb)\n";
        }
    } catch(exception& err) {
        cerr << "Error " << err.what() << endl; 
        return 2;
    } catch(...) {
        cerr << "Unknown Error." << endl;
        return 1;
    }

    return 0;
}
