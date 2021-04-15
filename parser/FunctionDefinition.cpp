#include "FunctionDefinition.hpp"

namespace parser {
    using namespace std;

    void FunctionDefinition::DebugPrint() const{
        cout << mName << (" (\n");

        for(ParameterDefinition param : mParameters) {
            param.DebugPrint(1);
        }

        cout << ") {\n";

        for(Statement statement : mStatements) {
            statement.DebugPrint(0);
        }
        cout << "}" << endl;
    }

    void ParameterDefinition::DebugPrint(size_t indent) const{
        cout << string(indent, '\t') << mType.mName << " " << mName << endl;
    }
}