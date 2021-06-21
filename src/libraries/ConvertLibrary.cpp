#include "ConvertLibrary.hpp"
#include "../variables/Array.hpp"
#include "../variables/Integer.hpp"
#include "../variables/Long.hpp"
#include "../variables/Char.hpp"
#include "../variables/String.hpp"
#include "../variables/Double.hpp"
#include "../variables/Boolean.hpp"
#include "../variables/Object.hpp"

#include <algorithm>

namespace library {

    void ConvertLibrary::linkGlobally(Scope &s) {
        // (F)to string
        s.functions["toString"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 1) throw runtime_error("toString function takes exactly one argument");
            return new String(args[0]->getValueString());
        };

        // (F)stoi
        s.functions["stoi"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 1) throw runtime_error("stoi function takes exactly one argument");
            Object* o = Object::checkAll("string", args[0]);
            return new Integer(stoi(o->getValueString()));
        };
        // (F)stod
        s.functions["stod"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 1) throw runtime_error("stoi function takes exactly one argument");
            Object* o = Object::checkAll("string", args[0]);
            return new Double(stod(o->getValueString()));
        };
        // (F)stol
        s.functions["stol"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 1) throw runtime_error("stoi function takes exactly one argument");
            Object* o = Object::checkAll("string", args[0]);
            return new Long(stol(o->getValueString()));
        };

    }

    void ConvertLibrary::linkNamespaced(string name, Scope &scope) {
        linkGlobally(scope);
    }

    Library* ConvertLibrary::getInstance() {
        return new ConvertLibrary();
    }
}