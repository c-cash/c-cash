#include "../variables/String.hpp"
#include "StringLibrary.hpp"
#include "../variables/Integer.hpp"

namespace library {

    void StringLibrary::linkGlobally(Scope &s) {
        // (F)tolower
        s.functions["toLower"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 1) throw runtime_error("toLower function takes exactly one argument");
            string s = args[0]->getValueString();
            for(int i=0; i<s.size(); i++) {
                s[i] = std::tolower(s[i]);
            }
            return new String(s);
        };
        // (F)toupper
        s.functions["toUpper"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 1) throw runtime_error("toLower function takes exactly one argument");
            string s = args[0]->getValueString();
            for(int i=0; i<s.size(); i++) {
                s[i] = std::toupper(s[i]);
            }
            return new String(s);
        };
        // (F)charat  
        s.functions["charAt"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 2) throw runtime_error("charAt function takes exactly two arguments");
            if(args[0]->getType() == "String" && (args[1]->getType() == "Integer" || args[1]->getType() == "Long")){
                return new String(to_string((args[0]->getValueString())[stoi(args[1]->getValueString())]));
            }
            throw runtime_error("charAt needs two args string and integer or long");
        }; 
        // (F)replace
        s.functions["replace"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 3) throw runtime_error("replace function takes exactly three arguments");
            string s = args[0]->getValueString();
            string f = args[1]->getValueString();
            string t = args[2]->getValueString();
            transpiler::Transpiler::replace(s, f, t);
            return new String(s);
        };
        // (F)substr
        s.functions["substr"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 3) throw runtime_error("replace function takes exactly three arguments");
            string s = args[0]->getValueString();
            if (args[1]->getType() != "Integer" || args[2]->getType() != "Integer") throw runtime_error("substr needs one string and two integers");
            int f = static_cast<Integer*>(args[1])->value;
            int t = static_cast<Integer*>(args[2])->value;
            return new String(s.substr(f, t));
        };
        // (F)split
        s.functions["split"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 2) throw runtime_error("split function takes exactly two arguments");
            string s = args[0]->getValueString();
            string c = args[1]->getValueString();
            
            vector<Object*> parts;
            size_t b = 0;
            size_t position = s.find(c);
            // Hello beautiful world
            while(position != string::npos) {
                parts.emplace_back(new String(s.substr(b, position)));
                s = s.substr(position-b+c.size(), s.size()+c.size());
                position = s.find(c);
            }
            parts.emplace_back(new String(s.substr(b, position)));

            return new Array(parts);
        };
        
    }

    void StringLibrary::linkNamespaced(string name, Scope &scope) {
        linkGlobally(scope);
    }

    Library* StringLibrary::getInstance() {
        return new StringLibrary();
    }
}