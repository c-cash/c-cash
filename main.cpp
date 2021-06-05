#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

#include "src/parser/Tokenizer.hpp"
#include "src/parser/Parser.hpp"
#include "src/interpreter/Interpreter.hpp"
#include "src/other/ParseSaver.hpp"
#include "src/transpiler/Transpiler.hpp"

using namespace std;
using namespace parser;
using namespace interpreter;
using namespace parsesaver;
using namespace transpiler;

int main (int argc, char **argv) {
    if(argc == 0){throw runtime_error("arguments must be given, type -H to get help..."); return 0;}

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
        bool isParsed = false;
        bool isFirst = true;
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
        
        if (isParsed) {
            ParseSaver saver;
            parser.mFunction = saver.load(args[1]);
        } else {
            tokens = tokenaizer.parse(allCode);

            //If debug mode
            vector<string>::iterator debugIterator = find(begin(args), end(args), "-D");
            if (debugIterator != end(args)) {
                // there is -D in argv
                for (Token &t : tokens) {
                    t.DebugPrint();
                }
            }

            parser.parse(tokens);
        }
        // debug print
        vector<string>::iterator debugIterator = find(begin(args), end(args), "-D");
        if (debugIterator != end(args)) {
            // there is -D in argv
            parser.DebugPrint();
        }

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
                Interpreter::interpret(functions);
            }
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
