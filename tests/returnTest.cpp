#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

int main(){
    system("./../build/c-cash ../tests/returnTest.ccash > ../tests/~retrunTest.txt");

    ifstream file("../tests/~retrunTest.txt");

    string line, allCode="";
    while (std::getline(file, line)){
        allCode += line;
    }

    if(allCode == "1") return 0; 
    return 1;
}