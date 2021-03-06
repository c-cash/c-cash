#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

using namespace std;

int main(){
    #ifdef _WIN32
        system("c-cash ../tests/loopTest.cash > ../tests/~loopTest.txt");
    #else
        system("./../build/c-cash ../tests/loopTest.cash > ../tests/~loopTest.txt");
    #endif

    ifstream file("../tests/~loopTest.txt");

    string line, allCode="";
    while (std::getline(file, line)){
        allCode += line;
    }

    if(allCode == "aaaaabbbbc012dddddd1098765432102468bbbbb") return 0;
    return 1;
}