#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

using namespace std;

int main(){   
    #ifdef _WIN32
        system("c-cash ../tests/ifTest.ccash > ../tests/~ifTest.txt");
    #else
        system("./../build/c-cash ../tests/ifTest.ccash > ../tests/~ifTest.txt");
    #endif


    ifstream file("../tests/~ifTest.txt");

    string line, allCode="";
    while (getline(file, line)){
        allCode += line;
    }

    if(allCode == "abcefghi") return 0; 
    return 1;
}