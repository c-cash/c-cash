#include "../variables/String.hpp"
#include "../variables/Boolean.hpp"
#include "RegexLibrary.hpp"
#include "../variables/Array.hpp"

#include <regex>

namespace library {

    void RegexLibrary::linkGlobally(Scope &s) {
        // (F)match
        s.functions["match"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 2) throw runtime_error("regex::match function takes exactly two arguments");
            if (args[0]->getType() != "String" || args[1]->getType() != "String") throw runtime_error("regex::match function takes two strings");

            regex e(args[1]->getValueString());
            string s = args[0]->getValueString();

            vector<Object*> captured_groups;

            smatch res;
            while(regex_search(s, res, e)) {
                if (res.size() > 1)
                    captured_groups.emplace_back(new String(res[1]));
                s = res.suffix();
            }

            return new Array(captured_groups);
        };

        // (F)replace
        s.functions["replace"] = [](vector<Object*> &args) -> Object* {
            if (args.size() != 3) throw runtime_error("regex::replace function takes exactly two arguments");
            if (args[0]->getType() != "String" || args[1]->getType() != "String" || args[2]->getType() != "String") throw runtime_error("regex::replace function takes two strings");

            regex e(args[1]->getValueString());
            string s = args[0]->getValueString();
            string tr = args[2]->getValueString();

            return new String(regex_replace(s, e, tr, regex_constants::format_no_copy | regex_constants::format_first_only));
        };
        
    }

    void RegexLibrary::linkNamespaced(string name, Scope &scope) {
        linkGlobally(scope);
    }

    Library* RegexLibrary::getInstance() {
        return new RegexLibrary();
    }
}