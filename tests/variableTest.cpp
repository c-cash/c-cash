#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

using namespace std;


int main(){
    #ifdef _WIN32
        system("c-cash ../tests/variableTest.cash > ../tests/~variableTest.txt");
    #else
        system("./../build/c-cash ../tests/variableTest.cash > ../tests/~variableTest.txt");
    #endif

    ifstream file("../tests/~variableTest.txt");

    string line, allCode="";
    while (std::getline(file, line)){
        allCode += line;
    }

    if(allCode == "251.21410000005.2") return 0; 
    return 1;
}