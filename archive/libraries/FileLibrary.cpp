#include "../variables/Integer.hpp"
#include "../variables/String.hpp"
#include "../variables/Boolean.hpp"

#include "FileLibrary.hpp"

namespace library {

    void FileLibrary::linkGlobally(Scope &s) {
        // (F)load file
        s.functions["loadFile"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 1) throw runtime_error("loadFile function takes exactly one argument");
            string t = args[0]->getType();
            if (t == "String"){
                ifstream f(args[0]->getValueString());
                string sf((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
                f.close();

                return new String(sf);
            }
            throw runtime_error("loadFile function takes a path");
        };
        // (F)save to file
        s.functions["saveFile"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 2) throw runtime_error("saveFile function takes exactly two arguments");
            string t = args[0]->getType();
            string tt = args[1]->getType();
            if (t == "String" && tt == "String"){
                ofstream f(args[0]->getValueString());
                f << args[1]->getValueString();
                f.close();

                return nullptr;
            }
            throw runtime_error("saveFile function takes path and a string");
        };   
        // (F)append to file
        s.functions["appendFile"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 2) throw runtime_error("appendFile function takes exactly two arguments");
            string t = args[0]->getType();
            string tt = args[1]->getType();
            if (t == "String" && tt == "String"){
                ofstream f(args[0]->getValueString(), ios::app);
                f << args[1]->getValueString();
                f.close();

                return nullptr;
            }
            throw runtime_error("appendFile function takes path and a string");
        };   
        // (F)clear file
        s.functions["clearFile"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 1) throw runtime_error("clearFile function takes exactly one argument");
            string t = args[0]->getType();
            if (t == "String"){
                ofstream f(args[0]->getValueString());
                f.clear();
                f.close();

                return nullptr;
            }
            throw runtime_error("clearFile function takes only path to file");
        }; 
        // (F)delete file
        s.functions["delFile"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 1) throw runtime_error("delFile function takes exactly one argument");
            string t = args[0]->getType();
            if (t == "String"){
                if (remove(args[0]->getValueString().c_str()) != 0) {
		            return new Boolean(false);
                }
                return new Boolean(true);
            }
            throw runtime_error("delFile function takes only path to file");
        }; 
        // (F)fileExists
        s.functions["fileExists"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 1) throw runtime_error("fileExists function takes exactly one argument");
            string t = args[0]->getType();
            if (t == "String"){
                if (FILE *file = fopen(args[0]->getValueString().c_str(), "r")) {
                    fclose(file);
                    return new Boolean(true);
                } else {
                    return new Boolean(false);
                }   
            }
            throw runtime_error("fileExists function takes only path to file");
        };
    }

    void FileLibrary::linkNamespaced(string name, Scope &scope) {
        linkGlobally(scope);
    }

    Library* FileLibrary::getInstance() {
        return new FileLibrary();
    }
}