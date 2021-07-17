#include "Linker.hpp"

namespace linker {
    void Linker::link(map<string, FunctionDefinition> &aFunctions, map<string, ClassDefinition> &aClasses) {
        for(Statement &s : aFunctions["*"].mStatements) {
            if(s.mName != "include") continue;
            linkInclude(s, aFunctions, aClasses);
        }   
    }

    void Linker::linkInclude(Statement &s, map<string, FunctionDefinition> &aFunctions, map<string, ClassDefinition> &aClasses) {
        for(Statement &include : s.mStatements) {
            if(!isNew(include.mName)) continue;
            includes.emplace_back(include.mName);
            
            parseInclude(includes[includes.size()-1], aFunctions, aClasses);
        }
        
    }

    bool Linker::isNew(string &name) {
        if(find(includes.cbegin(), includes.cend(), name) != includes.cend()){
            cout << "Warning: You can't include the same file more than once!";
            return false;
        }
        return true;
    }

    void Linker::parseInclude(string &name, map<string, FunctionDefinition> &aFunctions, map<string, ClassDefinition> &aClasses) {
        ifstream file(name);
        if(file.fail()) {
            throw runtime_error(string("Can't find file '") + name + string("' in include!!"));
        }
        
        string line, allCode;
        while (getline(file, line)){
            allCode += line + '\n';
        }
        

        Parser parser;
        Tokenaizer tokenaizer;
        vector<Token> tokens;
        tokens = tokenaizer.parse(allCode);
        parser.parse(tokens);
        file.close();

        for(auto it=aFunctions["*"].mStatements.begin(); it != aFunctions["*"].mStatements.end(); ++it) {
            //if(aFunctions["*"].mStatements[*it].mName != "include") continue;
            if (((Statement)*it).mName != "include") continue;
            aFunctions["*"].mStatements.erase(it);
            --it;
        }
        aFunctions["*"].mStatements.insert(aFunctions["*"].mStatements.end(), parser.mFunction["*"].mStatements.begin(), parser.mFunction["*"].mStatements.end());
        parser.mFunction.erase("*");
        aFunctions.insert(parser.mFunction.cbegin(), parser.mFunction.cend());
        aClasses.insert(parser.mClass.cbegin(), parser.mClass.cend());

        link(aFunctions, aClasses);
    }
}