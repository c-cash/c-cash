#include "../variables/String.hpp"
#include "StringLibrary.hpp"
#include "../variables/Integer.hpp"
#include "../variables/Boolean.hpp"

#include <algorithm>

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
        // (F)reverse
        s.functions["reverse"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 1) throw runtime_error("reverse function takes exactly one argument");
            string s = args[0]->getValueString();
            
            reverse(s.begin(), s.end());
            return new String(s);
        };
        // (F)capitalize
        s.functions["capitalize"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 1) throw runtime_error("capitalize function takes exactly one argument");
            string s = args[0]->getValueString();
            s[0] = std::toupper(s[0]);
            return new String(s);
        };
        // (F)swapCase
        s.functions["swapCase"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 1) throw runtime_error("swapCase function takes exactly one argument");
            string s = args[0]->getValueString();
            for(int i=0; i<s.size(); i++) {
                if(s[i] >= 65 && s[i] <= 90)
                    s[i] = std::tolower(s[i]);
                else if(s[i] >= 97 && s[i] <= 122)
                    s[i] = std::toupper(s[i]);
            }
            return new String(s);
        };
        // (F)endsWith
        s.functions["endsWith"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 2) throw runtime_error("endsWith function takes exactly two arguments");
            string s = args[0]->getValueString();
            string e = args[1]->getValueString();
            if(s.size() >= e.size()) {
                if (s.substr(s.size()-e.size(), s.size()) == e) {
                    return new Boolean(true);
                }
            }
            return new Boolean(false);
        };
        // (F)startsWith
        s.functions["startsWith"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 2) throw runtime_error("startsWith function takes exactly two arguments");
            string s = args[0]->getValueString();
            string e = args[1]->getValueString();
            if(s.size() >= e.size()) {
                if (s.substr(0, e.size()) == e) {
                    return new Boolean(true);
                }
            }
            return new Boolean(false);
        };
        // (F)find
        s.functions["find"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 2) throw runtime_error("find function takes exactly two arguments");
            string s = args[0]->getValueString();
            string p = args[1]->getValueString();
            size_t findedPosition = s.find(p);
            if(findedPosition == string::npos) {
                return new Integer(-1);
            }
            return new Integer(findedPosition);
        };
        // (F)deleteWhitespace
        s.functions["delWhitespace"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 1) throw runtime_error("delWhitespace function takes exactly one argument");
            string s = args[0]->getValueString();
            s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
            return new String(s);
        };
        // (F)strip
        s.functions["strip"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 1) throw runtime_error("strip function takes exactly one argument");
            string s = args[0]->getValueString();
            string es = "";
            int i=0, j=s.size()-1;
            while(isspace(s[i])) { ++i; }
            while(isspace(s[j])) { --j; }
            s = s.substr(i, j);
            size_t b = 0;
            size_t l = s.length();

            while (b < l)
            {
                size_t end = s.find_first_of(' ', b) + 1;
                es += s.substr(b, end - b);
                b = s.find_first_not_of(' ', end);
            }

            return new String(es);
        };
    }

    void StringLibrary::linkNamespaced(string name, Scope &scope) {
        linkGlobally(scope);
    }

    Library* StringLibrary::getInstance() {
        return new StringLibrary();
    }
}