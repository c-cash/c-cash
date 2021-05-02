#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

int main(){
    system("./../build/c-cash ../tests/ifTest.ccash > ../tests/~ifTest.txt");

    ifstream file("../tests/~ifTest.txt");

    string line, allCode="";
    while (std::getline(file, line)){
        allCode += line;
    }

    if(allCode == "abcefghi") return 0; 
    return 1;
}