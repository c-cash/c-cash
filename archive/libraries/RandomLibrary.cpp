#include "../variables/Integer.hpp"
#include "../variables/Double.hpp"
#include "../variables/String.hpp"
#include "../variables/Long.hpp"
#include "RandomLibrary.hpp"

#include <algorithm>
#include <string>

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
        // (F)randomChoice
        s.functions["randomChoice"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 1) throw runtime_error("randomChoice function takes exactly one argument");
            if (args[0]->getType().substr(0, 5) != "Array") throw runtime_error("Sosrt function takes only arrays!");
            
            srand ( time(NULL) );
            uint32_t size = Array::convert(args[0])->value.size() - 1;
            uint32_t index = rand()%(size + 1);

            if(args[0]->getType().find("String") != string::npos){
                return new String((Array::getIndex(index, args[0]))->getValueString());
            } else if(args[0]->getType().find("Double") != string::npos) {
                return new Double(stod((Array::getIndex(index, args[0]))->getValueString()));
            } else {
                return new Long(stod((Array::getIndex(index, args[0]))->getValueString()));
            }
        };

    }

    void RandomLibrary::linkNamespaced(string name, Scope &scope) {
        linkGlobally(scope);
    }

    Library* RandomLibrary::getInstance() {
        return new RandomLibrary();
    }
}