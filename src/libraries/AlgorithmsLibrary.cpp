#include "AlgorithmsLibrary.hpp"
#include "../variables/Array.hpp"
#include "../variables/Integer.hpp"
#include "../variables/Long.hpp"

#include <algorithm>

namespace library {

    void AlgorithmsLibrary::linkGlobally(Scope &s) {
        // (F)binarySearch
        s.functions["binarySearch"] = [](vector<Object*> &args) -> Object* {
            if(args.size() != 2) throw runtime_error("binarySearch function takes exactly two arguments");
            Array* a = Array::convert(args[0]);
            if (args[1]->getType() == "Integer") {
                vector<int> temp;
                for (Object* o : a->value) temp.emplace_back(static_cast<Integer*>(o)->value);
                return new Integer(binary_search(temp.cbegin(), temp.cend(), static_cast<Integer*>(args[1])->value));
            }
            else if (args[1]->getType() == "Long") {
                vector<long long> temp;
                for (Object* o : a->value) temp.emplace_back(static_cast<Long*>(o)->value);
                return new Integer(binary_search(temp.cbegin(), temp.cend(), static_cast<Long*>(args[1])->value));
            }
            return nullptr;
        };
        // (F)sieveOfEratosthenes
        s.functions["sieveOfEratosthenes"] = [](vector<Object*> &args) -> Object* {
            
        };

        // (F)euclidean
        s.functions["euclidean"] = [](vector<Object*> &args) -> Object* {
            
        };

        // (F)sort
        s.functions["sort"] = [](vector<Object*> &args) -> Object* {
            
        };

    }

    void AlgorithmsLibrary::linkNamespaced(string name, Scope &scope) {
        linkGlobally(scope);
    }

    Library* AlgorithmsLibrary::getInstance() {
        return new AlgorithmsLibrary();
    }
}