#include "../variables/Long.hpp"
#include "TimeLibrary.hpp"

#include <chrono>

namespace library {

    void TimeLibrary::linkGlobally(Scope &s) {
        // (F)nanotime
        s.functions["nanotime"] = [](vector<Object*> &args) -> Object* {
            return new Long(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
        };
        // (F)millitime
        s.functions["millitime"] = [](vector<Object*> &args) -> Object* {
            return new Long(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
        };
        // (F)secondtime
        s.functions["secondtime"] = [](vector<Object*> &args) -> Object* {
            return new Long(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());
        };
    }

    void TimeLibrary::linkNamespaced(string name, Scope &scope) {
        linkGlobally(scope);
    }

    Library* TimeLibrary::getInstance() {
        return new TimeLibrary();
    }
}