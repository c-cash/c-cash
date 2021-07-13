#include "FunctionDefinition.hpp"

namespace parser {
    using namespace std;

    void FunctionDefinition::DebugPrint(size_t indent) const{
        cout << string(indent, '\t') << "\u001B[36m" << "func " << "\u001B[32m" << mName << "\u001B[0m" << " (\n";

        for(ParameterDefinition param : mParameters) {
            param.DebugPrint(indent + 1);
        }

        cout << string(indent, '\t') << ") {\n";
        for(Statement statement : mStatements) {
            statement.DebugPrint(indent);
        }
        cout << string(indent, '\t') << "}" << '\n';
    }

    void ParameterDefinition::DebugPrint(size_t indent) const{
        cout << string(indent, '\t') << "\u001B[33m" << mType.mName << " " << "\u001B[32m" << mName << "\u001B[0m" << '\n';
    }
}