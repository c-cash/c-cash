#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "parser/Tokenizer.hpp"
#include "parser/Parser.hpp"

using namespace std;
using namespace parser;

int main (int argc, char **argv) {
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
    parser.DebugPrint();

    return 0;
}