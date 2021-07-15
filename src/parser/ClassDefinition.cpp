#include "ClassDefinition.hpp"

namespace parser {
    using namespace std;

    void ClassDefinition::DebugPrint() const {
        
        for(string keyword : mKeywords) {
            cout << "\u001B[33m" << keyword << ' ' << "\u001B[0m";
        }

        cout << "\u001B[36m" << "class " << "\u001B[32m" << mName << "\u001B[0m";
        
        if(!mInheritance.empty()) {
            cout << " : ";
            cout << "\u001B[32m" << mInheritance[0] << "\u001B[0m";
            for(int i=1; i< mInheritance.size(); ++i) {
                cout << ", " << "\u001B[32m" << mInheritance[i] << "\u001B[0m";
            }
        }
        cout << " {\n";

        for(FunctionDefinition func : mConDes) {
            func.DebugPrint(1);
        }

        for(FunctionDefinition func : mFunctions) {
            func.DebugPrint(1);
        }
        cout << "}" << '\n';
    }
}