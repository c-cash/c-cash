#include "FunctionDefinition.hpp"

namespace parser {
    using namespace std;

    void FunctionDefinition::DebugPrint() const{
        cout << mName << (" (\n");

        for(ParameterDefinition param : mParameters) {
            param.DebugPrint();
        }

        cout << ") {\n";

        for(Statement statement : mStatements) {
            statement.DebugPrint();
        }
        cout << "}" << endl;
    }

    void ParameterDefinition::DebugPrint() const{
        cout << mType.mName << " " << mName << endl;
    }
}