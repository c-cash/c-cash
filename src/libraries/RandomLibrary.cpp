#include "../variables/Integer.hpp"
#include "../variables/Double.hpp"
#include "RandomLibrary.hpp"

namespace library {

    void RandomLibrary::linkGlobally(Scope &s) {
        // (F)Rabdom
        s.functions["randomBetween"] = [](vector<Object*> args) -> Object* {
            if (args.size() != 2) throw runtime_error("random function takes exactly two arguments");
            string t = args[0]->getType();
            string t2 = args[1]->getType();
            if ((t == "Integer") && (t2 == "Integer")){
                srand ( time(NULL) );
                return new Integer(rand()%((int) stod(args[1]->getValueString()) - (int) stod(args[0]->getValueString()) + 1) + stod(args[0]->getValueString()));
            }
            throw runtime_error("You can random function only  integer");
        };
        // (F)Rabdom
        s.functions["random"] = [](vector<Object*> args) -> Object* {
            if (args.size() != 0) throw runtime_error("random function takes exactly zero arguments");
            srand ( time(NULL) );
            return new Integer(rand());
        };

    }

    void RandomLibrary::linkNamespaced(string name, Scope &scope) {
        linkGlobally(scope);
    }

    Library* RandomLibrary::getInstance() {
        return new RandomLibrary();
    }
}