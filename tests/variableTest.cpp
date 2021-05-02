#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

int main(){
    system("./../build/c-cash ../tests/variableTest.ccash > ../tests/~variableTest.txt");

    ifstream file("../tests/~variableTest.txt");

    string line, allCode="";
    while (std::getline(file, line)){
        allCode += line;
    }

    if(allCode == "251.2145.2") return 0; 
    return 1;
}