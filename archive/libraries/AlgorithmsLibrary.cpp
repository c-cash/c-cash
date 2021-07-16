#include "AlgorithmsLibrary.hpp"
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

    void AlgorithmsLibrary::linkGlobally(Scope &s) {
        // (F)binarySearch
        s.functions["binarySearch"] = [](vector<Object*> &args) -> Object* {
            if(args.size() != 2) throw runtime_error("binarySearch function takes exactly two arguments");
            Array* a = Array::convert(args[0]);
            if (args[1]->getType() == "Integer") {
                vector<int> temp;
                for (Object* o : a->value) temp.emplace_back(static_cast<Integer*>(o)->value);
                return new Boolean(binary_search(temp.cbegin(), temp.cend(), static_cast<Integer*>(args[1])->value));
            }
            else if (args[1]->getType() == "Long") {
                vector<long long> temp;
                for (Object* o : a->value) temp.emplace_back(static_cast<Long*>(o)->value);
                return new Boolean(binary_search(temp.cbegin(), temp.cend(), static_cast<Long*>(args[1])->value));
            }
            return nullptr;
        };

        // (F)euclidean
        s.functions["euclidean"] = [](vector<Object*> &args) -> Object* {
            if(args.size() != 2) throw runtime_error("eucliden function takes exactly two arguments");
            if(!(args[0]->getType()=="Integer"&&args[1]->getType()=="Integer") && !(args[0]->getType()=="Long"&&args[1]->getType()=="Long")) 
                throw runtime_error("eucliden function takes only two int or two long varibe");
            
            long long a = static_cast<Long*>(args[0])->value, b = static_cast<Long*>(args[1])->value;
            while(a!=b) {
                if(a>b) a-=b;
                else b-=a;
            }
            return new Long(a);
        };

        // (F)sort
        s.functions["sort"] = [](vector<Object*> &args) -> Object* {
            if(args.size() != 1) throw runtime_error("sort function takes exactly one argument");
            if (args[0]->getType().substr(0, 5) != "Array") throw runtime_error("Sosrt function takes only arrays!");
            Array* a = Array::convert(args[0]);
            string s = args[0]->getType();
            
            if(s.find("Integer") != string::npos){
                vector<int> temp;
                for (Object* o : a->value) temp.emplace_back(static_cast<Integer*>(o)->value);
                sort(temp.begin(), temp.end());
                vector<Object*> ret;
                for(int i : temp) ret.emplace_back(new Integer(i));
                return new Array(ret);
            } else if(s.find("Long") != string::npos){
                vector<long long> temp;
                for (Object* o : a->value) temp.emplace_back(static_cast<Long*>(o)->value);
                sort(temp.begin(), temp.end());
                vector<Object*> ret;
                for(long long i : temp) ret.emplace_back(new Long(i));
                return new Array(ret);
            } else if(s.find("Double") != string::npos){
                vector<double> temp;
                for (Object* o : a->value) temp.emplace_back(static_cast<Double*>(o)->value);
                sort(temp.begin(), temp.end());
                vector<Object*> ret;
                for(double i : temp) ret.emplace_back(new Double(i));
                return new Array(ret);
            } else if(s.find("String") != string::npos){
                vector<string> temp;
                for (Object* o : a->value) temp.emplace_back(static_cast<String*>(o)->value);
                sort(temp.begin(), temp.end());
                vector<Object*> ret;
                for(string i : temp) ret.emplace_back(new String(i));
                return new Array(ret);
            } else if(s.find("Char") != string::npos){
                vector<char> temp;
                for (Object* o : a->value) temp.emplace_back(static_cast<Char*>(o)->value);
                sort(temp.begin(), temp.end());
                vector<Object*> ret;
                for(char i : temp) ret.emplace_back(new Char(i));
                return new Array(ret);
            }
            throw runtime_error("excepted error;");
        };

    }

    void AlgorithmsLibrary::linkNamespaced(string name, Scope &scope) {
        linkGlobally(scope);
    }

    Library* AlgorithmsLibrary::getInstance() {
        return new AlgorithmsLibrary();
    }
}