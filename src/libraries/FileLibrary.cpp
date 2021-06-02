#include "../variables/Integer.hpp"
#include "../variables/Double.hpp"
#include "../variables/String.hpp"
#include "FileLibrary.hpp"

namespace library {

    void FileLibrary::linkGlobally(Scope &s) {
        // (F)Rabdom
        s.functions["openToString"] = [](vector<Object*> args) -> Object* {
            if (args.size() != 1) throw runtime_error("random function takes exactly one argument");
            string t = args[0]->getType();
            if (t == "String"){
                ifstream f(args[0]->getValueString());
                string sf((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
                f.close();

                return new String(sf);
            }
            throw runtime_error("You can random function only  integer");
        };
    }

    void FileLibrary::linkNamespaced(string name, Scope &scope) {
        linkGlobally(scope);
    }

    Library* FileLibrary::getInstance() {
        return new FileLibrary();
    }
}