#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

int main(){
    system("./../build/c-cash ../tests/mathTest.ccash > ../tests/~mathTest.txt");

    ifstream file("../tests/~mathTest.txt");

    string line, allCode="";
    while (std::getline(file, line)){
        allCode += line;
    }

    if(allCode == "4041664") return 0; 
    return 1;
}