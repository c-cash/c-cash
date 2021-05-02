#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

int main(){
    system("./../build/c-cash ../tests/loopTest.ccash > ../tests/~loopTest.txt");

    ifstream file("../tests/~loopTest.txt");

    string line, allCode="";
    while (std::getline(file, line)){
        allCode += line;
    }

    if(allCode == "aaaaabbbbc012dddddd") return 0; 
    return 1;
}