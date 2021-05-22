#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

#include "parser/Tokenizer.hpp"
#include "parser/Parser.hpp"
#include "interpreter/Interpreter.hpp"
#include "other/ParseSaver.hpp"
#include "transpiler/Transpiler.hpp"

using namespace std;
using namespace parser;
using namespace interpreter;
using namespace parsesaver;
using namespace transpiler;

int main (int argc, char **argv) {
    try{
        vector<string> args(argv, argv + argc);

        // run from code
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
        if (isParsed) {
            ParseSaver saver;
            parser.mFunction = saver.load(args[1]);
        } else {
            Tokenaizer tokenaizer;
            vector<Token> tokens = tokenaizer.parse(allCode);

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
                Interpreter interpreter;
                interpreter.interpreter(functions);
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
