#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

#include "parser/Tokenizer.hpp"
#include "parser/Parser.hpp"
#include "interpreter/Interpreter.hpp"
#include "other/ParseSaver.hpp"

using namespace std;
using namespace parser;
using namespace interpreter;
using namespace parsesaver;

int main (int argc, char **argv) {
    try{
        vector<string> args(argv, argv + argc);


        ifstream file(args[1]);
        string line, allCode="";
        while (getline(file, line)){
            allCode += line + '\n';
        }

    // cout << allCode << '\n';

        Tokenaizer tokenaizer;
        vector<Token> tokens = tokenaizer.parse(allCode);
    
    /*
        for(Token currToken : tokens){
            currToken.DebugPrint();
        }
    */

        Parser parser;
        parser.parse(tokens);
        // debug print
        vector<string>::iterator debugIterator = find(begin(args), end(args), "-D");
        if (debugIterator != end(args)) {
            // there is -D in argv
            parser.DebugPrint();
        }

        map<string, FunctionDefinition> functions = parser.mFunction;

        // saving
        vector<string>::iterator argIterator = find(begin(args), end(args), "-E");
        if (argIterator != end(args)) {
            // there is -E in argv
            int index = distance(begin(args), argIterator);
            if (args.size() < index+2) throw "You need to provide a path for -E";
            cout << "Saving to " << args[index+1] << '\n';

            ParseSaver saver;
            saver.save(args[index+1], functions);

        } else { // interpret
            Interpreter interpreter;
            interpreter.interpreter(functions);
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
