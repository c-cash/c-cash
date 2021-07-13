#include "ClassDefinition.hpp"

namespace parser {
    using namespace std;

    void ClassDefinition::DebugPrint() const{
        cout << "\u001B[36m" << "class " "\u001B[32m" << mName << "\u001B[0m" << " {\n";

        for(FunctionDefinition func : mFunctions) {
            func.DebugPrint(1);
        }
        cout << "}" << '\n';
    }
}