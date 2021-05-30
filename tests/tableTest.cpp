#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

using namespace std;

int main(){
    #ifdef _WIN32
        system("c-cash ../tests/tableTest.cash > ../tests/~tableTest.txt");
    #else
        system("./../build/c-cash ../tests/tableTest.cash > ../tests/~tableTest.txt");
    #endif

    ifstream file("../tests/~tableTest.txt");

    string line, allCode="";
    while (std::getline(file, line)){
        allCode += line;
    }

    if(allCode == "3465") return 0; 
    return 1;
}