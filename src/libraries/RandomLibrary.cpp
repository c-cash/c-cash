#include "../variables/Integer.hpp"
#include "../variables/Double.hpp"
#include "RandomLibrary.hpp"

namespace library {

    void RandomLibrary::linkGlobally(Scope &s) {
        // (F)random between
        s.functions["randomBetween"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 2) throw runtime_error("randomBetween function takes exactly two arguments");
            string t = args[0]->getType();
            string t2 = args[1]->getType();
            if ((t == "Integer") && (t2 == "Integer")){
                srand ( time(NULL) );
                return new Integer(rand()%((int) stod(args[1]->getValueString()) - (int) stod(args[0]->getValueString()) + 1) + stod(args[0]->getValueString()));
            }
            throw runtime_error("You can use random only on integers");
        };
        // (F)random
        s.functions["random"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 0) throw runtime_error("random function takes exactly zero arguments");
            srand ( time(NULL) );
            return new Double((double) rand()/RAND_MAX);
        };

    }

    void RandomLibrary::linkNamespaced(string name, Scope &scope) {
        linkGlobally(scope);
    }

    Library* RandomLibrary::getInstance() {
        return new RandomLibrary();
    }
}