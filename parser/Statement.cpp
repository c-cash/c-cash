#include "Statement.hpp"
#include <iostream>

namespace parser {
    using namespace std;

    void Statement::DebugPrint(size_t indent){
        cout << string(indent, '\t') << sStatementKindStrings[int(mKind)] << " ";
        cout << mType.mName << " " << mName << " (\n";
        for(Statement statement : mParameters){
            statement.DebugPrint(indent + 1);
        }
        cout << string(indent, '\t') << ")" << endl;
    }
}