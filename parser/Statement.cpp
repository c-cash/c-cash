#include "Statement.hpp"
#include <iostream>

namespace parser {
    using namespace std;

    void Statement::DebugPrint(){
        cout << sStatementKindStrings[int(mKind)] << " ";
        cout << mType.mName << " " << mName << " (\n";
        for(Statement statement : mParameters){
            statement.DebugPrint();
        }
        cout << ")" << endl;
    }
}