#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

using namespace std;


int main(){
    #ifdef _WIN32
        system("c-cash ../tests/mathTest.cash > ../tests/~mathTest.txt");
    #else
        system("./../build/c-cash ../tests/mathTest.cash > ../tests/~mathTest.txt");
    #endif

    ifstream file("../tests/~mathTest.txt");

    string line, allCode="";
    while (std::getline(file, line)){
        allCode += line;
    }

    if(allCode == "4041664868") return 0; 
    return 1;
}