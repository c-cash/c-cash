#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "parser/Tokenizer.hpp"
#include "parser/Parser.hpp"
#include "interpreter/Interpreter.hpp"

using namespace std;
using namespace parser;
using namespace interpreter;

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
    
        for(Token currToken : tokens){
            currToken.DebugPrint();
        }
    
        Parser parser;
        parser.parse(tokens);
        parser.DebugPrint();

        //map<string, FunctionDefinition> functions = parser.mFunction;
        
        //Interpreter interpreter;
        //interpreter.interpreter(functions);
    } catch(exception& err) {
        cerr << "Error: " << err.what() << endl;
        return 2;
    } catch(...) {
        cerr << "Unknow Error." << endl;
        return 1;
    }

    return 0;
}
