#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

using namespace std;


int main(){
    #ifdef _WIN32
        system("c-cash ../tests/returnTest.ccash > ../tests/~retrunTest.txt");
    #else
        system("./../build/c-cash ../tests/returnTest.ccash > ../tests/~retrunTest.txt");
    #endif

    ifstream file("../tests/~retrunTest.txt");

    string line, allCode="";
    while (std::getline(file, line)){
        allCode += line;
    }

    if(allCode == "1") return 0; 
    return 1;
}