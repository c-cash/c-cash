#include "Statement.hpp"
#include <iostream>

namespace parser {
    using namespace std;

    void Statement::DebugPrint(size_t indent){
        cout << string(indent, '\t') << "\u001B[36m" << sStatementKindStrings[int(mKind)] << " ";
        cout << "\u001B[33m" << mType.mName << " " << "\u001B[32m" << mName << "\u001B[0m" << " (\n";
        for(Statement statement : mStatements){
            statement.DebugPrint(indent + 1);
        }
        cout << string(indent, '\t') << ")" << endl;
    }
}